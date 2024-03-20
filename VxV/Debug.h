#pragma once

#include <iostream>
#include <string>

class Debug {
public:
	static void Log(std::string message) { std::cout << message; }
};