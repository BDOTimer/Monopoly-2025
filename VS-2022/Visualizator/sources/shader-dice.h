#ifndef SHADER_DICE_H
#define SHADER_DICE_H
///----------------------------------------------------------------------------|
/// "shader-dice.h"
///----------------------------------------------------------------------------:
#include "common.h"

void tests();

namespace vsl
{

    struct  ShaderDice : vsl::IObject
    {       ShaderDice  (vsl::Config& cfg)
                :   cfg    (cfg)
                ,   scrRect(cfg.szfWin)
            {
                scrRect.setPosition ({0, 0});
                scrRect.setFillColor(colFon);

                vsl::Config::setOrigin(scrRect);
            }

        vsl::Config& cfg;

        PLUG_IOBJECT

        using Scan = sf::Keyboard::Scancode;

        inline static const char* filename{"res/shaders/dice.frag"};

        sf::Shader         shader ;
        sf::RectangleShape scrRect;

        float        currentAngle {0.0f};
        float        rotSpeed     {0.0f};
        float        rotSpeedMax  {6.0f};
        const float  minSpeed     {0.1f};
        sf::Vector2i lastMousePos;
        bool         mousePressed{false};
        bool         isRot       {false};

        sf::Color colBorder{128, 0,   0, 254};
        sf::Color colFon   {  0, 0, 128, 254};

        void upRotSpeed()
        {   if(!isRot) return;
            rotSpeed += cfg.dt() * 4;
            if(rotSpeed > rotSpeedMax) rotSpeed = rotSpeedMax;
        }

        void downRotSpeed()
        {   if(isRot) return;
            rotSpeed -= cfg.dt() * 4;
            if(rotSpeed < 0.f) rotSpeed = 0.f;
        }

        void init()
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

            sf::RectangleShape scrRect(sf::Vector2f(window.getSize()));
                               scrRect.setPosition ({ 0, 0 });

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
                window.draw   (scrRect, &shader);
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

            const sf::Vector2f sz{float(window.getSize().x),
                                  float(window.getSize().y)};

            ShaderDice  dice   (vsl::Config::get());
                        dice.init();

            float TN{5.f};

            sf::Texture        texture("res/logo.jpg");
            sf::RectangleShape fon({sz.x-TN-TN, sz.y-TN-TN});

                fon.setFillColor       ({ 255, 255, 255 });
                fon.setOutlineColor    ({  64, 32,  127 });
                fon.setOutlineThickness( TN);
                fon.setPosition        ({TN, TN});
                fon.setTexture         (&texture);

            while (window.isOpen())
            {   while (auto event = window.pollEvent())
                {   if (event->is<sf::Event::Closed>()) window.close();
                }

                window.clear  (    );
                window.draw   ( fon);
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

            const auto& sz{scrRect.getSize()};

            p->shader.setUniform("resolution"  , sf::Glsl::Vec2(sz));
            p->shader.setUniform("currentAngle", currentAngle);

            target.draw(scrRect, states.shader = &shader);

            if(p->isRot) p->upRotSpeed();
            else         p->downRotSpeed();

            p->currentAngle += rotSpeed * cfg.dt();
        }

        static void test()
        {   if(const auto e = vsl::ShaderDice::test2(); e != "SUCCESS")
            {   std::cout << "ERROR: Shader-dice fail: " << e << " ...\n";
            }
        }

        friend void ::tests();
    };

}

#endif // SHADER_DICE_H
