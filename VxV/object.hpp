#ifndef OBJECT_HPP
#define OBJECT_HPP
#include <stdio.h>
#include <unordered_map>

#include <GLM/glm.hpp>
#include "Externes/stb/stb_image.h"
#include "Externes/tiny_obj_loader.h"
using namespace glm;

#include "loadingShader.hpp"
#include "controls.hpp"


class Vertex {
public:
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoord;

	bool operator==(const Vertex& other) const {
		return position == other.position && normal == other.normal && texCoord == other.texCoord;
	}
};

struct pair_hash {
	template <class T1, class T2>
	std::size_t operator () (const std::pair<T1, T2>& p) const {
		auto h1 = std::hash<T1>{}(p.first);
		auto h2 = std::hash<T2>{}(p.second);

		// Mainly for demonstration purposes, i.e. works but is overly simple
		// In the real world, use sth. like boost.hash_combine
		return h1 ^ h2;
	}
};

namespace std {
	template<> struct hash<glm::vec3> {
		size_t operator()(glm::vec3 const& vec) const {
			return ((hash<float>()(vec.x) ^ (hash<float>()(vec.y) << 1)) >> 1) ^ (hash<float>()(vec.z) << 1);
		}
	};

	template<> struct hash<glm::vec2> {
		size_t operator()(glm::vec2 const& vec) const {
			return hash<float>()(vec.x) ^ (hash<float>()(vec.y) << 1);
		}
	};

	template<> struct hash<Vertex> {
		size_t operator()(Vertex const& vertex) const {
			return ((hash<glm::vec3>()(vertex.position) ^ (hash<glm::vec3>()(vertex.normal) << 1)) >> 1) ^ (hash<glm::vec2>()(vertex.texCoord) << 1);
		}
	};
	struct Vec3Hash {
		size_t operator()(const glm::vec3& vec) const {
			return ((hash<float>()(vec.x) ^ (hash<float>()(vec.y) << 1)) >> 1) ^ (hash<float>()(vec.z) << 1);
		}
	};

	struct Vec2Hash {
		size_t operator()(const glm::vec2& vec) const {
			return hash<float>()(vec.x) ^ (hash<float>()(vec.y) << 1);
		}
	};

	struct VertexHash {
		size_t operator()(const Vertex& vertex) const {
			return ((Vec3Hash()(vertex.position) ^ (Vec3Hash()(vertex.normal) << 1)) >> 1) ^ (Vec2Hash()(vertex.texCoord) << 1);
		}
	};
}

