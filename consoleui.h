#ifndef CONSOLEUI_H
#define CONSOLEUI_H

#include<string>
#include<iostream>
#include "scientistservice.h"

class ConsoleUI
{
public:
    ConsoleUI();
    void start();
private:
    ScientistService scientistService;
};

#endif // CONSOLEUI_H
