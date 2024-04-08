#ifndef LOADINGSHADER_HPP
#define LOADINGSHADER_HPP
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <sstream>
using namespace std;

#include <stdlib.h>
#include <string.h>

#define GLEW_STATIC
#include <GL/glew.h>
GLuint LoadShaders(const char* vertex_file_path, const char* fragment_file_path);

#endif