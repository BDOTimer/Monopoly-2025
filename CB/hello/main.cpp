///----------------------------------------------------------------------------|
/// Модель игры Монополия-2025.
///----------------------------------------------------------------------------:
#define _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string_view>
#include <vector>
#include <tuple>

#include <locale>
#include <codecvt>

std::string WstrToUtf8(const std::wstring& str)
{   std::wstring_convert<std::codecvt_utf8<wchar_t> > strCnv;
    return strCnv.to_bytes(str);
}

std::wstring utf8ToWstr(const std::string& str)
{   std::wstring_convert< std::codecvt_utf8<wchar_t> > strCnv;
    return strCnv.from_bytes(str);
}

#define l(v)           std::cout << #v << " = " << (v) << std::endl;
#define TESTCLASS(F)   std::cout << "RUN: "#F; std::cout << '\n';\
                  F(); std::cout << '\n';

#define TEST friend void ::tests(); static void test()

///----------------------------------|
/// Для удобства чтения?             |
/// Array1U это одномерный           |
/// массив из натуральных числе.     |
///----------------------------------:
using Array1U = std::vector<unsigned>;
using Array2U = std::vector<Array1U >;

void tests();

namespace model
{

    ///------------------------------------------------------------------------|
    /// Правила.
    ///------------------------------------------------------------------------:
    struct Rules
    {

    };


    ///------------------------------------------------------------------------|
    /// Конфигурация Модели.
    ///------------------------------------------------------------------------:
    struct Config
    {
        bool loadFromFile(std::string_view filename)
        {   /// TODO ...
            return true;
        }

        ///------------------------------|
        /// Количество ячеек на поле.    |
        ///------------------------------:
        unsigned amountCells      {30};
        unsigned amountSatusesCell{ 3};

        std::vector<std::string> statuses
        { "123", "231", "312"
        };

        unsigned W{9};
        unsigned H{8};

        const Array2U getGen() const
        {     Array2U m(amountCells, Array1U(amountSatusesCell));

            for    (unsigned i = 0, k = 0; i < m.size()         ; ++i)
            {   const   auto s = statuses [k];
                for(unsigned j = 0       ; j < amountSatusesCell; ++j)
                {   m[i][j] = s[j] - '0' ;
                }

                if(++k == statuses.size()) k = 0;
            }

            return m;
        }

        bool doValidation() const
        {
            for(const auto& s : statuses)
            {   if(s.size() != amountSatusesCell) return false;
            }

            if(W * 2 + (H - 2) * 2 != amountCells) return false;

            /// TODO ...

            return true;
        }

        ///------------------------------|
        /// Получить базовый конфиг.     |
        ///------------------------------:
        static const Config& getDefault()
        {   static   Config  config;
            return           config;
        }

        void info() const
        {
            std::cout   << "nmodel::Config::doValidation() ---> "
                        << ( doValidation() ? "SUCCESS!" : "FAIL ...") << '\n';

            int cnt{};
            for(    const auto&  r : getGen())
            {   std::cout << ++cnt << "\t: ";
                for(const auto   n : r)
                {   std::cout << n ;
                }   std::cout << '\n';
            }       std::cout << '\n';
        }

        ///------------------------------|
        /// Тест класса.                 |
        ///------------------------------:
        TEST
        {
            {   Config config;
                     l(config. getDefault().amountCells)
                       config. info();
            }

            {   Config config{8, 4, {"4321","6789"}, 3, 3};
                     l(config.amountCells)
                       config.info();
            }
        }
    };


    ///------------------------------------------------------------------------|
    /// Описание(класс) ячейки на карте.
    ///------------------------------------------------------------------------:
    struct  Cell
    {       Cell(                   ) {}
            Cell(const Array1U& init) : statuses(init)
            {
            }

        ///------------------------------|
        /// Получить статус по номеру.   |
        ///------------------------------:
        unsigned getStatus(unsigned number) const { return statuses[number]; }

        void info() const
        {   std::cout << "id::" << id << "\t";
            for(const auto n : statuses) std::cout << n;
            std::cout   << '\n';
        }

    private:
        ///------------------------------|
        /// Массив со статусами.         |
        ///------------------------------:
        Array1U statuses;

        ///------------------------------|
        /// Ячейка может иметь ID.       |
        ///------------------------------:
        unsigned id;

        ///------------------------------|
        /// Тут тест этого класса.       |
        ///------------------------------:
        TEST
        {
            std::cout << "statusesCells: \n";

            {   ///----------------------|
                /// Создаем объект ячейки|
                ///----------------------:
                Cell         cell  ( {1,2,3} );

                ///----------------------|
                /// Дебажим в консоль.   |
                ///----------------------:
                std::cout << cell << std::endl;
            }

            {   ///----------------------|
                /// Данные для ячейки    |
                ///----------------------:
                Array1U           m{2,3, 1};

                ///----------------------|
                /// Создаем объект ячейки|
                ///----------------------:
                Cell         cell(m);

                ///----------------------|
                /// Дебажим в консоль.   |
                ///----------------------:
                std::cout << cell << std::endl;
            }
        }

