#ifndef FPS_H
#define FPS_H
///----------------------------------------------------------------------------|
/// "fps.h"
///----------------------------------------------------------------------------:
#include "common.h"

namespace vsl
{
    struct  Fps : sf::Drawable
    {       Fps(vsl::Config& cfg)
                :   cfg(cfg )
                ,   fon(size)
                ,   txt(vsl::Config::getFont(), "FPS: ....", 14)
            {
                position = { cfg.szfWin.x - size.x - 20, 0  };

                fon.setOutlineThickness(   1                );
                fon.setOutlineColor    ({  0,   0, 255, 128});
                fon.setFillColor       ({  0, 255, 128, 64 });
                txt.setFillColor       ({128, 255, 255     });

                fon.setPosition({position.x       ,  position.y + 5.f});
                txt.setPosition({position.x + 15.f,  position.y + 6.f});
            }

    vsl::Config& cfg;

    sf::Vector2f position ;
    sf::Vector2f size     {100, 24};

    const sf::Time& getDeltaTime()
    {
        auto&       dt = cfg.deltaTime;
                    dt = clock.restart();
        fpsTime +=  dt.asSeconds();
        fps     ++;
        calc    ();

        return dt;
    }

    private:
        sf::RectangleShape fon;
        sf::Text           txt;

        sf::Clock  clock;
        float  fpsTime{};
        unsigned   fps{};

        void calc()
        {   if (fpsTime >= 1.0f)
            {
                std::ostringstream ss;
                ss << "FPS: " << std::setw(4) << fps;

                fps      =   0 ;
                fpsTime -= 1.0f;

                txt.setString(ss.str());
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
    };
}


#endif // FPS_H
