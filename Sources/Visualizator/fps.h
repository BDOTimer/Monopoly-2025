#ifndef FPS_H
#define FPS_H
///----------------------------------------------------------------------------|
/// "fps.h"
///----------------------------------------------------------------------------:
#include "common.h"


namespace vsl
{
    struct  Fps : sf::Drawable
    {       Fps()
                :   fon(size)
                ,   txt(vsl::Resurces::getFont(), "FPS: ....", 14)
            {
                fon.setOutlineThickness(   1                );
                fon.setOutlineColor    ({  0,   0, 255, 128});
                fon.setFillColor       ({  0, 255, 128, 64 });
                txt.setFillColor       ({128, 255, 128     });
            }

        sf::Vector2f position;
        sf::Vector2f size    { 100, 24 };
        sf::Time     deltaTime;

        std::function<void()> fooTimers{[](){}};

        void init(sf::Vector2f szfWin)
        {   position = { szfWin.x - size.x - 20, 0  };
            fon.setPosition({position.x       ,  position.y + 5.f});
            txt.setPosition({position.x + 15.f,  position.y + 6.f});
        }

        const sf::Time& getDeltaTime()
        {
            auto&       dt = deltaTime;
                        dt = clock.restart();
            fpsTime +=  dt.asSeconds();
            fps     ++;
            calc    ();

            return  dt;
        }


    private:
        sf::RectangleShape fon;
        sf::Text           txt;

        sf::Clock    clock;
        float    fpsTime{};
        unsigned     fps{};
        uint64_t timeSec{};

        void calc()
        {   if (fpsTime >= 1.0f)
            {
                std::ostringstream ss;
                ss << "FPS: " << std::setw(4) << fps;

                fps      =   0 ;
                fpsTime -= 1.0f;
                timeSec++;

                txt.setString(ss.str());

                ///-------------------------------|
                /// Проверка таймеров 1 раз в сек.|
                ///-------------------------------:
                fooTimers();
            }
        }

        ///------------------------------------|
        /// На рендер.                         |
        ///------------------------------------:
        virtual void draw(sf::RenderTarget& target,
                          sf::RenderStates  states) const
        {   target.draw( fon, states);
            target.draw( txt, states);
        }

        friend struct Timers;
    };


    ///------------------------------------------------------------------------|
    /// Timer.
    ///------------------------------------------------------------------ Timer:
    struct Timer
    {
        uint64_t timeFoo;
        std::function<void()> foo;

        void setPeriod(uint64_t timeFoo) { this->timeFoo = timeFoo; }

    };


    using ITimer = std::list<Timer>::iterator;
    ///------------------------------------------------------------------------|
    /// Timers.
    ///----------------------------------------------------------------- Timers:
    struct  Timers
    {       Timers(Fps& fps) : fps(fps)
            {
            }

        ITimer add() { m.emplace_front(Timer()); return m.begin(); }

        void setPeriod(ITimer itimer, uint64_t timeFoo)
        {   itimer->timeFoo = timeFoo + fps.timeSec;
            mm.insert({itimer->timeFoo, itimer});
        }

    private:
        Fps&                           fps;
        std::list<Timer>                 m;
        std::multimap<uint64_t, ITimer> mm;

        TEST
        {
            std::cout << "Timers::test()\n";

            bool done{true};

            sf::RenderWindow window;
            Fps              fps;

            Timers     timers(fps);
            auto  it = timers.add();
                       timers.setPeriod(it, 7);
                       it->foo = [&done]()
                       {    done = false;
                       };

            fps.fooTimers = [&timers]()
            {   for(auto i = timers.mm.begin(); i != timers.mm.end();)
                {
                    const auto&[t, p] = *i;

                    if(t != timers.fps.timeSec) break;
                    p->foo();

                    const auto ii = i++;

                    timers.mm.erase(ii);
                }
                l(timers.fps.timeSec)
            };

            while(done)
            {
                fps.getDeltaTime();

                using namespace std::chrono_literals;
                std::this_thread::sleep_for(1ms);
            }
            std::cout << "Finished Timers::test()\n";
        }
    };
}


#endif // FPS_H
