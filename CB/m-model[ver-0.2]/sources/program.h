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
{       TestGame2  ()
        {   start();
        }

    visual:: Console vc;
    unsigned     idGame;
    model::Config   cfg;

    std::array<controller::Player, 3> players
    {          controller::Player (0),
               controller::Player (1),
               controller::Player (2)
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
        ss << "///-----------------------------------------|\n"
              "///            ИГРА НАЧАЛАСЬ!               |\n"
              "///---------------------------------------!!!\n" << '\n';

        showMessage(ss);

        unsigned cnt{0};

        unsigned isDump2File = model::Config::isDump2File()+ 1;

        l(isDump2File)

        while(done)
        {
            for(auto& pl : players)
            {
                unsigned& idPlayer = pl.id;

                std::stringstream ss; ss << std::setw(4) << ++cnt;

                vc  << "ПАУЗА::Нажмите ENTER, чтобы сделать "
                    << ss.str() << " шаг -->\n"
                /// << "---------------------------------------------..."
                    << "////////////////////////////////////////////|";

                if(0 == model::Config::isDump2File())
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

                if(!model::Config::getDefault().isScrollConsole)
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
