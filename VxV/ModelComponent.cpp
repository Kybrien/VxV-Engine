#include <GL/glew.h>

#define TINYOBJLOADER_IMPLEMENTATION

#include "ModelComponent.hpp"

#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "Externes/stb/stb_image.h"

Vertex createVertexFromIndex(const tinyobj::attrib_t& attrib, const tinyobj::index_t& index)
{
	Vertex vertex;

	// Position
	vertex.position.x = attrib.vertices[3 * index.vertex_index + 0];
	vertex.position.y = attrib.vertices[3 * index.vertex_index + 1];
	vertex.position.z = attrib.vertices[3 * index.vertex_index + 2];

	// Normal
	vertex.normal.x = attrib.normals[3 * index.normal_index + 0];
	vertex.normal.y = attrib.normals[3 * index.normal_index + 1];
	vertex.normal.z = attrib.normals[3 * index.normal_index + 2];

	// Texture Coordinate
	if (index.texcoord_index != -1)
	{
		vertex.texCoord.x = attrib.texcoords[2 * index.texcoord_index + 0];
		vertex.texCoord.y = 1.0f - attrib.texcoords[2 * index.texcoord_index + 1]; // Flip Y coordinate because OBJ's coordinate system is different from OpenGL's
	}
	else
	{
		vertex.texCoord = glm::vec2(0.0f, 0.0f);
	}

	return vertex;
}

GLuint loadTexture(const char* filename)
{
	int width, height, numComponents;
	//stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(filename, &width, &height, &numComponents, 4);
	if (data == NULL)
	{
		std::cerr << "Failed to load texture: " << filename << std::endl;
		return 0;
	}
	std::cout << "Loaded texture: " << filename << " (" << width << "x" << height << ")" << std::endl;
	GLuint texID;
	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	//glGenerateMipmap(GL_TEXTURE_2D);

	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(data);

	return texID;
}

void init(GLFWwindow*& window)
{
	// Initialise GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		exit(EXIT_FAILURE);
	}

	// Prepare the settings for the window
	glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We want OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We don't want the old OpenGL

	// Open a window and create its OpenGL context
	window = glfwCreateWindow(1280, 720, "VxV", NULL, NULL);
	if (window == NULL)
	{
		fprintf(stderr, "Failed to open GLFW window\n");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	// Initialize GLEW
	glfwMakeContextCurrent(window);
	glewExperimental = true;
	if (glewInit() != GLEW_OK)
	{
		fprintf(stderr, "Failed to initialize GLEW\n");
		exit(EXIT_FAILURE);
	}
}

void setupInput(GLFWwindow* window)
{
	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	// Set the mouse at the center of the screen
	glfwPollEvents();
	int width, height;
	glfwGetWindowSize(window, &width, &height);
	glfwSetCursorPos(window, width / 2, height / 2);
}

void initOpenGLSettings()
{
	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);
	// Cull triangles which normal is not towards the camera
	glEnable(GL_CULL_FACE);
}

void initializeVertexArrayObject(GLuint& VertexArrayID)
{
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);
}

glm::mat4 initializeProjectionMatrix()
{
	// Projection matrix: 45° Field of View, 4:3 ratio, display range: 0.1 unit <-> 100 units
	return glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
}

glm::mat4 initializeViewMatrix()
{
	// Camera matrix
	return glm::lookAt(glm::vec3(9, 5, 1), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
}

void setupVertexAttributes()
{
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, nullptr); // Position attribute
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(sizeof(float) * 3)); // Normal attribute
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(sizeof(float) * 6)); // Texture coordinate attribute
}

void setupBuffers(GLuint& vertexbuffer, const std::vector<Vertex>& vertices, GLuint& indexbuffer, const std::vector<unsigned int>& indices)
{
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &indexbuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexbuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
}

void cleanup(GLFWwindow* window, ModelComponent& model)
{
	// Cleanup VBO and shader
	glDeleteBuffers(1, &model.vertexbuffer);
	glDeleteBuffers(1, &model.indexbuffer);
	for (const auto& texID : model.textureIDs)
	{
		glDeleteTextures(1, &texID);
	}
	for (const auto& pair : model.vertexBufferIDs)
	{
		glDeleteBuffers(1, &pair.second);
		glDeleteBuffers(1, &pair.first.first);
	}
	for (const auto& pair : model.vertexBuffers)
	{
		glDeleteBuffers(1, &pair.first.first);
	}
}

