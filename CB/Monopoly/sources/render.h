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
{       Render() : window(sf::VideoMode({vsl::cfg.SW, vsl::cfg.SH}),
                          L"Монополия-2025",
                          sf::Style::Titlebar | sf::Style::Close)
                 , ui    (window)
                 , camFon(sf::FloatRect({0, 0}, {1344, 768}))
        {
            sf::Image      icon("icon.png");
            window.setIcon(icon);
        }

    sf::RenderWindow window;

    ///---------------------|
    /// Gui.                |
    ///---------------------:
    uii::UITest           ui;

    ///---------------------|
    /// Камера.             |
    ///---------------------:
    sf::View          camFon;

    void run()
    {   std::vector<vsl::IObject*> objects
        {   &logo,
            &tune,
            &game,
        };

        loop(objects);
    }

private:
    vsl::SceneLogo logo;
    vsl::SceneTune tune;
    vsl::SceneGame game;

    ///---------------------|
    /// Часы.               |
    ///---------------------:
    sf::Clock          clock;

    ///---------------------|
    /// Номер сцены.        |
    ///---------------------:
    unsigned       nScene{0};

    void loop(std::vector<vsl::IObject*>& scenes)
    {
        ui << "Привет, Монополия-2025!\n";

    /// using Key  = sf::Keyboard::Key ;
    /// using Scan = sf::Keyboard::Scan;

        #define ISKEYPRESED(a) sf::Keyboard::isKeyPressed(sf::Keyboard::Key::a)

        while (window.isOpen())
        {   while (const std::optional event = window.pollEvent())
            {   ImGui::SFML::ProcessEvent(window, *event);

                if (event->is<sf::Event::Closed>()) window.close();

                if (event->is<sf::Event::KeyPressed>())
                {   if (ISKEYPRESED(Enter))
                    {   nScene =  (nScene + 1) % scenes.size();
                    }
                }
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
            }

            /// window.clear   ({0, 30, 60});

            window.setView       (camFon);
            window.draw (*scenes[nScene]);

            ImGui::SFML::Render  (window);
            window.display       (      );
        }
        #undef ISKEYPRESED
    }

    ///--------------------------------------|
    /// Тест разраба.                        |
    ///--------------------------------------:
    TEST
    {   Render  render;
                render.run();
    }
};

#endif // X_H
