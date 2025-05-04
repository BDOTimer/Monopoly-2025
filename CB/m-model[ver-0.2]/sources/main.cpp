///----------------------------------------------------------------------------|
/// Модель игры Монополия-2025.
///----------------------------------------------------------------------------:
 #include "!m-model.h"


///----------------------------------------------------------------------------|
/// Тестовая игровая площадка.
///------------------------------------------------------------------- TestGame:
struct  TestGame   : model::Referee
{       TestGame() : model::Referee(model::Config::getDefault())
        {   printf(model::Config::getDefault().infoValidation().c_str());
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
int main()
{
    std::system( "chcp 65001>nul" );
/// SetConsoleOutputCP(65001);

    std::cout << "Старт " << LOGO << "\n\n";

    tests();

    std::cout << "Программа закончила работу.\n" << std::endl;

    return 0;
}
