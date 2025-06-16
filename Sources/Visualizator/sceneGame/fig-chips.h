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


    ///------------------------------------------------------------------------|
    /// FigureChip.
    ///------------------------------------------------------------- FigureChip:
    struct  FigureChip : sf::RectangleShape
    {       FigureChip(unsigned idPers)
                :   sf::RectangleShape(ConfigFigureChip::get().Size)
                ,   idPers (idPers)
            {
                setTexture(&HolderTexture::get(
                    ConfigFigureChip::filename[idPers]));

                snd = std::make_unique<vsl::Sound>(
                    ConfigFigureChip::filenameSound[idPers]
                );

                pr::setOrigin(*this);
            }

        unsigned idPers;
        unsigned idCell{};

        std::unique_ptr<vsl::Sound>  snd;

        void setPosition(sf::Vector2f pos)
        {   sf::RectangleShape::setPosition(pos);
        }

        void setPosition(sf::Vector2f ps, unsigned idCell, bool isSnd)
        {   parking(ps);
            if(isSnd) snd->play();
            this->idCell = idCell;
        }

        void setPosition(sf::Vector2f ps, bool isSnd)
        {   parking(ps);
            if(isSnd) snd->play();
        }

    private:
        void parking(sf::Vector2f ps)
        {   const auto  D {ConfigFigureChip::get().Size.y / 5};
            setPosition ( {ps.x, ps.y + D * idPers - D} );
        }
    };


    ///------------------------------------------------------------------------|
    /// FigureChips.
    ///------------------------------------------------------------ FigureChips:
    struct  FigureChips : std::vector<FigureChip>, sf::Drawable
    {       FigureChips()
            {
                reserve(3);
                for(unsigned i = 0; i < 3; ++i)
                {   emplace_back(FigureChip(i));
                }
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