void finishProgram(GLuint programID, GLuint VertexArrayID, GLuint& TextureID, GLuint& LightID, GLuint& MaterialAmbientColorID, GLuint& MaterialDiffuseColorID, GLuint& MaterialSpecularColorID, GLuint& MatrixID, GLuint& ViewMatrixID, GLuint& ModelMatrixID)
{
	glDeleteProgram(programID);
	glDeleteVertexArrays(1, &VertexArrayID);
	glDeleteVertexArrays(1, &TextureID);
	glDeleteVertexArrays(1, &LightID);
	glDeleteVertexArrays(1, &MaterialAmbientColorID);
	glDeleteVertexArrays(1, &MaterialDiffuseColorID);
	glDeleteVertexArrays(1, &MaterialSpecularColorID);
	glDeleteVertexArrays(1, &MatrixID);
	glDeleteVertexArrays(1, &ViewMatrixID);
	glDeleteVertexArrays(1, &ModelMatrixID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();
}

void loadingModel(const std::string& filename, ModelComponent& model)
{
	std::string warnings;
	std::string errors;
	tinyobj::LoadObj(&model.attributes, &model.shapes, &model.materials, &warnings, &errors, filename.c_str(), "Files/");
	// Before indexing
	size_t totalIndices = 0;
	for (const auto& shape : model.shapes)
	{
		totalIndices += shape.mesh.indices.size();
	}
	std::cout << "Before indexing - Total vertices: " << totalIndices / 3 << ", Total triangles: " << totalIndices / 3 << std::endl;

	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::unordered_map<Vertex, unsigned int> uniqueVertices;

	for (size_t s = 0; s < model.shapes.size(); s++)
	{
		tinyobj::mesh_t& mesh = model.shapes[s].mesh;
		for (size_t f = 0; f < mesh.indices.size(); f++)
		{
			tinyobj::index_t index = mesh.indices[f];

			Vertex vertex = {};
			vertex.position = {model.attributes.vertices[3 * index.vertex_index + 0], model.attributes.vertices[3 * index.vertex_index + 1], model.attributes.vertices[3 * index.vertex_index + 2]};
			vertex.normal = {model.attributes.normals[3 * index.normal_index + 0], model.attributes.normals[3 * index.normal_index + 1], model.attributes.normals[3 * index.normal_index + 2]};
			vertex.texCoord = {model.attributes.texcoords[2 * index.texcoord_index + 0], model.attributes.texcoords[2 * index.texcoord_index + 1]};

			if (uniqueVertices.count(vertex) == 0)
			{
				uniqueVertices[vertex] = static_cast<unsigned int>(vertices.size());
				vertices.push_back(vertex);
			}

			indices.push_back(uniqueVertices[vertex]);
		}
	}
	std::cout << "After indexing - Total vertices: " << uniqueVertices.size() << ", Total triangles: " << indices.size() / 3 << std::endl;
	setupBuffers(model.vertexbuffer, vertices, model.indexbuffer, indices);
}

void loadTextures(ModelComponent& model)
{
	for (const auto& material : model.materials)
	{
		if (!material.diffuse_texname.empty())
		{
			GLuint texID = loadTexture(material.diffuse_texname.c_str());
			model.textureIDs.push_back(texID);
		}
		else
		{
			model.textureIDs.push_back(0); // No texture for this material
		}
	}
	//if (textureIDs.size() == 0) {
	//	GLuint texID = loadTexture("image001.png");
	//	textureIDs.push_back(texID);
	//}
}

void loadObjAndTextures(const std::string& filename, ModelComponent& model)
{
	loadingModel(filename, model);
	printf("File name : %s\n", filename.c_str());
	if (model.materials.empty())
	{
		// Create a default material
		tinyobj::material_t defaultMaterial;
		defaultMaterial.name = "default";
		defaultMaterial.ambient[0] = 1.0f;
		defaultMaterial.ambient[1] = 1.0f;
		defaultMaterial.ambient[2] = 1.0f;

		defaultMaterial.diffuse[0] = 0.5f;
		defaultMaterial.diffuse[1] = 0.5f;
		defaultMaterial.diffuse[2] = 0.5f;

		defaultMaterial.specular[0] = 1.0f;
		defaultMaterial.specular[1] = 1.0f;
		defaultMaterial.specular[2] = 1.0f;

		// Add the default material to the materials vector
		model.materials.push_back(defaultMaterial);
	}
	loadTextures(model);
}

void batchingObj(ModelComponent& model)
{
	for (size_t s = 0; s < model.shapes.size(); s++)
	{
		tinyobj::mesh_t& mesh = model.shapes[s].mesh;
		for (size_t f = 0; f < mesh.indices.size(); f += 3)
		{
			int matID = mesh.material_ids[f / 3];
			GLuint texID;
			if (matID == -1)
			{
				// Use default material and texture
				texID = model.textureIDs.back();
				matID = model.materials.size() - 1;
			}
			else
			{
				texID = model.textureIDs[matID];
			}
			Vertex v1 = createVertexFromIndex(model.attributes, mesh.indices[f]);
			Vertex v2 = createVertexFromIndex(model.attributes, mesh.indices[f + 1]);
			Vertex v3 = createVertexFromIndex(model.attributes, mesh.indices[f + 2]);
			model.vertexBuffers[std::make_pair(texID, matID)].push_back(v1);
			model.vertexBuffers[std::make_pair(texID, matID)].push_back(v2);
			model.vertexBuffers[std::make_pair(texID, matID)].push_back(v3);
		}
	}

	for (const auto& pair : model.vertexBuffers)
	{
		GLuint vertexbuffer;
		glGenBuffers(1, &vertexbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * pair.second.size(), &pair.second[0], GL_STATIC_DRAW);
		model.vertexBufferIDs[pair.first] = vertexbuffer;
	}
}

void loadObjAndBatching(const std::string& filename, ModelComponent& model)
{
	loadObjAndTextures(filename, model);
	batchingObj(model);
}

void addNewModel(const std::string& filename, std::string& fileDirection, std::vector<ModelComponent*>& models)
{
	ModelComponent* model = new ModelComponent();
	model->fileName = filename;

	model->transform = glm::mat4(1.0f);
	loadObjAndBatching(fileDirection, *model);
	models.push_back(model);
}

void copyModelAndAdd(const ModelComponent& model, std::vector<ModelComponent>& models)
{
	ModelComponent newObj;
	newObj.attributes = model.attributes;
	newObj.shapes = model.shapes;
	newObj.materials = model.materials;
	newObj.textureIDs = model.textureIDs;
	newObj.vertexBuffers = model.vertexBuffers;
	newObj.vertexBufferIDs = model.vertexBufferIDs;
	newObj.transform = model.transform;
	models.push_back(newObj);
}

void setupHandlesForUniforms(GLuint& programID, GLuint& TextureID, GLuint& LightID, GLuint& MaterialAmbientColorID, GLuint& MaterialDiffuseColorID, GLuint& MaterialSpecularColorID, GLuint& MatrixID, GLuint& ViewMatrixID, GLuint& ModelMatrixID)
{
	glUseProgram(programID);
	TextureID = glGetUniformLocation(programID, "myTextureSampler");
	LightID = glGetUniformLocation(programID, "LightPosition_worldspace");
	MaterialAmbientColorID = glGetUniformLocation(programID, "MaterialAmbientColor");
	MaterialDiffuseColorID = glGetUniformLocation(programID, "MaterialDiffuseColor");
	MaterialSpecularColorID = glGetUniformLocation(programID, "MaterialSpecularColor");
	MatrixID = glGetUniformLocation(programID, "MVP");
	ViewMatrixID = glGetUniformLocation(programID, "V");
	ModelMatrixID = glGetUniformLocation(programID, "M");
}

void drawModel(ModelComponent* model, GLuint TextureID, GLuint MaterialAmbientColorID, GLuint MaterialDiffuseColorID, GLuint MaterialSpecularColorID)
{
	for (const auto& pair : model->vertexBuffers)
	{
		GLuint texID = pair.first.first;
		int matID = pair.first.second;
		const std::vector<Vertex>& vertices = pair.second;

		// Bind the texture
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texID);
		glUniform1i(TextureID, 0);

		// Set the material properties
		tinyobj::material_t& material = model->materials[matID];
		GLfloat ambient[3] = {material.ambient[0], material.ambient[1], material.ambient[2]};
		GLfloat diffuse[3] = {material.diffuse[0], material.diffuse[1], material.diffuse[2]};
		GLfloat specular[3] = {material.specular[0], material.specular[1], material.specular[2]};
		glUniform3fv(MaterialAmbientColorID, 1, ambient);
		glUniform3fv(MaterialDiffuseColorID, 1, diffuse);
		glUniform3fv(MaterialSpecularColorID, 1, specular);

		// Bind the vertex buffer
		glBindBuffer(GL_ARRAY_BUFFER, model->vertexBufferIDs[pair.first]);

		// Draw the vertices
		setupVertexAttributes();
		glDrawArrays(GL_TRIANGLES, 0, vertices.size());
	}
}

