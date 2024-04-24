#include <string>
#include "Model.h"

class GraphicAPI
{
	virtual ~GraphicAPI() = default;
	virtual void initialize() = 0;
	virtual void cleanup() = 0;
	virtual void setWindowSize(int width, int height) = 0;
	virtual void terminate() = 0;
	virtual void setBackgroundColor() = 0;
	virtual void setCamera() = 0;
	virtual void setShaders(std::string vertexShaderPath, std::string fragmentShaderPath) = 0;
	virtual void drawModel(Model test) = 0;
};

class OpenGLAPI : public GraphicAPI
{
	GLFWwindow* window;
	void initialize() override
	{
		init(window);
		setupInput(window);
	}
	void cleanup() override
	{
		
	}
	void setWindowSize(int width, int height) override;
	void terminate() override;
	void setBackgroundColor() override;
	void setCamera() override;
	void setShaders(std::string vertexShaderPath, std::string fragmentShaderPath) override;
	void drawModel(Model test) override;
};