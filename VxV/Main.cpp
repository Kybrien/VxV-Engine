#include "Engine.hpp"


const float DESIRED_FPS = 60.0f;
const float FRAME_TIME = 1.0f / DESIRED_FPS;

int main()
{

	Engine::GetInstance()->InitEngine();
	
	return 0;
}
