#ifndef FIG_CHIPS_H
#define FIG_CHIPS_H
///----------------------------------------------------------------------------|
/// "fig-chips.h"
///----------------------------------------------------------------------------:
#include "../common.h"

namespace vsl
{
    struct  Sound
    {       Sound(std::string_view fn) : snd(buf)
            {   bool   ok = buf.loadFromFile(fn.data());
                ASSERT(ok)
            }

        void play(){ snd.play(); }

    private:
        sf::SoundBuffer buf;
        sf::Sound       snd;
    };

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
            auto& o = (*this)[id];
                  o.setPosition( { pos.x, pos.y + o.id * SZ.y / 5 } );

            if(isSnd) o.snd->play();
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
