/* SPDX-FileCopyrightText: 2011-2022 Blender Foundation
 *
 * SPDX-License-Identifier: Apache-2.0 */

#pragma once

#include <cstdint>

#ifndef CCL_NAMESPACE_BEGIN
#define CCL_NAMESPACE_BEGIN namespace ccl {
#endif

#ifndef CCL_NAMESPACE_END
#define CCL_NAMESPACE_END }
#endif

CCL_NAMESPACE_BEGIN

/* Basic types */
typedef unsigned char uchar;
typedef unsigned int uint;

/* Basic vector types */
struct int2 {
  int x, y;
};

struct int3 {
  int x, y, z;
};

struct int4 {
  int x, y, z, w;
};

struct uint2 {
  uint x, y;
};

struct uint3 {
  uint x, y, z;
};

struct uint4 {
  uint x, y, z, w;
};

struct uchar2 {
  uchar x, y;
};

struct uchar3 {
  uchar x, y, z;
};

struct uchar4 {
  uchar x, y, z, w;
};

struct float2 {
  float x, y;
};

struct float3 {
  float x, y, z;
};

struct float4 {
  float x, y, z, w;
};

struct packed_float3 {
  float x, y, z;
};

CCL_NAMESPACE_END