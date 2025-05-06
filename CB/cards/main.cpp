#include <iostream>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <vector>

extern const char* test2;

namespace myl
{
    using Sv  = std::string_view;
    using V1s = std::vector<Sv >;
    using V2s = std::vector<V1s>;

    constexpr size_t NPOS = std::string::npos;

    V1s parseStr(const Sv s, const Sv x)
    {
        V1s m; m.reserve(32);

        for(size_t b = 0, e; (b = s.find_first_not_of(x, b)) != NPOS; b = e)
        {
            if(s[b] == '\"')
            {    e = s.find('\"', b + 1) + 1;
            }
            else if(s[b] == '/')
            {    e = s.find('\n', b + 1);
                 if(e != NPOS) e++;
                 continue;
            }
            else e = s.find_first_of(x, b);

            m.push_back(s.substr(b, e - b));
        }

        return m;
    }

    V2s parseStr(const Sv s, const Sv A, const Sv B, const Sv x)
    {   V2s m; m.reserve(32);

        for(size_t b = 0, e; (b = s.find(A, b)) != NPOS; b = e)
        {   b += A.size();
            e  = s.find(B, b);
            m.push_back(parseStr(s.substr(b, e - b), x));
        }

        return m;
    }

    void TEST_parseStr(Sv test)
    {
        std::cout << test << "\n\n";
        auto m = parseStr(test, "{", "}", " \t\n.,=;:");

        for    (const auto& ss :  m)
        {   for(const auto&  s : ss)
            {   std::cout << s << /*"---" << s.size() <<*/ '\n';
            }   std::cout << '\n';
        }
    }


    void TEST_parseStr()
    {   TEST_parseStr("{123, 456;789}...///MMM;{123, \"zxc ///qwe!\";::789}...");
        TEST_parseStr(test2);
    }
}

    struct  Card
    {
        ///------------------------------|
        /// Из "cards.inc"               |
        ///------------------------------:
        unsigned              id; /// Идентификатор.
        unsigned            type; /// Тип.
        int             money[3]; /// Монеты от статуса.
        int             steps[2]; /// Ходы.
        unsigned           count; /// Сила в кол-ве раз действии.

        ///------------------------------|
        /// Служебная инфа.              |
        ///------------------------------:
        int buf[3]{};

        bool empty() const { return 0 == count; }

        ///------------------------------|
        /// Получаем имя из type.        |
        ///------------------------------:
        std::string_view decodeName() const
        {   if(type >= N) return name[N - 1];
            return name[type];
        }

    private:
        ///------------------------------|
        /// Расшифровки типов.           |
        ///------------------------------:
        inline static const char*   name[]
        {   "Сюрприз",
            "Золотая",
            "Джокер" ,
            "Просроченная"
        };
        inline static unsigned N{sizeof name / sizeof *name};

        std::string infoWhat(std::string s)
        {   s.resize(11, ' ');
            std::stringstream ss;
            ss << "///--------------------------|\n"
               << "/// Событие Шанс: " << s << "| <--- " << decodeName() << '\n'
               << "///--------------------------|\n\n";
            return ss.str();
        }

        friend std::ostream& operator<<(std::ostream& o, const Card& card);
        friend struct Cards;
    };


    ///----------------------------------|
    /// Вывод Card в консоль.            |
    ///----------------------------------:
    std::ostream& operator<<(std::ostream& o, const Card& card)
    {   return o    << std::setw(3) << card.type     << ", "
                    << std::setw(4) << card.money[0] << ", "
                    << std::setw(4) << card.money[1] << ", "
                    << std::setw(3) << card.money[2] << ", "
                    << std::setw(3) << card.steps[0] << ", "
                    << std::setw(3) << card.steps[1] << ", "
                    << card.decodeName();
    }


    ///------------------------------------------------------------------------|
    /// Коллекция карточек.
    ///------------------------------------------------------------------- Card:
    struct  Cards   : std::vector<Card>
    {       Cards() : std::vector<Card>
                    {
                    }
            {   loadFromFile("cards.inc");
                init();
            }

    void loadFromFile(std::string_view filename)
    {

    }

    friend std::ostream& operator<<(std::ostream& o, const Cards& card);

    Card* getCard()
    {   Card*  pc = pcards.back();
        pcards.pop_back();
        return pc;
    }

    private:
        std::vector<Card*> pcards;

        void init()
        {   pcards.clear();
            pcards.reserve(size());
            for(Card& e : *this) pcards.push_back(&e);
        }
    };


    ///----------------------------------|
    /// Вывод Cards в консоль.           |
    ///----------------------------------:
    std::ostream& operator<<(std::ostream& o, const Cards& cards)
    {   for(const auto& card : cards)
        {          o << card << '\n';
        }   return o;
    }



int main()
{
    std::system("chcp 65001");

    std::cout << "Hello world!" << std::endl;

    //Cards cards;
    myl::TEST_parseStr();

    return 0;
}


const char* test2
{
  R"(
        ///------------------------------|
        /// Количество ячеек на поле.    |
        ///------------------------------:
        unsigned amountCells      { 30 };

        std::vector<std::string> worldGeometry
        {   "OOOOOOOOO",
            "O.......O",
            "O.......O",
            "O.......O",
            "O.......O",
            "O.......O",
            "O.......O",
            "OOOOOOOOO"
        };

        ///------------------------------|
        /// Если 0, то сид от часов.     |
        ///------------------------------:
        unsigned isSeed{12345};

        ///------------------------------|
        /// Денеги у игроков на старте.  |
        ///------------------------------:
        unsigned startMoney{300};

        ///------------------------------|
        /// Денеги Банка.                |
        ///------------------------------:
        unsigned moneyBank{3200};

        ///------------------------------|
        /// Количество игроков.          |
        ///------------------------------:
        unsigned amountPlayers{3};

        Field* pfield{nullptr};
  )"
};
