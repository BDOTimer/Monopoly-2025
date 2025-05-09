#ifndef SHADER_DICE_H
#define SHADER_DICE_H
///----------------------------------------------------------------------------|
/// "shader-dice.h"
///----------------------------------------------------------------------------:
#include "common.h"

void tests();

namespace vsl
{

    struct  ShaderDice   : vsl::IObject
    {       ShaderDice() : screenRect         ({1344-TN-TN, 768-TN-TN})
            {   screenRect.setFillColor       ({ 32,  64, 127});
                screenRect.setOutlineColor    ({ 64, 127, 32 });
                screenRect.setOutlineThickness( TN);
                screenRect.setPosition        ({TN, TN});
            }

        PLUG_IOBJECT

        inline static float TN{50.f};

        using Scan = sf::Keyboard::Scancode;

        inline static const char* filename{"res/shaders/dice.frag"};

        sf::Shader         shader;
        sf::RectangleShape screenRect;

        float        currentAngle = 0.0f;
        float        rotSpeed     = 1.0f;
        const float  minSpeed     = 0.1f;
        sf::Vector2i lastMousePos;
        bool         mousePressed = false;

        void init(const sf::RenderWindow& window)
        {
            if (!sf::Shader::isAvailable())
                return throw("!sf::Shader::isAvailable()");

            if (!shader.loadFromFile(filename, sf::Shader::Type::Fragment))
                return throw("shader.loadFromFile(...");

        }

    private:

        ///------------------------------------|
        /// SFML::Test::1                      |
        ///------------------------------------:
        static std::string_view test1()
        {   if (!sf::Shader::isAvailable()) return "!sf::Shader::isAvailable()";

            sf::RenderWindow window(sf::VideoMode({ 800, 600 }),
                                   "SFML::Test::1", sf::State::Windowed);

            window.setFramerateLimit(60);

            sf::Shader shader;

            if (!shader.loadFromFile(filename, sf::Shader::Type::Fragment))
                return "shader.loadFromFile(...";

            sf::RectangleShape screenRect(sf::Vector2f(window.getSize()));
            screenRect.setPosition ({ 0, 0 });

            float        currentAngle = 0.0f;
            float        rotSpeed     = 1.0f;
            const float  minSpeed     = 0.1f;
            sf::Vector2i lastMousePos;
            bool         mousePressed = false;

            using Scan = sf::Keyboard::Scancode;

            while (window.isOpen())
            {   while (auto event = window.pollEvent())
                {   if (event->is<sf::Event::Closed>()) window.close();

                    if (auto p = event->getIf<sf::Event::MouseButtonPressed>())
                    {   if ( p->button  == sf::Mouse::Button::Left)
                        {   lastMousePos = sf::Mouse::getPosition(window);
                            mousePressed = true;
                        }
                    }

                    if (auto p = event->getIf<sf::Event::MouseButtonReleased>())
                    {   if ( p->button  == sf::Mouse::Button::Left)
                        {   mousePressed = false;
                        }
                    }

                    if (const auto* key = event->getIf<sf::Event::KeyPressed>())
                    {   if (key->scancode == Scan::Escape   ) window.close();

                        if (key->scancode == Scan::Enter    )
                        {   currentAngle  += rotSpeed * 0.1f;
                            l(currentAngle)
                            break;
                        }

                        if (key->scancode == Scan::Space   ) currentAngle = 0.0f;
                        if (key->scancode == Scan::LShift  ) rotSpeed    += 0.2f;
                        if (key->scancode == Scan::LControl)
                            rotSpeed = std::max(rotSpeed - 0.2f, minSpeed);
                    }
                }

                if (mousePressed)
                {   sf::Vector2i currMousePos = sf::Mouse::getPosition(window);
                    float deltaX  = static_cast<float>( currMousePos.x
                                                      - lastMousePos.x);
                    currentAngle += deltaX * rotSpeed * 0.005f;
                    lastMousePos  = currMousePos;
                }

            shader.setUniform("resolution", sf::Glsl::Vec2(window.getSize()));
            shader.setUniform("currentAngle", currentAngle);

                /*
                shader.setUniform("mouse"       , sf::Glsl::Vec4(
                    static_cast<float>(sf::Mouse::getPosition(window).x),
                    static_cast<float>(sf::Mouse::getPosition(window).y),
                    mousePressed ? 1.0f : 0.0f,
                    0.0f
                ));
                */

                window.clear  ();
                window.draw   (screenRect, &shader);
                window.display();
            }

            return "SUCCESS";
        }

        ///------------------------------------|
        /// SFML::Test::2                      |
        ///------------------------------------:
        static std::string_view test2()
        {
            sf::RenderWindow window(sf::VideoMode({ 1344, 768 }),
                                   "SFML::Test::2", sf::State::Windowed);
            ShaderDice  dice;
                        dice.init(window);

            while (window.isOpen())
            {   while (auto event = window.pollEvent())
                {   if (event->is<sf::Event::Closed>()) window.close();
                }

                window.clear  (    );
                window.draw   (dice);
                window.display(    );
            }

            return "SUCCESS";
        }

        ///------------------------------------|
        /// На рендер.                         |
        ///------------------------------------:
        virtual void draw(sf::RenderTarget& target,
                          sf::RenderStates  states) const
        {
            auto p = const_cast<ShaderDice*>(this);

            p->shader.setUniform("resolution"  , sf::Glsl::Vec2({1344,768}));
            p->shader.setUniform("currentAngle", currentAngle);

            target.draw(screenRect, states.shader = &shader);

            p->currentAngle += rotSpeed * 0.1f;
        }

        friend void ::tests();
    };

}

#endif // SHADER_DICE_H
