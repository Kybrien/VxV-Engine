//#include "Component.h"
//#include <GL/glew.h>
//#include <glm/glm.hpp>
//
// 
//class Renderer : public Component { 
//public:
//    GLuint VAO; // Vertex array object
//    GLuint VBO; // Vertex buffer object
//    GLuint shaderProgram; // Shader program ID
//
//    Renderer(GLuint shaderProgram) : shaderProgram(shaderProgram) {
//        // Initialize VAO, VBO, etc.
//    }
//
//    void Render() {
//        // Bind the shader
//        glUseProgram(shaderProgram);
//
//        // Bind the VAO
//        glBindVertexArray(VAO);
//
//        // Draw the object
//        glDrawArrays(GL_TRIANGLES, 0, 3); // Adjust based on your geometry
//
//        // Unbind the VAO
//        glBindVertexArray(0);
//    }
//
//    ~Renderer() {
//        // Cleanup VAO, VBO, etc.
//    }
//};