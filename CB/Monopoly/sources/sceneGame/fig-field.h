#ifndef FIELD_H
#define FIELD_H
///----------------------------------------------------------------------------|
/// "fig-field.h"
///----------------------------------------------------------------------------:
#include "../common.h"

namespace vsl
{
    template<typename    T>
    struct  PrimFigure : T
    {       PrimFigure(const sf::Texture& t) : T(t) {}

        std::string nameTxtr;
        unsigned          id;
    };

    using PFS = PrimFigure<sf::Sprite>;

    struct  FigureField : vsl::IObject
    {       FigureField(vsl::Config&  cfg)
                :   cfg              (cfg)
                ,   cfgModel(cfg.cfgModel)
            {
                sps.reserve(cfgModel.amountCells);
                psp.reserve(cfgModel.amountCells);
                const auto& m{cfgModel.worldGeometry};

                for    (unsigned y  = 0; y < m   .size(); ++y)
                {   for(unsigned x  = 0; x < m[y].size(); ++x)
                    {
                        if(const int& ID = m[y][x]; ID >= 0)
                        {
                            std::string name{"res/img/field/"};
                                        name += std::to_string(ID);
                                        name += ".jpeg";

                            sps.emplace_back
                            (   PrimFigure    <sf::Sprite>
                                (   HolderTexture::get(name)
                                )
                            );

                            sps.back().nameTxtr = name;
                            sps.back().id       = ID;

                            sps.back().setPosition({x * szCell, y * szCell});

                            psp.push_back(&sps.back());
                        }
                    }
                }
/*
                std::sort(psp.begin(), psp.end(),
                    [](const PFS* a, const PFS* b)
                    {   return    a->id < b->id;
                    }
                );
*/
            }

        vsl  ::Config& cfg;
        model::Config& cfgModel;

        float szCell{280};

        PLUG_IOBJECT

    private:
        std::vector<PrimFigure<sf::Sprite> > sps;
        std::vector<PrimFigure<sf::Sprite>*> psp;

        ///------------------------------------|
        ///  На рендер.                         |
        ///------------------------------------:
        virtual void draw(sf::RenderTarget& target,
                          sf::RenderStates  states) const
        {/// auto p = const_cast<FigureField*>(this);

            for(const  auto& sp : sps)
            {   target.draw( sp , states);
            }
        }
    };
}


#endif // FIELD_H
