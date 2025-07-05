#include <string.h>
#include <cstring> // For std::memcpy if needed
#include <time.h>
#include <cstring>  // For std::memcpy if needed

// Define Cycles namespace macros
#define CCL_NAMESPACE_BEGIN namespace ccl {
#define CCL_NAMESPACE_END }

#include <string> // For std::string
#include "blender/oiio_output_driver.h"

// Core Cycles includes
#include "util/system.h"
#include "util/types.h"

// Scene includes
#include "scene/colorspace.h"
#include "util/image.h"
#include "util/unique_ptr.h"

#ifdef WITH_OCIO
#  include <OpenColorIO/OpenColorIO.h>
namespace OCIO = OCIO_NAMESPACE;
#endif

// OpenImageIO includes - full definitions first
#include <OpenImageIO/imagebuf.h>
#include <OpenImageIO/imagebufalgo.h>
#ifdef WITH_OCIO
#  include <OpenColorIO/OpenColorIO.h>
namespace OCIO = OCIO_NAMESPACE;
#endif

// Standard includes
#include <vector>

namespace OIIO = OpenImageIO_v2_5;

CCL_NAMESPACE_BEGIN

OIIOOutputDriver::OIIOOutputDriver(const string_view filepath,
                                   const string_view pass,
                                   LogFunction log)
    : filepath_(filepath), pass_(pass), log_(log) {}

OIIOOutputDriver::~OIIOOutputDriver() = default;

void OIIOOutputDriver::write_render_tile(const Tile &tile)
{
  /* Only write the full buffer, no intermediate tiles. */
  if (!(tile.size == tile.full_size)) {
    return;
  }

  log_(string_printf("Writing image %s", filepath_.c_str()));

  unique_ptr<ImageOutput> image_output(ImageOutput::create(filepath_));
  if (image_output == nullptr) {
    log_("Failed to create image file");
    return;
  }

  const int width = tile.size.x;
  const int height = tile.size.y;

  const ImageSpec spec(width, height, 4, TypeDesc::FLOAT);
  if (!image_output->open(filepath_, spec)) {
    log_("Failed to create image file");
    return;
  }

  vector<float> pixels(width * height * 4);
  if (!tile.get_pass_pixels(pass_, 4, pixels.data())) {
    log_("Failed to read render pass pixels");
    return;
  }

#ifdef WITH_OCIO
  bool applied_view_transform = false;
  try {
    OCIO::ConstConfigRcPtr config = OCIO::GetCurrentConfig();
    if (config) {
      const char *display = config->getDefaultDisplay();
      const char *view = config->getDefaultView(display);
      OCIO::ConstProcessorRcPtr processor = config->getProcessor(
          OCIO::ROLE_SCENE_LINEAR, display, view, OCIO::TRANSFORM_DIR_FORWARD);
      if (processor) {
        OCIO::ConstCPUProcessorRcPtr cpu = processor->getDefaultCPUProcessor();
        OCIO::PackedImageDesc desc(pixels.data(), width, height, 4);
        cpu->apply(desc);
        applied_view_transform = true;
      }
    }
  }
  catch (const OCIO::Exception &e) {
    log_(string_printf("OCIO error: %s", e.what()));
  }
#endif

  /* Manipulate offset and stride to convert from bottom-up to top-down convention. */
  OIIO::ImageBuf image_buffer(spec,
                              pixels.data() + (height - 1) * width * 4,
                              AutoStride,
                              -width * 4 * sizeof(float),
                              AutoStride);

  /* Apply view transform when possible, or simple gamma correction as fallback. */
#ifdef WITH_OCIO
  if (!applied_view_transform &&
      ColorSpaceManager::detect_known_colorspace(
          u_colorspace_auto, "", image_output->format_name(), true) == u_colorspace_srgb)
  {
    const float g = 1.0f / 2.2f;
    OIIO::ImageBufAlgo::pow(image_buffer, image_buffer, {g, g, g, 1.0f});
  }
#else
  if (ColorSpaceManager::detect_known_colorspace(
          u_colorspace_auto, "", image_output->format_name(), true) == u_colorspace_srgb)
  {
    const float g = 1.0f / 2.2f;
    OIIO::ImageBufAlgo::pow(image_buffer, image_buffer, {g, g, g, 1.0f});
  }
#endif

  /* Write to disk and close */
  image_buffer.set_write_format(TypeDesc::FLOAT);
  image_buffer.write(image_output.get());
  image_output->close();
}

CCL_NAMESPACE_END
