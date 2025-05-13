#ifndef CONTROLLER_H
#define CONTROLLER_H
///----------------------------------------------------------------------------|
/// "x.h"
///----------------------------------------------------------------------------:
#include "../debug.h"
#include "../user-model.h"
#include "model/user-model.h"

struct  Player
{       Player(unsigned id) : id(id)
        {
        }

    unsigned id;

    std::string input()
    {   std::string s; std::getline(std::cin, s); return s;
    }
};

#endif // X_H
