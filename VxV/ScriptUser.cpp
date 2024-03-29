#include "Script.h"
#include <iostream>

class UserScript : public Script {
public:
    void Start() {
        //code custom
        Script::Start();
        std::cout << "User-defined Start() function called." << std::endl;
    }

    void Update() {
        Script::Update();
        std::cout << "User-defined Update() function called." << std::endl;
    }
};