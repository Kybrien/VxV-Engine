#pragma once
#include <unordered_map>
#include <GLM/glm.hpp>

#include "controls.hpp"

#include "Externes/tiny_obj_loader.h"

class Vertex
{
public:
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoord;

	bool operator==(const Vertex& other) const
	{
		return position == other.position && normal == other.normal && texCoord == other.texCoord;
	}
};

struct pair_hash
{
	template<class T1, class T2>
	std::size_t operator ()(const std::pair<T1, T2>& p) const
	{
		auto h1 = std::hash<T1>{}(p.first);
		auto h2 = std::hash<T2>{}(p.second);

		// Mainly for demonstration purposes, i.e. works but is overly simple
		// In the real world, use sth. like boost.hash_combine
		return h1 ^ h2;
	}
};

namespace std
{
	template<>
	struct hash<glm::vec3>
	{
		size_t operator()(const glm::vec3& vec) const
		{
			return ((hash<float>()(vec.x) ^ (hash<float>()(vec.y) << 1)) >> 1) ^ (hash<float>()(vec.z) << 1);
		}
	};

	template<>
	struct hash<glm::vec2>
	{
		size_t operator()(const glm::vec2& vec) const
		{
			return hash<float>()(vec.x) ^ (hash<float>()(vec.y) << 1);
		}
	};

	template<>
	struct hash<Vertex>
	{
		size_t operator()(const Vertex& vertex) const
		{
			return ((hash<glm::vec3>()(vertex.position) ^ (hash<glm::vec3>()(vertex.normal) << 1)) >> 1) ^ (hash<glm::vec2>()(vertex.texCoord) << 1);
		}
	};

	struct Vec3Hash
	{
		size_t operator()(const glm::vec3& vec) const
		{
			return ((hash<float>()(vec.x) ^ (hash<float>()(vec.y) << 1)) >> 1) ^ (hash<float>()(vec.z) << 1);
		}
	};

	struct Vec2Hash
	{
		size_t operator()(const glm::vec2& vec) const
		{
			return hash<float>()(vec.x) ^ (hash<float>()(vec.y) << 1);
		}
	};

	struct VertexHash
	{
		size_t operator()(const Vertex& vertex) const
		{
			return ((Vec3Hash()(vertex.position) ^ (Vec3Hash()(vertex.normal) << 1)) >> 1) ^ (Vec2Hash()(vertex.texCoord) << 1);
		}
	};
}

class ModelComponent
{
public:
	std::string fileName;
	int numberUsed = 0;

	tinyobj::attrib_t attributes;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	GLuint vertexbuffer, indexbuffer;
	std::vector<GLuint> textureIDs;
	std::unordered_map<std::pair<GLuint, int>, std::vector<Vertex>, pair_hash> vertexBuffers;
	std::unordered_map<std::pair<GLuint, int>, GLuint, pair_hash> vertexBufferIDs;
	glm::mat4 transform;
};

/**
 * @brief Create a Vertex from the given index.
 * @param attrib 
 * @param index 
 * @return Vertex 
*/
Vertex createVertexFromIndex(const tinyobj::attrib_t& attrib, const tinyobj::index_t& index);

/**
 * @brief Load the texture from the given filename.
 * @param filename name of the file
 * @return GLuint returns the texture ID
 */
GLuint loadTexture(const char* filename);

/**
 * @brief Initialize the window and OpenGL settings for the program.
 * @param GLFWwindow*& reference to the pointer of the window
 * @return void
 */
void init(GLFWwindow*& window);

/**
 * @brief Setup the user inputs for the window.
 * @param GLFWwindow* window
 * @return void
 */
void setupInput(GLFWwindow* window);

/**
 * @brief Initialize the OpenGL settings and culling.
 * @return void
 */
void initOpenGLSettings();

/**
 * @brief Initialize and bind the vertex array object to the openGL context.
 * @param GLuint& reference to the vertex array object
 * @return void

 */
