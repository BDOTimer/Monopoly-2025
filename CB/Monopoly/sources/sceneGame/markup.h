#ifndef MARKUP_H
#define MARKUP_H
///----------------------------------------------------------------------------|
/// "markup.h"
///----------------------------------------------------------------------------:
#include "../common.h"

namespace vsl
{

    struct  MarkupSceneGame
    {       MarkupSceneGame()
            {
                calc();
            }

        float border{2.f};

        const sf::FloatRect& getWinBase() const
        {   return winBase;
        }

        sf::FloatRect winBase  {{0.3f, 0.1f},{0.7f, 0.8f}};
        sf::FloatRect winUp       ;
        sf::FloatRect winDown     ;
        sf::FloatRect winPlayer[3];

        void calc()
        {   const float& PX = winBase.position.x;
            const float& PY = winBase.position.y;
        /// const float& SX = winBase.size    .x;
            const float& SY = winBase.size    .y;

            {   auto& p = winUp.position;
                auto& s = winUp.size;
                    p.x = 0;
                    p.y = 0;
                    s.x = 1.0f;
                    s.y = PY;
            }
            {   auto& p = winDown.position;
                auto& s = winDown.size;
                p.x = 0.0f;
                p.y = SY + PY;
                s.x = 1.0f;
                s.y = 1.0f - p.y;
            }
            {   const float W = SY / 3.f;

                for(unsigned i = 0; i < 3; ++i)
                {   auto& p   = winPlayer[i].position;
                    auto& s   = winPlayer[i].size;
                    p.x = 0.0f;
                    p.y = PY + W * i;
                    s.x = PX;
                    s.y = W;
                }
            }
        }
    };

}

#endif // MARKUP_H
