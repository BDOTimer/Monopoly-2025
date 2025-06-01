#ifndef USER_MODEL_H
#define USER_MODEL_H
const char* const LOGO = "Model::Monopoly-2025[ver::0.2.4]";
///----------------------------------------------------------------------------|
/// "user-model.h"
///----------------------------------------------------------------------------:
#include <iostream>
#include <string>
#include <vector>
#include <array>

#include "debug.h"

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


    struct ConfigShare
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
        /// Профиль игрока.              |
        ///------------------------------:
        struct Player
        {   bool       isBot;
            std::string name;
        };

        ///------------------------------|
        /// Состав игроков.              |
        ///------------------------------:
        std::array<Player, 3> players
        {   Player{true , "bot::Noname"  }, /// Умный
            Player{true , "bot::aliskda" }, /// Срединий
            Player{true , "bot::gudleifr"}  /// Дурак
        //  {false, "Slava-rusi11" }
        //  {false, "Вася Пупкин"  }
        };

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

    ///---------------------------|
    /// Стейт визуализатора игры. |
    ///---------------------------:
    struct StateGame
    {
        enum eSTATE
        {    E_SIZE    ,
             E_IDPLAYER,
             E_NDICE   ,
             E_POSITION,
             E_STATUS_PERS,
             E_STATUS_CELL,
             E_SELL ,
             E_BYU  ,
             E_MONEY1,
             E_MONEY2,

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


    ///---------------------------|
    /// vsl.                      |
    ///---------------------------:
    ConfigShare* getConfig();

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
    /// Что должен знать игрок.   |
    ///---------------------------:
    const StateGame getStateGame(std::string_view command,
                                  const std::vector<int>& args);
}

#endif // USER_MODEL_H
