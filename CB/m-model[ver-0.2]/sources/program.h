#ifndef PROGRAM_H
#define PROGRAM_H
///----------------------------------------------------------------------------|
/// "program.h"
///----------------------------------------------------------------------------:
#include "debug.h"
#include "vsl/vsl.h"
#include "user-model.h"
#include "controller/controller.h"

struct  Program
{       Program  ()
        {   start();
        }

    visual:: Console vc;
    unsigned     idGame;
    model::Config   cfg;

    std::array<Player, 3> players
    {          Player (0),
               Player (1),
               Player (2)
    };

    bool done{true};

    void start()
    {   cfg    = model::getConfig();
        idGame = cfg.idGame;
        vc << model::getLogo(idGame) << "\n";
    }

    void loop()
    {
        std::stringstream ss;
        ss << "///-----------------------------------|\n"
              "///         ИГРА НАЧАЛАСЬ!            |\n"
              "///-----------------------------------:\n" << '\n';

        showMessage(ss);

        while(done)
        {
            for(auto& pl : players)
            {
                unsigned& idPlayer = pl.id;

                vc << model::doStep( "id", { (int)idGame,
                                             (int)idPlayer } );

                ss  << "ПАУЗА::Нажмите ENTER, чтобы сделать шаг ...";

                std::cin.get();
            }
        }
    }

private:
    std::string_view name{"Монополия-2025"};

    void showMessage(std::stringstream& ss)
    {   vc << ss.str(); ss.str("");
    }

    TEST
    {   Program program;

        program.vc << "Привет, я Program::"
                   << program.name << "!" << visual::endl{};

        program.loop();
    }
};

#endif // PROGRAM_H
