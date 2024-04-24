#include "GraphicAPI.hpp"
#include "loadingShader.hpp"

class APIopenGL : public GraphicAPI
{
	GLFWwindow* window;
	GLuint programID, VertexArrayID, TextureID, LightID, MaterialAmbientColorID, MaterialDiffuseColorID, MaterialSpecularColorID,
		MatrixID, ViewMatrixID, ModelMatrixID, LightColorID, LightPowerID;
	glm::mat4 Projection, View;
public :
	APIopenGL()
	{
		window = nullptr;
		programID = 0;
		VertexArrayID = 0;
		TextureID = 0;
		LightID = 0;
		MaterialAmbientColorID = 0;
		MaterialDiffuseColorID = 0;
		MaterialSpecularColorID = 0;
		MatrixID = 0;
		ViewMatrixID = 0;
		ModelMatrixID = 0;
		Projection = glm::mat4(1.0f);
		View = glm::mat4(1.0f);
	}

	void initialize() override
	{
		init(window);
		setupInput(window);
		initOpenGLSettings();
		initializeVertexArrayObject(VertexArrayID);
	}
	void cleanupModel(Model* _model) override
	{
		cleanup(window, *(_model->GetModel()));
	}
	void setWindowSize(int _width, int _height) override
	{
		glfwSetWindowSize(window, _width, _height);
	}
	void terminate() override
	{
		finishProgram(programID, VertexArrayID, TextureID, LightID, MaterialAmbientColorID, MaterialDiffuseColorID, MaterialSpecularColorID, MatrixID, ViewMatrixID, ModelMatrixID);
	}
	void setBackgroundColor(float _red, float _green, float _blue, float _alpha) override {
		glClearColor(_red, _green, _blue, _alpha);
	}
	void setCamera(float _fov, float _ratioWidth, float _ratioHeight, float _near, float _far,
		int _cameraPositionX, int _cameraPositionY, int _cameraPositionZ, int _cameraTargetX, int _cameraTargetY,
		int _cameraTargetZ, int _upVectorX, int _upVectorY, int _upVectorZ) override
	{
		Projection = initializeProjectionMatrix(_fov, _ratioWidth, _ratioHeight, _near, _far);
		View = initializeViewMatrix(_cameraPositionX, _cameraPositionY, _cameraPositionZ, _cameraTargetX,
			_cameraTargetY, _cameraTargetZ, _upVectorX, _upVectorY, _upVectorZ);
	}
	void setShaders(const char* _vertexShaderPath, const char* _fragmentShaderPath) override
	{
		programID = LoadShaders(_vertexShaderPath, _fragmentShaderPath);
	}
	void drawingModel(Model* _model) override
	{
		sendMVPData(*(_model->GetModel()), VertexArrayID, MatrixID, ModelMatrixID, ViewMatrixID);
		drawModel(_model->GetModel(), TextureID, MaterialAmbientColorID, MaterialDiffuseColorID, MaterialSpecularColorID);
	}

	void setHandles() override
	{
		setupHandlesForUniforms(programID, TextureID, MaterialAmbientColorID, MaterialDiffuseColorID, MaterialSpecularColorID, MatrixID, ViewMatrixID, ModelMatrixID);
		setupLightHandles(programID, LightID, LightColorID, LightPowerID);
		glBindVertexArray(0);
	}

	void clearScreen() override
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void unbindArrays() override
	{
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
		glBindVertexArray(0);
	}

	void swapBuffers() override
	{
		glfwSwapBuffers(window);
	}

	void useShader() override
	{
		glUseProgram(programID);
	}

	void setLightColor(glm::vec3 _lightColor) {
		glUniform3f(LightColorID, _lightColor.x, _lightColor.y, _lightColor.z);
	}

	void setLightPower(float _lightPower) {
		glUniform1f(LightPowerID, _lightPower);
	}

	GLFWwindow* getWindow() { return window; }

	GLuint getProgramID() { return programID; }

	GLuint getVertexArrayID() { return VertexArrayID; }

	GLuint getTextureID() { return TextureID; }

	GLuint getLightID() { return LightID; }

	GLuint getMaterialAmbientColorID() { return MaterialAmbientColorID; }

	GLuint getMaterialDiffuseColorID() { return MaterialDiffuseColorID; }

	GLuint getMaterialSpecularColorID() { return MaterialSpecularColorID; }

	GLuint getMatrixID() { return MatrixID; }

	GLuint getViewMatrixID() { return ViewMatrixID; }

	GLuint getModelMatrixID() { return ModelMatrixID; }

	glm::mat4 getProjection() { return Projection; }

	glm::mat4 getView() { return View; }
};