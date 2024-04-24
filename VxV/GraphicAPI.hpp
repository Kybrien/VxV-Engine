#include <string>
#include "Model.h"

class GraphicAPI
{
	public:
		virtual ~GraphicAPI() = default;
		virtual void initialize() = 0;
		virtual void cleanupModel(Model* _model) = 0;
		virtual void setWindowSize(int _width, int _height) = 0;
		virtual void terminate() = 0;
		virtual void setBackgroundColor(float _red,float _green, float _blue, float _alpha) = 0;
		virtual void setCamera(float _fov, float _ratioWidth, float _ratioHeight, float _near, float _far,
			int _cameraPositionX, int _cameraPositionY, int _cameraPositionZ, int _cameraTargetX, int _cameraTargetY,
			int _cameraTargetZ, int _upVectorX, int _upVectorY, int _upVectorZ) = 0;
		virtual void setShaders(const char* _vertexShaderPath, const char* _fragmentShaderPath) = 0;
		virtual void drawingModel(Model* _model) = 0;
		virtual void setHandles() = 0;
		virtual void clearScreen() = 0;
};
