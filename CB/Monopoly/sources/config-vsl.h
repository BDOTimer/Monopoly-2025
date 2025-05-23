﻿#ifndef X_H
#define X_H
///----------------------------------------------------------------------------|
/// "config-vsl.h"
///----------------------------------------------------------------------------:
#include "ui-imgui.h"

#include "user-model.h"
#include "model/config-model.h"
#include "controller/controller.h"

#include "sceneGame/markup.h"

#include "debug.h"

namespace vsl
{
    struct  Config
    {       Config (sf::RenderWindow& window)
                :   pwin            (&window)
                ,   uiTune           (window, "Настройки ...")
            {
                init           ();
                resizeFormImgui();
            }

        sf::RenderWindow*        pwin;

        MarkupSceneGame      markupSG;

        sf::Vector2u         szuWin  ;
        sf::Vector2f         szfWin  ;

        sf::View*     camFon{nullptr};
        sf::View*     camGui{nullptr};

		ScenesSwitcher scenesSwitcher;

        sf::Time            deltaTime;

		Music musicLogo{"res/snd/Maddix - Receive Life.mp3"};
		Music musicGame{"res/snd/Maddix - Acid Soul.mp3"   };

		uii::UITest uiTune   ;
        uii::UIGame uiGameLog;

        std::array<controller::Player, 3> players
        {   controller::Player (0),
            controller::Player (1),
            controller::Player (2)
        };

        model::Config cfgModel;

        ///////////////////////////////////////////////////////////////////////:
        unsigned    idGame;
        ///////////////////////////////////////////////////////////////////////.

        static sf::Font& getFont()
        {///static sf::Font font("consola.ttf");
			static sf::Font font("res/JetBrainsMono-Regular.ttf");
            return font;
        }

        void init()
        {
            szuWin = initWinSize();

            szfWin = {float(szuWin.x), float(szuWin.y)};

            getFont().setSmooth(true);
        }

        inline static constexpr float SCALE_WIN{0.9f};

        static sf::Vector2u initWinSize(sf::Vector2u szuWin = {0,0})
        {
            if(szuWin.x == 0)
            {   sf::VideoMode  dm = sf::VideoMode::getDesktopMode();
                    szuWin   = dm.size;
                    szuWin.x = unsigned(SCALE_WIN * szuWin.x);
                    szuWin.y = unsigned(SCALE_WIN * szuWin.y);
            }
            return  szuWin;
        }

        template<typename T>
        static void setOrigin(T& o)
        {   o.setOrigin({ o.getSize().x / 2, o.getSize().y / 2 });
        }

        void info_01(unsigned cnt)
        {
            uiGameLog << "ПАУЗА::\nНажмите ENTER, чтобы сделать "
                      << cnt << " шаг ...\n"
                      <<  "------------------------------"
                          "-------------------------...\n";
        }

        void      setFramerateLimit(unsigned fps = 10000)
        {   pwin->setFramerateLimit(fps);
        }

        const float dt() const { return deltaTime.asSeconds(); }

        static vsl::Config&    get()
        {   sf::RenderWindow window(sf::VideoMode({ 1344, 768 }),
                                   "SFML::Test::2", sf::State::Windowed);
            static vsl::Config cfg (window);
            return             cfg;
        }

        void resizeFormImgui()
        {
            ///-------------------|
            /// uiGameLog         |
            ///-------------------:
            {   const auto  px = 0;
                const auto& py = markupSG.winUp.size.y;
                const auto& sx = markupSG.winPlayer[0].size.x;
                const auto& sy = markupSG.getWinBase().size.y;

                uiGameLog.position = {px           , py * szfWin.y};
                uiGameLog.size     = {sx * szfWin.x, sy * szfWin.y};
            }
        }
    };
}

#endif // X_H
