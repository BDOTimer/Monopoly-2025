const char* const LOGO_VISUALIZATOR{"VISUALIZATOR::ver::0.0.2.1"};
///----------------------------------------------------------------------------|
/// C++20
/// ...
///-------------------------------------------------------------- sfml-Monopoly:
#include "render.h"


void tests()
{

///
TRY(Render::test())
/// TRY(vsl::ShaderDice::test(cfg))
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
              << "Старт " << LOGO_VISUALIZATOR << " ...\n\n";

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

    std::cin.get();
}
