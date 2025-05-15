#ifndef X_H
#define X_H
///----------------------------------------------------------------------------|
/// "config-vsl.h"
///----------------------------------------------------------------------------:
#include "ui-imgui.h"

#include "user-model.h"
#include "model/config-model.h"
#include "controller/controller.h"

#include "debug.h"

namespace vsl
{
    struct  Config
    {       Config (sf::RenderWindow& window)
                :   pwin            (&window)
                ,   uiTune           (window, "Настройки ...")
                ,   uiGameLog        (window, "Игра ...")
            {
                init();
            }

        sf::RenderWindow* pwin;

        sf::Vector2u szuWin  ;
        sf::Vector2f szfWin  ;

        sf::View* camFon{nullptr};
        sf::View* camGui{nullptr};

		ScenesSwitcher scenesSwitcher;

		Music musicLogo{"res/snd/Maddix - Receive Life.mp3"};
		Music musicGame{"res/snd/Maddix - Acid Soul.mp3"   };

		uii::UITest uiTune   ;
		uii::UITest uiGameLog;

        std::array<controller::Player, 3> players
        {   controller::Player (0),
            controller::Player (1),
            controller::Player (2)
        };

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

        inline static constexpr float SCALE_WIN{0.8f};

        static sf::Vector2u initWinSize()
        {   sf::VideoMode           dm = sf::VideoMode::getDesktopMode();
            sf::Vector2u szuWin   = dm.size;
                         szuWin.x = unsigned(SCALE_WIN * szuWin.x);
                         szuWin.y = unsigned(SCALE_WIN * szuWin.y);
            return       szuWin;
        }

        template<typename T>
        static void setOrigin(T& o)
        {   o.setOrigin({ o.getSize().x / 2, o.getSize().y / 2 });
        }

        void info_01(unsigned cnt)
        {
            uiGameLog << "ПАУЗА::Нажмите ENTER, чтобы сделать "
                      << cnt << " шаг ...\n"
                      <<  "------------------------------------"
                          "----------------------------------...\n";
        }
    };
}

#endif // X_H
