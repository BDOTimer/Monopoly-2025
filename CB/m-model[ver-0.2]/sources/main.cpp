///----------------------------------------------------------------------------|
/// Модель игры Монополия-2025.
///----------------------------------------------------------------------------:
 #include "!m-model.h"

namespace model
{
    model::Config Cfg;
}

///----------------------------------------------------------------------------|
/// Тестовая игровая площадка.
///------------------------------------------------------------------- TestGame:
struct  TestGame   : model::Referee
{       TestGame() : model::Referee(model::Cfg)
        {
        }

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

        for (bool isDone = true; isDone;)
        {

            ss  << "ПАУЗА::Нажмите ENTER, чтобы сделать "
                << ++cnt << " шаг ... или '0' для завершения ...\n";

            showMessage(ss);

            {
                std::string e; std::getline(std::cin, e);
                if(e.back() == '0') break;
            }

            if(!model::Config::getDefault().isScrollConsole)
            {   std::system("cls");
                ss  << "Процесс " << LOGO << "\n\n";
            }
            else ss <<  "...-----------------------------------"
                        "-------------------------------...\n\n";

            showMessage(ss);

            isDone = step();
        }
    }

    void showMessage(std::stringstream& ss)
    {   std::cout << ss.str(); ss.str("");
    }

    ///------------------------------|
    /// Тест класса.                 |
    ///------------------------------:
    TEST
    {   TestGame    testGame;
                    testGame.run();
    }
};


///----------------------------------------------------------------------------|
/// Все тесты классов.
///---------------------------------------------------------------------- tests:
void tests()
{
    /// myl::tests();

    /// TESTCLASS(model::Cards::test );
    /// TESTCLASS(model::Cell::test  );
    /// TESTCLASS(model::Config::test);
    /// TESTCLASS(model::Field::test );
    /// TESTCLASS(model::PersonBot::test);

    ///
    TESTCLASS(TestGame::test);
}


///----------------------------------------------------------------------------|
/// Старт программы.
///----------------------------------------------------------------------- main:
int main(int argc, char* argv[])
{
    std::system( "chcp 65001>nul" );
/// SetConsoleOutputCP(65001);

    std::cout << "Старт " << LOGO << "\n\n";

    if(argc > 1)
    {   try
        {   unsigned n = std::stoul(argv[1]);
            l(n)
        }
        catch(...)
        {   std::cout << "ERROR: Аргумент командной строки не число ...\n";
            return -1;
        }
    }

    printf(model::Cfg.infoValidation().c_str());

    tests();

    std::cout << "Программа закончила работу.\n" << std::endl;

    return 0;
}
