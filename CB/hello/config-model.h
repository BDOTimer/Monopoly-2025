#ifndef CONFIG_MODEL_H
#define CONFIG_MODEL_H
///----------------------------------------------------------------------------|
/// "config-model.h"
/// Дефолтный дизайн для детей! (детский вариант)
///----------------------------------------------------------------------------:
#define _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string_view>
#include <vector>
#include <format>
#include <tuple>
#include <map>


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
    ///------------------------------------------------------------------ Rules:
    struct Rules
    {
        std::vector<std::vector<std::vector<unsigned>>> prices
        {
            /// Ребенок:
            {   {   120,  1, 13, 22 },
                {   100, 16, 28,  7 },
                {    80,  7, 19, 25 },
                {    60, 10         }
            },

            /// Взрослый:
            {   {   120,  8, 17, 29 },
                {   100,  2, 11, 23 },
                {    80, 14, 26, 20 },
                {    60,  5         }
            },

            /// Родитель:
            {   {   120, 16, 18, 24 },
                {   100,  9, 21, 27 },
                {    80,  3, 12, 30 },
                {    60, 15         }
            }
        };
    };


    ///------------------------------------------------------------------------|
    /// Конфигурация Модели.
    ///----------------------------------------------------------------- Config:
    struct Field;
    struct Config
    {
        bool loadFromFile(std::string_view filename)
        {   /// TODO ...
            return true;
        }

        ///------------------------------|
        /// Количество ячеек на поле.    |
        ///------------------------------:
        unsigned amountCells      { 30 };
        unsigned amountSatusesCell{ 3  };

        std::vector<std::string> statuses
        { "123", "231", "312"
        };

        unsigned W{ 9 };
        unsigned H{ 8 };

        ///------------------------------|
        /// Ячейки с шансами.            |
        ///------------------------------:
        std::vector<unsigned> chances
        {   4, 7, 9, 12, 16, 19, 24, 26, 29
        };

        ///------------------------------|
        /// Дизайн карточек для поля.    |
        /// Ограничение:                 |
        ///     size == amountCells      |
        ///------------------------------:
        std::vector<std::vector<std::string>> cardNames
        {
            {   "Гражданская промышленность" , "texture1.jpg",
                ///——————————————————————————————————————————:
                "Будет танковый завод"       , "texture2.jpg",
                "Артиллерийский"             , "texture3.jpg",
                "Авиационный"                , "texture4.jpg",
                "Завод по выпуску автоматов" , "texture5.jpg",
                "Завод по выпуску пистолетов", "texture6.jpg",
                "Завод катюш"                , "texture7.jpg",
                "Тактических ракет"          , "texture8.jpg",
                "Вертолётов"                 , "texture9.jpg",
                "Военного обмундирования"    , "txtr10.jpg",
                "Военных грузовиков"         , "text11.jpg"
            },
            {
                "Детская промышленность"     , "txtr12.jpg",
                ///————————————————————————————————————————:
                "Завод воздушных шариков"    , "txtr13.jpg",
                "Фабрика велосипедов"        , "txtr14.jpg",
             "Фабрика костюмов для мальчиков", "txtr15.jpg",
                "Фабрика платьев для девочек", "txtr16.jpg",
                "Фабрика кукол"              , "txtr17.jpg",
                "Фабрика детский автомобилей", "txtr18.jpg",
   "Фабрика по выпуску роботов трансформеров", "txtr19.jpg",
                "Фабрика самокатов"          , "txtr20.jpg",
            //  "Фабрика детских роликов"    , "txtr21.jpg",
                "Фабрик детской обуви"       , "txtr22.jpg",
                "Фабрика плюшевых мишек"     , "txtr23.jpg"
            },
            {
                "Гражданский сектор продукты", "txtr24.jpg",
                ///————————————————————————————————————————:
                "Молоко завод"               , "txtr25.jpg",
                "Колбасный завод"            , "txtr26.jpg",
                "Хлебо завод"                , "txtr27.jpg",
                "Кондитерская"               , "txtr28.jpg",
                "Консервный завод"           , "txtr29.jpg",
                "Завод макаронных изделий"   , "txtr30.jpg",
                "Завод газированных напитков", "txtr31.jpg",
                "Завод по выпуску соков"     , "txtr32.jpg",
                "Фабрика мороженного"        , "txtr33.jpg",
                "Фруктовая ферма"            , "txtr34.jpg"
            }
        };

        ///------------------------------|
        /// Перемешать порядок карточек? |
        ///     false - нет              |
        ///     true  - да               |
        ///------------------------------:
        bool isMixerCards{false};

        ///------------------------------|
        /// Денеги у игроков на старте.  |
        ///------------------------------:
        unsigned startMoney{300};

        const char* statusNames[4]
        {   "Ребёнок ",
            "Взрослый",
            "Родитель",
            "Чужой   "
        };

        Field* pfield{nullptr};

        ///------------------------------|
        /// Скроллить в консоли?.        |
        ///------------------------------:
        bool isScrollConsole{/*ДА:*/true};

        ///------------------------------|
        /// Особые Правила.              |
        ///------------------------------:
        Rules rules;

        ///------------------------------|
        /// Расшифровка статуса.         |
        ///------------------------------:
        std::string_view decodeStatus(unsigned status) const
        {   constexpr unsigned N = sizeof statusNames / sizeof *statusNames ;
            return statusNames[status >= N ? N - 1 : status];
        }

        ///------------------------------|
        /// Билд массива статусов.       |
        ///------------------------------:
        const Array2U getGen() const
        {
            Array2U m(amountCells, Array1U(amountSatusesCell));

            for (unsigned i = 0, k = 0; i < m.size(); ++i)
            {
                const   auto s = statuses[k];
                for (unsigned j = 0; j < amountSatusesCell; ++j)
                {
                    m[i][j] = s[j] - '0';
                }

                if (++k == statuses.size()) k = 0;
            }

            return m;
        }

        ///------------------------------|
        /// Проверка конфига на ошибки.  |
        ///------------------------------:
        bool doValidation() const
        {
            for (const auto& s : statuses)
            {   if (s.size() != amountSatusesCell) return false;
            }

            if (W * 2 + (H - 2) * 2 != amountCells) return false;

            //if(chances.size() <= amountCells) chances.resize(amountCells);

            if(!chances.empty() && 0 == chances.front())
            {
                /// TODO: Сделать рандомную генерацию шансов ...
            }

            ///------------------------------|
            /// Все карточки должны иметь    |
            /// уникальные имена и кол-во    |
            /// имен должно быть равно       |
            /// кол-ву карточе.              |
            ///------------------------------:
            {   size_t cnt{};
                for(const auto& e : cardNames)
                {   cnt += e.size() - 2;
                }
                if(cnt != amountCells * 2) return false;
            }

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

        void infoValidation() const
        {   std::cout << "\nmodel::Config::doValidation() ---> "
                      << (doValidation() ? "SUCCESS!" : "FAIL ...") << "\n\n";
        }

        void info() const
        {
            infoValidation();

            int cnt{};
            for (const auto& r : getGen())
            {
                std::cout << ++cnt << "\t: ";
                for (const auto n : r)
                {
                    std::cout << n;
                }   std::cout << '\n';
            }       std::cout << '\n';
        }

        ///------------------------------|
        /// Тест класса.                 |
        ///------------------------------:
        TEST
        {
            {   Config config;
                     l(config.getDefault().amountCells)
                       config.info();
            }

            {   Config config{8, 4, {"4321","6789"}, 3, 3};
                l(config.amountCells)
                config.info();
            }
        }
    };

}

#endif // CONFIG_MODEL_H
