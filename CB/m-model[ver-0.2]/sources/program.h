#ifndef PROGRAM_H
#define PROGRAM_H
///----------------------------------------------------------------------------|
/// "program.h"
///----------------------------------------------------------------------------:
#include "debug.h"
#include "vsl/vsl.h"
#include "model/config-model.h"

struct  Program
{       Program()
        {   start();
        }


    visual::Console vc;

    bool done{true};

    void start()
    {
    }

    void loop()
    {
        while(done)
        {

        }
    }

private:
    std::string_view name{"Монополия-2025"};

    TEST
    {   Program program;

        program.vc << "Привет, я Program::"
                   << program.name << "!" << visual::endl{};

        ln(model::Config::getLogo())

        program.loop();
    }
};

#endif // PROGRAM_H
