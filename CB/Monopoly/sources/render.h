#ifndef RENDER_H
#define RENDER_H
///----------------------------------------------------------------------------|
/// "render.h"
///----------------------------------------------------------------------------:
#include "model.h"

struct  Render
{       Render()
        {
        }

    void run()
    {
        Object  object(Data4Sprites::get().front());

        loop(model );
        loop(object);
    }

private:
    Model model;

    template<typename T> void loop(T&  objects)
    {
        sf::RenderWindow window(sf::VideoMode({1344, 768}), L"Монополия-2025");

        sf::Image      icon("icon.png");
        window.setIcon(icon);

        while (window.isOpen())
        {
            while (const std::optional event = window.pollEvent())
            {
                if (event->is<sf::Event::Closed>()) window.close();
            }

            window.draw(objects);
            window.display    ();
        }
    }
};

#endif // X_H
