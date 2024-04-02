#include "Script.h"
#include <iostream>

class UserScript : public Script {
public:
    void Start() {
        //Votre code ici
        Script::Start();
        std::cout << "User-defined Start() function called." << std::endl;
    }

    void Update() {
        //Votre code ici
        Script::Update();
        std::cout << "User-defined Update() function called." << std::endl;
    }
};
