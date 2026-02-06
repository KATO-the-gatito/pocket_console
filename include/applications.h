#pragma once

#include "global.h"

class Application
{
public:

    Application(const char* name = "Untitled") : name(name)
    {}
    ~Application() 
    {}

    virtual int execute() {return 0;}
    virtual Application* create(){
        Application* app = new Application();
        return app;
    }

    const char* name;
};