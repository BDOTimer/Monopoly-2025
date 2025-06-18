#ifndef FIG_GAMEOVER_H
#define FIG_GAMEOVER_H
///----------------------------------------------------------------------------|
/// "fig-gameover.h"
///----------------------------------------------------------------------------:
#include "../common.h"

namespace vsl
{
    ///------------------------------------------------------------------------|
    /// FigureGameOver
    ///--------------------------------------------------------- FigureGameOver:
    struct  NameWinner : sf::Text
    {       NameWinner(vsl::Config& cfg)
                :   sf::Text(vsl::Config::getFont()),
                    cfg(cfg)
            {
                setCharacterSize(48);
                setFillColor({100,200,200});
                setOutlineThickness(2);
                setOutlineColor({200,200,200});
                setName("test::NameWinner");
            }

        void setName ( std::string_view name)
        {   setString( name.data());
            setOrigin({getGlobalBounds().size.x / 2,
                       getGlobalBounds().size.y / 2});
        }

    private:
        vsl::Config& cfg;
    };


    ///------------------------------------------------------------------------|
    /// FigureGameOver
    ///--------------------------------------------------------- FigureGameOver:
    struct  FigureGameOver : vsl::IObject
    {       FigureGameOver ( vsl::Config& cfg )
                :   cfg       (cfg)
                ,   nameWinner(cfg)
                ,   scrRect   (cfg.szfWin)
            {
                const auto& R{cfg.markupSG.getWinBase()};
                const sf::Vector2u sz = { (unsigned)(R.size.x * cfg.szfWin.x),
                                          (unsigned)(R.size.y * cfg.szfWin.y)};
                if(!rt.resize(sz))
                {   ASSERT(false)
                }

                //scrRect.setSize(pr::cv( rt.getSize(  )));
                scrRect.setTexture(    &rt.getTexture());
                pr::setOrigin     (scrRect);

                camRt.setSize  (scrRect.getSize());
                camRt.setCenter({0, 0});
                rt.setView     (camRt);
            }

        vsl  ::Config&      cfg;
        NameWinner   nameWinner;
        bool isGameOver {false};

        PLUG_IOBJECT

        inline static const char* filename{"res/shaders/firework-1.frag"};

        sf::RenderTexture       rt;
        sf::View             camRt;
        sf::RectangleShape scrRect;
        sf::Shader         shader ;
        float              iTime{};
        float          iResolution;
        float          speed{1.0f};

        void init()
        {
            if (!sf::Shader::isAvailable())
                return throw("!sf::Shader::isAvailable()");

            if (!shader.loadFromFile(filename, sf::Shader::Type::Fragment))
                return throw("shader.loadFromFile(...");
        }

    private:

        ///------------------------------------|
        ///  На рендер.                         |
        ///------------------------------------:
        virtual void draw(sf::RenderTarget& target,
                          sf::RenderStates  states) const
        {
            auto p = const_cast<FigureGameOver*>(this);

            const auto& sz{scrRect.getSize()};

            p->shader.setUniform("iResolution", sf::Glsl::Vec2(sz));
            p->shader.setUniform("iTime"      , p->iTime += speed * cfg.dt());

            p->rt .clear({0,0,0,0});
            p->rt .draw(scrRect    , states.shader = &shader);
            target.draw(scrRect   );
            target.draw(nameWinner);
        }
    };
}


#endif // FIG_GAMEOVER_H
