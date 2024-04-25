#include <string>
#include "Model.h"

class GraphicAPI
{
	public:
		virtual ~GraphicAPI() = default;

		/**
		* @brief Initialize the graphic API
		*/
		virtual void initialize() = 0;

		/**
		* @brief Clean up the model
		*/
		virtual void cleanupModel(Model* _model) = 0;

		/**
		* @brief Set the window size
		*/
		virtual void setWindowSize(int _width, int _height) = 0;

		/**
		* @brief Terminate the graphic API
		*/
		virtual void terminate() = 0;

		/**
		* @brief Set the background color
		*/
		virtual void setBackgroundColor(float _red,float _green, float _blue, float _alpha) = 0;

		/**
		* @brief Set the camera
		* @param _fov The field of view
		* @param _ratioWidth The ratio width
		* @param _ratioHeight The ratio height
		* @param _near The near plane of the fustrum
		* @param _far The far plane of the fustrum
		* @param _cameraPositionX The x position of the camera
		* @param _cameraPositionY The y position of the camera
		* @param _cameraPositionZ The z position of the camera
		* @param _cameraTargetX The x position of the target
		* @param _cameraTargetY The y position of the target
		* @param _cameraTargetZ The z position of the target
		* @param _upVectorX The x position of the up vector
		* @param _upVectorY The y position of the up vector
		* @param _upVectorZ The z position of the up vector
		*/
		virtual void setCamera(float _fov, float _ratioWidth, float _ratioHeight, float _near, float _far,
			int _cameraPositionX, int _cameraPositionY, int _cameraPositionZ, int _cameraTargetX, int _cameraTargetY,
			int _cameraTargetZ, int _upVectorX, int _upVectorY, int _upVectorZ) = 0;

		/**
		* @brief Set the shaders
		* @param _vertexShaderPath The path of the vertex shader
		* @param _fragmentShaderPath The path of the fragment shader
		*/
		virtual void setShaders(const char* _vertexShaderPath, const char* _fragmentShaderPath) = 0;

		/**
		* @brief Drawing the model
		* @param _gameObject The game object to draw
		*/
		virtual void drawingModel(GameObject* _gameObject) = 0;

		/**
		* @brief Set the handles
		*/
		virtual void setHandles() = 0;

		/**
		* @brief Clear the screen
		*/
		virtual void clearScreen() = 0;

		/**
		* @brief Swap the buffers
		*/
		virtual void swapBuffers() = 0;

		/**
		* @brief Unbind the arrays
		*/
		virtual void unbindArrays() = 0;

		/**
		* @brief Use the shader
		*/
		virtual void useShader() = 0;
};
