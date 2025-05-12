///----------------------------------------------------------------------------|
/// Модель игры Монополия-2025.
///----------------------------------------------------------------------------:
 #include "!m-model.h"

 unsigned isDump2File = 0;

///----------------------------------------------------------------------------|
/// Тестовая игровая площадка.
///------------------------------------------------------------------- TestGame:
struct  TestGame                     : model::Referee
{       TestGame(model::Config& Cfg) : model::Referee(Cfg)
                                     , cfg(Cfg)
        {
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

        unsigned isDump2File = cfg.isDump2File + 1;

        for (bool isDone = true; isDone;)
        {
            ss  << "ПАУЗА::Нажмите ENTER, чтобы сделать "
                << ++cnt << " шаг ... или '0' для завершения ..." << std::endl;

            showMessage(ss);

            if(0 == cfg.isDump2File)
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

            isDone = step();
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
    {   model::Config Cfg;
          Cfg.isDump2File = isDump2File;

        l(Cfg.isDump2File)

        printf(Cfg.infoValidation().c_str());

        TestGame testGame (Cfg);
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
    /// TESTCLASS(model::CellInfoTester::test);

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

    std::ofstream log;

    if(argc > 1)
    {   try
        {   isDump2File = std::stoul(argv[1]);

            log.open("logf.txt");

        /// std::streambuf *coutbuf = std::cout.rdbuf(); //save old buf
        /// std::cout.rdbuf(coutbuf); //reset to standard output again
            std::cout.rdbuf(log.rdbuf());
        }
        catch(...)
        {   std::cout << "ERROR: Аргумент командной строки не число ...\n";
            return -1;
        }
    }

    std::cout   << "Старт " << LOGO  << "\n"
                << myl::getTimeNow() << "\n";

    tests();

    std::cout << "Программа закончила работу.\n" << std::endl;
    if(0 == isDump2File) std::cin.get();
    return 0;
}
