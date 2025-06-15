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
        int             isBot;
        std::string nameInput{"имя..."};
        std::string      name;

        void initName()
        {   name = std::string(isBot ? "bot::" : "Человек::") + nameInput;
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
        {   Player{0, true , "aliskda"      }, /// Срединий.
        /// Player{1, true , "Noname"       }, /// Умный.
            Player{1, false, "Вася Пупкин"  },
        /// Player{1, false, "Slava-rusi11" },
            Player{2, true , "gudleifr"     }  /// Дурак.
        };

        void initNames()
        {   for(unsigned i  = 0; i < players.size(); ++i)
            {   players [i].initName();
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
        /// Денеги Банка.                |
        ///------------------------------:
        int moneyBank{3200};

        ///------------------------------|
        /// Инфа о ячейках для игроков.  |
        ///------------------------------:
        struct      CellUser{std::string name; int priseBase; };
        std::vector<CellUser>       cells;

        ///------------------------------|
        /// Мировая геометрия.           |
        ///------------------------------:
        #define o -1,
        std::vector<std::vector<std::vector<int>>> _worldGeometry
        {
            {   {  o  0,   1,  2,  o   6,  7,  8,  o },
                { 28, 29,  o   3,  4,  5,  o   9, 10 },
                { 27,  o   o   o   o   o   o   o  11 },
                { 26,  o   o   o   o   o   o   o  12 },
                { 25, 24,  o  20, 19, 18,  o  14, 13 },
                {  o  23, 22, 21,  o  17, 16, 15,  o }
            },
            {   {  0,  1,  2,  3,  o   7,  8,  9, 10 },
                { 29,  o   o   4,  5,  6,  o   o  11 },
                { 28,  o   o   o   o   o   o   o  12 },
                { 27,  o   o   o   o   o   o   o  13 },
                { 26,  o   o  21, 20, 19,  o   o  14 },
                { 25, 24, 23, 22,  o  18, 17, 16, 15 }
            },
            {   {  o   o   o   4,  5,  6,  o   o   o },
                {  o   1,  2,  3,  o   7,  8,  9, 10 },
                { 29,  0,  o   o   o   o   o   o  11 },
                { 28,  o   o   o   o   o   o   o  12 },
                { 27,  o   o   o   o   o   o   o  13 },
                { 26,  o   o   o   o   o   o  15, 14 },
                { 25, 24, 23, 22,  o  18, 17, 16,  o },
                {  o   o   o  21, 20, 19,  o   o   o },
            },
            {   {  0,  1,  2,  3,  4,  5,  6,  7,  8 },
                { 29,  o   o   o   o   o   o   o   9 },
                { 28,  o   o   o   o   o   o   o  10 },
                { 27,  o   o   o   o   o   o   o  11 },
                { 26,  o   o   o   o   o   o   o  12 },
                { 25,  o   o   o   o   o   o   o  13 },
                { 24,  o   o   o   o   o   o   o  14 },
                { 23, 22, 21, 20, 19, 18, 17, 16, 15 }
            }
        };
        #undef o

        unsigned worldGeometryN1{0};

        const std::vector<std::vector<int>>& getWorldGeometry() const
        {   return _worldGeometry[worldGeometryN1];
        }

        void nextWorldGeometry()
        {   worldGeometryN1 = (worldGeometryN1 + 1) % _worldGeometry.size();
        }

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
             E_SELL  ,  // Цена    продажи.
             E_BYU   ,  // Цена    покупки.
             E_ISBYU ,  // Событие покупки.
             E_ISSELL,  // Событие продажи.
             E_MONEY1,
             E_MONEY2,
             E_BANK1 ,
             E_BANK2 ,
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

        void reset()
        {    dat[E_ISSELL] = 0;
             dat[E_ISBYU ] = 0;
        }
    };

    ///------------------------------------------------------------------------|
    /// Стейт визуализатора игры.
    /// Данные посылаемые от клиента(игрока) на сервер.
    ///------------------------------------------------------- StateGame4Server:
    struct StateGame4Server
    {
        bool isBuy;
        std::vector<unsigned> isSellIds;

        ///----------------------------|
        /// Для верификации.           |
        ///----------------------------:
        int money;

        void resert()
        {
            isBuy     = false;
            isSellIds.clear();

            money   = -0xFFFF;
        }

        bool isWork() const { return isBuy || !isSellIds.empty(); }
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
                                   const StateGame4Server& stateGame4S );
}

#endif // USER_MODEL_H
