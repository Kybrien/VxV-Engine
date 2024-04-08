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

Vertex createVertexFromIndex(const tinyobj::attrib_t& attrib, const tinyobj::index_t& index);
GLuint loadTexture(const char* filename);
void init(GLFWwindow*& window);
void setupInput(GLFWwindow* window);
void initOpenGLSettings();
void initializeVertexArrayObject(GLuint& VertexArrayID);
glm::mat4 initializeProjectionMatrix();
glm::mat4 initializeViewMatrix();
void setupVertexAttributes();
void setupBuffers(GLuint& vertexbuffer, const std::vector<Vertex>& vertices, GLuint& indexbuffer, const std::vector<unsigned int>& indices);
void cleanup(GLFWwindow* window, GLuint vertexbuffer, GLuint programID, GLuint VertexArrayID);
void loadingObject(const std::string& filename, tinyobj::attrib_t& attributes, std::vector<tinyobj::shape_t>& shapes,
	std::vector<tinyobj::material_t>& materials, GLuint& vertexbuffer, GLuint& indexbuffer);
void loadTextures(const std::vector<tinyobj::material_t>& materials, std::vector<GLuint>& textureIDs);
void loadObjAndTextures(const std::string& filename, tinyobj::attrib_t& attributes, std::vector<tinyobj::shape_t>& shapes,
	std::vector<tinyobj::material_t>& materials, GLuint& vertexbuffer, GLuint& indexbuffer, std::vector<GLuint>& textureIDs);
void batchingObj(std::vector<tinyobj::shape_t>& shapes, tinyobj::attrib_t& attributes, std::vector<GLuint>& textureIDs,
	std::unordered_map<std::pair<GLuint, int>, std::vector<Vertex>, pair_hash>& vertexBuffers,
	std::unordered_map<std::pair<GLuint, int>, GLuint, pair_hash>& vertexBufferIDs);
void loadObjAndBatching(const std::string& filename, tinyobj::attrib_t& attributes, std::vector<tinyobj::shape_t>& shapes,
	std::vector<tinyobj::material_t>& materials, GLuint& vertexbuffer, GLuint& indexbuffer, std::vector<GLuint>& textureIDs,
	std::unordered_map<std::pair<GLuint, int>, std::vector<Vertex>, pair_hash>& vertexBuffers,
	std::unordered_map<std::pair<GLuint, int>, GLuint, pair_hash>& vertexBufferIDs);
void addNewObject(const std::string& filename, std::vector<Object>& objects);
void copyObjectAndAdd(const Object& obj, std::vector<Object>& objects);
void setupHandlesForUniforms(GLuint& programID, GLuint& TextureID, GLuint& LightID, GLuint& MaterialAmbientColorID,
	GLuint& MaterialDiffuseColorID, GLuint& MaterialSpecularColorID, GLuint& MatrixID, GLuint& ViewMatrixID, GLuint& ModelMatrixID);
void drawObjects(std::vector<Object>& objects, GLuint TextureID, GLuint MaterialAmbientColorID, GLuint MaterialDiffuseColorID,
	GLuint MaterialSpecularColorID);
void translateObject(Object& object, const glm::vec3& translation);
void rotateObject(Object& object, float angle, const glm::vec3& axis);
void scaleObject(Object& object, const glm::vec3& scale);
void sendMVPData(Object& object, float angle, const glm::vec3& axis, GLuint VertexArrayID, GLuint MatrixID,
	GLuint ModelMatrixID, GLuint ViewMatrixID);

#endif