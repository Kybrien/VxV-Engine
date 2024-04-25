#pragma once

#include <iostream>
#include <string>

class Debug {
public:
	/**
	* @brief Logs a message to the standard output stream.
	* @param message The message to be logged.
	*/
	static void Log(std::string message) { std::cout << message << std::endl; }

};