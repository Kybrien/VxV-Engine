#include "GameObject.h"
#include "Debug.h"


int main()
{
	GameObject test;

	test.name = "test";

	Debug::Log(test.name);


	return 0;
}