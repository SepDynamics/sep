#include <cmath>
#include <cstdlib>

#ifdef __cplusplus
extern "C" {
#endif

/* Simple structures used by the minimal runtime implementation. */
struct StubObject {
  int dummy;
};

void *pglNewDevice()
{
  return std::malloc(sizeof(StubObject));
}

void pglReleaseDevice(void *device)
{
  std::free(device);
}

void *pglNewPathSegmentStorage()
{
  return std::malloc(sizeof(StubObject));
}

void pglReleasePathSegmentStorage(void *storage)
{
  std::free(storage);
}

void pglPathSegmentStorageAddSegment(void *, void *)
{
  /* No-op for stub implementation. */
}

void pglPathSegmentStorageNextSegment(void *)
{
  /* No-op for stub implementation. */
}

void *pglNewSurfaceSamplingDistribution()
{
  return std::malloc(sizeof(StubObject));
}

void pglReleaseSurfaceSamplingDistribution(void *dist)
{
  std::free(dist);
}

void *pglNewVolumeSamplingDistribution()
{
  return std::malloc(sizeof(StubObject));
}

void pglReleaseVolumeSamplingDistribution(void *dist)
{
  std::free(dist);
}

void pglFieldInitSurfaceSamplingDistribution(void *)
{
  /* No-op for stub implementation. */
}

void pglFieldInitVolumeSamplingDistribution(void *)
{
  /* No-op for stub implementation. */
}

void pglSurfaceSamplingDistributionApplyCosineProduct(void *)
{
  /* No-op for stub implementation. */
}

double pglSurfaceSamplingDistributionPDF(void *)
{
  return 1.0; /* Uniform distribution. */
}

double pglSurfaceSamplingDistributionSamplePDF(void *)
{
  return 1.0; /* Uniform distribution. */
}

double pglSurfaceSamplingDistributionIncomingRadiancePDF(void *)
{
  return 1.0; /* Uniform distribution. */
}

void pglVolumeSamplingDistributionApplySingleLobeHenyeyGreensteinProduct(void *)
{
  /* No-op for stub implementation. */
}

double pglVolumeSamplingDistributionPDF(void *)
{
  return 1.0; /* Uniform distribution. */
}

double pglVolumeSamplingDistributionSamplePDF(void *)
{
  return 1.0; /* Uniform distribution. */
}

#ifdef __cplusplus
}
#endif
