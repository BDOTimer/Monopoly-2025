const char* const LOGO_VISUALIZATOR{"VISUALIZATOR::ver::0.0.2.6"};
///----------------------------------------------------------------------------|
/// C++20
/// ...
///-------------------------------------------------------------- sfml-Monopoly:
#include "render.h"
#include "ui-tune.h"

namespace uii
{
    void UITuneBase::doTuneAllClose() /// not used.
    {   cfg->doTuneAllClose();
    }
}


void exit2sys()
{
    vsl::Sound sound("res/snd/out.mp3"); sound.play();
    while(sound.getStatus() != sf::Sound::Status::Stopped)
    {   std::this_thread::sleep_for(std::chrono::milliseconds {100});
    }
}


void tests()
{
/// model::ConfigShare::test();
///
/// TRY(vsl::ShaderDice::test2())
///
TRY(Render::test())
}


///----------------------------------------------------------------------------|
/// Старт.
///----------------------------------------------------------------------- main:
int main(int argc, char* argv[])
{
#ifdef    _WIN32
    std::system("chcp 65001>nul");
#endif // _WIN32

	std::cout << "Compilator: " << win::COMPILATOR_NAME << '\n'
              << "Старт "       << LOGO_VISUALIZATOR << " ...\n\n";

    try
    {   tests();
    }
    catch(const char* err)
    {   std::cout << "FATALERROR: " << err << "\n";
    }
    catch(...)
    {   std::cout << "FATALERROR: ???\n";
    }

    std::cout << "Программа закончила работу ...\n";

    exit2sys();

    std::cin.get();
}
