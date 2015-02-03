#ifndef __DALI_MODEL_DATA_H__
#define __DALI_MODEL_DATA_H__

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
#include <string>

// INTERNAL INCLUDES
#include <dali/public-api/modeling/model-animation-map.h>
#include <dali/public-api/object/base-handle.h>

namespace Dali
{

class Entity;
class MeshData;
class Material;
class Light;

namespace Internal DALI_INTERNAL
{
class ModelData;
}

/**
 * @brief Encapsulates a Dali 3D model/scene.
 *
 * This is usually generated by an external model loader (and is also
 * used internally with the default model loader).
 */
class DALI_IMPORT_API ModelData : public BaseHandle
{
public:

  /**
   * @brief Constructs an uninitialized handle.
   */
  ModelData();

  /**
   * @brief Create an initialized ModelData.
   *
   * @param[in] name The name of the model.
   * @return A handle to a newly allocated Dali resource.
   */
  static ModelData New(const std::string& name);

  /**
   * @brief Downcast an Object handle to ModelData handle.
   *
   * If handle points to a ModelData object the downcast produces
   * valid handle. If not the returned handle is left uninitialized.
   *
   * @param[in] handle to An object
   * @return handle to a ModelData object or an uninitialized handle
   */
  static ModelData DownCast( BaseHandle handle );

  /**
   * @brief Destructor
   *
   * This is non-virtual since derived Handle types must not contain data or virtual methods.
   */
  ~ModelData();

  /**
   * @brief This copy constructor is required for (smart) pointer semantics.
   *
   * @param [in] handle A reference to the copied handle
   */
  ModelData(const ModelData& handle);

  /**
   * @brief This assignment operator is required for (smart) pointer semantics.
   *
   * @param [in] rhs  A reference to the copied handle
   * @return A reference to this
   */
  ModelData& operator=(const ModelData& rhs);

  /**
   * @brief Returns the name of the model.
   *
   * @return The model name
   */
  const std::string& GetName() const;

  /**
   * @brief Set the root Entity of the model.
   *
   * @param root A handle to the root entity
   */
  void SetRootEntity(Entity root);

  /**
   * @brief Get the root Entity of the model.
   *
   * @return The root entity
   */
  Entity GetRootEntity() const;

  /**
   * @brief Add a mesh to the model.
   *
   * @param mesh The mesh data to add.
   */
  void AddMesh(MeshData& mesh);

  /**
   * @brief Get a mesh by index.
   *
   * @param[in] index The zero based index to a mesh
   * @return          The mesh.
   */
  const MeshData& GetMesh(unsigned int index) const;

  /**
   * @brief Get a mesh by index.
   *
   * @param[in] index The zero based index to a mesh
   * @return          The mesh.
   */
  MeshData& GetMesh(unsigned int index);

  /**
   * @brief Get Mesh count.
   *
   * @returns number of meshes
   */
  unsigned int NumberOfMeshes() const;

  /**
   * @brief Add material to the model.
   *
   * @param[in] material - the material to add to the model
   */
  void AddMaterial(Material material);

  /**
   * @brief Get a material by index.
   *
   * @param[in] index The index to a material
   * @return          A handle to a material, or NULL.
   */
  Material GetMaterial(unsigned int index) const;

  /**
   * @brief Get material count.
   *
   * @return The Number of materials
   */
  unsigned int NumberOfMaterials() const;

  /**
   * @brief Get animation map container.
   *
   * @return reference to the animation map container.
   */
   ModelAnimationMapContainer& GetAnimationMapContainer();

  /**
   * @brief Find the index for the given animation name.
   *
   * @param[in] name The name of the animation map to search for.
   * @param[out] index The index of the found map
   * @return true if the name was found, false otherwise.
   */
  bool FindAnimation (const std::string& name, unsigned int& index) const;

  /**
   * @return the number of animations defined for this model
   */
  unsigned int NumberOfAnimationMaps() const;

  /**
   * @brief Add a light to the model.
   *
   * @param light - the light to add to the model
   */
  void AddLight(Light light);

  /**
   * @brief Get a light by index.
   *
   * @param[in] index The zero based index to a light
   * @return          A pointer to a light, or NULL.
   */
  Light GetLight(unsigned int index) const;

  /**
   * @brief Get the number of lights contained in the model.
   *
   * @return The number of lights contained in the model.
   */
  unsigned int NumberOfLights() const;

  /**
   * @brief Read the model data from an open streambuf.
   *
   * @param[in] buf A streambuf opened for reading
   * @return true if data was read successfully.
   */
  bool Read(std::streambuf& buf);

  /**
   * @brief Write the model data to an open streambuf.
   *
   * @param[in] buf A streambuf opened for writing
   * @return true if data was written successfully.
   */
  bool Write(std::streambuf& buf) const;

public: // Not intended for application developers
  /**
   * @brief This constructor is used by Dali New() methods.
   *
   * @param[in] modelData A pointer to a newly allocated modelData
   */
  explicit DALI_INTERNAL ModelData(Internal::ModelData* modelData);

}; // Class ModelData

} // namespace Dali

#endif // __DALI_MODEL_DATA_H__