void initializeVertexArrayObject(GLuint& VertexArrayID);

/**
 * @brief Initialize the projection matrix that corresponds to the camera space.
 * @param float _fov field of view
 * @param float _ratioWidth ratio width
 * @param float _ratioHeight ratio height
 * @param float _near near plane
 * @param float _far far plane
 * @return glm::mat4 returns the projection matrix
 */
glm::mat4 initializeProjectionMatrix(float _fov, float _ratioWidth, float _ratioHeight, float _near, float _far);

/**
 * @brief Initialize the view matrix that corresponds to the camera position and where it is looking.
 * @return glm::mat4 returns the view matrix
 */
glm::mat4 initializeViewMatrix(int _cameraPositionX, int _cameraPositionY, int _cameraPositionZ, int _cameraTargetX, int _cameraTargetY,
	int _cameraTargetZ, int _upVectorX, int _upVectorY, int _upVectorZ);

/**
 * @brief Setup the vertex attributes of the Model for the shader program to use.
 * @return void
 */
void setupVertexAttributes();

/**
 * @brief Setup the buffers for the Model to be drawn.
 * @param GLuint& reference to the vertex buffer
 * @param std::vector<Vertex> vertices
 * @param GLuint& reference to the index buffer
 * @param std::vector<unsigned int> indices
 * @return void
 */
void setupBuffers(GLuint& vertexbuffer, const std::vector<Vertex>& vertices, GLuint& indexbuffer, const std::vector<unsigned int>& indices);

/**
 * @brief Delete and clean all the handles of our render program.
 * @param GLFWwindow* window
 * @param std::vector<ModelComponent>& reference to the models
 * @param GLuint& program ID
 * @param GLuint& vertex array ID
 * @param GLuint& texture ID
 * @param GLuint& light ID
 * @param GLuint& reference to the material ambient color ID
 * @param GLuint& reference to the material diffuse color ID
 * @param GLuint& reference to the material specular color ID
 * @param GLuint& reference to the matrix ID
 * @param GLuint& reference to the view matrix ID
 * @param GLuint& reference to the model matrix ID
 * @return void
 */
void cleanup(GLFWwindow* window, ModelComponent& object);

/**
 * @brief Load the shaders from the given file paths.
 * @param const char* vertex_file_path
 * @param const char* fragment_file_path
 * @return GLuint returns the program ID
 * 
 */
void finishProgram(GLuint programID, GLuint VertexArrayID, GLuint& TextureID, GLuint& LightID, GLuint& MaterialAmbientColorID, GLuint& MaterialDiffuseColorID, GLuint& MaterialSpecularColorID, GLuint& MatrixID, GLuint& ViewMatrixID, GLuint& ModelMatrixID);

/**
* @brief Load the Model from the given filename and store the data in the model.
* @param filename name of the file
* @param ModelComponent reference to the model to store the data in
* @return void
*/
void loadingModel(const std::string& filename, ModelComponent& model);

/**
* @brief Load the textures from the Model.
* @param ModelComponent reference to the Model to store the data in
* @return void
*/
void loadTextures(ModelComponent& model);

/**
* @brief Load the mesh and textures from the given filename and store the data in obj.
* @param filename name of the file
* @param ModelComponent reference to the model
* @return void
*/
void loadObjAndTextures(const std::string& filename, ModelComponent& model);

/**
* @brief Apply batching to the model to regroup all faces using the same material for less draw calls.
* @param ModelComponent reference to the model
* @return void
*/
void batchingObj(ModelComponent& model);

/**
* @brief Load the obj and apply batching to the model.
* @param filename name of the file
* @param ModelComponent reference to the model
* @return void
*/
void loadObjAndBatching(const std::string& filename, ModelComponent& model);

/**
* @brief Add the new model to the models vector storing all the models to render.
* @param filename name of the file
* @param ModelComponent reference to the models vector
* @return void
*/
void addNewModel(const std::string& filename, std::string& fileDirection, std::vector<ModelComponent*>& models);

