﻿#ifndef CONTROLLER_H
#define CONTROLLER_H
///----------------------------------------------------------------------------|
/// "controller.h"
///----------------------------------------------------------------------------:
#include "../debug.h"
#include "../user-model.h"

namespace controller
{
    struct  Player
    {       Player(unsigned id) : id(id)
            {
            }

        unsigned id;

        std::string input()
        {   std::string s; std::getline(std::cin, s); return s;
        }
    };
}

#endif // X_H
