///----------------------------------------------------------------------------|
/// Модель игры Монополия-2025.
///----------------------------------------------------------------------------:
 #include "!m-model.h"


///----------------------------------------------------------------------------|
/// Тестовая игровая площадка.
///------------------------------------------------------------------- TestGame:
struct  TestGame   : model::Referee
{       TestGame() : model::Referee(model::Config::getDefault())
        {   model::Config::getDefault().infoValidation();
        }

    void run()
    {
        std::string_view bannerStartGame
        {   "///-----------------------------------|\n"
            "///         ИГРА НАЧАЛАСЬ!            |\n"
            "///-----------------------------------:\n"
        };

        std::cout << bannerStartGame << '\n';

        info();

        unsigned cnt{ 0 };

        for (bool isDone = true; isDone;)
        {
            std::cout   << "ПАУЗА::Нажмите ENTER, чтобы сделать "
                        << ++cnt << " шаг ... или '0' для завершения ...\n";

            {
                std::string e; std::getline(std::cin, e);
                if(e.back() == '0') break;
            }

            if(!model::Config::getDefault().isScrollConsole)
            {   std::system("cls");
                std::cout << "Процесс " << LOGO << "\n\n";
            }
            else std::cout << "...-----------------------------------"
                              "-------------------------------...\n\n";


            isDone = step();
        }
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

    /// TESTCLASS(model::Cell::test  );
    /// TESTCLASS(model::Config::test);
    /// TESTCLASS(model::Field::test );
    /// TESTCLASS(model::Person::test);

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
