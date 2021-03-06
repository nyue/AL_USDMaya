//
// Copyright 2017 Animal Logic
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.//
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
#include "AL/maya/NodeHelper.h"
#include "AL/maya/SIMD.h"
#include "AL/maya/DgNodeHelper.h"
#include "AL/maya/ALHalf.h"

#include "maya/MObject.h"
#include "maya/MStatus.h"
#include "maya/MGlobal.h"
#include "maya/MPlug.h"
#include "maya/MFnDependencyNode.h"
#include "maya/MMatrixArray.h"
#include "maya/MFnMatrixData.h"
#include "maya/MFnMatrixArrayData.h"
#include "maya/MMatrix.h"
#include "maya/MFloatMatrix.h"
#include "maya/MFnNumericAttribute.h"
#include "maya/MFnMatrixAttribute.h"
#include "maya/MFnTypedAttribute.h"
#include "maya/MFnCompoundAttribute.h"

#include <unordered_map>
#include <cstring>

namespace AL {
namespace maya {

//----------------------------------------------------------------------------------------------------------------------
MStatus DgNodeHelper::setFloat(const MObject node, const MObject attr, float value)
{
  const char* const errorString = "float error";
  MPlug plug(node, attr);
  AL_MAYA_CHECK_ERROR(plug.setValue(value), errorString);
  return MS::kSuccess;
}

//----------------------------------------------------------------------------------------------------------------------
MStatus DgNodeHelper::setAngle(const MObject node, const MObject attr, MAngle value)
{
  const char* const errorString = "DgNodeHelper::setAngle";
  MPlug plug(node, attr);
  AL_MAYA_CHECK_ERROR(plug.setValue(value), errorString);
  return MS::kSuccess;
}

//----------------------------------------------------------------------------------------------------------------------
MStatus DgNodeHelper::setTime(const MObject node, const MObject attr, MTime value)
{
  const char* const errorString = "DgNodeHelper::setTime";
  MPlug plug(node, attr);
  AL_MAYA_CHECK_ERROR(plug.setValue(value), errorString);
  return MS::kSuccess;
}

//----------------------------------------------------------------------------------------------------------------------
MStatus DgNodeHelper::setDistance(const MObject node, const MObject attr, MDistance value)
{
  const char* const errorString = "DgNodeHelper::setDistance";
  MPlug plug(node, attr);
  AL_MAYA_CHECK_ERROR(plug.setValue(value), errorString);
  return MS::kSuccess;
}

//----------------------------------------------------------------------------------------------------------------------
MStatus DgNodeHelper::setDouble(MObject node, MObject attr, double value)
{
  const char* const errorString = "double error";
  MPlug plug(node, attr);
  AL_MAYA_CHECK_ERROR(plug.setValue(value), errorString);
  return MS::kSuccess;
}

//----------------------------------------------------------------------------------------------------------------------
MStatus DgNodeHelper::setBool(MObject node, MObject attr, bool value)
{
  const char* const errorString = "int error";
  MPlug plug(node, attr);
  AL_MAYA_CHECK_ERROR(plug.setValue(value), errorString);
  return MS::kSuccess;
}

//----------------------------------------------------------------------------------------------------------------------
MStatus DgNodeHelper::setInt8(MObject node, MObject attr, int8_t value)
{
  const char* const errorString = "int error";
  MPlug plug(node, attr);
  AL_MAYA_CHECK_ERROR(plug.setChar(value), errorString);
  return MS::kSuccess;
}

//----------------------------------------------------------------------------------------------------------------------
MStatus DgNodeHelper::setInt16(MObject node, MObject attr, int16_t value)
{
  const char* const errorString = "int error";
  MPlug plug(node, attr);
  AL_MAYA_CHECK_ERROR(plug.setShort(value), errorString);
  return MS::kSuccess;
}

//----------------------------------------------------------------------------------------------------------------------
MStatus DgNodeHelper::setInt32(MObject node, MObject attr, int32_t value)
{
  const char* const errorString = "int error";
  MPlug plug(node, attr);
  AL_MAYA_CHECK_ERROR(plug.setValue(value), errorString);
  return MS::kSuccess;
}

//----------------------------------------------------------------------------------------------------------------------
MStatus DgNodeHelper::setInt64(MObject node, MObject attr, int64_t value)
{
  const char* const errorString = "int64 error";
  MPlug plug(node, attr);
  AL_MAYA_CHECK_ERROR(plug.setInt64(value), errorString);
  return MS::kSuccess;
}

//----------------------------------------------------------------------------------------------------------------------
MStatus DgNodeHelper::setVec3(MObject node, MObject attr, float x, float y, float z)
{
  const char* const errorString = "vec3f error";
  MPlug plug(node, attr);
  AL_MAYA_CHECK_ERROR(plug.child(0).setValue(x), errorString);
  AL_MAYA_CHECK_ERROR(plug.child(1).setValue(y), errorString);
  AL_MAYA_CHECK_ERROR(plug.child(2).setValue(z), errorString);
  return MS::kSuccess;
}

//----------------------------------------------------------------------------------------------------------------------
MStatus DgNodeHelper::setVec3(MObject node, MObject attr, double x, double y, double z)
{
  const char* const errorString = "vec3d error";
  MPlug plug(node, attr);
  AL_MAYA_CHECK_ERROR(plug.child(0).setValue(x), errorString);
  AL_MAYA_CHECK_ERROR(plug.child(1).setValue(y), errorString);
  AL_MAYA_CHECK_ERROR(plug.child(2).setValue(z), errorString);
  return MS::kSuccess;
}

//----------------------------------------------------------------------------------------------------------------------
MStatus DgNodeHelper::setVec3(MObject node, MObject attr, MAngle x, MAngle y, MAngle z)
{
  const char* const errorString = "vec3d error";
  MPlug plug(node, attr);
  AL_MAYA_CHECK_ERROR(plug.child(0).setValue(x), errorString);
  AL_MAYA_CHECK_ERROR(plug.child(1).setValue(y), errorString);
  AL_MAYA_CHECK_ERROR(plug.child(2).setValue(z), errorString);
  return MS::kSuccess;
}

//----------------------------------------------------------------------------------------------------------------------
MStatus DgNodeHelper::setBoolArray(MObject node, MObject attribute, const bool* const values, const size_t count)
{
  MPlug plug(node, attribute);
  if(!plug || !plug.isArray())
    return MS::kFailure;

  AL_MAYA_CHECK_ERROR(plug.setNumElements(count), "DgNodeHelper: attribute array could not be resized");

  for(size_t i = 0; i != count; ++i)
  {
    plug.elementByLogicalIndex(i).setBool(values[i]);
  }

  return MS::kSuccess;
}

//----------------------------------------------------------------------------------------------------------------------
MStatus DgNodeHelper::setBoolArray(const MObject& node, const MObject& attribute, const std::vector<bool>& values)
{
  MPlug plug(node, attribute);
  if(!plug || !plug.isArray())
    return MS::kFailure;

  AL_MAYA_CHECK_ERROR(plug.setNumElements(values.size()), "DgNodeHelper: attribute array could not be resized");

  for(size_t i = 0, n = values.size(); i != n; ++i)
  {
    plug.elementByLogicalIndex(i).setBool(values[i]);
  }

  return MS::kSuccess;
}

//----------------------------------------------------------------------------------------------------------------------
MStatus DgNodeHelper::setInt8Array(MObject node, MObject attribute, const int8_t* const values, const size_t count)
{
  MPlug plug(node, attribute);
  if(!plug || !plug.isArray())
    return MS::kFailure;

  AL_MAYA_CHECK_ERROR(plug.setNumElements(count), "DgNodeHelper: attribute array could not be resized");

  for(size_t i = 0; i != count; ++i)
  {
    plug.elementByLogicalIndex(i).setChar(values[i]);
  }

  return MS::kSuccess;
}

//----------------------------------------------------------------------------------------------------------------------
MStatus DgNodeHelper::setInt16Array(MObject node, MObject attribute, const int16_t* const values, const size_t count)
{
  MPlug plug(node, attribute);
  if(!plug || !plug.isArray())
    return MS::kFailure;

  AL_MAYA_CHECK_ERROR(plug.setNumElements(count), "DgNodeHelper: attribute array could not be resized");

  for(size_t i = 0; i != count; ++i)
  {
    plug.elementByLogicalIndex(i).setShort(values[i]);
  }

  return MS::kSuccess;
}

//----------------------------------------------------------------------------------------------------------------------
MStatus DgNodeHelper::setInt32Array(MObject node, MObject attribute, const int32_t* const values, const size_t count)
{
  MPlug plug(node, attribute);
  if(!plug || !plug.isArray())
    return MS::kFailure;

  AL_MAYA_CHECK_ERROR(plug.setNumElements(count), "DgNodeHelper: attribute array could not be resized");

  for(size_t i = 0; i != count; ++i)
  {
    plug.elementByLogicalIndex(i).setValue(values[i]);
  }

  return MS::kSuccess;
}

//----------------------------------------------------------------------------------------------------------------------
MStatus DgNodeHelper::setInt64Array(MObject node, MObject attribute, const int64_t* const values, const size_t count)
{
  MPlug plug(node, attribute);
  if(!plug || !plug.isArray())
    return MS::kFailure;

  AL_MAYA_CHECK_ERROR(plug.setNumElements(count), "DgNodeHelper: attribute array could not be resized");

  for(size_t i = 0; i != count; ++i)
  {
    plug.elementByLogicalIndex(i).setInt64(values[i]);
  }

  return MS::kSuccess;
}

//----------------------------------------------------------------------------------------------------------------------
MStatus DgNodeHelper::setHalfArray(MObject node, MObject attribute, const GfHalf* const values, const size_t count)
{
  MPlug plug(node, attribute);
  if(!plug || !plug.isArray())
    return MS::kFailure;

  AL_MAYA_CHECK_ERROR(plug.setNumElements(count), "DgNodeHelper: attribute array could not be resized");

  size_t count8 = count & ~0x7ULL;
  for(size_t j = 0; j != count8; j += 8)
  {
    float f[8];
    half2float_8f(values + j, f);
    plug.elementByLogicalIndex(j + 0).setFloat(f[0]);
    plug.elementByLogicalIndex(j + 1).setFloat(f[1]);
    plug.elementByLogicalIndex(j + 2).setFloat(f[2]);
    plug.elementByLogicalIndex(j + 3).setFloat(f[3]);
    plug.elementByLogicalIndex(j + 4).setFloat(f[4]);
    plug.elementByLogicalIndex(j + 5).setFloat(f[5]);
    plug.elementByLogicalIndex(j + 6).setFloat(f[6]);
    plug.elementByLogicalIndex(j + 7).setFloat(f[7]);
  }

  if(count & 0x4)
  {
    float f[4];
    half2float_4f(values + count8, f);
    plug.elementByLogicalIndex(count8 + 0).setFloat(f[0]);
    plug.elementByLogicalIndex(count8 + 1).setFloat(f[1]);
    plug.elementByLogicalIndex(count8 + 2).setFloat(f[2]);
    plug.elementByLogicalIndex(count8 + 3).setFloat(f[3]);
    count8 |= 0x4;
  }

  switch(count & 0x3)
  {
  case 3: plug.elementByLogicalIndex(count8 + 2).setFloat(half2float_1f(values[count8 + 2]));
  case 2: plug.elementByLogicalIndex(count8 + 1).setFloat(half2float_1f(values[count8 + 1]));
  case 1: plug.elementByLogicalIndex(count8 + 0).setFloat(half2float_1f(values[count8 + 0]));
  default:
    break;
  }

  return MS::kSuccess;
}

//----------------------------------------------------------------------------------------------------------------------
MStatus DgNodeHelper::setFloatArray(MObject node, MObject attribute, const float* const values, const size_t count)
{
  MPlug plug(node, attribute);
  if(!plug || !plug.isArray())
    return MS::kFailure;

  AL_MAYA_CHECK_ERROR(plug.setNumElements(count), "DgNodeHelper: attribute array could not be resized");

  for(size_t i = 0; i != count; ++i)
  {
    plug.elementByLogicalIndex(i).setFloat(values[i]);
  }

  return MS::kSuccess;
}

//----------------------------------------------------------------------------------------------------------------------
MStatus DgNodeHelper::setDoubleArray(MObject node, MObject attribute, const double* const values, const size_t count)
{
  MPlug plug(node, attribute);
  if(!plug || !plug.isArray())
    return MS::kFailure;

  AL_MAYA_CHECK_ERROR(plug.setNumElements(count), "DgNodeHelper: attribute array could not be resized");

  for(size_t i = 0; i != count; ++i)
  {
    plug.elementByLogicalIndex(i).setDouble(values[i]);
  }

  return MS::kSuccess;
}

//----------------------------------------------------------------------------------------------------------------------
MStatus DgNodeHelper::setVec2Array(MObject node, MObject attribute, const int32_t* const values, const size_t count)
{
  MPlug plug(node, attribute);
  if(!plug || !plug.isArray())
    return MS::kFailure;

  AL_MAYA_CHECK_ERROR(plug.setNumElements(count), "DgNodeHelper: attribute array could not be resized");

  for(size_t i = 0, j = 0; i != count; ++i, j += 2)
  {
    auto v = plug.elementByLogicalIndex(i);
    v.child(0).setInt(values[j]);
    v.child(1).setInt(values[j + 1]);
  }

  return MS::kSuccess;
}

//----------------------------------------------------------------------------------------------------------------------
MStatus DgNodeHelper::setVec2Array(MObject node, MObject attribute, const GfHalf* const values, const size_t count)
{
  MPlug plug(node, attribute);
  if(!plug || !plug.isArray())
    return MS::kFailure;

  AL_MAYA_CHECK_ERROR(plug.setNumElements(count), "DgNodeHelper: attribute array could not be resized");

  size_t count4 = count & ~0x3ULL;
  for(size_t i = 0, j = 0; i != count4; i += 4, j += 8)
  {
    float f[8];
    half2float_8f(values + j, f);
    auto v0 = plug.elementByLogicalIndex(i + 0);
    auto v1 = plug.elementByLogicalIndex(i + 1);
    auto v2 = plug.elementByLogicalIndex(i + 2);
    auto v3 = plug.elementByLogicalIndex(i + 3);
    v0.child(0).setFloat(f[0]);
    v0.child(1).setFloat(f[1]);
    v1.child(0).setFloat(f[2]);
    v1.child(1).setFloat(f[3]);
    v2.child(0).setFloat(f[4]);
    v2.child(1).setFloat(f[5]);
    v3.child(0).setFloat(f[6]);
    v3.child(1).setFloat(f[7]);
  }

  if(count & 0x2)
  {
    float f[4];
    half2float_4f(values + count4 * 2, f);
    auto v0 = plug.elementByLogicalIndex(count4 + 0);
    auto v1 = plug.elementByLogicalIndex(count4 + 1);
    v0.child(0).setFloat(f[0]);
    v0.child(1).setFloat(f[1]);
    v1.child(0).setFloat(f[2]);
    v1.child(1).setFloat(f[3]);
    count4 += 2;
  }

  if(count & 0x1)
  {
    auto v0 = plug.elementByLogicalIndex(count4);
    v0.child(0).setFloat(half2float_1f(values[count4 * 2]));
    v0.child(1).setFloat(half2float_1f(values[count4 * 2 + 1]));
  }

  return MS::kSuccess;
}


//----------------------------------------------------------------------------------------------------------------------
MStatus DgNodeHelper::setVec2Array(MObject node, MObject attribute, const float* const values, const size_t count)
{
  MPlug plug(node, attribute);
  if(!plug || !plug.isArray())
    return MS::kFailure;

  AL_MAYA_CHECK_ERROR(plug.setNumElements(count), "DgNodeHelper: attribute array could not be resized");

  for(size_t i = 0, j = 0; i != count; ++i, j += 2)
  {
    auto v = plug.elementByLogicalIndex(i);
    v.child(0).setFloat(values[j]);
    v.child(1).setFloat(values[j + 1]);
  }

  return MS::kSuccess;
}

//----------------------------------------------------------------------------------------------------------------------
MStatus DgNodeHelper::setVec2Array(MObject node, MObject attribute, const double* const values, const size_t count)
{
  MPlug plug(node, attribute);
  if(!plug || !plug.isArray())
    return MS::kFailure;

  AL_MAYA_CHECK_ERROR(plug.setNumElements(count), "DgNodeHelper: attribute array could not be resized");

  for(size_t i = 0, j = 0; i != count; ++i, j += 2)
  {
    auto v = plug.elementByLogicalIndex(i);
    v.child(0).setDouble(values[j]);
    v.child(1).setDouble(values[j + 1]);
  }

  return MS::kSuccess;
}

//----------------------------------------------------------------------------------------------------------------------
MStatus DgNodeHelper::setVec3Array(MObject node, MObject attribute, const int32_t* const values, const size_t count)
{
  MPlug plug(node, attribute);
  if(!plug || !plug.isArray())
    return MS::kFailure;

  AL_MAYA_CHECK_ERROR(plug.setNumElements(count), "DgNodeHelper: attribute array could not be resized");
  for(size_t i = 0, j = 0; i != count; ++i, j += 3)
  {
    auto v = plug.elementByLogicalIndex(i);
    v.child(0).setInt(values[j]);
    v.child(1).setInt(values[j + 1]);
    v.child(2).setInt(values[j + 2]);
  }
  return MS::kSuccess;
}

//----------------------------------------------------------------------------------------------------------------------
MStatus DgNodeHelper::setVec3Array(MObject node, MObject attribute, const GfHalf* const values, const size_t count)
{
  MPlug plug(node, attribute);
  if(!plug || !plug.isArray())
    return MS::kFailure;

  AL_MAYA_CHECK_ERROR(plug.setNumElements(count), "DgNodeHelper: attribute array could not be resized");
  size_t count8 = count & ~0x7ULL;
  for(size_t i = 0, j = 0; i != count8; i += 8, j += 24)
  {
    float f[24];
    half2float_8f(values + j, f);
    half2float_8f(values + j + 8, f + 8);
    half2float_8f(values + j + 16, f + 16);

    for(int k = 0; k < 8; ++k)
    {
      auto v = plug.elementByLogicalIndex(i + k);
      v.child(0).setFloat(f[k * 3 + 0]);
      v.child(1).setFloat(f[k * 3 + 1]);
      v.child(2).setFloat(f[k * 3 + 2]);
    }
  }

  for(size_t i = count8, j = count8 * 3; i != count; ++i, j += 3)
  {
    float f[4];
    GfHalf h[4];
    h[0] = values[j];
    h[1] = values[j + 1];
    h[2] = values[j + 2];
    half2float_4f(h, f);
    auto v = plug.elementByLogicalIndex(i);
    v.child(0).setFloat(f[0]);
    v.child(1).setFloat(f[1]);
    v.child(2).setFloat(f[2]);
  }

  return MS::kSuccess;
}

//----------------------------------------------------------------------------------------------------------------------
MStatus DgNodeHelper::setVec3Array(MObject node, MObject attribute, const float* const values, const size_t count)
{
  MPlug plug(node, attribute);
  if(!plug || !plug.isArray())
    return MS::kFailure;

  AL_MAYA_CHECK_ERROR(plug.setNumElements(count), "DgNodeHelper: attribute array could not be resized");

  for(size_t i = 0, j = 0; i != count; ++i, j += 3)
  {
    auto v = plug.elementByLogicalIndex(i);
    v.child(0).setFloat(values[j]);
    v.child(1).setFloat(values[j + 1]);
    v.child(2).setFloat(values[j + 2]);
  }

  return MS::kSuccess;
}

//----------------------------------------------------------------------------------------------------------------------
MStatus DgNodeHelper::setVec3Array(MObject node, MObject attribute, const double* const values, const size_t count)
{
  MPlug plug(node, attribute);
  if(!plug || !plug.isArray())
    return MS::kFailure;

  AL_MAYA_CHECK_ERROR(plug.setNumElements(count), "DgNodeHelper: attribute array could not be resized");

  for(size_t i = 0, j = 0; i != count; ++i, j += 3)
  {
    auto v = plug.elementByLogicalIndex(i);
    v.child(0).setDouble(values[j]);
    v.child(1).setDouble(values[j + 1]);
    v.child(2).setDouble(values[j + 2]);
  }

  return MS::kSuccess;
}

//----------------------------------------------------------------------------------------------------------------------
MStatus DgNodeHelper::setVec4Array(MObject node, MObject attribute, const GfHalf* const values, const size_t count)
{
  MPlug plug(node, attribute);
  if(!plug || !plug.isArray())
  {
    return MS::kFailure;
  }

  AL_MAYA_CHECK_ERROR(plug.setNumElements(count), "DgNodeHelper: attribute array could not be resized");
  size_t count2 = count & ~0x1ULL;

  for(size_t i = 0, j = 0; i != count2; i += 2, j += 8)
  {
    float f[8];
    half2float_8f(values + j, f);
    auto v0 = plug.elementByLogicalIndex(i);
    auto v1 = plug.elementByLogicalIndex(i + 1);
    v0.child(0).setFloat(f[0]);
    v0.child(1).setFloat(f[1]);
    v0.child(2).setFloat(f[2]);
    v0.child(3).setFloat(f[3]);
    v1.child(0).setFloat(f[4]);
    v1.child(1).setFloat(f[5]);
    v1.child(2).setFloat(f[6]);
    v1.child(3).setFloat(f[7]);
  }
  if(count & 0x1)
  {
    float f[4];
    half2float_4f(values + count2 * 4, f);
    auto v0 = plug.elementByLogicalIndex(count2);
    v0.child(0).setFloat(f[0]);
    v0.child(1).setFloat(f[1]);
    v0.child(2).setFloat(f[2]);
    v0.child(3).setFloat(f[3]);
  }

  return MS::kSuccess;
}

//----------------------------------------------------------------------------------------------------------------------
MStatus DgNodeHelper::setVec4Array(MObject node, MObject attribute, const int* const values, const size_t count)
{
  MPlug plug(node, attribute);
  if(!plug || !plug.isArray())
    return MS::kFailure;

  AL_MAYA_CHECK_ERROR(plug.setNumElements(count), "DgNodeHelper: attribute array could not be resized");

  for(size_t i = 0, j = 0; i != count; ++i, j += 4)
  {
    auto v = plug.elementByLogicalIndex(i);
    v.child(0).setInt(values[j]);
    v.child(1).setInt(values[j + 1]);
    v.child(2).setInt(values[j + 2]);
    v.child(3).setInt(values[j + 3]);
  }

  return MS::kSuccess;
}

//----------------------------------------------------------------------------------------------------------------------
MStatus DgNodeHelper::setVec4Array(MObject node, MObject attribute, const float* const values, const size_t count)
{
  MPlug plug(node, attribute);
  if(!plug || !plug.isArray())
    return MS::kFailure;

  AL_MAYA_CHECK_ERROR(plug.setNumElements(count), "DgNodeHelper: attribute array could not be resized");

  for(size_t i = 0, j = 0; i != count; ++i, j += 4)
  {
    auto v = plug.elementByLogicalIndex(i);
    v.child(0).setFloat(values[j]);
    v.child(1).setFloat(values[j + 1]);
    v.child(2).setFloat(values[j + 2]);
    v.child(3).setFloat(values[j + 3]);
  }

  return MS::kSuccess;
}

//----------------------------------------------------------------------------------------------------------------------
MStatus DgNodeHelper::setVec4Array(MObject node, MObject attribute, const double* const values, const size_t count)
{
  MPlug plug(node, attribute);
  if(!plug || !plug.isArray())
    return MS::kFailure;

  AL_MAYA_CHECK_ERROR(plug.setNumElements(count), "DgNodeHelper: attribute array could not be resized");

  for(size_t i = 0, j = 0; i != count; ++i, j += 4)
  {
    auto v = plug.elementByLogicalIndex(i);
    v.child(0).setDouble(values[j]);
    v.child(1).setDouble(values[j + 1]);
    v.child(2).setDouble(values[j + 2]);
    v.child(3).setDouble(values[j + 3]);
  }

  return MS::kSuccess;
}

//----------------------------------------------------------------------------------------------------------------------
MStatus DgNodeHelper::setMatrix4x4Array(MObject node, MObject attribute, const double* const values, const size_t count)
{
  MPlug plug(node, attribute);

  if(!plug)
    return MS::kFailure;
  if(!plug.isArray())
  {
    MStatus status;
    MMatrixArray arrayData;
    arrayData.setLength(count);
    memcpy(&arrayData[0], values, sizeof(MMatrix) * count);

    MFnMatrixArrayData fn;
    MObject data = fn.create(arrayData, &status);
    AL_MAYA_CHECK_ERROR2(status, MString("Count not set array value"));
    status = plug.setValue(data);
    AL_MAYA_CHECK_ERROR2(status, MString("Count not set array value"));
  }
  else
  {
    // Yes this is horrible. It would appear that as of Maya 2017, setting the contents of matrix array attributes doesn't work.
    // Well, at least for dynamic attributes. Using an array builder inside a compute method would be one way
    char tempStr[1024] = {0};
    for(uint32_t i = 0; i < 16 * count; i += 16)
    {
      sprintf(tempStr, "setAttr \"%s[%d]\" -type \"matrix\" %lf %lf %lf %lf  %lf %lf %lf %lf  %lf %lf %lf %lf  %lf %lf %lf %lf;", plug.name().asChar(),
          (i >> 4),
          values[i + 0],
          values[i + 1],
          values[i + 2],
          values[i + 3],
          values[i + 4],
          values[i + 5],
          values[i + 6],
          values[i + 7],
          values[i + 8],
          values[i + 9],
          values[i + 10],
          values[i + 11],
          values[i + 12],
          values[i + 13],
          values[i + 14],
          values[i + 15] );
      MGlobal::executeCommand(tempStr);
    }
  }
  return MS::kSuccess;
}

//----------------------------------------------------------------------------------------------------------------------
MStatus DgNodeHelper::setMatrix4x4Array(MObject node, MObject attribute, const float* const values, const size_t count)
{
  MPlug plug(node, attribute);
  if(!plug)
    return MS::kFailure;

  if(!plug.isArray())
  {
    MStatus status;
    MMatrixArray arrayData;
    arrayData.setLength(count);

    double* ptr = &arrayData[0].matrix[0][0];
    for(size_t i = 0, j = 0; i != count; ++i, j += 16)
    {
      double* const dptr = ptr + j;
      const float* const fptr = values + j;
      #if AL_MAYA_ENABLE_SIMD
      # if __AVX__
      const f256 d0 = loadu8f(fptr);
      const f256 d1 = loadu8f(fptr + 8);
      storeu4d(dptr     , cvt4f_to_4d(extract4f(d0, 0)));
      storeu4d(dptr +  4, cvt4f_to_4d(extract4f(d0, 1)));
      storeu4d(dptr +  8, cvt4f_to_4d(extract4f(d1, 0)));
      storeu4d(dptr + 12, cvt4f_to_4d(extract4f(d1, 1)));
      # else
      const f128 d0 = loadu4f(fptr);
      const f128 d1 = loadu4f(fptr + 4);
      const f128 d2 = loadu4f(fptr + 8);
      const f128 d3 = loadu4f(fptr + 12);
      storeu2d(dptr    , cvt2f_to_2d(d0));
      storeu2d(dptr + 2, cvt2f_to_2d(movehl4f(d0, d0)));
      storeu2d(dptr + 4, cvt2f_to_2d(d1));
      storeu2d(dptr + 6, cvt2f_to_2d(movehl4f(d1, d1)));
      storeu2d(dptr + 8, cvt2f_to_2d(d2));
      storeu2d(dptr + 10, cvt2f_to_2d(movehl4f(d2, d2)));
      storeu2d(dptr + 12, cvt2f_to_2d(d3));
      storeu2d(dptr + 14, cvt2f_to_2d(movehl4f(d3, d3)));
      # endif
      #else
      for(int k = 0; k < 16; ++k)
      {
        dptr[k] = fptr[k];
      }
      #endif
    }

    MFnMatrixArrayData fn;
    MObject data = fn.create(arrayData, &status);
    AL_MAYA_CHECK_ERROR2(status, MString("Count not set array value"));
    status = plug.setValue(data);
    AL_MAYA_CHECK_ERROR2(status, MString("Count not set array value"));
  }
  else
  {
    // I can't seem to create a multi of arrays within the Maya API (without using an array data builder within a compute).
    char tempStr[2048] = {0};
    for(uint32_t i = 0; i < 16 * count; i += 16)
    {
      sprintf(tempStr, "setAttr \"%s[%d]\" -type \"matrix\" %f %f %f %f  %f %f %f %f  %f %f %f %f  %f %f %f %f;", plug.name().asChar(),
          (i >> 4),
          values[i + 0],
          values[i + 1],
          values[i + 2],
          values[i + 3],
          values[i + 4],
          values[i + 5],
          values[i + 6],
          values[i + 7],
          values[i + 8],
          values[i + 9],
          values[i + 10],
          values[i + 11],
          values[i + 12],
          values[i + 13],
          values[i + 14],
          values[i + 15] );
      MGlobal::executeCommand(tempStr);
    }
  }

  return MS::kSuccess;
}

//----------------------------------------------------------------------------------------------------------------------
MStatus DgNodeHelper::setTimeArray(MObject node, MObject attribute, const float* const values, const size_t count, MTime::Unit unit)
{
  // determine how much we need to modify the
  const MTime mod(1.0, unit);
  const float unitConversion = float(mod.as(MTime::k6000FPS));

  MPlug plug(node, attribute);
  if(!plug || !plug.isArray())
    return MS::kFailure;

  AL_MAYA_CHECK_ERROR(plug.setNumElements(count), "DgNodeHelper: attribute array could not be resized");

#if AL_MAYA_ENABLE_SIMD
  const f128 unitConversion128 = splat4f(unitConversion);
  const size_t count4 = count & ~3ULL;
  size_t i = 0;
  for(; i < count4; i += 4)
  {
    MPlug v0 = plug.elementByLogicalIndex(i);
    MPlug v1 = plug.elementByLogicalIndex(i + 1);
    MPlug v2 = plug.elementByLogicalIndex(i + 2);
    MPlug v3 = plug.elementByLogicalIndex(i + 3);

    const f128 temp = mul4f(unitConversion128, loadu4f(values + i));
    ALIGN16(float tempf[4]);
    store4f(tempf, temp);
    v0.setFloat(tempf[0]);
    v1.setFloat(tempf[1]);
    v2.setFloat(tempf[2]);
    v3.setFloat(tempf[3]);
  }
  switch(count & 3)
  {
  case 3: plug.elementByLogicalIndex(i + 2).setFloat(unitConversion * values[i + 2]);
  case 2: plug.elementByLogicalIndex(i + 1).setFloat(unitConversion * values[i + 1]);
  case 1: plug.elementByLogicalIndex(i    ).setFloat(unitConversion * values[i    ]);
  default: break;
  }
#else
  for(size_t i = 0; i < count; ++i)
  {
    plug.elementByLogicalIndex(i).setFloat(unitConversion * values[i]);
  }
#endif
  return MS::kSuccess;
}

//----------------------------------------------------------------------------------------------------------------------
MStatus DgNodeHelper::setAngleArray(MObject node, MObject attribute, const float* const values, const size_t count, MAngle::Unit unit)
{
  // determine how much we need to modify the
  const MAngle mod(1.0, unit);
  const float unitConversion = float(mod.as(MAngle::internalUnit()));

  MPlug plug(node, attribute);
  if(!plug || !plug.isArray())
    return MS::kFailure;

  AL_MAYA_CHECK_ERROR(plug.setNumElements(count), "DgNodeHelper: attribute array could not be resized");

#if AL_MAYA_ENABLE_SIMD
  const f128 unitConversion128 = splat4f(unitConversion);
  const size_t count4 = count & ~3ULL;
  size_t i = 0;
  for(; i < count4; i += 4)
  {
    MPlug v0 = plug.elementByLogicalIndex(i);
    MPlug v1 = plug.elementByLogicalIndex(i + 1);
    MPlug v2 = plug.elementByLogicalIndex(i + 2);
    MPlug v3 = plug.elementByLogicalIndex(i + 3);

    const f128 temp = mul4f(unitConversion128, loadu4f(values + i));
    ALIGN16(float tempf[4]);
    store4f(tempf, temp);
    v0.setFloat(tempf[0]);
    v1.setFloat(tempf[1]);
    v2.setFloat(tempf[2]);
    v3.setFloat(tempf[3]);
  }
  switch(count & 3)
  {
  case 3: plug.elementByLogicalIndex(i + 2).setFloat(unitConversion * values[i + 2]);
  case 2: plug.elementByLogicalIndex(i + 1).setFloat(unitConversion * values[i + 1]);
  case 1: plug.elementByLogicalIndex(i    ).setFloat(unitConversion * values[i]);
  default: break;
  }
#else
  for(size_t i = 0; i < count; ++i)
  {
    plug.elementByLogicalIndex(i).setFloat(unitConversion * values[i]);
  }
#endif
  return MS::kSuccess;
}

//----------------------------------------------------------------------------------------------------------------------
MStatus DgNodeHelper::setDistanceArray(MObject node, MObject attribute, const float* const values, const size_t count, MDistance::Unit unit)
{
  // determine how much we need to modify the
  const MDistance mod(1.0, unit);
  const float unitConversion = float(mod.as(MDistance::internalUnit()));

  MPlug plug(node, attribute);
  if(!plug || !plug.isArray())
    return MS::kFailure;

  AL_MAYA_CHECK_ERROR(plug.setNumElements(count), "DgNodeHelper: attribute array could not be resized");

#if AL_MAYA_ENABLE_SIMD
  const f128 unitConversion128 = splat4f(unitConversion);
  const size_t count4 = count & ~3ULL;
  size_t i = 0;
  for(; i < count4; i += 4)
  {
    MPlug v0 = plug.elementByLogicalIndex(i);
    MPlug v1 = plug.elementByLogicalIndex(i + 1);
    MPlug v2 = plug.elementByLogicalIndex(i + 2);
    MPlug v3 = plug.elementByLogicalIndex(i + 3);

    const f128 temp = mul4f(unitConversion128, loadu4f(values + i));
    ALIGN16(float tempf[4]);
    store4f(tempf, temp);
    v0.setFloat(tempf[0]);
    v1.setFloat(tempf[1]);
    v2.setFloat(tempf[2]);
    v3.setFloat(tempf[3]);
  }
  switch(count & 0x3)
  {
  case 3: plug.elementByLogicalIndex(i + 2).setFloat(unitConversion * values[i + 2]);
  case 2: plug.elementByLogicalIndex(i + 1).setFloat(unitConversion * values[i + 1]);
  case 1: plug.elementByLogicalIndex(i    ).setFloat(unitConversion * values[i]);
  default: break;
  }
#else
  for(size_t i = 0; i < count; ++i)
  {
    plug.elementByLogicalIndex(i).setFloat(unitConversion * values[i]);
  }
#endif
  return MS::kSuccess;
}

//----------------------------------------------------------------------------------------------------------------------
MStatus DgNodeHelper::getBoolArray(const MObject& node, const MObject& attr, std::vector<bool>& values)
{
  //
  // Handle the oddity that is std::vector<bool>
  //

  MPlug plug(node, attr);
  if(!plug || !plug.isArray())
    return MS::kFailure;

  uint32_t num = plug.numElements();
  values.resize(num);
  for(uint32_t i = 0; i < num; ++i)
  {
    values[i] = plug.elementByLogicalIndex(i).asBool();
  }
  return MS::kSuccess;
}

//----------------------------------------------------------------------------------------------------------------------
MStatus DgNodeHelper::getBoolArray(MObject node, MObject attribute, bool* const values, const size_t count)
{
  MPlug plug(node, attribute);
  if(!plug || !plug.isArray())
    return MS::kFailure;

  uint32_t num = plug.numElements();
  if(num != count)
  {
    MGlobal::displayError("array is sized incorrectly");
    return MS::kFailure;
  }

#if AL_MAYA_ENABLE_SIMD
  uint32_t count16 = count & ~0xF;
  for(uint32_t i = 0; i < count16; i += 16)
  {
    ALIGN16(bool temp[16]);
    temp[0] = plug.elementByLogicalIndex(i).asBool();
    temp[1] = plug.elementByLogicalIndex(i + 1).asBool();
    temp[2] = plug.elementByLogicalIndex(i + 2).asBool();
    temp[3] = plug.elementByLogicalIndex(i + 3).asBool();
    temp[4] = plug.elementByLogicalIndex(i + 4).asBool();
    temp[5] = plug.elementByLogicalIndex(i + 5).asBool();
    temp[6] = plug.elementByLogicalIndex(i + 6).asBool();
    temp[7] = plug.elementByLogicalIndex(i + 7).asBool();
    temp[8] = plug.elementByLogicalIndex(i + 8).asBool();
    temp[9] = plug.elementByLogicalIndex(i + 9).asBool();
    temp[10] = plug.elementByLogicalIndex(i + 10).asBool();
    temp[11] = plug.elementByLogicalIndex(i + 11).asBool();
    temp[12] = plug.elementByLogicalIndex(i + 12).asBool();
    temp[13] = plug.elementByLogicalIndex(i + 13).asBool();
    temp[14] = plug.elementByLogicalIndex(i + 14).asBool();
    temp[15] = plug.elementByLogicalIndex(i + 15).asBool();
    storeu4i(values + i, load4i(temp));
  }

  for(uint32_t i = count16; i < num; ++i)
  {
    values[i] = plug.elementByLogicalIndex(i).asBool();
  }
#else
  for(uint32_t i = 0; i < num; ++i)
  {
    values[i] = plug.elementByLogicalIndex(i).asBool();
  }
#endif

  return MS::kSuccess;
}

//----------------------------------------------------------------------------------------------------------------------
MStatus DgNodeHelper::getInt64Array(MObject node, MObject attribute, int64_t* const values, const size_t count)
{
  MPlug plug(node, attribute);
  if(!plug || !plug.isArray())
    return MS::kFailure;

  uint32_t num = plug.numElements();
  if(num != count)
  {
    MGlobal::displayError(plug.name() + ", error array is sized incorrectly");
    return MS::kFailure;
  }

#if AL_MAYA_ENABLE_SIMD

#ifdef __AVX__
  uint32_t count4 = count & ~0x3;
  uint32_t i = 0;
  for(; i < count4; i += 4)
  {
    ALIGN32(int64_t temp[4]);
    temp[0] = plug.elementByLogicalIndex(i).asInt64();
    temp[1] = plug.elementByLogicalIndex(i + 1).asInt64();
    temp[2] = plug.elementByLogicalIndex(i + 2).asInt64();
    temp[3] = plug.elementByLogicalIndex(i + 3).asInt64();
    storeu8i(values + i, load8i(temp));
  }

  if(count & 0x2)
  {
    ALIGN16(int64_t temp[2]);
    temp[0] = plug.elementByLogicalIndex(i).asInt64();
    temp[1] = plug.elementByLogicalIndex(i + 1).asInt64();
    storeu4i(values + i, load4i(temp));
    i += 2;
  }

#else
  uint32_t count2 = count & ~0x1;
  uint32_t i = 0;
  for(; i < count2; i += 2)
  {
    ALIGN16(int64_t temp[2]);
    temp[0] = plug.elementByLogicalIndex(i).asInt64();
    temp[1] = plug.elementByLogicalIndex(i + 1).asInt64();
    storeu4i(values + i, load4i(temp));
  }
#endif

  if(count & 1)
  {
    values[i] = plug.elementByLogicalIndex(i).asInt64();
  }

#else
  for(uint32_t i = 0; i < num; ++i)
  {
    values[i] = plug.elementByLogicalIndex(i).asInt64();
  }
#endif

  return MS::kSuccess;
}

//----------------------------------------------------------------------------------------------------------------------
MStatus DgNodeHelper::getInt32Array(MObject node, MObject attribute, int32_t* const values, const size_t count)
{
  MPlug plug(node, attribute);
  if(!plug || !plug.isArray())
    return MS::kFailure;

  uint32_t num = plug.numElements();
  if(num != count)
  {
    MGlobal::displayError("array is sized incorrectly");
    return MS::kFailure;
  }

#if AL_MAYA_ENABLE_SIMD

#ifdef __AVX__
  uint32_t count8 = count & ~0x7;
  uint32_t i = 0;
  for(; i < count8; i += 8)
  {
    ALIGN32(int32_t temp[8]);
    temp[0] = plug.elementByLogicalIndex(i).asInt();
    temp[1] = plug.elementByLogicalIndex(i + 1).asInt();
    temp[2] = plug.elementByLogicalIndex(i + 2).asInt();
    temp[3] = plug.elementByLogicalIndex(i + 3).asInt();
    temp[4] = plug.elementByLogicalIndex(i + 4).asInt();
    temp[5] = plug.elementByLogicalIndex(i + 5).asInt();
    temp[6] = plug.elementByLogicalIndex(i + 6).asInt();
    temp[7] = plug.elementByLogicalIndex(i + 7).asInt();
    storeu8i(values + i, load8i(temp));
  }

  if(count & 0x4)
  {
    ALIGN16(int32_t temp[4]);
    temp[0] = plug.elementByLogicalIndex(i).asInt();
    temp[1] = plug.elementByLogicalIndex(i + 1).asInt();
    temp[2] = plug.elementByLogicalIndex(i + 2).asInt();
    temp[3] = plug.elementByLogicalIndex(i + 3).asInt();
    storeu4i(values + i, load4i(temp));
    i += 4;
  }

#else
  uint32_t count4 = count & ~0x3;
  uint32_t i = 0;
  for(; i < count4; i += 4)
  {
    ALIGN16(int32_t temp[4]);
    temp[0] = plug.elementByLogicalIndex(i).asInt();
    temp[1] = plug.elementByLogicalIndex(i + 1).asInt();
    temp[2] = plug.elementByLogicalIndex(i + 2).asInt();
    temp[3] = plug.elementByLogicalIndex(i + 3).asInt();
    storeu4i(values + i, load4i(temp));
  }
#endif

  switch(count & 3)
  {
  case 3: values[i + 2] = plug.elementByLogicalIndex(i + 2).asInt();
  case 2: values[i + 1] = plug.elementByLogicalIndex(i + 1).asInt();
  case 1: values[i] = plug.elementByLogicalIndex(i).asInt();
  default: break;
  }

#else
  for(uint32_t i = 0; i < num; ++i)
  {
    values[i] = plug.elementByLogicalIndex(i).asInt();
  }
#endif

  return MS::kSuccess;
}

//----------------------------------------------------------------------------------------------------------------------
MStatus DgNodeHelper::getInt8Array(MObject node, MObject attribute, int8_t* const values, const size_t count)
{
  MPlug plug(node, attribute);
  if(!plug || !plug.isArray())
    return MS::kFailure;

  uint32_t num = plug.numElements();
  if(num != count)
  {
    MGlobal::displayError("array is sized incorrectly");
    return MS::kFailure;
  }

  for(uint32_t i = 0; i < num; ++i)
  {
    values[i] = plug.elementByLogicalIndex(i).asChar();
  }

  return MS::kSuccess;
}

//----------------------------------------------------------------------------------------------------------------------
MStatus DgNodeHelper::getInt16Array(MObject node, MObject attribute, int16_t* const values, const size_t count)
{
  MPlug plug(node, attribute);
  if(!plug || !plug.isArray())
    return MS::kFailure;

  uint32_t num = plug.numElements();
  if(num != count)
  {
    MGlobal::displayError("array is sized incorrectly");
    return MS::kFailure;
  }

  for(uint32_t i = 0; i < num; ++i)
  {
    values[i] = plug.elementByLogicalIndex(i).asShort();
  }

  return MS::kSuccess;
}

//----------------------------------------------------------------------------------------------------------------------
MStatus DgNodeHelper::getFloatArray(MObject node, MObject attribute, float* const values, const size_t count)
{
  MPlug plug(node, attribute);
  if(!plug || !plug.isArray())
    return MS::kFailure;

  uint32_t num = plug.numElements();
  if(num != count)
  {
    MGlobal::displayError("array is sized incorrectly");
    return MS::kFailure;
  }

#if AL_MAYA_ENABLE_SIMD

#ifdef __AVX__
  uint32_t count8 = count & ~0x7;
  uint32_t i = 0;
  for(; i < count8; i += 8)
  {
    ALIGN32(float temp[8]);
    temp[0] = plug.elementByLogicalIndex(i).asFloat();
    temp[1] = plug.elementByLogicalIndex(i + 1).asFloat();
    temp[2] = plug.elementByLogicalIndex(i + 2).asFloat();
    temp[3] = plug.elementByLogicalIndex(i + 3).asFloat();
    temp[4] = plug.elementByLogicalIndex(i + 4).asFloat();
    temp[5] = plug.elementByLogicalIndex(i + 5).asFloat();
    temp[6] = plug.elementByLogicalIndex(i + 6).asFloat();
    temp[7] = plug.elementByLogicalIndex(i + 7).asFloat();
    storeu8f(values + i, load8f(temp));
  }

  if(count & 0x4)
  {
    ALIGN16(float temp[4]);
    temp[0] = plug.elementByLogicalIndex(i).asFloat();
    temp[1] = plug.elementByLogicalIndex(i + 1).asFloat();
    temp[2] = plug.elementByLogicalIndex(i + 2).asFloat();
    temp[3] = plug.elementByLogicalIndex(i + 3).asFloat();
    storeu4f(values + i, load4f(temp));
    i += 4;
  }

#else
  uint32_t count4 = count & ~0x3;
  uint32_t i = 0;
  for(; i < count4; i += 4)
  {
    ALIGN16(float temp[4]);
    temp[0] = plug.elementByLogicalIndex(i).asFloat();
    temp[1] = plug.elementByLogicalIndex(i + 1).asFloat();
    temp[2] = plug.elementByLogicalIndex(i + 2).asFloat();
    temp[3] = plug.elementByLogicalIndex(i + 3).asFloat();
    storeu4f(values + i, load4f(temp));
  }

#endif

  switch(count & 3)
  {
  case 3: values[i + 2] = plug.elementByLogicalIndex(i + 2).asFloat();
  case 2: values[i + 1] = plug.elementByLogicalIndex(i + 1).asFloat();
  case 1: values[i] = plug.elementByLogicalIndex(i).asFloat();
  default: break;
  }

#else
  for(uint32_t i = 0; i < num; ++i)
  {
    values[i] = plug.elementByLogicalIndex(i).asFloat();
  }
#endif
  return MS::kSuccess;
}


//----------------------------------------------------------------------------------------------------------------------
MStatus DgNodeHelper::getHalfArray(MObject node, MObject attribute, GfHalf* const values, const size_t count)
{
  MPlug plug(node, attribute);
  if(!plug || !plug.isArray())
    return MS::kFailure;

  uint32_t num = plug.numElements();
  if(num != count)
  {
    MGlobal::displayError("array is sized incorrectly");
    return MS::kFailure;
  }

  size_t count8 = count & ~0x7ULL;
  for(uint32_t i = 0; i < count8; i += 8)
  {
    float f[8];
    f[0] = plug.elementByLogicalIndex(i + 0).asFloat();
    f[1] = plug.elementByLogicalIndex(i + 1).asFloat();
    f[2] = plug.elementByLogicalIndex(i + 2).asFloat();
    f[3] = plug.elementByLogicalIndex(i + 3).asFloat();
    f[4] = plug.elementByLogicalIndex(i + 4).asFloat();
    f[5] = plug.elementByLogicalIndex(i + 5).asFloat();
    f[6] = plug.elementByLogicalIndex(i + 6).asFloat();
    f[7] = plug.elementByLogicalIndex(i + 7).asFloat();
    float2half_8f(f, values + i);
  }

  if(count & 0x4)
  {
    float f[4];
    f[0] = plug.elementByLogicalIndex(count8 + 0).asFloat();
    f[1] = plug.elementByLogicalIndex(count8 + 1).asFloat();
    f[2] = plug.elementByLogicalIndex(count8 + 2).asFloat();
    f[3] = plug.elementByLogicalIndex(count8 + 3).asFloat();
    float2half_4f(f, values + count8);
    count8 += 4;
  }

  switch(count & 0x3)
  {
  case 3: values[count8 + 2] = float2half_1f(plug.elementByLogicalIndex(count8 + 2).asFloat());
  case 2: values[count8 + 1] = float2half_1f(plug.elementByLogicalIndex(count8 + 1).asFloat());
  case 1: values[count8 + 0] = float2half_1f(plug.elementByLogicalIndex(count8 + 0).asFloat());
  default: break;
  }
  return MS::kSuccess;
}

//----------------------------------------------------------------------------------------------------------------------
MStatus DgNodeHelper::getDoubleArray(MObject node, MObject attribute, double* const values, const size_t count)
{
  MPlug plug(node, attribute);
  if(!plug || !plug.isArray())
    return MS::kFailure;

  uint32_t num = plug.numElements();
  if(num != count)
  {
    MGlobal::displayError("array is sized incorrectly");
    return MS::kFailure;
  }
#if AL_MAYA_ENABLE_SIMD

#ifdef __AVX__
  uint32_t count8 = count & ~0x7;
  uint32_t i = 0;
  for(; i < count8; i += 8)
  {
    ALIGN32(double temp[8]);
    temp[0] = plug.elementByLogicalIndex(i).asDouble();
    temp[1] = plug.elementByLogicalIndex(i + 1).asDouble();
    temp[2] = plug.elementByLogicalIndex(i + 2).asDouble();
    temp[3] = plug.elementByLogicalIndex(i + 3).asDouble();
    temp[4] = plug.elementByLogicalIndex(i + 4).asDouble();
    temp[5] = plug.elementByLogicalIndex(i + 5).asDouble();
    temp[6] = plug.elementByLogicalIndex(i + 6).asDouble();
    temp[7] = plug.elementByLogicalIndex(i + 7).asDouble();
    storeu4d(values + i, load4d(temp));
    storeu4d(values + i + 4, load4d(temp + 4));
  }

  if(count & 0x4)
  {
    ALIGN16(double temp[4]);
    temp[0] = plug.elementByLogicalIndex(i).asDouble();
    temp[1] = plug.elementByLogicalIndex(i + 1).asDouble();
    temp[2] = plug.elementByLogicalIndex(i + 2).asDouble();
    temp[3] = plug.elementByLogicalIndex(i + 3).asDouble();
    storeu4d(values + i, load4d(temp));
    i += 4;
  }

#else
  uint32_t count4 = count & ~0x3;
  uint32_t i = 0;
  for(; i < count4; i += 4)
  {
    ALIGN16(double temp[4]);
    temp[0] = plug.elementByLogicalIndex(i).asDouble();
    temp[1] = plug.elementByLogicalIndex(i + 1).asDouble();
    temp[2] = plug.elementByLogicalIndex(i + 2).asDouble();
    temp[3] = plug.elementByLogicalIndex(i + 3).asDouble();
    storeu2d(values + i, load2d(temp));
    storeu2d(values + i + 2, load2d(temp + 2));
  }

#endif

  switch(count & 3)
  {
  case 3: values[i + 2] = plug.elementByLogicalIndex(i + 2).asDouble();
  case 2: values[i + 1] = plug.elementByLogicalIndex(i + 1).asDouble();
  case 1: values[i] = plug.elementByLogicalIndex(i).asDouble();
  default: break;
  }

#else
  for(uint32_t i = 0; i < num; ++i)
  {
    values[i] = plug.elementByLogicalIndex(i).asDouble();
  }
#endif
  return MS::kSuccess;
}

//----------------------------------------------------------------------------------------------------------------------
MStatus DgNodeHelper::getVec2Array(MObject node, MObject attribute, double* const values, const size_t count)
{
  MPlug plug(node, attribute);
  if(!plug || !plug.isArray())
    return MS::kFailure;

  uint32_t num = plug.numElements();
  if(num != count)
  {
    MGlobal::displayError("array is sized incorrectly");
    return MS::kFailure;
  }

#if AL_MAYA_ENABLE_SIMD

#ifdef __AVX__
  uint32_t count2 = count & ~0x1;
  uint32_t i = 0;
  for(; i < count2; i += 2)
  {
    ALIGN32(double temp[4]);
    temp[0] = plug.elementByLogicalIndex(i).child(0).asDouble();
    temp[1] = plug.elementByLogicalIndex(i).child(1).asDouble();
    temp[2] = plug.elementByLogicalIndex(i + 1).child(0).asDouble();
    temp[3] = plug.elementByLogicalIndex(i + 1).child(1).asDouble();
    storeu4d(values + i * 2, load4d(temp));
  }

  if(count & 1)
  {
    ALIGN16(double temp[2]);
    temp[0] = plug.elementByLogicalIndex(i).child(0).asDouble();
    temp[1] = plug.elementByLogicalIndex(i).child(1).asDouble();
    storeu2d(values + i * 2, load2d(temp));
  }
#else
  uint32_t i = 0;
  for(; i < count; ++i)
  {
    ALIGN16(double temp[2]);
    temp[0] = plug.elementByLogicalIndex(i).child(0).asDouble();
    temp[1] = plug.elementByLogicalIndex(i).child(1).asDouble();
    storeu2d(values + i * 2, load2d(temp));
  }
#endif

#else
  for(uint32_t i = 0,  j = 0; i < num; ++i, j += 2)
  {
    values[j] = plug.elementByLogicalIndex(i).child(0).asDouble();
    values[j + 1] = plug.elementByLogicalIndex(i).child(1).asDouble();
  }
#endif
  return MS::kSuccess;
}

//----------------------------------------------------------------------------------------------------------------------
MStatus DgNodeHelper::getVec2Array(MObject node, MObject attribute, float* const values, const size_t count)
{
  MPlug plug(node, attribute);
  if(!plug || !plug.isArray())
    return MS::kFailure;

  uint32_t num = plug.numElements();
  if(num != count)
  {
    MGlobal::displayError("array is sized incorrectly");
    return MS::kFailure;
  }

#if AL_MAYA_ENABLE_SIMD

#ifdef __AVX__
  uint32_t count4 = count & ~0x3;
  uint32_t i = 0;
  for(; i < count4; i += 4)
  {
    ALIGN32(float temp[8]);
    temp[0] = plug.elementByLogicalIndex(i).child(0).asFloat();
    temp[1] = plug.elementByLogicalIndex(i).child(1).asFloat();
    temp[2] = plug.elementByLogicalIndex(i + 1).child(0).asFloat();
    temp[3] = plug.elementByLogicalIndex(i + 1).child(1).asFloat();
    temp[4] = plug.elementByLogicalIndex(i + 2).child(0).asFloat();
    temp[5] = plug.elementByLogicalIndex(i + 2).child(1).asFloat();
    temp[6] = plug.elementByLogicalIndex(i + 3).child(0).asFloat();
    temp[7] = plug.elementByLogicalIndex(i + 3).child(1).asFloat();
    storeu8f(values + i * 2, load8f(temp));
  }

  if(count & 0x2)
  {
    ALIGN16(float temp[4]);
    temp[0] = plug.elementByLogicalIndex(i).child(0).asFloat();
    temp[1] = plug.elementByLogicalIndex(i).child(1).asFloat();
    temp[2] = plug.elementByLogicalIndex(i + 1).child(0).asFloat();
    temp[3] = plug.elementByLogicalIndex(i + 1).child(1).asFloat();
    storeu4f(values + i * 2, load4f(temp));
    i += 2;
  }

#else
  uint32_t count2 = count & ~0x1;
  uint32_t i = 0;
  for(; i < count2; i += 2)
  {
    ALIGN16(float temp[4]);
    temp[0] = plug.elementByLogicalIndex(i).child(0).asFloat();
    temp[1] = plug.elementByLogicalIndex(i).child(1).asFloat();
    temp[2] = plug.elementByLogicalIndex(i + 1).child(0).asFloat();
    temp[3] = plug.elementByLogicalIndex(i + 1).child(1).asFloat();
    storeu4f(values + i * 2, load4f(temp));
  }

#endif

  switch(count & 1)
  {
  case 1: values[i * 2] = plug.elementByLogicalIndex(i).child(0).asFloat();
          values[i * 2 + 1] = plug.elementByLogicalIndex(i).child(1).asFloat();
  default: break;
  }

#else
  for(uint32_t i = 0,  j = 0; i < num; ++i, j += 2)
  {
    values[j] = plug.elementByLogicalIndex(i).child(0).asFloat();
    values[j + 1] = plug.elementByLogicalIndex(i).child(1).asFloat();
  }
#endif
  return MS::kSuccess;
}


//----------------------------------------------------------------------------------------------------------------------
MStatus DgNodeHelper::getVec2Array(MObject node, MObject attribute, GfHalf* const values, const size_t count)
{
  MPlug plug(node, attribute);
  if(!plug || !plug.isArray())
    return MS::kFailure;

  uint32_t num = plug.numElements();
  if(num != count)
  {
    MGlobal::displayError("array is sized incorrectly");
    return MS::kFailure;
  }

  for(uint32_t i = 0,  j = 0; i < num; ++i, j += 2)
  {
    values[j] = plug.elementByLogicalIndex(i).child(0).asFloat();
    values[j + 1] = plug.elementByLogicalIndex(i).child(1).asFloat();
  }


  size_t count4 = count & ~0x3FULL;
  for(uint32_t i = 0, j = 0; i < count4; i += 4, j += 8)
  {
    float f[8];
    auto v0 = plug.elementByLogicalIndex(i + 0);
    auto v1 = plug.elementByLogicalIndex(i + 1);
    auto v2 = plug.elementByLogicalIndex(i + 2);
    auto v3 = plug.elementByLogicalIndex(i + 3);
    f[0] = v0.child(0).asFloat();
    f[1] = v0.child(1).asFloat();
    f[2] = v1.child(0).asFloat();
    f[3] = v1.child(1).asFloat();
    f[4] = v2.child(0).asFloat();
    f[5] = v2.child(1).asFloat();
    f[6] = v3.child(0).asFloat();
    f[7] = v3.child(1).asFloat();
    float2half_8f(f, values + j);
  }

  if(count & 0x2)
  {
    float f[4];
    auto v0 = plug.elementByLogicalIndex(count4 + 0);
    auto v1 = plug.elementByLogicalIndex(count4 + 1);
    f[0] = v0.child(0).asFloat();
    f[1] = v0.child(1).asFloat();
    f[2] = v1.child(0).asFloat();
    f[3] = v1.child(1).asFloat();
    float2half_4f(f, values + count4 * 2);
    count4 += 2;
  }

  if(count & 0x1)
  {
    auto v = plug.elementByLogicalIndex(count4);
    values[count4 * 2 + 0] = float2half_1f(v.child(0).asFloat());
    values[count4 * 2 + 1] = float2half_1f(v.child(1).asFloat());
  }

  return MS::kSuccess;
}

//----------------------------------------------------------------------------------------------------------------------
MStatus DgNodeHelper::getVec2Array(MObject node, MObject attribute, int32_t* const values, const size_t count)
{
  MPlug plug(node, attribute);
  if(!plug || !plug.isArray())
    return MS::kFailure;

  uint32_t num = plug.numElements();
  if(num != count)
  {
    MGlobal::displayError("array is sized incorrectly");
    return MS::kFailure;
  }

#if AL_MAYA_ENABLE_SIMD

#ifdef __AVX__
  uint32_t count4 = count & ~0x3;
  uint32_t i = 0;
  for(; i < count4; i += 4)
  {
    ALIGN32(int32_t temp[8]);
    temp[0] = plug.elementByLogicalIndex(i).child(0).asInt();
    temp[1] = plug.elementByLogicalIndex(i).child(1).asInt();
    temp[2] = plug.elementByLogicalIndex(i + 1).child(0).asInt();
    temp[3] = plug.elementByLogicalIndex(i + 1).child(1).asInt();
    temp[4] = plug.elementByLogicalIndex(i + 2).child(0).asInt();
    temp[5] = plug.elementByLogicalIndex(i + 2).child(1).asInt();
    temp[6] = plug.elementByLogicalIndex(i + 3).child(0).asInt();
    temp[7] = plug.elementByLogicalIndex(i + 3).child(1).asInt();
    storeu8i(values + i * 2, load8i(temp));
  }

  if(count & 0x2)
  {
    ALIGN16(int32_t temp[4]);
    temp[0] = plug.elementByLogicalIndex(i).child(0).asInt();
    temp[1] = plug.elementByLogicalIndex(i).child(1).asInt();
    temp[2] = plug.elementByLogicalIndex(i + 1).child(0).asInt();
    temp[3] = plug.elementByLogicalIndex(i + 1).child(1).asInt();
    storeu4i(values + i * 2, load4i(temp));
    i += 2;
  }

#else
  uint32_t count2 = count & ~0x1;
  uint32_t i = 0;
  for(; i < count2; i += 2)
  {
    ALIGN16(int32_t temp[4]);
    temp[0] = plug.elementByLogicalIndex(i).child(0).asInt();
    temp[1] = plug.elementByLogicalIndex(i).child(1).asInt();
    temp[2] = plug.elementByLogicalIndex(i + 1).child(0).asInt();
    temp[3] = plug.elementByLogicalIndex(i + 1).child(1).asInt();
    storeu4i(values + i * 2, load4i(temp));
  }

#endif

  if(count & 1)
  {
    values[i * 2] = plug.elementByLogicalIndex(i).child(0).asInt();
    values[i * 2 + 1] = plug.elementByLogicalIndex(i).child(1).asInt();
  }

#else
  for(uint32_t i = 0,  j = 0; i < num; ++i, j += 2)
  {
    values[j] = plug.elementByLogicalIndex(i).child(0).asInt();
    values[j + 1] = plug.elementByLogicalIndex(i).child(1).asInt();
  }
#endif
  return MS::kSuccess;
}

//----------------------------------------------------------------------------------------------------------------------
MStatus DgNodeHelper::getVec3Array(MObject node, MObject attribute, float* const values, const size_t count)
{
  MPlug plug(node, attribute);
  if(!plug || !plug.isArray())
    return MS::kFailure;

  uint32_t num = plug.numElements();
  if(num != count)
  {
    MGlobal::displayError("array is sized incorrectly");
    return MS::kFailure;
  }

#if AL_MAYA_ENABLE_SIMD

#ifdef __AVX__
  uint32_t count8 = count & ~0x7;
  uint32_t i = 0;
  for(; i < count8; i += 8)
  {
    ALIGN32(float temp[24]);
    const MPlug c0 = plug.elementByLogicalIndex(i    );
    const MPlug c1 = plug.elementByLogicalIndex(i + 1);
    const MPlug c2 = plug.elementByLogicalIndex(i + 2);
    const MPlug c3 = plug.elementByLogicalIndex(i + 3);
    const MPlug c4 = plug.elementByLogicalIndex(i + 4);
    const MPlug c5 = plug.elementByLogicalIndex(i + 5);
    const MPlug c6 = plug.elementByLogicalIndex(i + 6);
    const MPlug c7 = plug.elementByLogicalIndex(i + 7);
    temp[ 0] = c0.child(0).asFloat();
    temp[ 1] = c0.child(1).asFloat();
    temp[ 2] = c0.child(2).asFloat();
    temp[ 3] = c1.child(0).asFloat();
    temp[ 4] = c1.child(1).asFloat();
    temp[ 5] = c1.child(2).asFloat();
    temp[ 6] = c2.child(0).asFloat();
    temp[ 7] = c2.child(1).asFloat();
    temp[ 8] = c2.child(2).asFloat();
    temp[ 9] = c3.child(0).asFloat();
    temp[10] = c3.child(1).asFloat();
    temp[11] = c3.child(2).asFloat();
    temp[12] = c4.child(0).asFloat();
    temp[13] = c4.child(1).asFloat();
    temp[14] = c4.child(2).asFloat();
    temp[15] = c5.child(0).asFloat();
    temp[16] = c5.child(1).asFloat();
    temp[17] = c5.child(2).asFloat();
    temp[18] = c6.child(0).asFloat();
    temp[19] = c6.child(1).asFloat();
    temp[20] = c6.child(2).asFloat();
    temp[21] = c7.child(0).asFloat();
    temp[22] = c7.child(1).asFloat();
    temp[23] = c7.child(2).asFloat();
    float* optr = values + (i * 3);
    storeu8f(optr, load8f(temp));
    storeu8f(optr + 8, load8f(temp + 8));
    storeu8f(optr + 16, load8f(temp + 16));
  }

  if(count & 0x4)
  {
    ALIGN32(float temp[12]);
    const MPlug c0 = plug.elementByLogicalIndex(i    );
    const MPlug c1 = plug.elementByLogicalIndex(i + 1);
    const MPlug c2 = plug.elementByLogicalIndex(i + 2);
    const MPlug c3 = plug.elementByLogicalIndex(i + 3);
    temp[ 0] = c0.child(0).asFloat();
    temp[ 1] = c0.child(1).asFloat();
    temp[ 2] = c0.child(2).asFloat();
    temp[ 3] = c1.child(0).asFloat();
    temp[ 4] = c1.child(1).asFloat();
    temp[ 5] = c1.child(2).asFloat();
    temp[ 6] = c2.child(0).asFloat();
    temp[ 7] = c2.child(1).asFloat();
    temp[ 8] = c2.child(2).asFloat();
    temp[ 9] = c3.child(0).asFloat();
    temp[10] = c3.child(1).asFloat();
    temp[11] = c3.child(2).asFloat();
    float* optr = values + (i * 3);
    storeu8f(optr, load8f(temp));
    storeu4f(optr + 8, load4f(temp + 8));
    i += 4;
  }

#else
  uint32_t count4 = count & ~0x3;
  uint32_t i = 0;
  for(; i < count4; i += 4)
  {
    ALIGN16(float temp[12]);
    const MPlug c0 = plug.elementByLogicalIndex(i    );
    const MPlug c1 = plug.elementByLogicalIndex(i + 1);
    const MPlug c2 = plug.elementByLogicalIndex(i + 2);
    const MPlug c3 = plug.elementByLogicalIndex(i + 3);
    temp[ 0] = c0.child(0).asFloat();
    temp[ 1] = c0.child(1).asFloat();
    temp[ 2] = c0.child(2).asFloat();
    temp[ 3] = c1.child(0).asFloat();
    temp[ 4] = c1.child(1).asFloat();
    temp[ 5] = c1.child(2).asFloat();
    temp[ 6] = c2.child(0).asFloat();
    temp[ 7] = c2.child(1).asFloat();
    temp[ 8] = c2.child(2).asFloat();
    temp[ 9] = c3.child(0).asFloat();
    temp[10] = c3.child(1).asFloat();
    temp[11] = c3.child(2).asFloat();
    float* optr = values + (i * 3);
    storeu4f(optr + 0, load4f(temp + 0));
    storeu4f(optr + 4, load4f(temp + 4));
    storeu4f(optr + 8, load4f(temp + 8));
  }

#endif

  float* optr = values + (i * 3);
  MPlug elem;
  switch(count & 3)
  {
  case 3: elem = plug.elementByLogicalIndex(i + 2);
          optr[6] = elem.child(0).asFloat();
          optr[7] = elem.child(1).asFloat();
          optr[8] = elem.child(2).asFloat();
  case 2: elem = plug.elementByLogicalIndex(i + 1);
          optr[3] = elem.child(0).asFloat();
          optr[4] = elem.child(1).asFloat();
          optr[5] = elem.child(2).asFloat();
  case 1: elem = plug.elementByLogicalIndex(i);
          optr[0] = elem.child(0).asFloat();
          optr[1] = elem.child(1).asFloat();
          optr[2] = elem.child(2).asFloat();
  default: break;
  }

#else
  for(uint32_t i = 0, j = 0; i < num; ++i, j += 3)
  {
    MPlug elem = plug.elementByLogicalIndex(i);
    values[j    ] = elem.child(0).asFloat();
    values[j + 1] = elem.child(1).asFloat();
    values[j + 2] = elem.child(2).asFloat();
  }
#endif
  return MS::kSuccess;
}

//----------------------------------------------------------------------------------------------------------------------
MStatus DgNodeHelper::getVec3Array(MObject node, MObject attribute, double* const values, const size_t count)
{
  MPlug plug(node, attribute);
  if(!plug || !plug.isArray())
    return MS::kFailure;

  uint32_t num = plug.numElements();
  if(num != count)
  {
    MGlobal::displayError("array is sized incorrectly");
    return MS::kFailure;
  }

#if AL_MAYA_ENABLE_SIMD

#ifdef __AVX__
  uint32_t count4 = count & ~0x3;
  uint32_t i = 0;
  for(; i < count4; i += 4)
  {
    ALIGN32(double temp[12]);
    const MPlug c0 = plug.elementByLogicalIndex(i    );
    const MPlug c1 = plug.elementByLogicalIndex(i + 1);
    const MPlug c2 = plug.elementByLogicalIndex(i + 2);
    const MPlug c3 = plug.elementByLogicalIndex(i + 3);
    temp[ 0] = c0.child(0).asDouble();
    temp[ 1] = c0.child(1).asDouble();
    temp[ 2] = c0.child(2).asDouble();
    temp[ 3] = c1.child(0).asDouble();
    temp[ 4] = c1.child(1).asDouble();
    temp[ 5] = c1.child(2).asDouble();
    temp[ 6] = c2.child(0).asDouble();
    temp[ 7] = c2.child(1).asDouble();
    temp[ 8] = c2.child(2).asDouble();
    temp[ 9] = c3.child(0).asDouble();
    temp[10] = c3.child(1).asDouble();
    temp[11] = c3.child(2).asDouble();
    double* optr = values + (i * 3);
    storeu4d(optr, load4d(temp));
    storeu4d(optr + 4, load4d(temp + 4));
    storeu4d(optr + 8, load4d(temp + 8));
  }

  if(count & 0x2)
  {
    ALIGN32(double temp[6]);
    const MPlug c0 = plug.elementByLogicalIndex(i    );
    const MPlug c1 = plug.elementByLogicalIndex(i + 1);
    temp[ 0] = c0.child(0).asDouble();
    temp[ 1] = c0.child(1).asDouble();
    temp[ 2] = c0.child(2).asDouble();
    temp[ 3] = c1.child(0).asDouble();
    temp[ 4] = c1.child(1).asDouble();
    temp[ 5] = c1.child(2).asDouble();
    double* optr = values + (i * 3);
    storeu4d(optr, load4d(temp));
    storeu2d(optr + 4, load2d(temp + 4));
    i += 2;
  }

#else
  uint32_t count2 = count & ~0x1;
  uint32_t i = 0;
  for(; i < count2; i += 2)
  {
    ALIGN16(double temp[6]);
    const MPlug c0 = plug.elementByLogicalIndex(i    );
    const MPlug c1 = plug.elementByLogicalIndex(i + 1);
    temp[ 0] = c0.child(0).asDouble();
    temp[ 1] = c0.child(1).asDouble();
    temp[ 2] = c0.child(2).asDouble();
    temp[ 3] = c1.child(0).asDouble();
    temp[ 4] = c1.child(1).asDouble();
    temp[ 5] = c1.child(2).asDouble();
    double* optr = values + (i * 3);
    storeu2d(optr + 0, load2d(temp + 0));
    storeu2d(optr + 2, load2d(temp + 2));
    storeu2d(optr + 4, load2d(temp + 4));
  }

#endif

  if(count & 1)
  {
    double* optr = values + (i * 3);
    MPlug elem = plug.elementByLogicalIndex(i);
    optr[0] = elem.child(0).asDouble();
    optr[1] = elem.child(1).asDouble();
    optr[2] = elem.child(2).asDouble();
  }

#else
  for(uint32_t i = 0, j = 0; i < num; ++i, j += 3)
  {
    MPlug elem = plug.elementByLogicalIndex(i);
    values[j    ] = elem.child(0).asFloat();
    values[j + 1] = elem.child(1).asFloat();
    values[j + 2] = elem.child(2).asFloat();
  }
#endif
  return MS::kSuccess;
}

//----------------------------------------------------------------------------------------------------------------------
MStatus DgNodeHelper::getVec3Array(MObject node, MObject attribute, GfHalf* const values, const size_t count)
{
  MPlug plug(node, attribute);
  if(!plug || !plug.isArray())
    return MS::kFailure;

  uint32_t num = plug.numElements();
  if(num != count)
  {
    MGlobal::displayError("array is sized incorrectly");
    return MS::kFailure;
  }

  size_t count8 = count & ~0x7ULL;

  for(uint32_t i = 0, j = 0; i < count8; i += 8, j += 24)
  {
    float r[24];
    MPlug v0 = plug.elementByLogicalIndex(i + 0);
    MPlug v1 = plug.elementByLogicalIndex(i + 1);
    MPlug v2 = plug.elementByLogicalIndex(i + 2);
    MPlug v3 = plug.elementByLogicalIndex(i + 3);
    MPlug v4 = plug.elementByLogicalIndex(i + 4);
    MPlug v5 = plug.elementByLogicalIndex(i + 5);
    MPlug v6 = plug.elementByLogicalIndex(i + 6);
    MPlug v7 = plug.elementByLogicalIndex(i + 7);
    r[ 0] = v0.child(0).asFloat();
    r[ 1] = v0.child(1).asFloat();
    r[ 2] = v0.child(2).asFloat();
    r[ 3] = v1.child(0).asFloat();
    r[ 4] = v1.child(1).asFloat();
    r[ 5] = v1.child(2).asFloat();
    r[ 6] = v2.child(0).asFloat();
    r[ 7] = v2.child(1).asFloat();
    r[ 8] = v2.child(2).asFloat();
    r[ 9] = v3.child(0).asFloat();
    r[10] = v3.child(1).asFloat();
    r[11] = v3.child(2).asFloat();
    r[12] = v4.child(0).asFloat();
    r[13] = v4.child(1).asFloat();
    r[14] = v4.child(2).asFloat();
    r[15] = v5.child(0).asFloat();
    r[16] = v5.child(1).asFloat();
    r[17] = v5.child(2).asFloat();
    r[18] = v6.child(0).asFloat();
    r[19] = v6.child(1).asFloat();
    r[20] = v6.child(2).asFloat();
    r[21] = v7.child(0).asFloat();
    r[22] = v7.child(1).asFloat();
    r[23] = v7.child(2).asFloat();
    float2half_8f(r, values + j);
    float2half_8f(r + 8, values + j + 8);
    float2half_8f(r + 16, values + j + 16);
  }

  for(uint32_t i = count8, j = count8 * 3; i < count; ++i, j += 3)
  {
    float v[4];
    GfHalf h[4];
    MPlug elem = plug.elementByLogicalIndex(i);
    v[0] = elem.child(0).asFloat();
    v[1] = elem.child(1).asFloat();
    v[2] = elem.child(2).asFloat();
    float2half_4f(v, h);
    values[j + 0] = h[0];
    values[j + 1] = h[1];
    values[j + 2] = h[2];
  }

  return MS::kSuccess;
}

//----------------------------------------------------------------------------------------------------------------------
MStatus DgNodeHelper::getVec3Array(MObject node, MObject attribute, int32_t* const values, const size_t count)
{
  MPlug plug(node, attribute);
  if(!plug || !plug.isArray())
    return MS::kFailure;

  uint32_t num = plug.numElements();
  if(num != count)
  {
    MGlobal::displayError("array is sized incorrectly");
    return MS::kFailure;
  }

#if AL_MAYA_ENABLE_SIMD

#ifdef __AVX__
  uint32_t count8 = count & ~0x7;
  uint32_t i = 0;
  for(; i < count8; i += 8)
  {
    ALIGN32(int32_t temp[24]);
    const MPlug c0 = plug.elementByLogicalIndex(i    );
    const MPlug c1 = plug.elementByLogicalIndex(i + 1);
    const MPlug c2 = plug.elementByLogicalIndex(i + 2);
    const MPlug c3 = plug.elementByLogicalIndex(i + 3);
    const MPlug c4 = plug.elementByLogicalIndex(i + 4);
    const MPlug c5 = plug.elementByLogicalIndex(i + 5);
    const MPlug c6 = plug.elementByLogicalIndex(i + 6);
    const MPlug c7 = plug.elementByLogicalIndex(i + 7);
    temp[ 0] = c0.child(0).asInt();
    temp[ 1] = c0.child(1).asInt();
    temp[ 2] = c0.child(2).asInt();
    temp[ 3] = c1.child(0).asInt();
    temp[ 4] = c1.child(1).asInt();
    temp[ 5] = c1.child(2).asInt();
    temp[ 6] = c2.child(0).asInt();
    temp[ 7] = c2.child(1).asInt();
    temp[ 8] = c2.child(2).asInt();
    temp[ 9] = c3.child(0).asInt();
    temp[10] = c3.child(1).asInt();
    temp[11] = c3.child(2).asInt();
    temp[12] = c4.child(0).asInt();
    temp[13] = c4.child(1).asInt();
    temp[14] = c4.child(2).asInt();
    temp[15] = c5.child(0).asInt();
    temp[16] = c5.child(1).asInt();
    temp[17] = c5.child(2).asInt();
    temp[18] = c6.child(0).asInt();
    temp[19] = c6.child(1).asInt();
    temp[20] = c6.child(2).asInt();
    temp[21] = c7.child(0).asInt();
    temp[22] = c7.child(1).asInt();
    temp[23] = c7.child(2).asInt();
    int32_t* optr = values + (i * 3);
    storeu8i(optr + 0, load8i(temp + 0));
    storeu8i(optr + 8, load8i(temp + 8));
    storeu8i(optr + 16, load8i(temp + 16));
  }

  if(count & 0x4)
  {
    ALIGN32(int32_t temp[12]);
    const MPlug c0 = plug.elementByLogicalIndex(i    );
    const MPlug c1 = plug.elementByLogicalIndex(i + 1);
    const MPlug c2 = plug.elementByLogicalIndex(i + 2);
    const MPlug c3 = plug.elementByLogicalIndex(i + 3);
    temp[ 0] = c0.child(0).asInt();
    temp[ 1] = c0.child(1).asInt();
    temp[ 2] = c0.child(2).asInt();
    temp[ 3] = c1.child(0).asInt();
    temp[ 4] = c1.child(1).asInt();
    temp[ 5] = c1.child(2).asInt();
    temp[ 6] = c2.child(0).asInt();
    temp[ 7] = c2.child(1).asInt();
    temp[ 8] = c2.child(2).asInt();
    temp[ 9] = c3.child(0).asInt();
    temp[10] = c3.child(1).asInt();
    temp[11] = c3.child(2).asInt();
    int32_t* optr = values + (i * 3);
    storeu8i(optr + 0, load8i(temp + 0));
    storeu4i(optr + 8, load4i(temp + 8));
    i += 4;
  }

#else
  uint32_t count4 = count & ~0x3;
  uint32_t i = 0;
  for(; i < count4; i += 4)
  {
    ALIGN16(int32_t temp[12]);
    const MPlug c0 = plug.elementByLogicalIndex(i    );
    const MPlug c1 = plug.elementByLogicalIndex(i + 1);
    const MPlug c2 = plug.elementByLogicalIndex(i + 2);
    const MPlug c3 = plug.elementByLogicalIndex(i + 3);
    temp[ 0] = c0.child(0).asInt();
    temp[ 1] = c0.child(1).asInt();
    temp[ 2] = c0.child(2).asInt();
    temp[ 3] = c1.child(0).asInt();
    temp[ 4] = c1.child(1).asInt();
    temp[ 5] = c1.child(2).asInt();
    temp[ 6] = c2.child(0).asInt();
    temp[ 7] = c2.child(1).asInt();
    temp[ 8] = c2.child(2).asInt();
    temp[ 9] = c3.child(0).asInt();
    temp[10] = c3.child(1).asInt();
    temp[11] = c3.child(2).asInt();
    int32_t* optr = values + (i * 3);
    storeu4i(optr + 0, load4i(temp + 0));
    storeu4i(optr + 4, load4i(temp + 4));
    storeu4i(optr + 8, load4i(temp + 8));
  }

#endif

  int32_t* optr = values + (i * 3);
  MPlug elem;
  switch(count & 3)
  {
  case 3: elem = plug.elementByLogicalIndex(i + 2);
          optr[6] = elem.child(0).asInt();
          optr[7] = elem.child(1).asInt();
          optr[8] = elem.child(2).asInt();
  case 2: elem = plug.elementByLogicalIndex(i + 1);
          optr[3] = elem.child(0).asInt();
          optr[4] = elem.child(1).asInt();
          optr[5] = elem.child(2).asInt();
  case 1: elem = plug.elementByLogicalIndex(i);
          optr[0] = elem.child(0).asInt();
          optr[1] = elem.child(1).asInt();
          optr[2] = elem.child(2).asInt();
  default: break;
  }

#else
  for(uint32_t i = 0, j = 0; i < num; ++i, j += 3)
  {
    MPlug elem = plug.elementByLogicalIndex(i);
    values[j    ] = elem.child(0).asInt();
    values[j + 1] = elem.child(1).asInt();
    values[j + 2] = elem.child(2).asInt();
  }
#endif
  return MS::kSuccess;
}

//----------------------------------------------------------------------------------------------------------------------
MStatus DgNodeHelper::getVec4Array(MObject node, MObject attribute, int32_t* const values, const size_t count)
{
  MPlug plug(node, attribute);
  if(!plug || !plug.isArray())
    return MS::kFailure;

  uint32_t num = plug.numElements();
  if(num != count)
  {
    MGlobal::displayError("array is sized incorrectly");
    return MS::kFailure;
  }

  size_t count2 = count & ~1;
  size_t i = 0, j = 0;
  for(; i < count2; i += 2, j += 8)
  {
    ALIGN32(int32_t temp[8]);
    MPlug elem0 = plug.elementByLogicalIndex(i);
    MPlug elem1 = plug.elementByLogicalIndex(i + 1);

    temp[0] = elem0.child(0).asInt();
    temp[1] = elem0.child(1).asInt();
    temp[2] = elem0.child(2).asInt();
    temp[3] = elem0.child(3).asInt();
    temp[4] = elem1.child(0).asInt();
    temp[5] = elem1.child(1).asInt();
    temp[6] = elem1.child(2).asInt();
    temp[7] = elem1.child(3).asInt();

    #if AL_MAYA_ENABLE_SIMD
    # ifdef __AVX__
    storeu8i(values + j, load8i(temp));
    # else
    storeu4i(values + j, load4i(temp));
    storeu4i(values + j + 4, load4i(temp + 4));
    # endif
    #else
    values[j + 0] = temp[0];
    values[j + 1] = temp[1];
    values[j + 2] = temp[2];
    values[j + 3] = temp[3];
    values[j + 4] = temp[4];
    values[j + 5] = temp[5];
    values[j + 6] = temp[6];
    values[j + 7] = temp[7];
    #endif
  }

  if(count & 1)
  {
    ALIGN16(int32_t temp[4]);
    MPlug elem0 = plug.elementByLogicalIndex(i);
    temp[0] = elem0.child(0).asInt();
    temp[1] = elem0.child(1).asInt();
    temp[2] = elem0.child(2).asInt();
    temp[3] = elem0.child(3).asInt();

    #if AL_MAYA_ENABLE_SIMD
    storeu4i(values + j, load4i(temp));
    #else
    values[j + 0] = temp[0];
    values[j + 1] = temp[1];
    values[j + 2] = temp[2];
    values[j + 3] = temp[3];
    #endif
  }

  return MS::kSuccess;
}

//----------------------------------------------------------------------------------------------------------------------
MStatus DgNodeHelper::getVec4Array(MObject node, MObject attribute, float* const values, const size_t count)
{
  MPlug plug(node, attribute);
  if(!plug || !plug.isArray())
    return MS::kFailure;

  uint32_t num = plug.numElements();
  if(num != count)
  {
    MGlobal::displayError("array is sized incorrectly");
    return MS::kFailure;
  }

  size_t count2 = count & ~1;
  size_t i = 0, j = 0;
  for(; i < count2; i += 2, j += 8)
  {
    ALIGN32(float temp[8]);
    MPlug elem0 = plug.elementByLogicalIndex(i);
    MPlug elem1 = plug.elementByLogicalIndex(i + 1);

    temp[0] = elem0.child(0).asFloat();
    temp[1] = elem0.child(1).asFloat();
    temp[2] = elem0.child(2).asFloat();
    temp[3] = elem0.child(3).asFloat();
    temp[4] = elem1.child(0).asFloat();
    temp[5] = elem1.child(1).asFloat();
    temp[6] = elem1.child(2).asFloat();
    temp[7] = elem1.child(3).asFloat();

    #if AL_MAYA_ENABLE_SIMD
    # ifdef __AVX__
    storeu8f(values + j, load8f(temp));
    # else
    storeu4f(values + j, load4f(temp));
    storeu4f(values + j + 4, load4f(temp + 4));
    # endif
    #else
    values[j + 0] = temp[0];
    values[j + 1] = temp[1];
    values[j + 2] = temp[2];
    values[j + 3] = temp[3];
    values[j + 4] = temp[4];
    values[j + 5] = temp[5];
    values[j + 6] = temp[6];
    values[j + 7] = temp[7];
    #endif
  }

  if(count & 1)
  {
    ALIGN16(float temp[4]);
    MPlug elem0 = plug.elementByLogicalIndex(i);
    temp[0] = elem0.child(0).asFloat();
    temp[1] = elem0.child(1).asFloat();
    temp[2] = elem0.child(2).asFloat();
    temp[3] = elem0.child(3).asFloat();

    #if AL_MAYA_ENABLE_SIMD
    storeu4f(values + j, load4f(temp));
    #else
    values[j + 0] = temp[0];
    values[j + 1] = temp[1];
    values[j + 2] = temp[2];
    values[j + 3] = temp[3];
    #endif
  }

  return MS::kSuccess;
}

//----------------------------------------------------------------------------------------------------------------------
MStatus DgNodeHelper::getVec4Array(MObject node, MObject attribute, double* const values, const size_t count)
{
  MPlug plug(node, attribute);
  if(!plug || !plug.isArray())
    return MS::kFailure;

  uint32_t num = plug.numElements();
  if(num != count)
  {
    MGlobal::displayError("array is sized incorrectly");
    return MS::kFailure;
  }

  for(uint32_t i = 0, j = 0; i < num; ++i, j += 4)
  {
    ALIGN32(double temp[4]);
    MPlug elem = plug.elementByLogicalIndex(i);

    temp[0] = elem.child(0).asDouble();
    temp[1] = elem.child(1).asDouble();
    temp[2] = elem.child(2).asDouble();
    temp[3] = elem.child(3).asDouble();

    #if AL_MAYA_ENABLE_SIMD
    # ifdef __AVX__
    storeu4d(values + j, load4d(temp));
    # else
    storeu2d(values + j, load2d(temp));
    storeu2d(values + j + 2, load2d(temp + 2));
    # endif
    #else
    values[j + 0] = temp[0];
    values[j + 1] = temp[1];
    values[j + 2] = temp[2];
    values[j + 3] = temp[3];
    #endif
  }

  return MS::kSuccess;
}

//----------------------------------------------------------------------------------------------------------------------
MStatus DgNodeHelper::getVec4Array(MObject node, MObject attribute, GfHalf* const values, const size_t count)
{
  MPlug plug(node, attribute);
  if(!plug || !plug.isArray())
    return MS::kFailure;

  uint32_t num = plug.numElements();
  if(num != count)
  {
    MGlobal::displayError("array is sized incorrectly");
    return MS::kFailure;
  }

  size_t count2 = count & ~0x1ULL;
  for(uint32_t i = 0, j = 0; i < count2; i += 2, j += 8)
  {
    MPlug v0 = plug.elementByLogicalIndex(i + 0);
    MPlug v1 = plug.elementByLogicalIndex(i + 1);
    float f[8];
    f[0] = v0.child(0).asFloat();
    f[1] = v0.child(1).asFloat();
    f[2] = v0.child(2).asFloat();
    f[3] = v0.child(3).asFloat();
    f[4] = v1.child(0).asFloat();
    f[5] = v1.child(1).asFloat();
    f[6] = v1.child(2).asFloat();
    f[7] = v1.child(3).asFloat();
    float2half_8f(f, values + j);
  }
  if(count & 0x1)
  {
    MPlug v0 = plug.elementByLogicalIndex(count2);
    float f[4];
    f[0] = v0.child(0).asFloat();
    f[1] = v0.child(1).asFloat();
    f[2] = v0.child(2).asFloat();
    f[3] = v0.child(3).asFloat();
    float2half_4f(f, values + count2 * 4);
  }

  return MS::kSuccess;
}

//----------------------------------------------------------------------------------------------------------------------
MStatus DgNodeHelper::getQuatArray(MObject node, MObject attr, GfHalf* const values, const size_t count)
{
  return getVec4Array(node, attr, values, count);
}

//----------------------------------------------------------------------------------------------------------------------
MStatus DgNodeHelper::getQuatArray(MObject node, MObject attr, float* const values, const size_t count)
{
  return getVec4Array(node, attr, values, count);
}

//----------------------------------------------------------------------------------------------------------------------
MStatus DgNodeHelper::getQuatArray(MObject node, MObject attr, double* const values, const size_t count)
{
  return getVec4Array(node, attr, values, count);
}

//----------------------------------------------------------------------------------------------------------------------
MStatus DgNodeHelper::getMatrix2x2Array(MObject node, MObject attribute, double* const values, const size_t count)
{
  const char* const errorString = "getMatrix2x2Array error";
  MPlug arrayPlug(node, attribute);
  for(uint32_t i = 0; i < count; ++i)
  {
    double* const str = values + i * 4;
    MPlug plug = arrayPlug.elementByLogicalIndex(i);
    AL_MAYA_CHECK_ERROR(plug.child(0).child(0).getValue(str[0]), errorString);
    AL_MAYA_CHECK_ERROR(plug.child(0).child(1).getValue(str[1]), errorString);
    AL_MAYA_CHECK_ERROR(plug.child(1).child(0).getValue(str[2]), errorString);
    AL_MAYA_CHECK_ERROR(plug.child(1).child(1).getValue(str[3]), errorString);
  }
  return MS::kSuccess;
}

//----------------------------------------------------------------------------------------------------------------------
MStatus DgNodeHelper::getMatrix2x2Array(MObject node, MObject attribute, float* const values, const size_t count)
{
  const char* const errorString = "getMatrix2x2Array error";
  MPlug arrayPlug(node, attribute);
  for(uint32_t i = 0; i < count; ++i)
  {
    float* const str = values + i * 4;
    MPlug plug = arrayPlug.elementByLogicalIndex(i);
    AL_MAYA_CHECK_ERROR(plug.child(0).child(0).getValue(str[0]), errorString);
    AL_MAYA_CHECK_ERROR(plug.child(0).child(1).getValue(str[1]), errorString);
    AL_MAYA_CHECK_ERROR(plug.child(1).child(0).getValue(str[2]), errorString);
    AL_MAYA_CHECK_ERROR(plug.child(1).child(1).getValue(str[3]), errorString);
  }
  return MS::kSuccess;
}

//----------------------------------------------------------------------------------------------------------------------
MStatus DgNodeHelper::getMatrix3x3Array(MObject node, MObject attribute, double* const values, const size_t count)
{
  const char* const errorString = "getMatrix3x3Array error";
  MPlug arrayPlug(node, attribute);
  for(uint32_t i = 0; i < count; ++i)
  {
    double* const str = values + i * 9;
    MPlug plug = arrayPlug.elementByLogicalIndex(i);
    AL_MAYA_CHECK_ERROR(plug.child(0).child(0).getValue(str[0]), errorString);
    AL_MAYA_CHECK_ERROR(plug.child(0).child(1).getValue(str[1]), errorString);
    AL_MAYA_CHECK_ERROR(plug.child(0).child(2).getValue(str[2]), errorString);
    AL_MAYA_CHECK_ERROR(plug.child(1).child(0).getValue(str[3]), errorString);
    AL_MAYA_CHECK_ERROR(plug.child(1).child(1).getValue(str[4]), errorString);
    AL_MAYA_CHECK_ERROR(plug.child(1).child(2).getValue(str[5]), errorString);
    AL_MAYA_CHECK_ERROR(plug.child(2).child(0).getValue(str[6]), errorString);
    AL_MAYA_CHECK_ERROR(plug.child(2).child(1).getValue(str[7]), errorString);
    AL_MAYA_CHECK_ERROR(plug.child(2).child(2).getValue(str[8]), errorString);
  }
  return MS::kSuccess;
}

//----------------------------------------------------------------------------------------------------------------------
MStatus DgNodeHelper::getMatrix3x3Array(MObject node, MObject attribute, float* const values, const size_t count)
{
  const char* const errorString = "getMatrix3x3Array error";
  MPlug arrayPlug(node, attribute);
  for(uint32_t i = 0; i < count; ++i)
  {
    float* const str = values + i * 9;
    MPlug plug = arrayPlug.elementByLogicalIndex(i);
    AL_MAYA_CHECK_ERROR(plug.child(0).child(0).getValue(str[0]), errorString);
    AL_MAYA_CHECK_ERROR(plug.child(0).child(1).getValue(str[1]), errorString);
    AL_MAYA_CHECK_ERROR(plug.child(0).child(2).getValue(str[2]), errorString);
    AL_MAYA_CHECK_ERROR(plug.child(1).child(0).getValue(str[3]), errorString);
    AL_MAYA_CHECK_ERROR(plug.child(1).child(1).getValue(str[4]), errorString);
    AL_MAYA_CHECK_ERROR(plug.child(1).child(2).getValue(str[5]), errorString);
    AL_MAYA_CHECK_ERROR(plug.child(2).child(0).getValue(str[6]), errorString);
    AL_MAYA_CHECK_ERROR(plug.child(2).child(1).getValue(str[7]), errorString);
    AL_MAYA_CHECK_ERROR(plug.child(2).child(2).getValue(str[8]), errorString);
  }
  return MS::kSuccess;
}


//----------------------------------------------------------------------------------------------------------------------
MStatus DgNodeHelper::getMatrix4x4Array(MObject node, MObject attribute, float* const values, const size_t count)
{
  MPlug plug(node, attribute);
  if(!plug)
    return MS::kFailure;

  if(plug.isArray())
  {
    uint32_t num = plug.numElements();
    if(num != count)
    {
      MGlobal::displayError("array is sized incorrectly");
      return MS::kFailure;
    }

    MFnMatrixData fn;
    MObject elementValue;
    for(uint32_t i = 0, j = 0; i < count; ++i, j += 16)
    {
      plug.elementByLogicalIndex(i).getValue(elementValue);
      fn.setObject(elementValue);
      const MMatrix& m = fn.matrix();

      #if AL_MAYA_ENABLE_SIMD
      # ifdef __AVX__
      const f128 m0 = cvt4d_to_4f(loadu4d(&m.matrix[0][0]));
      const f128 m1 = cvt4d_to_4f(loadu4d(&m.matrix[1][0]));
      const f128 m2 = cvt4d_to_4f(loadu4d(&m.matrix[2][0]));
      const f128 m3 = cvt4d_to_4f(loadu4d(&m.matrix[3][0]));
      # else
      const f128 m0a = cvt2d_to_2f(loadu2d(&m.matrix[0][0]));
      const f128 m0b = cvt2d_to_2f(loadu2d(&m.matrix[0][2]));
      const f128 m1a = cvt2d_to_2f(loadu2d(&m.matrix[1][0]));
      const f128 m1b = cvt2d_to_2f(loadu2d(&m.matrix[1][2]));
      const f128 m2a = cvt2d_to_2f(loadu2d(&m.matrix[2][0]));
      const f128 m2b = cvt2d_to_2f(loadu2d(&m.matrix[2][2]));
      const f128 m3a = cvt2d_to_2f(loadu2d(&m.matrix[3][0]));
      const f128 m3b = cvt2d_to_2f(loadu2d(&m.matrix[3][2]));
      const f128 m0 = movelh4f(m0a, m0b);
      const f128 m1 = movelh4f(m1a, m1b);
      const f128 m2 = movelh4f(m2a, m2b);
      const f128 m3 = movelh4f(m3a, m3b);
      # endif
      float* optr = values + j;
      storeu4f(optr, m0);
      storeu4f(optr + 4, m1);
      storeu4f(optr + 8, m2);
      storeu4f(optr + 12, m3);
      #else
      float* optr = values + j;
      optr[0] = m.matrix[0][0];
      optr[1] = m.matrix[0][1];
      optr[2] = m.matrix[0][2];
      optr[3] = m.matrix[0][3];
      optr[4] = m.matrix[1][0];
      optr[5] = m.matrix[1][1];
      optr[6] = m.matrix[1][2];
      optr[7] = m.matrix[1][3];
      optr[8] = m.matrix[2][0];
      optr[9] = m.matrix[2][1];
      optr[10] = m.matrix[2][2];
      optr[11] = m.matrix[2][3];
      optr[12] = m.matrix[3][0];
      optr[13] = m.matrix[3][1];
      optr[14] = m.matrix[3][2];
      optr[15] = m.matrix[3][3];
      #endif
    }
  }
  else
  {
    MObject value;
    plug.getValue(value);
    MFnMatrixArrayData fn(value);

    for(uint32_t i = 0, n = fn.length(); i < n; ++i)
    {
      float* optr = values + i * 16;
      double* iptr = &fn[i].matrix[0][0];
      #if AL_MAYA_ENABLE_SIMD
      # ifdef __AVX__
      const f128 m0 = cvt4d_to_4f(loadu4d(iptr));
      const f128 m1 = cvt4d_to_4f(loadu4d(iptr + 4));
      const f128 m2 = cvt4d_to_4f(loadu4d(iptr + 8));
      const f128 m3 = cvt4d_to_4f(loadu4d(iptr + 12));
      # else
      const f128 m0a = cvt2d_to_2f(loadu2d(iptr));
      const f128 m0b = cvt2d_to_2f(loadu2d(iptr + 2));
      const f128 m1a = cvt2d_to_2f(loadu2d(iptr + 4));
      const f128 m1b = cvt2d_to_2f(loadu2d(iptr + 6));
      const f128 m2a = cvt2d_to_2f(loadu2d(iptr + 8));
      const f128 m2b = cvt2d_to_2f(loadu2d(iptr + 10));
      const f128 m3a = cvt2d_to_2f(loadu2d(iptr + 12));
      const f128 m3b = cvt2d_to_2f(loadu2d(iptr + 14));
      const f128 m0 = movelh4f(m0a, m0b);
      const f128 m1 = movelh4f(m1a, m1b);
      const f128 m2 = movelh4f(m2a, m2b);
      const f128 m3 = movelh4f(m3a, m3b);
      # endif
      storeu4f(optr, m0);
      storeu4f(optr + 4, m1);
      storeu4f(optr + 8, m2);
      storeu4f(optr + 12, m3);
      #else
      for(int k = 0; k < 16; ++k)
      {
        optr[k] = iptr[k];
      }
      #endif
    }
  }
  return MS::kSuccess;
}

//----------------------------------------------------------------------------------------------------------------------
MStatus DgNodeHelper::getMatrix4x4Array(MObject node, MObject attribute, double* const values, const size_t count)
{
  MPlug plug(node, attribute);
  if(!plug)
    return MS::kFailure;

  if(plug.isArray())
  {
    uint32_t num = plug.numElements();
    if(num != count)
    {
      MGlobal::displayError("array is sized incorrectly");
      return MS::kFailure;
    }

    MFnMatrixData fn;
    MObject elementValue;
    for(uint32_t i = 0, j = 0; i < count; ++i, j += 16)
    {
      plug.elementByLogicalIndex(i).getValue(elementValue);
      fn.setObject(elementValue);
      const MMatrix& m = fn.matrix();

      #if AL_MAYA_ENABLE_SIMD
      # ifdef __AVX__
      double* optr = values + j;
      storeu4d(optr +  0, loadu4d(&m.matrix[0][0]));
      storeu4d(optr +  4, loadu4d(&m.matrix[1][0]));
      storeu4d(optr +  8, loadu4d(&m.matrix[2][0]));
      storeu4d(optr + 12, loadu4d(&m.matrix[3][0]));

      # else
      double* optr = values + j;
      storeu2d(optr +  0, loadu2d(&m.matrix[0][0]));
      storeu2d(optr +  2, loadu2d(&m.matrix[0][2]));
      storeu2d(optr +  4, loadu2d(&m.matrix[1][0]));
      storeu2d(optr +  6, loadu2d(&m.matrix[1][2]));
      storeu2d(optr +  8, loadu2d(&m.matrix[2][0]));
      storeu2d(optr + 10, loadu2d(&m.matrix[2][2]));
      storeu2d(optr + 12, loadu2d(&m.matrix[3][0]));
      storeu2d(optr + 14, loadu2d(&m.matrix[3][2]));
      # endif
      #else
      double* optr = values + j;
      optr[0] = m.matrix[0][0];
      optr[1] = m.matrix[0][1];
      optr[2] = m.matrix[0][2];
      optr[3] = m.matrix[0][3];
      optr[4] = m.matrix[1][0];
      optr[5] = m.matrix[1][1];
      optr[6] = m.matrix[1][2];
      optr[7] = m.matrix[1][3];
      optr[8] = m.matrix[2][0];
      optr[9] = m.matrix[2][1];
      optr[10] = m.matrix[2][2];
      optr[11] = m.matrix[2][3];
      optr[12] = m.matrix[3][0];
      optr[13] = m.matrix[3][1];
      optr[14] = m.matrix[3][2];
      optr[15] = m.matrix[3][3];
      #endif
    }
  }
  else
  {
    MObject value;
    plug.getValue(value);
    MFnMatrixArrayData fn(value);

    for(uint32_t i = 0, n = fn.length(); i < n; ++i)
    {
      double* optr = values + i * 16;
      double* iptr = &fn[i].matrix[0][0];

      #if AL_MAYA_ENABLE_SIMD
      # ifdef __AVX__
      storeu4d(optr, loadu4d(iptr));
      storeu4d(optr + 4, loadu4d(iptr + 4));
      storeu4d(optr + 8, loadu4d(iptr + 8));
      storeu4d(optr + 12, loadu4d(iptr + 12));
      # else
      storeu2d(optr    , loadu2d(iptr));
      storeu2d(optr + 2, loadu2d(iptr + 2));
      storeu2d(optr + 4, loadu2d(iptr + 4));
      storeu2d(optr + 6, loadu2d(iptr + 6));
      storeu2d(optr + 8, loadu2d(iptr + 8));
      storeu2d(optr + 10, loadu2d(iptr + 10));
      storeu2d(optr + 12, loadu2d(iptr + 12));
      storeu2d(optr + 14, loadu2d(iptr + 14));
      # endif
      #else
      for(uint32_t k = 0; k < 16; ++k)
      {
        optr[k] = iptr[k];
      }
      #endif
    }
  }

  return MS::kSuccess;
}

//----------------------------------------------------------------------------------------------------------------------
MStatus DgNodeHelper::getTimeArray(MObject node, MObject attribute, float* const values, const size_t count, MTime::Unit unit)
{
  const MTime mod(1.0, MTime::k6000FPS);
  const float unitConversion = float(mod.as(unit));

  MPlug plug(node, attribute);
  if(!plug || !plug.isArray())
    return MS::kFailure;

  uint32_t num = plug.numElements();
  if(num != count)
  {
    MGlobal::displayError("array is sized incorrectly");
    return MS::kFailure;
  }

#if AL_MAYA_ENABLE_SIMD

#if defined(__AVX__) && ENABLE_SOME_AVX_ROUTINES
  const f256 unitConversion256 = splat8f(unitConversion);
  const f128 unitConversion128 = splat4f(unitConversion);
  uint32_t count8 = count & ~0x7;
  uint32_t i = 0;
  for(; i < count8; i += 8)
  {
    ALIGN32(float temp[8]);
    temp[0] = plug.elementByLogicalIndex(i).asFloat();
    temp[1] = plug.elementByLogicalIndex(i + 1).asFloat();
    temp[2] = plug.elementByLogicalIndex(i + 2).asFloat();
    temp[3] = plug.elementByLogicalIndex(i + 3).asFloat();
    temp[4] = plug.elementByLogicalIndex(i + 4).asFloat();
    temp[5] = plug.elementByLogicalIndex(i + 5).asFloat();
    temp[6] = plug.elementByLogicalIndex(i + 6).asFloat();
    temp[7] = plug.elementByLogicalIndex(i + 7).asFloat();
    storeu8f(values + i, mul8f(unitConversion256, load8f(temp)));
  }

  if(count & 0x4)
  {
    ALIGN16(float temp[4]);
    temp[0] = plug.elementByLogicalIndex(i).asFloat();
    temp[1] = plug.elementByLogicalIndex(i + 1).asFloat();
    temp[2] = plug.elementByLogicalIndex(i + 2).asFloat();
    temp[3] = plug.elementByLogicalIndex(i + 3).asFloat();
    storeu4f(values + i, mul4f(unitConversion128, load4f(temp)));
    i += 4;
  }

#else
  const f128 unitConversion128 = splat4f(unitConversion);
  uint32_t count4 = count & ~0x3;
  uint32_t i = 0;
  for(; i < count4; i += 4)
  {
    ALIGN16(float temp[4]);
    temp[0] = plug.elementByLogicalIndex(i).asFloat();
    temp[1] = plug.elementByLogicalIndex(i + 1).asFloat();
    temp[2] = plug.elementByLogicalIndex(i + 2).asFloat();
    temp[3] = plug.elementByLogicalIndex(i + 3).asFloat();
    storeu4f(values + i, mul4f(unitConversion128, load4f(temp)));
  }

#endif

  switch(count & 3)
  {
  case 3: values[i + 2] = unitConversion * plug.elementByLogicalIndex(i + 2).asFloat();
  case 2: values[i + 1] = unitConversion * plug.elementByLogicalIndex(i + 1).asFloat();
  case 1: values[i    ] = unitConversion * plug.elementByLogicalIndex(i    ).asFloat();
  default: break;
  }

#else
  for(uint32_t i = 0; i < num; ++i)
  {
    values[i] = unitConversion * plug.elementByLogicalIndex(i).asFloat();
  }
#endif
  return MS::kSuccess;
}

//----------------------------------------------------------------------------------------------------------------------
MStatus DgNodeHelper::getAngleArray(MObject node, MObject attribute, float* const values, const size_t count, MAngle::Unit unit)
{
  const MAngle mod(1.0, MAngle::internalUnit());
  const float unitConversion = float(mod.as(unit));

  MPlug plug(node, attribute);
  if(!plug || !plug.isArray())
    return MS::kFailure;

  uint32_t num = plug.numElements();
  if(num != count)
  {
    MGlobal::displayError("array is sized incorrectly");
    return MS::kFailure;
  }

#if AL_MAYA_ENABLE_SIMD

#if defined(__AVX__) && ENABLE_SOME_AVX_ROUTINES
  const f256 unitConversion256 = splat8f(unitConversion);
  const f128 unitConversion128 = splat4f(unitConversion);
  uint32_t count8 = count & ~0x7;
  uint32_t i = 0;
  for(; i < count8; i += 8)
  {
    ALIGN32(float temp[8]);
    temp[0] = plug.elementByLogicalIndex(i).asFloat();
    temp[1] = plug.elementByLogicalIndex(i + 1).asFloat();
    temp[2] = plug.elementByLogicalIndex(i + 2).asFloat();
    temp[3] = plug.elementByLogicalIndex(i + 3).asFloat();
    temp[4] = plug.elementByLogicalIndex(i + 4).asFloat();
    temp[5] = plug.elementByLogicalIndex(i + 5).asFloat();
    temp[6] = plug.elementByLogicalIndex(i + 6).asFloat();
    temp[7] = plug.elementByLogicalIndex(i + 7).asFloat();
    storeu8f(values + i, mul8f(unitConversion256, load8f(temp)));
  }

  if(count & 0x4)
  {
    ALIGN16(float temp[4]);
    temp[0] = plug.elementByLogicalIndex(i).asFloat();
    temp[1] = plug.elementByLogicalIndex(i + 1).asFloat();
    temp[2] = plug.elementByLogicalIndex(i + 2).asFloat();
    temp[3] = plug.elementByLogicalIndex(i + 3).asFloat();
    storeu4f(values + i, mul4f(unitConversion128, load4f(temp)));
    i += 4;
  }

#else
  const f128 unitConversion128 = splat4f(unitConversion);
  uint32_t count4 = count & ~0x3;
  uint32_t i = 0;
  for(; i < count4; i += 4)
  {
    ALIGN16(float temp[4]);
    temp[0] = plug.elementByLogicalIndex(i).asFloat();
    temp[1] = plug.elementByLogicalIndex(i + 1).asFloat();
    temp[2] = plug.elementByLogicalIndex(i + 2).asFloat();
    temp[3] = plug.elementByLogicalIndex(i + 3).asFloat();
    storeu4f(values + i, mul4f(unitConversion128, load4f(temp)));
  }

#endif

  switch(count & 3)
  {
  case 3: values[i + 2] = unitConversion * plug.elementByLogicalIndex(i + 2).asFloat();
  case 2: values[i + 1] = unitConversion * plug.elementByLogicalIndex(i + 1).asFloat();
  case 1: values[i] = unitConversion * plug.elementByLogicalIndex(i).asFloat();
  default: break;
  }

#else
  for(uint32_t i = 0; i < num; ++i)
  {
    values[i] = unitConversion * plug.elementByLogicalIndex(i).asFloat();
  }
#endif
  return MS::kSuccess;
}

//----------------------------------------------------------------------------------------------------------------------
MStatus DgNodeHelper::getDistanceArray(MObject node, MObject attribute, float* const values, const size_t count, MDistance::Unit unit)
{
  const MDistance mod(1.0, MDistance::internalUnit());
  const float unitConversion = float(mod.as(unit));

  MPlug plug(node, attribute);
  if(!plug || !plug.isArray())
    return MS::kFailure;

  uint32_t num = plug.numElements();
  if(num != count)
  {
    MGlobal::displayError("array is sized incorrectly");
    return MS::kFailure;
  }

#if AL_MAYA_ENABLE_SIMD

#if defined(__AVX__) && ENABLE_SOME_AVX_ROUTINES
  const f256 unitConversion256 = splat8f(unitConversion);
  const f128 unitConversion128 = splat4f(unitConversion);
  uint32_t count8 = count & ~0x7;
  uint32_t i = 0;
  for(; i < count8; i += 8)
  {
    ALIGN32(float temp[8]);
    temp[0] = plug.elementByLogicalIndex(i).asFloat();
    temp[1] = plug.elementByLogicalIndex(i + 1).asFloat();
    temp[2] = plug.elementByLogicalIndex(i + 2).asFloat();
    temp[3] = plug.elementByLogicalIndex(i + 3).asFloat();
    temp[4] = plug.elementByLogicalIndex(i + 4).asFloat();
    temp[5] = plug.elementByLogicalIndex(i + 5).asFloat();
    temp[6] = plug.elementByLogicalIndex(i + 6).asFloat();
    temp[7] = plug.elementByLogicalIndex(i + 7).asFloat();
    storeu8f(values + i, mul8f(unitConversion256, load8f(temp)));
  }

  if(count & 0x4)
  {
    ALIGN16(float temp[4]);
    temp[0] = plug.elementByLogicalIndex(i).asFloat();
    temp[1] = plug.elementByLogicalIndex(i + 1).asFloat();
    temp[2] = plug.elementByLogicalIndex(i + 2).asFloat();
    temp[3] = plug.elementByLogicalIndex(i + 3).asFloat();
    storeu4f(values + i, mul4f(unitConversion128, load4f(temp)));
    i += 4;
  }

#else
  const f128 unitConversion128 = splat4f(unitConversion);
  uint32_t count4 = count & ~0x3;
  uint32_t i = 0;
  for(; i < count4; i += 4)
  {
    ALIGN16(float temp[4]);
    temp[0] = plug.elementByLogicalIndex(i).asFloat();
    temp[1] = plug.elementByLogicalIndex(i + 1).asFloat();
    temp[2] = plug.elementByLogicalIndex(i + 2).asFloat();
    temp[3] = plug.elementByLogicalIndex(i + 3).asFloat();
    storeu4f(values + i, mul4f(unitConversion128, load4f(temp)));
  }

#endif

  switch(count & 3)
  {
  case 3: values[i + 2] = unitConversion * plug.elementByLogicalIndex(i + 2).asFloat();
  case 2: values[i + 1] = unitConversion * plug.elementByLogicalIndex(i + 1).asFloat();
  case 1: values[i] = unitConversion * plug.elementByLogicalIndex(i).asFloat();
  default: break;
  }

#else
  for(uint32_t i = 0; i < num; ++i)
  {
    values[i] = unitConversion * plug.elementByLogicalIndex(i).asFloat();
  }
#endif
  return MS::kSuccess;
}

//----------------------------------------------------------------------------------------------------------------------
MStatus DgNodeHelper::setString(MObject node, MObject attr, const std::string& str)
{
  return setString(node, attr, str.c_str());
}

//----------------------------------------------------------------------------------------------------------------------
MStatus DgNodeHelper::setVec2(MObject node, MObject attr, const int* const xy)
{
  const char* const errorString = "vec2i error";
  MPlug plug(node, attr);
  AL_MAYA_CHECK_ERROR(plug.child(0).setValue(xy[0]), errorString);
  AL_MAYA_CHECK_ERROR(plug.child(1).setValue(xy[1]), errorString);
  return MS::kSuccess;
}

//----------------------------------------------------------------------------------------------------------------------
MStatus DgNodeHelper::setVec2(MObject node, MObject attr, const float* const xy)
{
  const char* const errorString = "vec2f error";
  MPlug plug(node, attr);
  AL_MAYA_CHECK_ERROR(plug.child(0).setValue(xy[0]), errorString);
  AL_MAYA_CHECK_ERROR(plug.child(1).setValue(xy[1]), errorString);
  return MS::kSuccess;
}

//----------------------------------------------------------------------------------------------------------------------
MStatus DgNodeHelper::setVec2(MObject node, MObject attr, const GfHalf* const xy)
{
  const char* const errorString = "vec2h error";
  MPlug plug(node, attr);
  AL_MAYA_CHECK_ERROR(plug.child(0).setValue(float2half_1f(xy[0])), errorString);
  AL_MAYA_CHECK_ERROR(plug.child(1).setValue(float2half_1f(xy[1])), errorString);
  return MS::kSuccess;
}

//----------------------------------------------------------------------------------------------------------------------
MStatus DgNodeHelper::setVec2(MObject node, MObject attr, const double* const xy)
{
  const char* const errorString = "vec2d error";
  MPlug plug(node, attr);
  AL_MAYA_CHECK_ERROR(plug.child(0).setValue(xy[0]), errorString);
  AL_MAYA_CHECK_ERROR(plug.child(1).setValue(xy[1]), errorString);
  return MS::kSuccess;
}

//----------------------------------------------------------------------------------------------------------------------
MStatus DgNodeHelper::setVec3(MObject node, MObject attr, const int* const xyz)
{
  const char* const errorString = "vec3i error";
  MPlug plug(node, attr);
  AL_MAYA_CHECK_ERROR(plug.child(0).setValue(xyz[0]), errorString);
  AL_MAYA_CHECK_ERROR(plug.child(1).setValue(xyz[1]), errorString);
  AL_MAYA_CHECK_ERROR(plug.child(2).setValue(xyz[2]), errorString);
  return MS::kSuccess;
}

//----------------------------------------------------------------------------------------------------------------------
MStatus DgNodeHelper::setVec3(MObject node, MObject attr, const float* const xyz)
{
  const char* const errorString = "vec3f error";
  MPlug plug(node, attr);
  AL_MAYA_CHECK_ERROR(plug.child(0).setValue(xyz[0]), errorString);
  AL_MAYA_CHECK_ERROR(plug.child(1).setValue(xyz[1]), errorString);
  AL_MAYA_CHECK_ERROR(plug.child(2).setValue(xyz[2]), errorString);
  return MS::kSuccess;
}

//----------------------------------------------------------------------------------------------------------------------
MStatus DgNodeHelper::setVec3(MObject node, MObject attr, const GfHalf* const xyz)
{
  const char* const errorString = "vec3h error";
  MPlug plug(node, attr);
  AL_MAYA_CHECK_ERROR(plug.child(0).setValue(float2half_1f(xyz[0])), errorString);
  AL_MAYA_CHECK_ERROR(plug.child(1).setValue(float2half_1f(xyz[1])), errorString);
  AL_MAYA_CHECK_ERROR(plug.child(2).setValue(float2half_1f(xyz[2])), errorString);
  return MS::kSuccess;
}

//----------------------------------------------------------------------------------------------------------------------
MStatus DgNodeHelper::setVec3(MObject node, MObject attr, const double* const xyz)
{
  const char* const errorString = "vec3d error";
  MPlug plug(node, attr);
  AL_MAYA_CHECK_ERROR(plug.child(0).setValue(xyz[0]), errorString);
  AL_MAYA_CHECK_ERROR(plug.child(1).setValue(xyz[1]), errorString);
  AL_MAYA_CHECK_ERROR(plug.child(2).setValue(xyz[2]), errorString);
  return MS::kSuccess;
}

//----------------------------------------------------------------------------------------------------------------------
MStatus DgNodeHelper::setVec4(MObject node, MObject attr, const int* const xyzw)
{
  const char* const errorString = "vec4i error";
  MPlug plug(node, attr);
  AL_MAYA_CHECK_ERROR(plug.child(0).setValue(xyzw[0]), errorString);
  AL_MAYA_CHECK_ERROR(plug.child(1).setValue(xyzw[1]), errorString);
  AL_MAYA_CHECK_ERROR(plug.child(2).setValue(xyzw[2]), errorString);
  AL_MAYA_CHECK_ERROR(plug.child(3).setValue(xyzw[3]), errorString);
  return MS::kSuccess;
}

//----------------------------------------------------------------------------------------------------------------------
MStatus DgNodeHelper::setVec4(MObject node, MObject attr, const float* const xyzw)
{
  const char* const errorString = "vec4f error";
  MPlug plug(node, attr);
  AL_MAYA_CHECK_ERROR(plug.child(0).setValue(xyzw[0]), errorString);
  AL_MAYA_CHECK_ERROR(plug.child(1).setValue(xyzw[1]), errorString);
  AL_MAYA_CHECK_ERROR(plug.child(2).setValue(xyzw[2]), errorString);
  AL_MAYA_CHECK_ERROR(plug.child(3).setValue(xyzw[3]), errorString);
  return MS::kSuccess;
}

//----------------------------------------------------------------------------------------------------------------------
MStatus DgNodeHelper::setVec4(MObject node, MObject attr, const double* const xyzw)
{
  const char* const errorString = "vec4d error";
  MPlug plug(node, attr);
  AL_MAYA_CHECK_ERROR(plug.child(0).setValue(xyzw[0]), errorString);
  AL_MAYA_CHECK_ERROR(plug.child(1).setValue(xyzw[1]), errorString);
  AL_MAYA_CHECK_ERROR(plug.child(2).setValue(xyzw[2]), errorString);
  AL_MAYA_CHECK_ERROR(plug.child(3).setValue(xyzw[3]), errorString);
  return MS::kSuccess;
}

//----------------------------------------------------------------------------------------------------------------------
MStatus DgNodeHelper::setVec4(MObject node, MObject attr, const GfHalf* const xyzw)
{
  const char* const errorString = "vec4h error";
  MPlug plug(node, attr);
  AL_MAYA_CHECK_ERROR(plug.child(0).setValue(float2half_1f(xyzw[0])), errorString);
  AL_MAYA_CHECK_ERROR(plug.child(1).setValue(float2half_1f(xyzw[1])), errorString);
  AL_MAYA_CHECK_ERROR(plug.child(2).setValue(float2half_1f(xyzw[2])), errorString);
  AL_MAYA_CHECK_ERROR(plug.child(3).setValue(float2half_1f(xyzw[3])), errorString);
  return MS::kSuccess;
}

//----------------------------------------------------------------------------------------------------------------------
MStatus DgNodeHelper::setQuat(MObject node, MObject attr, const float* const xyzw)
{
  const char* const errorString = "quatf error";
  MPlug plug(node, attr);
  AL_MAYA_CHECK_ERROR(plug.child(0).setValue(xyzw[0]), errorString);
  AL_MAYA_CHECK_ERROR(plug.child(1).setValue(xyzw[1]), errorString);
  AL_MAYA_CHECK_ERROR(plug.child(2).setValue(xyzw[2]), errorString);
  AL_MAYA_CHECK_ERROR(plug.child(3).setValue(xyzw[3]), errorString);
  return MS::kSuccess;
}

//----------------------------------------------------------------------------------------------------------------------
MStatus DgNodeHelper::setQuat(MObject node, MObject attr, const double* const xyzw)
{
  const char* const errorString = "quatd error";
  MPlug plug(node, attr);
  AL_MAYA_CHECK_ERROR(plug.child(0).setValue(xyzw[0]), errorString);
  AL_MAYA_CHECK_ERROR(plug.child(1).setValue(xyzw[1]), errorString);
  AL_MAYA_CHECK_ERROR(plug.child(2).setValue(xyzw[2]), errorString);
  AL_MAYA_CHECK_ERROR(plug.child(3).setValue(xyzw[3]), errorString);
  return MS::kSuccess;
}

//----------------------------------------------------------------------------------------------------------------------
MStatus DgNodeHelper::setQuat(MObject node, MObject attr, const GfHalf* const xyzw)
{
  const char* const errorString = "quath error";
  MPlug plug(node, attr);
  AL_MAYA_CHECK_ERROR(plug.child(0).setValue(float2half_1f(xyzw[0])), errorString);
  AL_MAYA_CHECK_ERROR(plug.child(1).setValue(float2half_1f(xyzw[1])), errorString);
  AL_MAYA_CHECK_ERROR(plug.child(2).setValue(float2half_1f(xyzw[2])), errorString);
  AL_MAYA_CHECK_ERROR(plug.child(3).setValue(float2half_1f(xyzw[3])), errorString);
  return MS::kSuccess;
}

//----------------------------------------------------------------------------------------------------------------------
MStatus DgNodeHelper::setString(MObject node, MObject attr, const char* const str)
{
  const char* const errorString = "string error";
  MPlug plug(node, attr);
  AL_MAYA_CHECK_ERROR(plug.setString(str), errorString);
  return MS::kSuccess;
}

//----------------------------------------------------------------------------------------------------------------------
MStatus DgNodeHelper::setMatrix4x4(MObject node, MObject attr, const double* const str)
{
  const char* const errorString = "matrix4x4 error - unimplemented";
  MPlug plug(node, attr);
  MFnMatrixData fn;
  typedef double hack[4];
  MObject data = fn.create(MMatrix((const hack*)str));
  AL_MAYA_CHECK_ERROR(plug.setValue(data), errorString);
  return MS::kSuccess;
}

//----------------------------------------------------------------------------------------------------------------------
MStatus DgNodeHelper::setMatrix4x4(MObject node, MObject attr, const float* const ptr)
{
  const char* const errorString = "matrix4x4 error - unimplemented";
  MPlug plug(node, attr);
  MFnMatrixData fn;
  MMatrix m;
#if AL_MAYA_ENABLE_SIMD
# if __AVX__
  const f256 d0 = loadu8f(ptr);
  const f256 d1 = loadu8f(ptr + 8);
  storeu4d(&m.matrix[0][0], cvt4f_to_4d(extract4f(d0, 0)));
  storeu4d(&m.matrix[1][0], cvt4f_to_4d(extract4f(d0, 1)));
  storeu4d(&m.matrix[2][0], cvt4f_to_4d(extract4f(d1, 0)));
  storeu4d(&m.matrix[3][0], cvt4f_to_4d(extract4f(d1, 1)));
# else
  const f128 d0 = loadu4f(ptr);
  const f128 d1 = loadu4f(ptr + 4);
  const f128 d2 = loadu4f(ptr + 8);
  const f128 d3 = loadu4f(ptr + 12);
  storeu2d(&m.matrix[0][0], cvt2f_to_2d(d0));
  storeu2d(&m.matrix[0][2], cvt2f_to_2d(movehl4f(d0, d0)));
  storeu2d(&m.matrix[1][0], cvt2f_to_2d(d1));
  storeu2d(&m.matrix[1][2], cvt2f_to_2d(movehl4f(d1, d1)));
  storeu2d(&m.matrix[2][0], cvt2f_to_2d(d2));
  storeu2d(&m.matrix[2][2], cvt2f_to_2d(movehl4f(d2, d2)));
  storeu2d(&m.matrix[3][0], cvt2f_to_2d(d3));
  storeu2d(&m.matrix[3][2], cvt2f_to_2d(movehl4f(d3, d3)));
# endif
#else
  m[0][0] = ptr[0]; m[0][1] = ptr[1]; m[0][2] = ptr[2]; m[0][3] = ptr[3];
  m[1][0] = ptr[4]; m[1][1] = ptr[5]; m[1][2] = ptr[6]; m[1][3] = ptr[7];
  m[2][0] = ptr[8]; m[2][1] = ptr[9]; m[2][2] = ptr[10]; m[2][3] = ptr[11];
  m[3][0] = ptr[12]; m[3][1] = ptr[13]; m[3][2] = ptr[14]; m[3][3] = ptr[15];
#endif

  MObject data = fn.create(m);
  AL_MAYA_CHECK_ERROR(plug.setValue(data), errorString);
  return MS::kSuccess;
}


//----------------------------------------------------------------------------------------------------------------------
MStatus DgNodeHelper::setMatrix3x3(MObject node, MObject attr, const double* const str)
{
  const char* const errorString = "matrix3x3 error";
  MPlug plug(node, attr);
  AL_MAYA_CHECK_ERROR(plug.child(0).child(0).setValue(str[0]), errorString);
  AL_MAYA_CHECK_ERROR(plug.child(0).child(1).setValue(str[1]), errorString);
  AL_MAYA_CHECK_ERROR(plug.child(0).child(2).setValue(str[2]), errorString);
  AL_MAYA_CHECK_ERROR(plug.child(1).child(0).setValue(str[3]), errorString);
  AL_MAYA_CHECK_ERROR(plug.child(1).child(1).setValue(str[4]), errorString);
  AL_MAYA_CHECK_ERROR(plug.child(1).child(2).setValue(str[5]), errorString);
  AL_MAYA_CHECK_ERROR(plug.child(2).child(0).setValue(str[6]), errorString);
  AL_MAYA_CHECK_ERROR(plug.child(2).child(1).setValue(str[7]), errorString);
  AL_MAYA_CHECK_ERROR(plug.child(2).child(2).setValue(str[8]), errorString);
  return MS::kSuccess;
}

//----------------------------------------------------------------------------------------------------------------------
MStatus DgNodeHelper::setMatrix3x3(MObject node, MObject attr, const float* const str)
{
  const char* const errorString = "matrix3x3 error";
  MPlug plug(node, attr);
  AL_MAYA_CHECK_ERROR(plug.child(0).child(0).setValue(str[0]), errorString);
  AL_MAYA_CHECK_ERROR(plug.child(0).child(1).setValue(str[1]), errorString);
  AL_MAYA_CHECK_ERROR(plug.child(0).child(2).setValue(str[2]), errorString);
  AL_MAYA_CHECK_ERROR(plug.child(1).child(0).setValue(str[3]), errorString);
  AL_MAYA_CHECK_ERROR(plug.child(1).child(1).setValue(str[4]), errorString);
  AL_MAYA_CHECK_ERROR(plug.child(1).child(2).setValue(str[5]), errorString);
  AL_MAYA_CHECK_ERROR(plug.child(2).child(0).setValue(str[6]), errorString);
  AL_MAYA_CHECK_ERROR(plug.child(2).child(1).setValue(str[7]), errorString);
  AL_MAYA_CHECK_ERROR(plug.child(2).child(2).setValue(str[8]), errorString);
  return MS::kSuccess;
}


//----------------------------------------------------------------------------------------------------------------------
MStatus DgNodeHelper::setMatrix2x2(MObject node, MObject attr, const double* const str)
{
  const char* const errorString = "matrix2x2 error";
  MPlug plug(node, attr);
  AL_MAYA_CHECK_ERROR(plug.child(0).child(0).setValue(str[0]), errorString);
  AL_MAYA_CHECK_ERROR(plug.child(0).child(1).setValue(str[1]), errorString);
  AL_MAYA_CHECK_ERROR(plug.child(1).child(0).setValue(str[2]), errorString);
  AL_MAYA_CHECK_ERROR(plug.child(1).child(1).setValue(str[3]), errorString);
  return MS::kSuccess;
}

//----------------------------------------------------------------------------------------------------------------------
MStatus DgNodeHelper::setMatrix2x2(MObject node, MObject attr, const float* const str)
{
  const char* const errorString = "matrix2x2 error";
  MPlug plug(node, attr);
  AL_MAYA_CHECK_ERROR(plug.child(0).child(0).setValue(str[0]), errorString);
  AL_MAYA_CHECK_ERROR(plug.child(0).child(1).setValue(str[1]), errorString);
  AL_MAYA_CHECK_ERROR(plug.child(1).child(0).setValue(str[2]), errorString);
  AL_MAYA_CHECK_ERROR(plug.child(1).child(1).setValue(str[3]), errorString);
  return MS::kSuccess;
}

//----------------------------------------------------------------------------------------------------------------------
MStatus DgNodeHelper::setMatrix2x2Array(MObject node, MObject attribute, const double* const values, const size_t count)
{
  const char* const errorString = "setMatrix2x2Array error";
  MPlug arrayPlug(node, attribute);
  arrayPlug.setNumElements(count);
  for(uint32_t i = 0; i < count; ++i)
  {
    const double* const str = values + i * 4;
    MPlug plug = arrayPlug.elementByLogicalIndex(i);
    AL_MAYA_CHECK_ERROR(plug.child(0).child(0).setValue(str[0]), errorString);
    AL_MAYA_CHECK_ERROR(plug.child(0).child(1).setValue(str[1]), errorString);
    AL_MAYA_CHECK_ERROR(plug.child(1).child(0).setValue(str[2]), errorString);
    AL_MAYA_CHECK_ERROR(plug.child(1).child(1).setValue(str[3]), errorString);
  }
  return MS::kSuccess;
}

//----------------------------------------------------------------------------------------------------------------------
MStatus DgNodeHelper::setMatrix2x2Array(MObject node, MObject attribute, const float* const values, const size_t count)
{
  const char* const errorString = "setMatrix2x2Array error";
  MPlug arrayPlug(node, attribute);
  arrayPlug.setNumElements(count);
  for(uint32_t i = 0; i < count; ++i)
  {
    const float* const str = values + i * 4;
    MPlug plug = arrayPlug.elementByLogicalIndex(i);
    AL_MAYA_CHECK_ERROR(plug.child(0).child(0).setValue(str[0]), errorString);
    AL_MAYA_CHECK_ERROR(plug.child(0).child(1).setValue(str[1]), errorString);
    AL_MAYA_CHECK_ERROR(plug.child(1).child(0).setValue(str[2]), errorString);
    AL_MAYA_CHECK_ERROR(plug.child(1).child(1).setValue(str[3]), errorString);
  }
  return MS::kSuccess;
}

//----------------------------------------------------------------------------------------------------------------------
MStatus DgNodeHelper::setMatrix3x3Array(MObject node, MObject attribute, const double* const values, const size_t count)
{
  const char* const errorString = "setMatrix3x3Array error";
  MPlug arrayPlug(node, attribute);
  arrayPlug.setNumElements(count);
  for(uint32_t i = 0; i < count; ++i)
  {
    const double* const str = values + i * 9;
    MPlug plug = arrayPlug.elementByLogicalIndex(i);
    AL_MAYA_CHECK_ERROR(plug.child(0).child(0).setValue(str[0]), errorString);
    AL_MAYA_CHECK_ERROR(plug.child(0).child(1).setValue(str[1]), errorString);
    AL_MAYA_CHECK_ERROR(plug.child(0).child(2).setValue(str[2]), errorString);
    AL_MAYA_CHECK_ERROR(plug.child(1).child(0).setValue(str[3]), errorString);
    AL_MAYA_CHECK_ERROR(plug.child(1).child(1).setValue(str[4]), errorString);
    AL_MAYA_CHECK_ERROR(plug.child(1).child(2).setValue(str[5]), errorString);
    AL_MAYA_CHECK_ERROR(plug.child(2).child(0).setValue(str[6]), errorString);
    AL_MAYA_CHECK_ERROR(plug.child(2).child(1).setValue(str[7]), errorString);
    AL_MAYA_CHECK_ERROR(plug.child(2).child(2).setValue(str[8]), errorString);
  }
  return MS::kSuccess;
}

//----------------------------------------------------------------------------------------------------------------------
MStatus DgNodeHelper::setMatrix3x3Array(MObject node, MObject attribute, const float* const values, const size_t count)
{
  const char* const errorString = "setMatrix3x3Array error";
  MPlug arrayPlug(node, attribute);
  arrayPlug.setNumElements(count);
  for(uint32_t i = 0; i < count; ++i)
  {
    const float* const str = values + i * 9;
    MPlug plug = arrayPlug.elementByLogicalIndex(i);
    AL_MAYA_CHECK_ERROR(plug.child(0).child(0).setValue(str[0]), errorString);
    AL_MAYA_CHECK_ERROR(plug.child(0).child(1).setValue(str[1]), errorString);
    AL_MAYA_CHECK_ERROR(plug.child(0).child(2).setValue(str[2]), errorString);
    AL_MAYA_CHECK_ERROR(plug.child(1).child(0).setValue(str[3]), errorString);
    AL_MAYA_CHECK_ERROR(plug.child(1).child(1).setValue(str[4]), errorString);
    AL_MAYA_CHECK_ERROR(plug.child(1).child(2).setValue(str[5]), errorString);
    AL_MAYA_CHECK_ERROR(plug.child(2).child(0).setValue(str[6]), errorString);
    AL_MAYA_CHECK_ERROR(plug.child(2).child(1).setValue(str[7]), errorString);
    AL_MAYA_CHECK_ERROR(plug.child(2).child(2).setValue(str[8]), errorString);
  }
  return MS::kSuccess;
}

//----------------------------------------------------------------------------------------------------------------------
MStatus DgNodeHelper::setStringArray(MObject node, MObject attribute, const std::string* const values, const size_t count)
{
  const char* const errorString = "DgNodeHelper::setStringArray error";
  MPlug plug(node, attribute);
  if(!plug)
  {
    MGlobal::displayError(errorString);
    return MS::kFailure;
  }

  plug.setNumElements(count);
  for(uint32_t i = 0; i < count; ++i)
  {
    MPlug elem = plug.elementByLogicalIndex(i);
    elem.setString(MString(values[i].c_str(), values[i].size()));
  }
  return MS::kSuccess;
}

//----------------------------------------------------------------------------------------------------------------------
MStatus DgNodeHelper::setQuatArray(MObject node, MObject attr, const GfHalf* const values, const size_t count)
{
  return setVec4Array(node, attr, values, count);
}

//----------------------------------------------------------------------------------------------------------------------
MStatus DgNodeHelper::setQuatArray(MObject node, MObject attr, const float* const values, const size_t count)
{
  return setVec4Array(node, attr, values, count);
}

//----------------------------------------------------------------------------------------------------------------------
MStatus DgNodeHelper::setQuatArray(MObject node, MObject attr, const double* const values, const size_t count)
{
  return setVec4Array(node, attr, values, count);
}

//----------------------------------------------------------------------------------------------------------------------
MStatus DgNodeHelper::getFloat(MObject node, MObject attr, float& value)
{
  const char* const errorString = "DgNodeHelper::getFloat error";
  MPlug plug(node, attr);
  if(!plug)
  {
    MGlobal::displayError(errorString);
    return MS::kFailure;
  }
  return plug.getValue(value);
}

//----------------------------------------------------------------------------------------------------------------------
MStatus DgNodeHelper::getDouble(MObject node, MObject attr, double& value)
{
  const char* const errorString = "DgNodeHelper::getDouble error";
  MPlug plug(node, attr);
  if(!plug)
  {
    MGlobal::displayError(errorString);
    return MS::kFailure;
  }
  return plug.getValue(value);
}


//----------------------------------------------------------------------------------------------------------------------
MStatus DgNodeHelper::getTime(MObject node, MObject attr, MTime& value)
{
  const char* const errorString = "DgNodeHelper::getTime error";
  MPlug plug(node, attr);
  if(!plug)
  {
    MGlobal::displayError(errorString);
    return MS::kFailure;
  }
  return plug.getValue(value);
}


//----------------------------------------------------------------------------------------------------------------------
MStatus DgNodeHelper::getDistance(MObject node, MObject attr, MDistance& value)
{
  const char* const errorString = "DgNodeHelper::getDistance error";
  MPlug plug(node, attr);
  if(!plug)
  {
    MGlobal::displayError(errorString);
    return MS::kFailure;
  }
  return plug.getValue(value);
}


//----------------------------------------------------------------------------------------------------------------------
MStatus DgNodeHelper::getAngle(MObject node, MObject attr, MAngle& value)
{
  const char* const errorString = "DgNodeHelper::getAngle error";
  MPlug plug(node, attr);
  if(!plug)
  {
    MGlobal::displayError(errorString);
    return MS::kFailure;
  }
  return plug.getValue(value);
}

//----------------------------------------------------------------------------------------------------------------------
MStatus DgNodeHelper::getBool(MObject node, MObject attr, bool& value)
{
  const char* const errorString = "DgNodeHelper::getBool error";
  MPlug plug(node, attr);
  if(!plug)
  {
    MGlobal::displayError(errorString);
    return MS::kFailure;
  }
  return plug.getValue(value);
}

//----------------------------------------------------------------------------------------------------------------------
MStatus DgNodeHelper::getInt8(MObject node, MObject attr, int8_t& value)
{
  const char* const errorString = "DgNodeHelper::getInt32 error";
  MPlug plug(node, attr);
  if(!plug)
  {
    MGlobal::displayError(errorString);
    return MS::kFailure;
  }
  char c;
  MStatus status = plug.getValue(c);
  value = c;
  return status;
}

//----------------------------------------------------------------------------------------------------------------------
MStatus DgNodeHelper::getInt16(MObject node, MObject attr, int16_t& value)
{
  const char* const errorString = "DgNodeHelper::getInt32 error";
  MPlug plug(node, attr);
  if(!plug)
  {
    MGlobal::displayError(errorString);
    return MS::kFailure;
  }
  return plug.getValue(value);
}

//----------------------------------------------------------------------------------------------------------------------
MStatus DgNodeHelper::getInt32(MObject node, MObject attr, int32_t& value)
{
  const char* const errorString = "DgNodeHelper::getInt32 error";
  MPlug plug(node, attr);
  if(!plug)
  {
    MGlobal::displayError(errorString);
    return MS::kFailure;
  }
  return plug.getValue(value);
}

//----------------------------------------------------------------------------------------------------------------------
MStatus DgNodeHelper::getInt64(MObject node, MObject attr, int64_t& value)
{
  const char* const errorString = "DgNodeHelper::getInt32 error";
  MPlug plug(node, attr);
  if(!plug)
  {
    MGlobal::displayError(errorString);
    return MS::kFailure;
  }
  MStatus status;
  value = plug.asInt64(MDGContext::fsNormal, &status);
  return status;
}

//----------------------------------------------------------------------------------------------------------------------
MStatus DgNodeHelper::getMatrix2x2(MObject node, MObject attr, float* const str)
{
  const char* const errorString = "DgNodeHelper::getMatrix2x2 error";
  MPlug plug(node, attr);
  if(!plug)
  {
    MGlobal::displayError(errorString);
    return MS::kFailure;
  }
  AL_MAYA_CHECK_ERROR(plug.child(0).child(0).getValue(str[0]), errorString);
  AL_MAYA_CHECK_ERROR(plug.child(0).child(1).getValue(str[1]), errorString);
  AL_MAYA_CHECK_ERROR(plug.child(1).child(0).getValue(str[2]), errorString);
  AL_MAYA_CHECK_ERROR(plug.child(1).child(1).getValue(str[3]), errorString);
  return MS::kSuccess;
}

//----------------------------------------------------------------------------------------------------------------------
MStatus DgNodeHelper::getMatrix3x3(MObject node, MObject attr, float* const str)
{
  const char* const errorString = "getMatrix3x3 error";
  MPlug plug(node, attr);
  if(!plug)
  {
    MGlobal::displayError(errorString);
    return MS::kFailure;
  }
  AL_MAYA_CHECK_ERROR(plug.child(0).child(0).getValue(str[0]), errorString);
  AL_MAYA_CHECK_ERROR(plug.child(0).child(1).getValue(str[1]), errorString);
  AL_MAYA_CHECK_ERROR(plug.child(0).child(2).getValue(str[2]), errorString);
  AL_MAYA_CHECK_ERROR(plug.child(1).child(0).getValue(str[3]), errorString);
  AL_MAYA_CHECK_ERROR(plug.child(1).child(1).getValue(str[4]), errorString);
  AL_MAYA_CHECK_ERROR(plug.child(1).child(2).getValue(str[5]), errorString);
  AL_MAYA_CHECK_ERROR(plug.child(2).child(0).getValue(str[6]), errorString);
  AL_MAYA_CHECK_ERROR(plug.child(2).child(1).getValue(str[7]), errorString);
  AL_MAYA_CHECK_ERROR(plug.child(2).child(2).getValue(str[8]), errorString);
  return MS::kSuccess;
}

//----------------------------------------------------------------------------------------------------------------------
MStatus DgNodeHelper::getMatrix4x4(MObject node, MObject attr, float* const values)
{
  const char* const errorString = "DgNodeHelper::getMatrix4x4 error";
  MPlug plug(node, attr);
  if(!plug)
  {
    MGlobal::displayError(errorString);
    return MS::kFailure;
  }
  MObject data;
  AL_MAYA_CHECK_ERROR(plug.getValue(data), errorString);
  MFnMatrixData fn(data);
  const MMatrix& mat = fn.matrix();

#if AL_MAYA_ENABLE_SIMD
# ifdef __AVX__
  const f128 r0 = cvt4d_to_4f(loadu4d(&mat.matrix[0][0]));
  const f128 r1 = cvt4d_to_4f(loadu4d(&mat.matrix[1][0]));
  const f128 r2 = cvt4d_to_4f(loadu4d(&mat.matrix[2][0]));
  const f128 r3 = cvt4d_to_4f(loadu4d(&mat.matrix[3][0]));
# else
  const f128 r0 = movelh4f(
      cvt2d_to_2f(loadu2d(&mat.matrix[0][0])),
      cvt2d_to_2f(loadu2d(&mat.matrix[0][2])));
  const f128 r1 = movelh4f(
      cvt2d_to_2f(loadu2d(&mat.matrix[1][0])),
      cvt2d_to_2f(loadu2d(&mat.matrix[1][2])));
  const f128 r2 = movelh4f(
      cvt2d_to_2f(loadu2d(&mat.matrix[2][0])),
      cvt2d_to_2f(loadu2d(&mat.matrix[2][2])));
  const f128 r3 = movelh4f(
      cvt2d_to_2f(loadu2d(&mat.matrix[3][0])),
      cvt2d_to_2f(loadu2d(&mat.matrix[3][2])));
# endif
  storeu4f(values, r0);
  storeu4f(values + 4, r1);
  storeu4f(values + 8, r2);
  storeu4f(values + 12, r3);
#else
  values[0] = mat.matrix[0][0];
  values[1] = mat.matrix[0][1];
  values[2] = mat.matrix[0][2];
  values[3] = mat.matrix[0][3];
  values[4] = mat.matrix[1][0];
  values[5] = mat.matrix[1][1];
  values[6] = mat.matrix[1][2];
  values[7] = mat.matrix[1][3];
  values[8] = mat.matrix[2][0];
  values[9] = mat.matrix[2][1];
  values[10] = mat.matrix[2][2];
  values[11] = mat.matrix[2][3];
  values[12] = mat.matrix[3][0];
  values[13] = mat.matrix[3][1];
  values[14] = mat.matrix[3][2];
  values[15] = mat.matrix[3][3];
#endif

  return MS::kSuccess;
}

//----------------------------------------------------------------------------------------------------------------------
MStatus DgNodeHelper::getMatrix2x2(MObject node, MObject attr, double* const str)
{
  const char* const errorString = "DgNodeHelper::getMatrix2x2 error";
  MPlug plug(node, attr);
  if(!plug)
  {
    MGlobal::displayError(errorString);
    return MS::kFailure;
  }
  AL_MAYA_CHECK_ERROR(plug.child(0).child(0).getValue(str[0]), errorString);
  AL_MAYA_CHECK_ERROR(plug.child(0).child(1).getValue(str[1]), errorString);
  AL_MAYA_CHECK_ERROR(plug.child(1).child(0).getValue(str[2]), errorString);
  AL_MAYA_CHECK_ERROR(plug.child(1).child(1).getValue(str[3]), errorString);
  return MS::kSuccess;
}

//----------------------------------------------------------------------------------------------------------------------
MStatus DgNodeHelper::getMatrix3x3(MObject node, MObject attr, double* const str)
{
  const char* const errorString = "DgNodeHelper::getMatrix3x3 error";
  MPlug plug(node, attr);
  if(!plug)
  {
    MGlobal::displayError(errorString);
    return MS::kFailure;
  }
  AL_MAYA_CHECK_ERROR(plug.child(0).child(0).getValue(str[0]), errorString);
  AL_MAYA_CHECK_ERROR(plug.child(0).child(1).getValue(str[1]), errorString);
  AL_MAYA_CHECK_ERROR(plug.child(0).child(2).getValue(str[2]), errorString);
  AL_MAYA_CHECK_ERROR(plug.child(1).child(0).getValue(str[3]), errorString);
  AL_MAYA_CHECK_ERROR(plug.child(1).child(1).getValue(str[4]), errorString);
  AL_MAYA_CHECK_ERROR(plug.child(1).child(2).getValue(str[5]), errorString);
  AL_MAYA_CHECK_ERROR(plug.child(2).child(0).getValue(str[6]), errorString);
  AL_MAYA_CHECK_ERROR(plug.child(2).child(1).getValue(str[7]), errorString);
  AL_MAYA_CHECK_ERROR(plug.child(2).child(2).getValue(str[8]), errorString);
  return MS::kSuccess;
}

//----------------------------------------------------------------------------------------------------------------------
MStatus DgNodeHelper::getMatrix4x4(MObject node, MObject attr, double* const values)
{
  const char* const errorString = "DgNodeHelper::getMatrix4x4 error";
  MPlug plug(node, attr);
  if(!plug)
  {
    MGlobal::displayError(errorString);
    return MS::kFailure;
  }
  MObject data;
  AL_MAYA_CHECK_ERROR(plug.getValue(data), errorString);
  MFnMatrixData fn(data);
  const MMatrix& mat = fn.matrix();

#if AL_MAYA_ENABLE_SIMD
# ifdef __AVX__
  const d256 r0 = loadu4d(&mat.matrix[0][0]);
  const d256 r1 = loadu4d(&mat.matrix[1][0]);
  const d256 r2 = loadu4d(&mat.matrix[2][0]);
  const d256 r3 = loadu4d(&mat.matrix[3][0]);
  storeu4d(values, r0);
  storeu4d(values + 4, r1);
  storeu4d(values + 8, r2);
  storeu4d(values + 12, r3);
# else
  const d128 r0a = loadu2d(&mat.matrix[0][0]);
  const d128 r0b = loadu2d(&mat.matrix[0][2]);
  const d128 r1a = loadu2d(&mat.matrix[1][0]);
  const d128 r1b = loadu2d(&mat.matrix[1][2]);
  const d128 r2a = loadu2d(&mat.matrix[2][0]);
  const d128 r2b = loadu2d(&mat.matrix[2][2]);
  const d128 r3a = loadu2d(&mat.matrix[3][0]);
  const d128 r3b = loadu2d(&mat.matrix[3][2]);
  storeu2d(values, r0a);
  storeu2d(values + 2, r0b);
  storeu2d(values + 4, r1a);
  storeu2d(values + 6, r1b);
  storeu2d(values + 8, r2a);
  storeu2d(values + 10, r2b);
  storeu2d(values + 12, r3a);
  storeu2d(values + 14, r3b);
# endif
#else
  values[0] = mat.matrix[0][0];
  values[1] = mat.matrix[0][1];
  values[2] = mat.matrix[0][2];
  values[3] = mat.matrix[0][3];
  values[4] = mat.matrix[1][0];
  values[5] = mat.matrix[1][1];
  values[6] = mat.matrix[1][2];
  values[7] = mat.matrix[1][3];
  values[8] = mat.matrix[2][0];
  values[9] = mat.matrix[2][1];
  values[10] = mat.matrix[2][2];
  values[11] = mat.matrix[2][3];
  values[12] = mat.matrix[3][0];
  values[13] = mat.matrix[3][1];
  values[14] = mat.matrix[3][2];
  values[15] = mat.matrix[3][3];
#endif

  return MS::kSuccess;
}

//----------------------------------------------------------------------------------------------------------------------
MStatus DgNodeHelper::getString(MObject node, MObject attr, std::string& str)
{
  const char* const errorString = "DgNodeHelper::getString error";
  MPlug plug(node, attr);
  if(!plug)
  {
    MGlobal::displayError(errorString);
    return MS::kFailure;
  }
  MString value;
  AL_MAYA_CHECK_ERROR(plug.getValue(value), errorString);

  str.assign(value.asChar(), value.asChar() + value.length());
  return MS::kSuccess;
}

//----------------------------------------------------------------------------------------------------------------------
MStatus DgNodeHelper::getVec2(MObject node, MObject attr, int* xy)
{
  const char* const errorString = "DgNodeHelper::getVec2 error";
  MPlug plug(node, attr);
  if(!plug)
  {
    MGlobal::displayError(errorString);
    return MS::kFailure;
  }
  AL_MAYA_CHECK_ERROR(plug.child(0).getValue(xy[0]), errorString);
  AL_MAYA_CHECK_ERROR(plug.child(1).getValue(xy[1]), errorString);
  return MS::kSuccess;
}

//----------------------------------------------------------------------------------------------------------------------
MStatus DgNodeHelper::getVec2(MObject node, MObject attr, float* xy)
{
  const char* const errorString = "DgNodeHelper::getVec2 error";
  MPlug plug(node, attr);
  if(!plug)
  {
    MGlobal::displayError(errorString);
    return MS::kFailure;
  }
  AL_MAYA_CHECK_ERROR(plug.child(0).getValue(xy[0]), errorString);
  AL_MAYA_CHECK_ERROR(plug.child(1).getValue(xy[1]), errorString);
  return MS::kSuccess;
}

//----------------------------------------------------------------------------------------------------------------------
MStatus DgNodeHelper::getVec2(MObject node, MObject attr, double* xy)
{
  const char* const errorString = "DgNodeHelper::getVec2 error";
  MPlug plug(node, attr);
  if(!plug)
  {
    MGlobal::displayError(errorString);
    return MS::kFailure;
  }
  AL_MAYA_CHECK_ERROR(plug.child(0).getValue(xy[0]), errorString);
  AL_MAYA_CHECK_ERROR(plug.child(1).getValue(xy[1]), errorString);
  return MS::kSuccess;
}

//----------------------------------------------------------------------------------------------------------------------
MStatus DgNodeHelper::getVec2(MObject node, MObject attr, GfHalf* xy)
{
  float fxy[2];
  MStatus status = getVec2(node, attr, fxy);
  xy[0] = float2half_1f(fxy[0]);
  xy[1] = float2half_1f(fxy[1]);
  return status;
}

//----------------------------------------------------------------------------------------------------------------------
MStatus DgNodeHelper::getVec3(MObject node, MObject attr, int* xyz)
{
  const char* const errorString = "DgNodeHelper::getVec3 error";
  MPlug plug(node, attr);
  if(!plug)
  {
    MGlobal::displayError(errorString);
    return MS::kFailure;
  }
  AL_MAYA_CHECK_ERROR(plug.child(0).getValue(xyz[0]), errorString);
  AL_MAYA_CHECK_ERROR(plug.child(1).getValue(xyz[1]), errorString);
  AL_MAYA_CHECK_ERROR(plug.child(2).getValue(xyz[2]), errorString);
  return MS::kSuccess;
}

//----------------------------------------------------------------------------------------------------------------------
MStatus DgNodeHelper::getVec3(MObject node, MObject attr, float* xyz)
{
  const char* const errorString = "DgNodeHelper::getVec3 error";
  MPlug plug(node, attr);
  if(!plug)
  {
    MGlobal::displayError(errorString);
    return MS::kFailure;
  }
  AL_MAYA_CHECK_ERROR(plug.child(0).getValue(xyz[0]), errorString);
  AL_MAYA_CHECK_ERROR(plug.child(1).getValue(xyz[1]), errorString);
  AL_MAYA_CHECK_ERROR(plug.child(2).getValue(xyz[2]), errorString);
  return MS::kSuccess;
}

//----------------------------------------------------------------------------------------------------------------------
MStatus DgNodeHelper::getVec3(MObject node, MObject attr, double* xyz)
{
  const char* const errorString = "DgNodeHelper::getVec3 error";
  MPlug plug(node, attr);
  if(!plug)
  {
    MGlobal::displayError(errorString);
    return MS::kFailure;
  }
  AL_MAYA_CHECK_ERROR(plug.child(0).getValue(xyz[0]), errorString);
  AL_MAYA_CHECK_ERROR(plug.child(1).getValue(xyz[1]), errorString);
  AL_MAYA_CHECK_ERROR(plug.child(2).getValue(xyz[2]), errorString);
  return MS::kSuccess;
}

//----------------------------------------------------------------------------------------------------------------------
MStatus DgNodeHelper::getVec3(MObject node, MObject attr, GfHalf* xyz)
{
  float fxyz[4];
  GfHalf xyzw[4];
  MStatus status = getVec3(node, attr, fxyz);
  float2half_4f(fxyz, xyzw);
  xyz[0] = xyzw[0];
  xyz[1] = xyzw[1];
  xyz[2] = xyzw[2];
  return status;
}

//----------------------------------------------------------------------------------------------------------------------
MStatus DgNodeHelper::getVec4(MObject node, MObject attr, int* xyzw)
{
  const char* const errorString = "DgNodeHelper::getVec4 error";
  MPlug plug(node, attr);
  if(!plug)
  {
    MGlobal::displayError(errorString);
    return MS::kFailure;
  }
  AL_MAYA_CHECK_ERROR(plug.child(0).getValue(xyzw[0]), errorString);
  AL_MAYA_CHECK_ERROR(plug.child(1).getValue(xyzw[1]), errorString);
  AL_MAYA_CHECK_ERROR(plug.child(2).getValue(xyzw[2]), errorString);
  AL_MAYA_CHECK_ERROR(plug.child(3).getValue(xyzw[3]), errorString);
  return MS::kSuccess;
}

//----------------------------------------------------------------------------------------------------------------------
MStatus DgNodeHelper::getVec4(MObject node, MObject attr, float* xyzw)
{
  const char* const errorString = "DgNodeHelper::getVec4 error";
  MPlug plug(node, attr);
  if(!plug)
  {
    MGlobal::displayError(errorString);
    return MS::kFailure;
  }
  AL_MAYA_CHECK_ERROR(plug.child(0).getValue(xyzw[0]), errorString);
  AL_MAYA_CHECK_ERROR(plug.child(1).getValue(xyzw[1]), errorString);
  AL_MAYA_CHECK_ERROR(plug.child(2).getValue(xyzw[2]), errorString);
  AL_MAYA_CHECK_ERROR(plug.child(3).getValue(xyzw[3]), errorString);
  return MS::kSuccess;
}

//----------------------------------------------------------------------------------------------------------------------
MStatus DgNodeHelper::getVec4(MObject node, MObject attr, double* xyzw)
{
  const char* const errorString = "DgNodeHelper::getVec4 error";
  MPlug plug(node, attr);
  if(!plug)
  {
    MGlobal::displayError(errorString);
    return MS::kFailure;
  }
  AL_MAYA_CHECK_ERROR(plug.child(0).getValue(xyzw[0]), errorString);
  AL_MAYA_CHECK_ERROR(plug.child(1).getValue(xyzw[1]), errorString);
  AL_MAYA_CHECK_ERROR(plug.child(2).getValue(xyzw[2]), errorString);
  AL_MAYA_CHECK_ERROR(plug.child(3).getValue(xyzw[3]), errorString);
  return MS::kSuccess;
}

//----------------------------------------------------------------------------------------------------------------------
MStatus DgNodeHelper::getVec4(MObject node, MObject attr, GfHalf* xyzw)
{
  float fxyzw[4];
  MStatus status = getVec4(node, attr, fxyzw);
  float2half_4f(fxyzw, xyzw);
  return status;
}

//----------------------------------------------------------------------------------------------------------------------
MStatus DgNodeHelper::getQuat(MObject node, MObject attr, float* xyzw)
{
  return getVec4(node, attr, xyzw);
}

//----------------------------------------------------------------------------------------------------------------------
MStatus DgNodeHelper::getQuat(MObject node, MObject attr, double* xyzw)
{
  return getVec4(node, attr, xyzw);
}

//----------------------------------------------------------------------------------------------------------------------
MStatus DgNodeHelper::getQuat(MObject node, MObject attr, GfHalf* xyzw)
{
  return getVec4(node, attr, xyzw);
}

//----------------------------------------------------------------------------------------------------------------------
MStatus DgNodeHelper::getStringArray(MObject node, MObject attr, std::string* const values, const size_t count)
{
  const char* const errorString = "DgNodeHelper::getStringArray";
  MPlug plug(node, attr);
  if(!plug)
  {
    MGlobal::displayError(errorString);
    return MS::kFailure;
  }

  if(count != plug.numElements())
  {
    return MS::kFailure;
  }

  for(size_t i = 0; i < count; ++i)
  {
    const MString& str = plug.elementByLogicalIndex(i).asString();
    values[i].assign(str.asChar(), str.length());
  }

  return MS::kSuccess;
}

//----------------------------------------------------------------------------------------------------------------------
MStatus DgNodeHelper::addStringValue(MObject node, const char* const attrName, const char* const stringValue)
{
  MFnTypedAttribute fnT;
  MObject attribute = fnT.create(attrName, attrName, MFnData::kString);
  fnT.setArray(false);
  fnT.setReadable(true);
  fnT.setWritable(true);
  MFnDependencyNode fn(node);
  if(fn.addAttribute(attribute))
  {
    MPlug plug(node, attribute);
    plug.setValue(stringValue);
    return MS::kSuccess;
  }
  return MS::kFailure;
}

//----------------------------------------------------------------------------------------------------------------------
MStatus DgNodeHelper::setMatrix4x4(MObject node, MObject attr, const MFloatMatrix& value)
  { return setMatrix4x4(node, attr, &value[0][0]); }
MStatus DgNodeHelper::setMatrix4x4(MObject node, MObject attr, const MMatrix& value)
  { return setMatrix4x4(node, attr, &value[0][0]); }
MStatus DgNodeHelper::getMatrix4x4(MObject node, MObject attr, MFloatMatrix& value)
  { return getMatrix4x4(node, attr, &value[0][0]); }
MStatus DgNodeHelper::getMatrix4x4(MObject node, MObject attr, MMatrix& value)
  { return getMatrix4x4(node, attr, &value[0][0]); }

//----------------------------------------------------------------------------------------------------------------------
} // maya
} // AL
//----------------------------------------------------------------------------------------------------------------------
