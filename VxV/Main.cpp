#include <stdio.h>
#include <stdlib.h>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <loadShader.hpp>

using namespace glm;

#include <controls.hpp>
#include <texture.hpp>
#include <objloader.hpp>

int main() {
	if (!glfwInit()) {
		fprintf(stderr, "Failed to initialize GLFW\n");
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We want OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We don't want the old OpenGL

	// Open a window and create its OpenGL context
	GLFWwindow* window;
	window = glfwCreateWindow(1280, 720, "VxV", NULL, NULL);
	if (window == NULL) {
		fprintf(stderr, "Failed to open GLFW window\n");
		glfwTerminate();
		return -1;
	}
	
	glfwMakeContextCurrent(window);// Initialize GLEW

	glewExperimental = true;
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		return -1;
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	// Hide the mouse and enable unlimited movement
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Set the mouse at the center of the screen
	glfwPollEvents();
	// Reset mouse position for next frame
	int width, height;
	glfwGetWindowSize(window, &width, &height);
	glfwSetCursorPos(window, width / 2, height / 2);
	glfwSetCursorPos(window, width / 2, height / 2);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);

	// Cull triangles which normal is not towards the camera
	glEnable(GL_CULL_FACE);

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// Create and compile our GLSL program from the shaders
	GLuint programID = LoadShaders("SimpleVertexShader.MIKU", "SimpleFragmentShader.VALORANT");
	glClearColor(0.0f, 0.0f, 0.5f, 0.0f);

	// Projection matrix: 45° Field of View, 4:3 ratio, display range: 0.1 unit <-> 100 units
	glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
	// Or, for an ortho camera:
	//glm::mat4 Projection = glm::ortho(-10.0f,10.0f,-10.0f,10.0f,6.0f,100.0f); // In world coordinates

	// Camera matrix
	glm::mat4 View = glm::lookAt(
		glm::vec3(9, 5, 10), // Camera is at (4,3,3), in World Space
		glm::vec3(0, 0, 0), // and looks at the origin
		glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
	);

	/*glm::mat4 myScalingMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(4.f, 0.5f, 0.5f));
	glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.5f, 0.0f, 0.0f));
	glm::mat4 myRotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(90.f), glm::vec3(0.0f, 0.0f, 1.0f));*/


	// Get a handle for our "MVP" uniform
	// Only during the initialisation
	GLuint MatrixID = glGetUniformLocation(programID, "MVP");
	GLuint ViewMatrixID = glGetUniformLocation(programID, "V");
	GLuint ModelMatrixID = glGetUniformLocation(programID, "M");

	// Load the texture using any two methods
	//GLuint Texture = loadBMP_custom("uvtemplate.bmp");
	GLuint Texture = loadDDS("uvmap.DDS");

	// Get a handle for our "myTextureSampler" uniform
	GLuint TextureID = glGetUniformLocation(programID, "myTextureSampler");

	//// Send our transformation to the currently bound shader, in the "MVP" uniform
	//// This is done in the main loop since each model will have a different MVP matrix (At least for the M part)
	//glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

	//// Our vertices. Tree consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
	//// A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
	//static const GLfloat g_vertex_buffer_data[] = {
	//	-1.0f,-1.0f,-1.0f,
	//	-1.0f,-1.0f, 1.0f,
	//	-1.0f, 1.0f, 1.0f,
	//	 1.0f, 1.0f,-1.0f,
	//	-1.0f,-1.0f,-1.0f,
	//	-1.0f, 1.0f,-1.0f,
	//	 1.0f,-1.0f, 1.0f,
	//	-1.0f,-1.0f,-1.0f,
	//	 1.0f,-1.0f,-1.0f,
	//	 1.0f, 1.0f,-1.0f,
	//	 1.0f,-1.0f,-1.0f,
	//	-1.0f,-1.0f,-1.0f,
	//	-1.0f,-1.0f,-1.0f,
	//	-1.0f, 1.0f, 1.0f,
	//	-1.0f, 1.0f,-1.0f,
	//	 1.0f,-1.0f, 1.0f,
	//	-1.0f,-1.0f, 1.0f,
	//	-1.0f,-1.0f,-1.0f,
	//	-1.0f, 1.0f, 1.0f,
	//	-1.0f,-1.0f, 1.0f,
	//	 1.0f,-1.0f, 1.0f,
	//	 1.0f, 1.0f, 1.0f,
	//	 1.0f,-1.0f,-1.0f,
	//	 1.0f, 1.0f,-1.0f,
	//	 1.0f,-1.0f,-1.0f,
	//	 1.0f, 1.0f, 1.0f,
	//	 1.0f,-1.0f, 1.0f,
	//	 1.0f, 1.0f, 1.0f,
	//	 1.0f, 1.0f,-1.0f,
	//	-1.0f, 1.0f,-1.0f,
	//	 1.0f, 1.0f, 1.0f,
	//	-1.0f, 1.0f,-1.0f,
	//	-1.0f, 1.0f, 1.0f,
	//	 1.0f, 1.0f, 1.0f,
	//	-1.0f, 1.0f, 1.0f,
	//	 1.0f,-1.0f, 1.0f
	//};
	// Read our .obj file
	std::vector< glm::vec3 > vertices;
	std::vector< glm::vec2 > uvs;
	std::vector< glm::vec3 > normals; // Won't be used at the moment.
	bool res = loadOBJ("suzanne.obj", vertices, uvs, normals);
	//// One color for each vertex. They were generated randomly.
	//static const GLfloat g_color_buffer_data[] = {
	//	1.0f, 0.737f, 0.85f,
	//	1.0f, 0.737f, 0.85f,
	//	1.0f, 0.737f, 0.85f,
	//	1.0f, 0.737f, 0.85f,
	//	1.0f, 0.737f, 0.85f,
	//	1.0f, 0.737f, 0.85f,
	//	1.0f, 0.737f, 0.85f,
	//	1.0f, 0.737f, 0.85f,
	//	1.0f, 0.737f, 0.85f,
	//	1.0f, 0.737f, 0.85f,
	//	1.0f, 0.737f, 0.85f,
	//	1.0f, 0.737f, 0.85f,
	//	1.0f, 0.737f, 0.85f,
	//	1.0f, 0.737f, 0.85f,
	//	1.0f, 0.737f, 0.85f,
	//	1.0f, 0.737f, 0.85f,
	//	1.0f, 0.737f, 0.85f,
	//	1.0f, 0.737f, 0.85f,
	//	1.0f, 0.737f, 0.85f,
	//	1.0f, 0.737f, 0.85f,
	//	1.0f, 0.737f, 0.85f,
	//	1.0f, 0.737f, 0.85f,
	//	1.0f, 0.737f, 0.85f,
	//	1.0f, 0.737f, 0.85f,
	//	1.0f, 0.737f, 0.85f,
	//	1.0f, 0.737f, 0.85f,
	//	1.0f, 0.737f, 0.85f,
	//	1.0f, 0.737f, 0.85f,
	//	1.0f, 0.737f, 0.85f,
	//	1.0f, 0.737f, 0.85f,
	//	1.0f, 0.737f, 0.85f,
	//	1.0f, 0.737f, 0.85f,
	//	1.0f, 0.737f, 0.85f,
	//	1.0f, 0.737f, 0.85f,
	//	0.0f, 0.0f, 0.0f,
	//	0.0f, 0.0f, 0.0f,
	//};

	// Two UV coordinatesfor each vertex. They were created with Blender.
	/*static const GLfloat g_uv_buffer_data[] = {
		0.000059f, 1.0f - 0.000004f,
		0.000103f, 1.0f - 0.336048f,
		0.335973f, 1.0f - 0.335903f,
		1.000023f, 1.0f - 0.000013f,
		0.667979f, 1.0f - 0.335851f,
		0.999958f, 1.0f - 0.336064f,
		0.667979f, 1.0f - 0.335851f,
		0.336024f, 1.0f - 0.671877f,
		0.667969f, 1.0f - 0.671889f,
		1.000023f, 1.0f - 0.000013f,
		0.668104f, 1.0f - 0.000013f,
		0.667979f, 1.0f - 0.335851f,
		0.000059f, 1.0f - 0.000004f,
		0.335973f, 1.0f - 0.335903f,
		0.336098f, 1.0f - 0.000071f,
		0.667979f, 1.0f - 0.335851f,
		0.335973f, 1.0f - 0.335903f,
		0.336024f, 1.0f - 0.671877f,
		1.000004f, 1.0f - 0.671847f,
		0.999958f, 1.0f - 0.336064f,
		0.667979f, 1.0f - 0.335851f,
		0.668104f, 1.0f - 0.000013f,
		0.335973f, 1.0f - 0.335903f,
		0.667979f, 1.0f - 0.335851f,
		0.335973f, 1.0f - 0.335903f,
		0.668104f, 1.0f - 0.000013f,
		0.336098f, 1.0f - 0.000071f,
		0.000103f, 1.0f - 0.336048f,
		0.000004f, 1.0f - 0.671870f,
		0.336024f, 1.0f - 0.671877f,
		0.000103f, 1.0f - 0.336048f,
		0.336024f, 1.0f - 0.671877f,
		0.335973f, 1.0f - 0.335903f,
		0.667969f, 1.0f - 0.671889f,
		1.000004f, 1.0f - 0.671847f,
		0.667979f, 1.0f - 0.335851f
	};*/
	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

	GLuint uvbuffer;
	glGenBuffers(1, &uvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);

	GLuint normalbuffer;
	glGenBuffers(1, &normalbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);

	//GLuint colorbuffer;
	//glGenBuffers(1, &colorbuffer);
	//glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);

	glBindVertexArray(0);

	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(3.f, 0.0f, 0.0f));
	//glm::mat4 myRotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(90.f), glm::vec3(1.0f, 0.0f, 0.0));
	glm::mat4 model2 = translationMatrix ;
	glm::mat4 MVP2 = Projection * View * model2;

	static const GLfloat g_vertex_buffer_data2[] = {
		// Positions        
		-1.0f, 1.0f, 1.0f,
		1.0f, -1.0f,1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f,-1.0f,-1.0f,
		1.0f, -1.0f,1.0f,
		-1.0f,-1.0f, 1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f,-1.0f, 1.0f,
		-1.0f,1.0f, 1.0f,
		1.0f, -1.0f,1.0f,
		-1.0f,1.0f, 1.0f,
		-1.0f,-1.0f, 1.0f,

	};

	//static const GLfloat g_color_buffer_data2[] = {
	//	// Positions        
	//	1.0f, 0.0f, 0.0f,
	//	0.0f, 1.0f, 0.0f,
	//	0.0f, 0.0f, 1.0f,
	//	1.0f, 0.0f, 0.0f,
	//	0.0f, 1.0f, 0.0f,
	//	0.0f, 0.0f, 1.0f,
	//	1.0f, 0.0f, 0.0f,
	//	0.0f, 1.0f, 0.0f,
	//	0.0f, 0.0f, 1.0f,
	//	1.0f, 0.0f, 0.0f,
	//	0.0f, 1.0f, 0.0f,
	//	0.0f, 0.0f, 1.0f
	//};

	GLuint vertexbuffer2;
	glGenBuffers(1, &vertexbuffer2);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data2), g_vertex_buffer_data2, GL_STATIC_DRAW);

	/*GLuint colorbuffer2;
	glGenBuffers(1, &colorbuffer2);
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data2), g_color_buffer_data2, GL_STATIC_DRAW);*/

	// Get a handle for our "LightPosition" uniform
	glUseProgram(programID);
	GLuint LightID = glGetUniformLocation(programID, "LightPosition_worldspace");

	glBindVertexArray(0);
	do {
		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Use our shader
		glUseProgram(programID);
		
		// Compute the MVP matrix from keyboard and mouse input
		computeMatricesFromInputs(window);
		glm::mat4 ProjectionMatrix = getProjectionMatrix();
		glm::mat4 ViewMatrix = getViewMatrix();
		glm::mat4 ModelMatrix = glm::mat4(1.0);
		glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
		
		glBindVertexArray(VertexArrayID);
		// Send our transformation to the currently bound shader, 
		// in the "MVP" uniform
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
		glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &ViewMatrix[0][0]);
		// 1st attribute buffer : vertices
		// Bind our texture in Texture Unit 0
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, Texture);
		// Set our "myTextureSampler" sampler to use Texture Unit 0
		glUniform1i(TextureID, 0);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(
			0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);
		// 2nd attribute buffer : colors
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
		glVertexAttribPointer(
			1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
			2,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);

		// 3rd attribute buffer : normals
		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
		glVertexAttribPointer(
			2,                                // attribute
			3,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);


		// Draw the triangle !
		glDrawArrays(GL_TRIANGLES, 0, vertices.size()); // 12*3 indices starting at 0 -> 12 triangles

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
		glBindVertexArray(0);

		//// Compute the MVP matrix from keyboard and mouse input
		//computeMatricesFromInputs(window);
		//glm::mat4 ProjectionMatrix2 = getProjectionMatrix();
		//glm::mat4 ViewMatrix2 = getViewMatrix();
		//glm::mat4 ModelMatrix2 = model2;
		//glm::mat4 MVP2 = ProjectionMatrix2 * ViewMatrix2 * ModelMatrix2;

		//glBindVertexArray(VAO);

		//glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP2[0][0]);
		//// 1st attribute buffer : vertices
		//glEnableVertexAttribArray(0);
		//glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer2);
		//glVertexAttribPointer(
		//	0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		//	3,                  // size
		//	GL_FLOAT,           // type
		//	GL_FALSE,           // normalized?
		//	0,                  // stride
		//	(void*)0            // array buffer offset
		//);
		//// 2nd attribute buffer : colors
		//glEnableVertexAttribArray(1);
		//glBindBuffer(GL_ARRAY_BUFFER, colorbuffer2);
		//glVertexAttribPointer(
		//	1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
		//	3,                                // size
		//	GL_FLOAT,                         // type
		//	GL_FALSE,                         // normalized?
		//	0,                                // stride
		//	(void*)0                          // array buffer offset
		//);

		//// Draw the triangle !
		//glDrawArrays(GL_TRIANGLES, 0, 4 * 3); // 12*3 indices starting at 0 -> 12 triangles

		//glDisableVertexAttribArray(0);
		//glDisableVertexAttribArray(1);
		//glBindVertexArray(0);
		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	} while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);

	// Cleanup VBO and shader
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteBuffers(1, &uvbuffer);
	glDeleteBuffers(1, &normalbuffer);
	glDeleteProgram(programID);
	glDeleteTextures(1, &Texture);
	glDeleteVertexArrays(1, &VertexArrayID);
	glDeleteVertexArrays(1, &VAO);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}	
