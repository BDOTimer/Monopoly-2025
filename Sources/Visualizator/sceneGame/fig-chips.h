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

        inline static const char* const filenameSound[3]
        {   "res/snd/na-pol.mp3",
            "res/snd/vetki.mp3" ,
            "res/snd/zvuk3.mp3"
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
                ,   id (id)
            {
                setTexture(&HolderTexture::get(ConfigFigureChip::filename[id]));

                snd = std::make_unique<vsl::Sound>(
                    ConfigFigureChip::filenameSound[id]
                );

                pr::setOrigin(*this);
            }

        unsigned id;

        std::unique_ptr<vsl::Sound>  snd;
    };

    struct  FigureChips : std::vector<FigureChip>, sf::Drawable
    {       FigureChips()
            {
                reserve(3);
                for(unsigned i = 0; i < 3; ++i)
                {   emplace_back(FigureChip(i));
                }
            }

        void setPosition(unsigned id, sf::Vector2f pos, bool isSnd = true)
        {   const auto& SZ = ConfigFigureChip::get().Size;

            const auto D{SZ.y / 4};

            auto& o = (*this)[id];
                  o.setPosition( { pos.x, pos.y + D * o.id - D } );

            if(isSnd) o.snd->play();
        }

    private:
        ///------------------------------------|
        ///  На рендер.                         |
        ///------------------------------------:
        virtual void draw(sf::RenderTarget& target,
                          sf::RenderStates  states) const
        {///auto p = const_cast<FigureField*>(this);

            for(const  auto& f : *this )
            {   target.draw( f , states);
            }
        }
    };

}

#endif // FIG_CHIPS_H
