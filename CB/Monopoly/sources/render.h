#ifndef RENDER_H
#define RENDER_H
///----------------------------------------------------------------------------|
/// "render.h"
///----------------------------------------------------------------------------:
#include "model.h"
#include "ui-imgui.h"

struct  Render
{       Render() : window(sf::VideoMode({1344, 768}), L"Монополия-2025")
                 , ui    (window)
        {   sf::Image      icon("icon.png");
            window.setIcon(icon);
        }

    sf::RenderWindow window;

    ///---------------------|
    /// Gui.                |
    ///---------------------:
    uii::UITest           ui;

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

            window.draw(objects);

            ImGui::SFML::Render(window);
            window.display     (      );
        }
    }

    ///--------------------------------------|
    /// initImgui                            |
    ///--------------------------------------:
    void initImgui()
    {   bool
        isGood = ImGui::SFML::Init(window);
        if (!isGood)
        {   ASSERTM(false, "ImGui::SFML::Init() is failed ...")
        }

        ImGuiIO& io = ImGui::GetIO();

        io.Fonts->Clear();
        isGood = io.Fonts->AddFontFromFileTTF("consola.ttf", 18.f, NULL,
                                              io.Fonts->GetGlyphRangesCyrillic());
        if (!isGood)
        {   ASSERTM(false, "io.Fonts->AddFontFromFileTTF() is failed ...")
        }

        isGood = ImGui::SFML::UpdateFontTexture();
        if (!isGood)
        {   ASSERTM(false, "ImGui::SFML::UpdateFontTexture()is failed ...")
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
