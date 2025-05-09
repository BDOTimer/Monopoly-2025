const char* const LOGO_VISUALIZATOR{"VISUALIZATOR::ver::0.0.1"};
///----------------------------------------------------------------------------|
/// C++20
/// ...
///-------------------------------------------------------------- sfml-Monopoly:
#include "render.h"


void tests()
{
    //TRY(Render::test())

    if(const auto e = vsl::ShaderDice::test2(); e != "SUCCESS")
    {   std::cout << "ERROR: Shader-dice fail: " << e << " ...\n";
    }
}


///----------------------------------------------------------------------------|
/// Старт.
///----------------------------------------------------------------------- main:
int main(int argc, char* argv[])
{
#ifdef    _WIN32
    std::system("chcp 65001>nul");
#endif // _WIN32

    std::cout << "Старт " << LOGO_VISUALIZATOR << " ...\n\n";

    tests();

    std::cout << "Программа закончила работу ...\n";

    std::cin.get();
}