/**
* @brief Copy the model and add it to the models vector storing all the models to render.
* @param ModelComponent reference to the model
* @param models reference to the models vector
* @return void
*/
void copyModelAndAdd(const ModelComponent& model, std::vector<ModelComponent>& models);

/**
* @brief Setup the handles for the uniforms of the shader program.
* @param programID reference to the program ID
* @param TextureID reference to the texture ID
* @param MaterialAmbientColorID reference to the material ambient color ID
* @param MaterialDiffuseColorID reference to the material diffuse color ID
* @param MaterialSpecularColorID reference to the material specular color ID
* @param MatrixID reference to the matrix ID
* @param ViewMatrixID reference to the view matrix ID
* @param ModelMatrixID reference to the model matrix ID
* @return void
*/
void setupHandlesForUniforms(GLuint& programID, GLuint& TextureID, GLuint& MaterialAmbientColorID, GLuint& MaterialDiffuseColorID, GLuint& MaterialSpecularColorID, GLuint& MatrixID, GLuint& ViewMatrixID, GLuint& ModelMatrixID);

/**
* @brief Setup the handles for the light of the shader program.
* @param programID reference to the program ID
* @param LightID reference to the light ID
* @param LightColorID reference to the light color ID
* @param LightPowerID reference to the light power ID
*/
void setupLightHandles(GLuint& programID, GLuint& LightID, GLuint& LightColorID, GLuint& LightPowerID);

/**
* @brief Draw the model to the screen.
* @param ModelComponent reference to the model
* @param TextureID reference to the texture ID
* @param MaterialAmbientColorID reference to the material ambient color ID
* @param MaterialDiffuseColorID reference to the material diffuse color ID
* @param MaterialSpecularColorID reference to the material specular color ID
* @return void
*/
void drawModel(ModelComponent* model, GLuint TextureID, GLuint MaterialAmbientColorID, GLuint MaterialDiffuseColorID, GLuint MaterialSpecularColorID);

/**
* @brief Set the rotation of the model.
* @param ModelComponent reference to the model
* @param angle angle to rotate
* @param axis axis of the rotation
*/
void setRotationModel(ModelComponent& model, float angle, const glm::vec3& axis);

/**
* @brief Set the translation of the model.
* @param ModelComponent reference to the model
* @param translation translation vector
*/
void setTranslationModel(ModelComponent& model, const glm::vec3& translation);

/**
* @brief Set the scale of the model.
* @param ModelComponent reference to the model
* @param scale scale vector
*/
void setScaleModel(ModelComponent& model, const glm::vec3& scale);

/**
* @brief Translate the model by the given translation vector.
* @param model reference to the model
* @param translation translation vector
* @return void
*/
void translateModel(ModelComponent& model, const glm::vec3& translation);

/**
* @brief Rotate the model by the given angle and axis.
* @param ModelComponent reference to the model
* @param angle angle to rotate
* @param axis axis to rotate
* @return void
*/
void rotateModel(ModelComponent& model, float angle, const glm::vec3& axis);

/**
* @brief Scale the model by the given scale vector.
* @param ModelComponent reference to the model
* @param scale scale vector
* @return void
*/
void scaleModel(ModelComponent& model, const glm::vec3& scale);

/**
* @brief Send the MVP data to the shader program.
* @param ModelComponent reference to the model
* @param angle angle to rotate
* @param axis axis to rotate
* @param VertexArrayID reference to the vertex array ID
* @param MatrixID reference to the matrix ID
* @param ModelMatrixID reference to the model matrix ID
* @param ViewMatrixID reference to the view matrix ID
* @return void
*/
void sendMVPData(ModelComponent& model, GLuint VertexArrayID, GLuint MatrixID, GLuint ModelMatrixID, GLuint ViewMatrixID);

//#endif
