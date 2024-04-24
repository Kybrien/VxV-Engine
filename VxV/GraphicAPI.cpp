#include <string>
#include "Model.h"

class GraphicAPI
{
	virtual ~GraphicAPI() = default;
	virtual void initialize() = 0;
	virtual void cleanupModel(Model* _model) = 0;
	virtual void setWindowSize(int _width, int _height) = 0;
	virtual void terminate() = 0;
	virtual void setBackgroundColor(float _red,float _green, float _blue, float _alpha) = 0;
	virtual void setCamera() = 0;
	virtual void setShaders(std::string _vertexShaderPath, std::string _fragmentShaderPath) = 0;
	virtual void drawModel(Model* _model) = 0;
	virtual void setHandles() = 0;
};

class OpenGLAPI : public GraphicAPI
{
	GLFWwindow* window;
	GLuint VertexArrayID,TextureID, LightID, MaterialAmbientColorID, MaterialDiffuseColorID, MaterialSpecularColorID,
		MatrixID, ViewMatrixID, ModelMatrixID;
	glm::mat4 Projection, View;
	void initialize() override
	{
		init(window);
		setupInput(window);
	}
	void cleanupModel(Model* _model) override
	{
		cleanup(window, _model->GetModel);
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
	void setShaders(std::string vertexShaderPath, std::string fragmentShaderPath) override
	{
		programID = LoadShaders(vertexShaderPath, fragmentShaderPath);
	}
	void drawModel(Model* _model) override
	{
		sendMVPData(*(_model->GetModel()), VertexArrayID, MatrixID, ModelMatrixID, ViewMatrixID);
		drawModel(_model->GetModel(), TextureID, MaterialAmbientColorID, MaterialDiffuseColorID, MaterialSpecularColorID);
	}

	void setHandle() override
	{
		setupHandlesForUniforms(programID, TextureID, LightID, MaterialAmbientColorID, MaterialDiffuseColorID, MaterialSpecularColorID, MatrixID, ViewMatrixID, ModelMatrixID);
	}
};