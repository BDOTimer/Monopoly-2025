///----------------------------------------------------------------------------|
/// Модель игры Монополия-2025.
///----------------------------------------------------------------------------:
#include "program.h"
#include "model/!m-model.h"


namespace implants
{   void  tests();
}


#if       __has_include(<SFML/Graphics.hpp>)
    #define THIS_SFML_PROJECT
#else
    struct  _$StartProgram
    {       _$StartProgram()
            {   std::system( "chcp 65001>nul" );
                if(bool test = false; test)
                {   std::string  s;
                    std::getline(std::cin, s);
                    std::cout << s << '\n';
                }
            }
    }_$sp;
#endif // __has_include


///----------------------------------------------------------------------------|
/// Тестовая игровая площадка.
///------------------------------------------------------------------ TestGame1:
struct  TestGame1                     : model::Referee
{       TestGame1(model::Config& Cfg) : model::Referee(Cfg)
                                     , cfg(Cfg)
        {
            std::cout << model::Referee::infoField() << '\n';
        }

    model::Config& cfg;

    void run()
    {
        std::stringstream ss;

        std::string_view bannerStartGame
        {   "///-----------------------------------|\n"
            "///         ИГРА НАЧАЛАСЬ!            |\n"
            "///-----------------------------------:\n"
        };

        ss << bannerStartGame << '\n';
        ss << info();

        showMessage(ss);

        unsigned cnt{ 0 };

        unsigned isDump2File = model::Config::isDump2File()+ 1;

        for (bool isDone = true; isDone;)
        {
            ss  << "ПАУЗА::Нажмите ENTER, чтобы сделать "
                << ++cnt << " шаг ... или '0' для завершения ..." << std::endl;

            showMessage(ss);

            if(0 == model::Config::isDump2File())
            {   std::string e; std::getline(std::cin, e);
                if(e.back() == '0') break;
            }
            else
            {   if(0 == --isDump2File) break;
            }

            if(!model::Config::getDefault().isScrollConsole)
            {   std::system("cls");
                ss  << "Процесс " << LOGO << "\n\n";
            }
            else ss <<  "...-----------------------------------"
                        "-------------------------------...\n\n";

            showMessage(ss);

            isDone = !isGameOver();

            std::cout << doStep();
        }

        std::cout << "ИГРА ЗАКОНЧЕНА!\n" << std::endl;
    }

    void showMessage(std::stringstream& ss)
    {   std::cout << ss.str(); ss.str("");
    }

    ///------------------------------|
    /// Тест класса.                 |
    ///------------------------------:
    TEST
    {
        model::Config Cfg;

        ln(model::Config::getLogo    ())
         l(model::Config::isDump2File())

        printf(Cfg.infoValidation().c_str());

        TestGame1 testGame (Cfg);
                  testGame.run();
    }
};


///----------------------------------------------------------------------------|
/// Все тесты классов.
///---------------------------------------------------------------------- tests:
void tests()
{
    /// myl::tests();

    /// implants::tests();

    /// TESTCLASS(model::Cards::test );
    /// TESTCLASS(model::Cell::test  );
    /// TESTCLASS(model::Config::test);
    /// TESTCLASS(model::Field::test );
    /// TESTCLASS(model::PersonBot::test);
    /// TESTCLASS(model::CellInfoTester::test);

    /// TESTCLASS(TestGame1::test);
    ///
    TESTCLASS(TestGame2::test);
}


#ifndef THIS_SFML_PROJECT
///----------------------------------------------------------------------------|
/// Старт программы.
///----------------------------------------------------------------------- main:
int main(int argc, char* argv[])
{
    std::ofstream log;

    if(argc == 3)
    {   try
        {   model::Config::isDump2File() = std::stoul(argv[1]);

            l(model::Config::isDump2File())

            log.open(argv[2]);  /// logf.txt.py

            if(!log.is_open())
            {   std::cerr << "ERROR CMD: !log.is_open() ...\n";
                return -1;
            }
        /// std::streambuf *coutbuf = std::cout.rdbuf(); //save old buf
        /// std::cout.rdbuf(coutbuf); //reset to standard output again
            std::cout.rdbuf(log.rdbuf());
        }
        catch(...)
        {   std::cout << "ERROR: Аргумент командной строки не число ...\n";
            return -1;
        }
    }

    std::cout << '\n' << model::currencySymbol << '\n';

    tests();

    std::cout << "Программа закончила работу.\n" << std::endl;
    if(0 == model::Config::isDump2File()) std::cin.get();
    return 0;
}
#endif
