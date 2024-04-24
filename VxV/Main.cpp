#include "EngineSetup.hpp"

int main()
{

	Engine* engine = new Engine();
	EngineGUI* gui = new EngineGUI();
	APIopenGL* apiGraphic = new APIopenGL();
	startup(gui,apiGraphic);
	engine->Init();

	Manager* manager = Manager::GetInstance();
	SceneManager* sceneManager = manager->GetManager<SceneManager>();
	/*while (engine->GetBootingState() != EngineState::BootingState::Stopped)
	{
		if (engine->GetActiveState() != EngineState::ActiveState::Edition) {

		}
		else if (engine->GetActiveState() != EngineState::ActiveState::RunTime) {
			double currentTime = glfwGetTime();
			double deltaTime = currentTime - engine->getLastTime();
			engine->setLastTime(currentTime);
		}
	}*/
	return 0;
}
