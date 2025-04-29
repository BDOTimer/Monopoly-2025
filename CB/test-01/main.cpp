
///----------------------------------------------------------------------------|
/// �� ������ ����:
///
/// "������ �������� �����. ����� ����������, ��� ����� ������ ������.
///  ��� ������ ������� �������: ��� ������ ����� ������,
///  ��� ������ ����� �����."
///
/// �����:
///     -   ���� ������ ������ ������ ��������, �� ������������ ���������
///         ����������� � ������������� ����������, �� ��� ����������,
///         ����� ����������.
///     -   ��� ����� ��� ���������, �������������, �����������...
///     -   getFastOrder() ������� ������� - ��� ��������� ������� �����.
///----------------------------------------------------------------------------:

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>


template<typename T>
std::ostream& operator<<(std::ostream& o, const std::vector<T>& m)
{   for(auto& e : m) o << e << ' ';
    return o;
}

int main();

///---------|
/// my lib  |
///---------:
namespace myl
{

    ///------------------------------------------------------------------------|
    /// ���������� ����, ��� ����� ������ ������.
    ///     -   ������������������
    ///------------------------------------------------------------------------:
    struct  WhoFirstPlayer
    {       WhoFirstPlayer(unsigned nPlayers) : gen(nPlayers)
            {
            }

        ///-------------------------|
        /// ������.                 |
        ///-------------------------:
        struct IdRnd
        {   unsigned id, rnd, sumRnd;
            void set(unsigned r)
            {   rnd     = r;
                sumRnd += r;
            }
        };

        ///-------------------------|
        /// ����� ����������.       |
        ///-------------------------:
        void start()
        {   unsigned  i{ };
            for(auto&[id, rnd, s] : gen)
            {   rnd = 0  ;
                id  = i++;
            }
        }

        ///-------------------------|
        /// ����� ����������.       |
        ///-------------------------:
        bool isReady() const
        {   return ready;
        }

        ///-------------------------|
        /// �������� �������.       |
        ///-------------------------:
        const std::vector<IdRnd>& getOrder()
        {   ready = doGen();
            return gen;
        }

        ///-------------------------|
        /// ��� ���������.          |
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
        ///  ������.                |
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
        ///  ����.                  |
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

        friend int ::main();
    };

}


///----------------------------------------------------------------------------|
/// main
///----------------------------------------------------------------------------:
int main()
{   std::cout << "Hello world!" << std::endl;

    myl::WhoFirstPlayer::test();

    return 0;
}
