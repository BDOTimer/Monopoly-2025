#ifndef FIG_CHIPS_H
#define FIG_CHIPS_H
///----------------------------------------------------------------------------|
/// "fig-chips.h"
///----------------------------------------------------------------------------:
#include "../common.h"

namespace vsl
{
    struct ConfigFigureChip
    {
        inline static const char* const filename[3]
        {   "res/img/chips/0.png",
            "res/img/chips/1.png",
            "res/img/chips/2.png"
        };

        sf::Vector2f Size{200, 200};

        static const ConfigFigureChip& get()
        {   static ConfigFigureChip cfc;
            return                  cfc;
        }
    };

    struct  FigureChip : sf::RectangleShape
    {       FigureChip(unsigned id)
                :   sf::RectangleShape(ConfigFigureChip::get().Size)
                ,   id(id )
            {   setTexture(&HolderTexture::get(ConfigFigureChip::filename[id]));
            }

        unsigned id;
    };

    struct  FigureChips : std::vector<FigureChip>, sf::Drawable
    {       FigureChips()
                :   std::vector<FigureChip>
                    {   FigureChip  (0),
                        FigureChip  (1),
                        FigureChip  (2)
                    }
            {
            }

        void setPosition(unsigned id, sf::Vector2f pos)
        {   const auto& SZ = ConfigFigureChip::get().Size;
            auto& o = (*this)[id];
                  o.setPosition( { pos.x, pos.y + o.id * SZ.y / 5 } );
        }

    private:
        ///------------------------------------|
        ///  На рендер.                         |
        ///------------------------------------:
        virtual void draw(sf::RenderTarget& target,
                          sf::RenderStates  states) const
        {/// auto p = const_cast<FigureField*>(this);

            for(const  auto& f : *this )
            {   target.draw( f , states);
            }
        }
    };

}

#endif // FIG_CHIPS_H
