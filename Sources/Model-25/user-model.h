#ifndef USER_MODEL_H
#define USER_MODEL_H
const char* const LOGO = "Model::Monopoly-2025[ver::0.2.5]";
///----------------------------------------------------------------------------|
/// "user-model.h"
///----------------------------------------------------------------------------:
#include <iostream>
#include <string>
#include <vector>
#include <array>

#include "debug.h"

namespace BackDoor
{
    struct  Data
    {       Data()
            {   std::ifstream f(fn);
                if(!f.is_open()) return;
                std::vector<std::string> m;
                for(std::string s; std::getline(f, s);) m.push_back(s);

                isSeed = std::stoi(m[0].c_str());
            }
           ~Data()
            {   std::ofstream f(fn);
                f << isSeed << '\n';

                std::cout << "Файл \"BackDoor.txt\" обновлен.\n";
            }

        int isSeed{1234560};

    private:
        const char* fn{"BackDoor.txt"};
    };
}

namespace model
{
    struct Decode2Str
    {
        std::array<std::string, 4> statusPlayer
        {   "Ребёнок ",
            "Взрослый",
            "Родитель",
            "Чужой   "
        };

        ///------------------------------|
        /// Расшифровка статуса игрока.  |
        ///------------------------------:
        std::string_view getPlayer(unsigned i) const
        {   if(i >= statusPlayer.size()) i = 3;
            return statusPlayer[i];
        }

        std::array<std::string, 4> statusCell
        {   "Дети "   ,
            "Война"   ,
            "Продукты",
            "Артефакт"
        };

        ///------------------------------|
        /// Расшифровка статуса ячейки.  |
        ///------------------------------:
        std::string_view getCell(unsigned i) const
        {   if(i >= statusCell.size()) i = 3;
            return statusCell[i];
        }
    };


    ///------------------------------|
    /// Профиль игрока.              |
    ///------------------------------:
    struct Player
    {
        unsigned           id;
        bool            isBot;
        std::string nameInput{"имя..."};
        std::string      name;

        void initName()
        {   name = std::string(isBot ? "bot::" : "user::") + nameInput;
        }
    };


    ///------------------------------|
    /// UserInit4Model               |
    ///------------------------------:
    struct UserInit4Model
    {
        ///------------------------------|
        /// Состав игроков.              |
        ///------------------------------:
        std::array<Player, 3> players
        {   Player{0, true, "Noname"  }, /// Умный.
            Player{1, true, "aliskda" }, /// Срединий.
            Player{2, true, "gudleifr"}  /// Дурак.
        //  {false, "Slava-rusi11" }
        //  {false, "Вася Пупкин"  }
        };

        void initNames()
        {   for(unsigned i  = 0; i < players.size(); ++i)
            {   players [i].id = i;
                players [i].initName();
            }
        }
    };

    struct ConfigShare : UserInit4Model
    {

        ///------------------------------|
        /// Индекс игры.                 |
        ///------------------------------:
        unsigned idGame             { 0 };

        ///------------------------------|
        /// Количество ячеек на поле.    |
        ///------------------------------:
        unsigned amountCells       { 30 };

        ///------------------------------|
        /// Мировая геометрия.           |
        ///------------------------------:
        std::vector<std::vector<int>> worldGeometry
        {   { -1,  0,  1,  2, -1,  6,  7,  8, -1 },
            { 28, 29, -1,  3,  4,  5, -1,  9, 10 },
            { 27, -1, -1, -1, -1, -1, -1, -1, 11 },
            { 26, -1, -1, -1, -1, -1, -1, -1, 12 },
            { 25, 24, -1, 20, 19, 18, -1, 14, 13 },
            { -1, 23, 22, 21, -1, 17, 16, 15, -1 }
        };

        ///------------------------------|
        /// Жеребьёвка.                  |
        ///------------------------------:
        std::vector<unsigned>       order;

        ///-----------------------------------------------|
        /// Дебажные нстройки.                            |
        ///-----------------------------------------------:
        ///------------------------------|
        /// Кол-во итераций в файл.      |
        /// 0 - вывод в консоль.         |
        ///------------------------------:
        static unsigned& isDump2File()
        {   static unsigned CountCircles{0};
            return CountCircles;
        }

        ///------------------------------|
        /// Скроллить консоль?.          |
        ///------------------------------:
        bool isScrollConsole{/*ДА:*/true};
        ///-----------------------------------------------.

        Decode2Str decode2Str;

        static void test()
        {   ConfigShare configShare;
            std::cout
                << "configShare.decode2Str.getPlayer(0): "
                <<  configShare.decode2Str.getPlayer(0) << '\n';
        }
    };

    ///------------------------------------------------------------------------|
    /// Стейт визуализатора игры.
    /// Данные посылаемые от сервера к клиенту(игроку).
    ///------------------------------------------------------------------------:
    struct StateGame
    {
        enum eSTATE
        {    E_SIZE    ,
             E_IDPLAYER,
             E_NDICE   ,
             E_POSITION,
             E_ISBUSYCELL,
             E_STATUS_PERS,
             E_STATUS_CELL,
             E_SELL ,
             E_BYU  ,
             E_MONEY1,
             E_MONEY2,
             E_GAMEOVER,

             /// ...
             E_DAT
        };

        std::array<int, eSTATE::E_DAT> dat
        {    E_DAT
        };

        enum eSTATESTR
        {    E_      ,
             E_NAME  ,
             E_CELL  ,

             /// ...
             E_STR
        };

        std::array<std::string, eSTATESTR::E_STR> str
        {   "3"
        };
    };

    ///------------------------------------------------------------------------|
    /// Стейт визуализатора игры.
    /// Данные посылаемые от клиента(игрока) на сервер.
    ///------------------------------------------------------------------------:
    struct StateGameClient2Server
    {
        bool isBuy;
        std::vector<unsigned> isSellIds;

        ///----------------------------|
        /// Для верификации.           |
        ///----------------------------:
        int money;

        void init()
        {
            isBuy     = false;
            isSellIds.clear();

            money   = -0xFFFF;
        }
    };


    ///---------------------------|
    /// vsl.                      |
    ///---------------------------:
    ConfigShare* getConfig(const BackDoor::Data& data,
                           const UserInit4Model& userInit4Model);

    std::string  getLogo(unsigned id);

    ///---------------------------|
    /// controller.               |
    ///---------------------------:
    std::string doStep(std::string_view command, const std::vector<int>& args);

    ///---------------------------|
    /// Кто Победитель.           |
    ///---------------------------:
    size_t whoVictor(unsigned idGame);

    ///---------------------------|
    /// Сервер ---> Клиент.       |
    ///---------------------------:
    const StateGame getStateGame( std::string_view command,
                                  const std::vector<int>& args);

    ///---------------------------|
    /// Клиент ---> Сервер.       |
    ///---------------------------:
    const StateGame sendStateGame( std::string_view        command,
                                   const std::vector<int>& args,
                                   const StateGameClient2Server& stateGameC2S );
}

#endif // USER_MODEL_H
