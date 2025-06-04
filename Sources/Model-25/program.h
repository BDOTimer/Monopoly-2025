#ifndef PROGRAM_H
#define PROGRAM_H
///----------------------------------------------------------------------------|
/// "program.h"
///----------------------------------------------------------------------------:
#include "debug.h"
#include "user-model.h"
#include "vsl/vsl.h"
#include "controller/controller.h"


///----------------------------------------------------------------------------|
/// Клиентская игра.
///------------------------------------------------------------------ TestGame2:
struct  TestGame2
{       TestGame2()
        {   start();
        }

    visual:: Console vc;
    unsigned     idGame;
    model::ConfigShare* cfg;

    std::array<controller::Player, 3> players
    {          controller::Player (true, 0, "Игрок-1"),
               controller::Player (true, 1, "Игрок-2"),
               controller::Player (true, 2, "Игрок-3")
    };

    size_t whoVictor{NPOS};

    model::UserInit4Model userInit4Model;
    BackDoor::Data              backDoor;

    void start()
    {   cfg    = model::getConfig(backDoor, userInit4Model);
        idGame = cfg->idGame;
        vc << model::getLogo(idGame) << "\n";
    }

    void loop()
    {
        std::stringstream ss;
        ss << "///-----------------------------------------|\n"
              "///            ИГРА НАЧАЛАСЬ!               |\n"
              "///---------------------------------------!!!\n" << '\n';

        showMessage(ss);

        unsigned cnt{0};

        unsigned isDump2File = model::ConfigShare::isDump2File()+ 1;

        l(isDump2File)

        const char* const LINE{"/////////////////////////////////////////|"};

        for(; whoVictor == NPOS; whoVictor = model::whoVictor(idGame))
        {
            for(auto& pl : players)
            {
                unsigned& idPlayer = pl.id;

                std::stringstream ss; ss << std::setw(4) << ++cnt;

                vc  << "ПАУЗА::\nНажмите ENTER, чтобы сделать:"
                    << ss.str() << " шаг -->\n"
                /// << "---------------------------------------------..."
                    << LINE << (cfg->isDump2File() == 0 ? "" : LINE);

                if(0 == model::ConfigShare::isDump2File())
                {   std::string e; std::getline(std::cin, e);
                    if(e.back() == '0')
                    {   goto m;
                    }
                }
                else
                {   vc    << visual::endl{};
                    if(0  == --isDump2File)
                    {   goto m;
                    }
                }

                vc  << model::doStep( "bot", { (int)idGame,
                                               (int)idPlayer } );

                if(!cfg->isScrollConsole)
                {   std::system("cls");
                    vc  << "Процесс " << LOGO << "\n\n";
                }
            }
        }

    m:  ;

    }

private:
    std::string_view name{"Монополия-2025"};

    void showMessage(std::stringstream& ss)
    {   vc << ss.str(); ss.str("");
    }

    TEST
    {   TestGame2 program;

        program.vc << "Привет, я Program::"
                   << program.name << "!\n" << visual::endl{};

        program.loop();
    }
};

#endif // PROGRAM_H