        ///------------------------------|
        /// Друзья приватных методов.    |
        ///------------------------------:
        friend std::ostream& operator<<(std::ostream& o, const Cell& cell);
        friend struct Field;
    };


    ///----------------------------------|
    /// Вывод Cell в консоль.            |
    ///----------------------------------:
    std::ostream& operator<<(std::ostream& o, const Cell& cell)
    {   for(const auto status : cell.statuses) o << status;
        return o;
    }


    ///------------------------------------------------------------------------|
    /// Поле Модели.
    ///------------------------------------------------------------------------:
    struct  Field : std::vector<Cell>
    {       Field(const Config& cfg)
            {   init           (cfg);
            }

        struct PersonQ {unsigned pos; bool isStart{false}; };

        PersonQ add(unsigned pos, unsigned step) const
        {   size_t res  = pos + step;
            if(      res >= size()) { return { unsigned(res -= size()), true }; }
            return  {(unsigned)res};
        }

        unsigned sub(unsigned pos, unsigned step) const
        {   size_t res  = pos - step;
            if(      res >= size()) res += size();
            return (unsigned)res;
        }

        void info() const
        {
            l(size())

            for(const auto& cell : *this)
            {   cell.info();
            }
        }

    private:
        void init(const Config& cfg)
        {
            const Array2U statuses = cfg.getGen();
            reserve(cfg.amountCells);

            for(unsigned i = 0; i < cfg.amountCells; ++i)
            {   emplace_back(Cell());

                Cell& cell    = back();
                      cell.id = i;
                      cell.statuses.reserve(cfg.amountSatusesCell);

                for(unsigned j = 0; j < cfg.amountSatusesCell; ++j)
                {   cell.statuses.push_back(statuses[i][j]);
                }
            }
        }

        ///------------------------------|
        /// Тест класса.                 |
        ///------------------------------:
        TEST
        {
            {   Field   field(Config::getDefault());
                        field.info();

                Cell&   cell = field[0];
                        cell.info();
            }
        }
    };

    ///------------------------------------------------------------------------|
    /// Персонаж.
    ///------------------------------------------------------------------------:
    struct  Person
    {
        ///------------------------------|
        /// Имя персонажа.               |
        ///------------------------------:
        std::string name {"Noname"};

        ///------------------------------|
        /// Позиция на поле.             |
        /// Поле у нас лента...          |
        ///------------------------------:
        unsigned position{0};
        unsigned status  {0};
        unsigned circle  {1};

        void info() const
        {   auto n = 15 - nn + name.size();
            std::cout << "Имя: "       << std::setw(n) << name     << ",  "
                      << "position = " << std::setw(3) << position << ",  "
                      << "status = "   << std::setw(2) << status+1 << ",  "
                      << "circle = "   << std::setw(4) << circle   << '\n';
        }

        ///------------------------------|
        /// Для utf8 в std::setw.        |
        ///------------------------------:
        unsigned nn;
        void   init()
        {   nn = (unsigned)utf8ToWstr(name).size();
        }

    private:
        ///------------------------------|
        /// Тест класса.                 |
        ///------------------------------:
        TEST
        {   Person  person;
                    person.info();
        }
    };
}


///----------------------------------------------------------------------------|
/// Тестовая игровая площадка.
///----------------------------------------------------------------------------:
struct  TestGame
{       TestGame() : field(model::Config::getDefault())
        {
            for(auto& pers : perses)
            {   pers.init();
            }
        }

    void run()
    {   loop();
    }

    void info() const
    {   for(const auto& pers : perses)
        {   pers.info();
        }
    }

private:
    std::vector<model::Person> perses
    {   {"Bot:Pete"   },
        {"Bot:Ann"    },
        {"Вася Пупкин"}
    };

    model::Field field;

    void  loop()
    {
        info();

        unsigned step{0};

        for(bool isDone = true; isDone;)
        {
            std::cout   << "\nПАУЗА::Нажмите ENTER, чтобы сделать "
                        << ++step << " шаг ... \n";

            std::cin.get();

            for(auto& pers : perses)
            {
                const unsigned randNumber = rand() % 6;
                l(randNumber)

                const auto&[pos, isStart]
                    = field.add(pers.position, randNumber);

                if(isStart)
                {   if(++pers.status == 3) pers.status  = 0;

                    ++pers.circle;
                }

                pers.position = pos;

                pers.info();
            }
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
///----------------------------------------------------------------------------:
void tests()
{
/// TESTCLASS(model::Cell::test  );
/// TESTCLASS(model::Config::test);
/// TESTCLASS(model::Field::test );
/// TESTCLASS(model::Person::test);

    TESTCLASS(TestGame::test);
}


///----------------------------------------------------------------------------|
/// Старт программы.
///----------------------------------------------------------------------------:
int main()
{   std::system ("chcp 65001>nul");
    std::cout << "Привет Мир!\n" << std::endl;

    tests ();

    std::cout << "Программа закончила работу.\n" << std::endl;

    return 0;
}
