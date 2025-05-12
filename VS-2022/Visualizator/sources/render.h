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

    void loop(vsl::ScenesAll& scenes)
    {
        ui << "Привет, Монополия-2025!\n";

    /// using Key  = sf::Keyboard::Key ;
    /// using Scan = sf::Keyboard::Scan;

		auto& nScene   = cfg.scenesSwitcher.nScene;
		auto& nowScene = cfg.scenesSwitcher.nowScene;

        while (window.isOpen())
        {   while (const std::optional event = window.pollEvent())
            {   ImGui::SFML::ProcessEvent(window, *event);

                if (event->is<sf::Event::Closed>()) window.close();

                if (event->is<sf::Event::KeyPressed>())
                {   if (ISKEYPRESSED(Enter))
                    {   /// vsl::cfg.scenesSwitcher.next();
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