void translateModel(ModelComponent& model, const glm::vec3& translation)
{
	model.transform = glm::translate(model.transform, translation);
}

void rotateModel(ModelComponent& model, float angle, const glm::vec3& axis)
{
	model.transform = glm::rotate(model.transform, glm::radians(angle), axis);
}

void scaleModel(ModelComponent& model, const glm::vec3& scale)
{
	model.transform = glm::scale(model.transform, scale);
}

void sendMVPData(ModelComponent& model, float angle, const glm::vec3& axis, GLuint VertexArrayID, GLuint MatrixID, GLuint ModelMatrixID, GLuint ViewMatrixID)
{
	glm::vec3 position = glm::vec3(model.transform[3].x, model.transform[3].y, model.transform[3].z);
	glm::vec3 scale = glm::vec3(glm::length(model.transform[0]), glm::length(model.transform[1]), glm::length(model.transform[2]));
	model.transform = glm::mat4(1.0f);
	model.transform = glm::scale(model.transform, scale);
	model.transform = glm::rotate(glm::mat4(1.0f), glm::radians(angle), axis) * model.transform;
	model.transform = glm::translate(glm::mat4(1.0f), position) * model.transform;

	glm::mat4 ModelMatrix = glm::mat4(1.0f) * model.transform;
	glm::mat4 ViewMatrix = getViewMatrix();
	glm::mat4 MVP = getProjectionMatrix() * ViewMatrix * ModelMatrix;

	glBindVertexArray(VertexArrayID);
	// Send our transformation to the currently bound shader, 
	// in the "MVP" uniform
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
	glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
	glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &ViewMatrix[0][0]);
}
