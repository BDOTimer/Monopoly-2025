#ifndef RENDER_H
#define RENDER_H
///----------------------------------------------------------------------------|
/// "render.h"
///----------------------------------------------------------------------------:
#include "model.h"
#include "ui-imgui.h"

#include "scene-logo.h"
#include "scene-tune.h"
#include "scene-game.h"

#include "user-model.h"
#include "model/config-model.h"
#include "controller/controller.h"

struct  Render
{       Render(vsl::Config& cfg)
			:	cfg(cfg)
			,	window(	sf::VideoMode(cfg.szuWin),
                        L"Монополия-2025",
                        sf::Style::Titlebar | sf::Style::Close)
            ,	ui    (window)
            ,	camFon(window.getDefaultView())
        {
            cfg.pwin = &window;

            sf::Image      icon("icon.png");
            window.setIcon(icon);
            window.setFramerateLimit(60);

            camGui = window.getDefaultView();
            camFon.setCenter({0,0});

            cfg.camFon = &camFon;
            cfg.camGui = &camGui;
			cfg.scenesSwitcher.init(&scenes);

            startModel();
        }

	vsl::Config&        cfg;
    sf::RenderWindow window;

    ///---------------------|
    /// Gui.                |
    ///---------------------:
    uii::UITest           ui;

    ///---------------------|
    /// Камера.             |
    ///---------------------:
    sf::View          camFon;
    sf::View          camGui;

	vsl::ScenesAll scenes
    {   &logo,
        &tune,
        &game,
    };

    void run()
    {   loop(scenes);
    }

private:
    vsl::SceneLogo logo{cfg};
    vsl::SceneTune tune{cfg};
    vsl::SceneGame game{cfg};

    ///---------------------|
    /// Часы.               |
    ///---------------------:
    sf::Clock          clock;

    ////////////////////////////////////////////////////////////////////////////
    unsigned    idGame;
    model::Config cfgM;

    std::array<controller::Player, 3> players
    {          controller::Player (0),
               controller::Player (1),
               controller::Player (2)
    };

    bool done{true};

    void startModel()
    {   cfgM   = model::getConfig();
        idGame = cfgM.idGame;
        ui << model::getLogo(idGame) << "\n";
    }
    ////////////////////////////////////////////////////////////////////////////

    void loop(vsl::ScenesAll& scenes)
    {
    /// ui << "Привет, Монополия-2025!\n";

    /// using Key  = sf::Keyboard::Key ;
    /// using Scan = sf::Keyboard::Scan;

		auto& nScene   = cfg.scenesSwitcher.nScene;
		auto& nowScene = cfg.scenesSwitcher.nowScene;

        unsigned IDPLAYER{0};
        unsigned cnt     {0};
        info_01      (++cnt);

        ui << "///-----------------------------------|\n"
              "///         ИГРА НАЧАЛАСЬ!            |\n"
              "///-----------------------------------:\n" << '\n';

        while (window.isOpen())
        {   while (const std::optional event = window.pollEvent())
            {   ImGui::SFML::ProcessEvent(window, *event);

                if (event->is<sf::Event::Closed>()) window.close();

                if (event->is<sf::Event::KeyPressed>())
                {   if (ISKEYPRESSED(Num1))
                    {   /// vsl::cfg.scenesSwitcher.next();
                    }

                    if (ISKEYPRESSED(Enter))
                    {   
                        unsigned& idPlayer = players[IDPLAYER].id;
                        ui  << model::doStep( "bot", { (int)idGame,
                                                       (int)idPlayer } );
                        if(++IDPLAYER == players.size()) IDPLAYER = 0;

                        info_01(++cnt);
                    }
                }
				nowScene->input(event);
            }

            const auto delta  = clock.restart();

            ///----------------------|
            /// ImGui::SFML.         |
            ///----------------------:
            ImGui::SFML::Update(window, delta);
            /// ...
            /// ImGui::ShowDemoWindow();

            switch(nScene)
            {   case 0:            break;
                case 1: ui.show(); break;
                case 2:            break;
            }

            /// window.clear   ({0, 30, 60});

            window.setView       (camFon);
            window.draw       (*nowScene);

            window.setView       (camGui);

            ImGui::SFML::Render  (window);
            window.display       (      );
        }
        #undef ISKEYPRESED
    }

    void info_01(unsigned cnt)
    {   ui  << "ПАУЗА::Нажмите ENTER, чтобы сделать "
            << cnt << " шаг ...\n"
            <<  "------------------------------------"
                "----------------------------------...\n";
    }

    ///--------------------------------------|
    /// Тест разраба.                        |
    ///--------------------------------------:
    TEST
    {   vsl::Config     cfg;
		Render  render (cfg);
                render.run();
    }
};

#endif // X_H
