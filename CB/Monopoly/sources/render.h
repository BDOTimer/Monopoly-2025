#ifndef RENDER_H
#define RENDER_H
///----------------------------------------------------------------------------|
/// "render.h"
///----------------------------------------------------------------------------:
#include "model.h"
#include "ui-imgui.h"

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
    {   Object  object(Data4Sprites::get().front());

        loop(model );
        loop(object);
    }

private:
    Model model;

    ///---------------------|
    /// Часы.               |
    ///---------------------:
    sf::Clock          clock;

    template<typename T> void loop(T&  objects)
    {
        ui << "Привет, Монополия-2025!\n";

        while (window.isOpen())
        {   while (const std::optional event = window.pollEvent())
            {   ImGui::SFML::ProcessEvent(window, *event);

                if (event->is<sf::Event::Closed>()) window.close();
            }

            const auto delta  = clock.restart();

            ///----------------------|
            /// ImGui::SFML.         |
            ///----------------------:
            ImGui::SFML::Update(window, delta);
            /// ...
            /// ImGui::ShowDemoWindow();
            ui.show();

            /// window.clear   ({0, 30, 60});

            window.setView(camFon);
            window.draw  (objects);

            ImGui::SFML::Render(window);
            window.display     (      );
        }
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
