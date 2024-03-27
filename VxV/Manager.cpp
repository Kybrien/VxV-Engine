#include "Manager.h"
#include "SceneManager.h"

Manager* Manager::instance = nullptr;

void Manager::Init() {
	new SceneManager(this);
}