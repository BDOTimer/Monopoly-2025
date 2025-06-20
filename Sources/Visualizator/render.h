#ifndef RENDER_H
#define RENDER_H
///----------------------------------------------------------------------------|
/// "render.h"
///----------------------------------------------------------------------------:
#include "model.h"

#include "scene-logo.h"
#include "scene-tune.h"


struct  Render
{       Render( vsl::Config& cfg)
            :   cfg         (cfg)
            ,   window (    *cfg.pwin)
            ,   camFon (  window.getDefaultView() )
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
            /*
            const auto cursor = sf::Cursor::createFromSystem(
                sf::Cursor::Type::Hand).value();
            cfg.pwin->setMouseCursor(cursor);

            cfg.pwin->setMouseCursorVisible(false);
            */

            cfg.uiTuneBase.fooHover = [this]()
            {   this->myHover.test();
            };

            cfg.uiUpLog.fooHover = [this]()
            {   this->myHover.test();
            };
        }

    vsl::Config&         cfg;
    sf::RenderWindow& window;

    ///---------------------|
    /// Камера.             |
    ///---------------------:
    sf::View          camFon;
    sf::View          camGui;

    uii::MyHover     myHover;

    void run()
    {   loop(scenes);
    }

private:
    vsl::SceneLogo logo{cfg};
    vsl::SceneTune tune{cfg};
    vsl::SceneGame game{cfg};

    vsl::ScenesAll scenes
    {   &logo,
        &tune,
        &game,
    };

    ///---------------------|
    /// Часы.               |
    ///---------------------:
    sf::Clock          clock;

    bool done{true};

    void loop(vsl::ScenesAll& scenes)
    {
    /// ui << "Привет, Монополия-2025!\n";

    /// using Key  = sf::Keyboard::Key ;
    /// using Scan = sf::Keyboard::Scan;

    /// auto& nScene    = cfg.scenesSwitcher.nScene;
        auto& nowScene = cfg.scenesSwitcher.nowScene;

        // Создаем кастомный курсор
        /*
        const sf::Cursor cursor = sf::Cursor::createFromPixels(
            imgCursor1.getPixelsPtr(),
            sf::Vector2u(32, 32),
            sf::Vector2u(0, 0)).value();
        window.setMouseCursor(cursor);
        */

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

            myHover.cursor.setPosition({
                static_cast<float>(sf::Mouse::getPosition(window).x),
                static_cast<float>(sf::Mouse::getPosition(window).y)
            });
            myHover.reset();

            const auto& delta  = cfg.fps.getDeltaTime();

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
            window.draw         (cfg.fps);

            ImGui::SFML::Render  (window);

            window.draw  (myHover.cursor);
            window.display       (      );
        }
        #undef ISKEYPRESED
    }

    ///--------------------------------------|
    /// Тест разраба.                        |
    ///--------------------------------------:
    TEST
    {   sf::RenderWindow window
        (   sf::VideoMode(vsl::Config::initWinSize()),
            L"Монополия-2025",
        /// sf::Style::Titlebar | sf::Style::Close
            sf::Style::None
        );

        window.setPosition({window.getPosition().x, 0});

        vsl::Config     cfg(window);
        Render  render (cfg);
                render.run();
    }
};

#endif // X_H
