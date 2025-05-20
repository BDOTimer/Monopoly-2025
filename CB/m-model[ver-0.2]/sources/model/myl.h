#ifndef MYL_H
#define MYL_H
///----------------------------------------------------------------------------|
/// "myl.h"
///----------------------------------------------------------------------------:
#include "../debug.h"
#include "config-model.h"

#include <locale>
#include <codecvt>


template<typename T>
std::ostream& operator<<(std::ostream& o, const std::vector<T>& m)
{   for(auto& e : m) o << e << ' ';
    return o;
}


inline std::string WstrToUtf8(const std::wstring& str)
{   std::wstring_convert<std::codecvt_utf8<wchar_t> > strCnv;
    return strCnv.to_bytes(str);
}

inline std::wstring utf8ToWstr(const std::string& str)
{   std::wstring_convert< std::codecvt_utf8<wchar_t> > strCnv;
    return strCnv.from_bytes(str);
}

///---------|
/// my lib  |
///---------:
namespace myl
{

    ///------------------------------------------------------------------------|
    /// Получение строки utf8 заданой длины.
    ///------------------------------------------------------------------------:
    inline std::string& setwUtf8(const unsigned n, std::string& s)
    {   s.resize (n + s.size() - utf8ToWstr(s).size(), ' ');
        return s;
    }

    ///------------------------------------------------------------------------|
    /// Рандомный перемешиватель массива.
    ///------------------------------------------------------------------------:
    template<typename T>
    void mixer(T& m)
    {   for(unsigned i = 0; i < m.size(); ++i)
        {   const unsigned  rndN = rand() % m.size();
            std::swap(    m[rndN], m[i]);
        }
    }


    ///------------------------------------------------------------------------|
    /// Из Правил Игры:
    ///
    /// "Игроки выбирают фишки. Далее определяют, кто будет ходить первым.
    ///  Это делают броском кубиков: чем больше число выпало,
    ///  тем раньше ходит игрок."
    ///
    /// Далее:
    ///     -   если кубики выдали равные значения, то продолжается повторное
    ///         вбрасывание с суммированием результата, по все результаты,
    ///         будут отличаться.
    ///     -   это нужно для симуляции, играбельности, визуализаци...
    ///     -   getFastOrder() быстрый вариант - даёт рандомный порядок сразу.
    ///------------------------------------------------------------------------:

    ///------------------------------------------------------------------------|
    /// Решальщьик того, кто будет играть первым.
    ///     -   переиспользоваемый
    ///------------------------------------------------------------------------:
    struct  WhoFirstPlayer
    {   WhoFirstPlayer(unsigned nPlayers) : gen(nPlayers)
        {
        }

        ///-------------------------|
        /// Данные.                 |
        ///-------------------------:
        struct IdRnd
        {   unsigned id, rnd, sumRnd;
            void set(unsigned r)
            {   rnd     = r;
                sumRnd += r;
            }
        };

        ///-------------------------|
        /// Старт жеребьевки.       |
        ///-------------------------:
        void start()
        {   unsigned  i{ };
            for(auto&[id, rnd, s] : gen)
            {   rnd = 0  ;
                id  = i++;
            }
        }

        ///-------------------------|
        /// Финиш жеребьевки.       |
        ///-------------------------:
        bool isReady() const
        {   return ready;
        }

        ///-------------------------|
        /// Получить порядок.       |
        ///-------------------------:
        const std::vector<IdRnd>& getOrder()
        {   ready = doGen();
            return gen;
        }

        ///-------------------------|
        /// Без симуляции.          |
        ///-------------------------:
        static std::vector<unsigned> getFastOrder(const unsigned Size)
        {   std::vector<unsigned>       m( Size);
            unsigned i{};
            for(auto& e : m) e = i++;
            for(unsigned i = 0; i < m.size();  i++)
            {   std::swap(m[i], m[rand()%Size]);
            }
            return m;
        }

        ///-------------------------|
        ///  debug.                 |
        ///-------------------------:
        void debug() const
        {   for(const auto&[id, rnd, sum] : gen)
            {   std::cout <<    "id: " << (id + 1)
                          << ", rnd: " << rnd
                          << ", sum: " << sum << '\n';
            }
            std::cout << '\n';
        }

        ///-------------------------|
        ///  Подвал.                |
        ///-------------------------:
private:
        bool               ready{false};
        std::vector<IdRnd   > gen      ;

        bool doGen()
        {   for(auto& e : gen)
            {   e.set(rand() % 6 + 1);
            }
            return checkIsReady();
        }

        bool checkIsReady()
        {   std::sort(gen.begin(), gen.end(),
                      [](const IdRnd &a, const IdRnd &b)
            {   return a.rnd > b.rnd;
            }
                     );

            for(unsigned i = 1; i < gen.size(); ++i)
            {   if(gen[i].rnd == gen[i-1].rnd) return false;
            }

            return true;
        }

        ///-------------------------|
        ///  Тест.                  |
        ///-------------------------:
        static void test()
        {   srand(2025);

            WhoFirstPlayer whoFirstPlayer     (5);
            whoFirstPlayer.start();

            while(!whoFirstPlayer.isReady())
            {   const auto result = whoFirstPlayer.getOrder();
                whoFirstPlayer.debug   ();
            }

            std::cout << "getFastOrder(): " << getFastOrder(5) << '\n';
        }

        friend void tests();
    };

    ///------------------------------------------------------------------------|
    /// myl::tests()
    ///------------------------------------------------------------- myl::tests:
    inline void tests ()
    {   std::cout << "myl::tests():" << std::endl;

        myl::WhoFirstPlayer::test();
    }
}

#endif // MYL_H
