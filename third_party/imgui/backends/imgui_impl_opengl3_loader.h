//-----------------------------------------------------------------------------
// GLEW - OpenGL Loading Library (Modified for ImGui integration)
//-----------------------------------------------------------------------------

#ifndef IMGUI_IMPL_OPENGL3_LOADER_H
#define IMGUI_IMPL_OPENGL3_LOADER_H

// We are using GLEW as our preferred loader
#include <GL/glew.h>

// Stub out the functions that are normally provided by ImGui's OpenGL3 loader
// since we're using GLEW instead
#define IMGUI_IMPL_OPENGL_LOADER_CUSTOM
#define IMGUI_IMPL_OPENGL_ES2     0
#define IMGUI_IMPL_OPENGL_ES3     0

// We're using modern OpenGL, so set the appropriate version
#if !defined(IMGUI_IMPL_OPENGL_LOADER_GLEW)
#define IMGUI_IMPL_OPENGL_LOADER_GLEW
#endif

// Prevent any imgl3w initialization code from being executed
#define imgl3wInit() 0

#endif // IMGUI_IMPL_OPENGL3_LOADER_H