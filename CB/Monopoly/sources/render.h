#ifndef RENDER_H
#define RENDER_H
///----------------------------------------------------------------------------|
/// "render.h"
///----------------------------------------------------------------------------:
#include "model.h"

#include "scene-logo.h"
#include "scene-tune.h"
#include "scene-game.h"

#include "fps.h"

struct  Render
{       Render( vsl::Config& cfg)
			:	cfg         (cfg)
			,	window(     *cfg.pwin)
            ,	camFon(   window.getDefaultView() )
            ,   fps   (      cfg)
        {
            cfg.pwin = &window;

            sf::Image         icon("icon.png");
            window.setIcon   (icon);
            cfg.setFramerateLimit();

            camGui = window.getDefaultView();
            camFon.setCenter({0,0});

            cfg.camFon = &camFon;
            cfg.camGui = &camGui;
			cfg.scenesSwitcher.init(&scenes);

            startModel();
        }

	vsl::Config&         cfg;
    sf::RenderWindow& window;

    ///---------------------|
    /// Камера.             |
    ///---------------------:
    sf::View          camFon;
    sf::View          camGui;

    ///---------------------|
    /// Fps.                |
    ///---------------------:
    vsl::Fps             fps;

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

    bool done{true};

    ///////////////////////////////////////////////////////////////////////////:

    void startModel()
    {   cfg.cfgModel = model::getConfig();
        cfg.idGame   = cfg.cfgModel.idGame;
        cfg.uiTune << model::getLogo(cfg.idGame) << "\n";
    }
    ///////////////////////////////////////////////////////////////////////////.

    void loop(vsl::ScenesAll& scenes)
    {
    /// ui << "Привет, Монополия-2025!\n";

    /// using Key  = sf::Keyboard::Key ;
    /// using Scan = sf::Keyboard::Scan;

	/// auto& nScene   = cfg.scenesSwitcher.nScene;
		auto& nowScene = cfg.scenesSwitcher.nowScene;

        cfg.uiGameLog << "///-----------------------------------|\n"
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
                    }

                    nowScene->input(event);
                }

                if (/*auto p = */event->getIf<sf::Event::MouseButtonPressed>())
                {   nowScene->input(event);
                }
            }

            const auto& delta  = fps.getDeltaTime();

            ///----------------------|
            /// ImGui::SFML.         |
            ///----------------------:
            ImGui::SFML::Update(window, delta);
            /// ...
            /// ImGui::ShowDemoWindow();

            /// window.clear   ({0, 30, 60});

            window.setView       (camFon);
            window.draw       (*nowScene);

            window.setView       (camGui);
            window.draw             (fps);

            ImGui::SFML::Render  (window);
            window.display       (      );
        }
        #undef ISKEYPRESED
    }

    ///--------------------------------------|
    /// Тест разраба.                        |
    ///--------------------------------------:
    TEST
    {   sf::RenderWindow window
        (	sf::VideoMode(vsl::Config::initWinSize()),
            L"Монополия-2025",
            sf::Style::Titlebar | sf::Style::Close
        );

        window.setPosition({window.getPosition().x, 0});

        vsl::Config     cfg(window);
		Render  render (cfg);
                render. run();
    }
};

#endif // X_H