class Object {
public:
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
 * @brief Create a Vertex object from the given index.
 * * @param attrib 
 * * @param index 
 * * @return Vertex 
 * 
*/
Vertex createVertexFromIndex(const tinyobj::attrib_t& attrib, const tinyobj::index_t& index);
/**
 * @brief Load the texture from the given filename.
 * * @param filename name of the file
 * * @return GLuint returns the texture ID
 * 
 */
GLuint loadTexture(const char* filename);
/**
 * @brief Initialize the window and OpenGL settings for the program.
 * * @param GLFWwindow*& reference to the pointer of the window
 * * @return void
 * 
 */
void init(GLFWwindow*& window);
/**
 * @brief Setup the user inputs for the window.
 * * @param GLFWwindow* window
 * * @return void
 * 
 */
void setupInput(GLFWwindow* window);
/**
 * @brief Initialize the OpenGL settings and culling.
 * * @return void
 * 
 */
void initOpenGLSettings();
/**
 * @brief Initialize and bind the vertex array object to the openGL context.
 * * @param GLuint& reference to the vertex array object
 * * @return void
 * 
 */
void initializeVertexArrayObject(GLuint& VertexArrayID);
/**
 * @brief Initialize the projection matrix that corresponds to the camera space.
 * * @return glm::mat4 returns the projection matrix
 * 
 */
glm::mat4 initializeProjectionMatrix();
/**
 * @brief Initialize the view matrix that corresponds to the camera position and where it is looking.
 * * @return glm::mat4 returns the view matrix
 * 
 */
glm::mat4 initializeViewMatrix();
/**
 * @brief Setup the vertex attributes for the object for the shader program to use.
 * * @return void
 * 
 */
void setupVertexAttributes();
/**
 * @brief Setup the buffers for the object to be drawn.
 * * @param GLuint& reference to the vertex buffer
 * * @param std::vector<Vertex> vertices
 * * @param GLuint& reference to the index buffer
 * * @param std::vector<unsigned int> indices
 * * @return void
 * 
 */
void setupBuffers(GLuint& vertexbuffer, const std::vector<Vertex>& vertices, GLuint& indexbuffer, const std::vector<unsigned int>& indices);
/**
 * @brief Delete and clean all the handles of our render program.
 * * @param GLFWwindow* window
 * * @param std::vector<Object>& reference to the objects
 * * @param GLuint& program ID
 * * @param GLuint& vertex array ID
 * * @param GLuint& texture ID
 * * @param GLuint& light ID
 * * @param GLuint& reference to the material ambient color ID
 * * @param GLuint& reference to the material diffuse color ID
 * * @param GLuint& reference to the material specular color ID
 * * @param GLuint& reference to the matrix ID
 * * @param GLuint& reference to the view matrix ID
 * * @param GLuint& reference to the model matrix ID
 * * @return void
 * 
 */
void cleanup(GLFWwindow* window, std::vector<Object>& objects, GLuint programID, GLuint VertexArrayID, GLuint& TextureID, GLuint& LightID,
	GLuint& MaterialAmbientColorID, GLuint& MaterialDiffuseColorID, GLuint& MaterialSpecularColorID, GLuint& MatrixID, GLuint& ViewMatrixID,
	GLuint& ModelMatrixID);
/**
* @brief Load the object from the given filename and store the data in obj.
* @param filename name of the file
* @param obj reference to the object
* @return void
*/
void loadingObject(const std::string& filename, Object& obj);
/**
* @brief Load the textures from the object.
* @param obj reference to the object
* @return void
*/
void loadTextures(Object& obj);
/**
* @brief Load the object and textures from the given filename and store the data in obj.
* @param filename name of the file
* @param obj reference to the object
* @return void
*/
void loadObjAndTextures(const std::string& filename, Object& obj);
/**
* @brief Apply batching to the object to regroup all faces using the same material for less draw calls.
* @param obj reference to the object
* @return void
*/ 
void batchingObj(Object& obj);
/**
* @brief Load the object and apply batching to the object.
* @param filename name of the file
* @param obj reference to the object
* @return void
*/ 
void loadObjAndBatching(const std::string& filename, Object& obj);
/**
* @brief Add the new object to the objects vector storing all the objects to render.
* @param filename name of the file
* @param objects reference to the objects vector
* @return void
*/ 
void addNewObject(const std::string& filename, std::vector<Object>& objects);
/**
* @brief Copy the object and add it to the objects vector storing all the objects to render.
* @param obj reference to the object
* @param objects reference to the objects vector
* @return void
*/
void copyObjectAndAdd(const Object& obj, std::vector<Object>& objects);
/**
* @brief Setup the handles for the uniforms of the shader program.
* @param programID reference to the program ID
* @param TextureID reference to the texture ID
* @param LightID reference to the light ID
* @param MaterialAmbientColorID reference to the material ambient color ID
* @param MaterialDiffuseColorID reference to the material diffuse color ID
* @param MaterialSpecularColorID reference to the material specular color ID
* @param MatrixID reference to the matrix ID
* @param ViewMatrixID reference to the view matrix ID
* @param ModelMatrixID reference to the model matrix ID
* @return void
*/ 
void setupHandlesForUniforms(GLuint& programID, GLuint& TextureID, GLuint& LightID, GLuint& MaterialAmbientColorID,
	GLuint& MaterialDiffuseColorID, GLuint& MaterialSpecularColorID, GLuint& MatrixID, GLuint& ViewMatrixID, GLuint& ModelMatrixID);
/**
* @brief Draw the object to the screen.
* @param object reference to the object
* @param TextureID reference to the texture ID
* @param MaterialAmbientColorID reference to the material ambient color ID
* @param MaterialDiffuseColorID reference to the material diffuse color ID
* @param MaterialSpecularColorID reference to the material specular color ID
* @return void
*/
void drawObject(Object& object, GLuint TextureID, GLuint MaterialAmbientColorID, GLuint MaterialDiffuseColorID,
	GLuint MaterialSpecularColorID);
/**
* @brief Translate the object by the given translation vector.
* @param object reference to the object
* @param translation translation vector
* @return void
*/
void translateObject(Object& object, const glm::vec3& translation);
/**
* @brief Rotate the object by the given angle and axis.
* @param object reference to the object
* @param angle angle to rotate
* @param axis axis to rotate
* @return void
*/
void rotateObject(Object& object, float angle, const glm::vec3& axis);
/**
* @brief Scale the object by the given scale vector.
* @param object reference to the object
* @param scale scale vector
* @return void
*/
void scaleObject(Object& object, const glm::vec3& scale);
/**
* @brief Send the MVP data to the shader program.
* @param object reference to the object
* @param angle angle to rotate
* @param axis axis to rotate
* @param VertexArrayID reference to the vertex array ID
* @param MatrixID reference to the matrix ID
* @param ModelMatrixID reference to the model matrix ID
* @param ViewMatrixID reference to the view matrix ID
* @return void
*/
void sendMVPData(Object& object, float angle, const glm::vec3& axis, GLuint VertexArrayID, GLuint MatrixID,
	GLuint ModelMatrixID, GLuint ViewMatrixID);

#endif