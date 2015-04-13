#ifndef __DALI_INTEGRATION_RESOURCE_TYPES_H__
#define __DALI_INTEGRATION_RESOURCE_TYPES_H__

/*
 * Copyright (c) 2014 Samsung Electronics Co., Ltd.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

// EXTERNAL INCLUDES
#include <stdint.h>
#include <string>

// INTERNAL INCLUDES
#include <dali/public-api/common/dali-common.h>
#include <dali/public-api/common/vector-wrapper.h>
#include <dali/public-api/images/image-attributes.h>
#include <dali/integration-api/resource-declarations.h>

namespace Dali
{

namespace Integration
{

// Resource Types

/**
 * Extendable set of resource types
 */
enum ResourceTypeId
{
  ResourceBitmap,
  ResourceNativeImage,
  ResourceTargetImage,
  ResourceShader,
  ResourceMesh
};

/**
 * The abstract base class for resource types.
 */
struct ResourceType
{
  /**
   * Constructor.
   * @param[in] typeId resource type id
   */
  ResourceType(ResourceTypeId typeId)
  : id(typeId) {}

  /**
   * Destructor.
   */
  virtual ~ResourceType() {}

  /**
   * Create a copy of the resource type with the same attributes.
   * @return pointer to the new ResourceType.
   */
  virtual ResourceType* Clone() const = 0;

  const ResourceTypeId id;

private:

  // Undefined copy constructor.
  ResourceType(const ResourceType& typePath);

  // Undefined assignment operator.
  ResourceType& operator=(const ResourceType& rhs);
};

/**
 * BitmapResourceType describes a bitmap resource, which can be requested
 * from ResourceLoader::LoadResource() or AllocateBitmapImage.
 */
struct BitmapResourceType : public ResourceType
{
  /**
   * Constructor.
   * @param[in] attribs parameters for image loading request
   */
  BitmapResourceType(const ImageAttributes& attribs)
  : ResourceType(ResourceBitmap),
    imageAttributes(attribs) {}

  /**
   * Destructor.
   */
  virtual ~BitmapResourceType() {}

  /**
   * @copydoc ResourceType::Clone
   */
  virtual ResourceType* Clone() const
  {
    return new BitmapResourceType(imageAttributes);
  }

  /**
   * Attributes are copied from the request.
   */
  ImageAttributes imageAttributes;

private:

  // Undefined copy constructor.
  BitmapResourceType(const BitmapResourceType& typePath);

  // Undefined assignment operator.
  BitmapResourceType& operator=(const BitmapResourceType& rhs);
};

/**
 * NativeImageResourceType describes a native image resource, which can be injected
 * through ResourceManager::AddNativeImage() or requested through ResourceLoader::LoadResource().
 * If the adaptor does not support NativeImages, it can fall back to Bitmap type.
 */
struct NativeImageResourceType : public ResourceType
{
  /**
   * Constructor.
   */
  NativeImageResourceType()
  : ResourceType(ResourceNativeImage) {}

  /**
   * Constructor.
   * @param[in] attribs parameters for image loading request
   */
  NativeImageResourceType(const ImageAttributes& attribs)
  : ResourceType(ResourceNativeImage),
    imageAttributes(attribs) {}

  /**
   * Destructor.
   */
  virtual ~NativeImageResourceType() {}

 /**
  * @copydoc ResourceType::Clone
  */
  virtual ResourceType* Clone() const
  {
    return new NativeImageResourceType(imageAttributes);
  }

  /**
   * Attributes are copied from the request (if supplied).
   */
  ImageAttributes imageAttributes;

private:

  // Undefined copy constructor.
  NativeImageResourceType(const NativeImageResourceType& typePath);

  // Undefined assignment operator.
  NativeImageResourceType& operator=(const NativeImageResourceType& rhs);
};

/**
 * RenderTargetResourceType describes a bitmap resource, which can injected
 * through ResourceManager::AddTargetImage()
 */
struct RenderTargetResourceType : public ResourceType
{
  /**
   * Constructor.
   */
  RenderTargetResourceType()
  : ResourceType(ResourceTargetImage) {}

  /**
   * Constructor.
   * @param[in] attribs parameters for image loading request
   */
  RenderTargetResourceType(const ImageAttributes& attribs)
  : ResourceType(ResourceTargetImage),
    imageAttributes(attribs) {}

  /**
   * Destructor.
   */
  virtual ~RenderTargetResourceType() {}

  /**
   * @copydoc ResourceType::Clone
   */
  virtual ResourceType* Clone() const
  {
    return new RenderTargetResourceType(imageAttributes);
  }

  /**
   * Attributes are copied from the request.
   */
  ImageAttributes imageAttributes;

private:

  // Undefined copy constructor.
  RenderTargetResourceType(const RenderTargetResourceType& typePath);

  // Undefined assignment operator.
  RenderTargetResourceType& operator=(const RenderTargetResourceType& rhs);
};

/**
 * ShaderResourceType describes a shader program resource, which can be requested
 * from PlatformAbstraction::LoadResource()
 */
struct ShaderResourceType : public ResourceType
{
  /**
   * Constructor.
   */
  ShaderResourceType(size_t shaderHash, const std::string& vertexSource, const std::string& fragmentSource)
  : ResourceType(ResourceShader),
    hash(shaderHash),
    vertexShader(vertexSource),
    fragmentShader(fragmentSource)
  {
  }

  /**
   * Destructor.
   */
  virtual ~ShaderResourceType()
  {
  }

  /**
   * @copydoc ResourceType::Clone
   */
  virtual ResourceType* Clone() const
  {
    return new ShaderResourceType(hash, vertexShader, fragmentShader);
  }

public: // Attributes
  size_t            hash;              ///< Hash of the vertex/fragment sources
  const std::string vertexShader;      ///< source code for vertex program
  const std::string fragmentShader;    ///< source code for fragment program

private:

  // Undefined copy constructor.
  ShaderResourceType(const ShaderResourceType& typePath);

  // Undefined assignment operator.
  ShaderResourceType& operator=(const ShaderResourceType& rhs);
};

/**
 * MeshResourceType describes a mesh program resource, which can be created
 * using ResourceManager::AllocateMesh.
 */
struct MeshResourceType : public ResourceType
{
  /**
   * Constructor.
   */
  MeshResourceType()
  : ResourceType(ResourceMesh) {}

  /**
   * Destructor.
   */
  virtual ~MeshResourceType() {}

  /**
   * @copydoc ResourceType::Clone
   */
  virtual ResourceType* Clone() const
  {
    return new MeshResourceType();
  }

private:

  // Undefined copy constructor.
  MeshResourceType(const MeshResourceType& typePath);

  // Undefined assignment operator.
  MeshResourceType& operator=(const MeshResourceType& rhs);
};

} // namespace Integration

} // namespace Dali

#endif // __DALI_INTEGRATION_RESOURCE_TYPES_H__