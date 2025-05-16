#ifndef FIELD_H
#define FIELD_H
///----------------------------------------------------------------------------|
/// "fig-field.h"
///----------------------------------------------------------------------------:
#include "../common.h"

namespace vsl
{
    struct  FigureField : vsl::IObject
    {       FigureField(vsl::Config& cfg)
                :   cfg(cfg)
                ,   cfgModel(cfg.cfgModel)
            {
                sps.reserve(cfgModel.amountCells);
                const auto& m{cfgModel.worldGeometry};
                unsigned cnt{};

                for    (unsigned y =   0; y < m   .size(); ++y)
                {   for(unsigned x =   0; x < m[y].size(); ++x)
                    {   if(m[y][x] == 'O')
                        {
                            std::string name{"res/img/field/"};
                                        name += std::to_string(cnt++);
                                        name += ".jpeg";

                            sps.emplace_back(sf::Sprite(
                                HolderTexture::get(name)));
                            sps.back().setPosition({x * szCell, y * szCell});
                        }
                    }
                }
            }

        vsl  ::Config& cfg;
        model::Config& cfgModel;

        float szCell{280};

        PLUG_IOBJECT

    private:
        std::vector<sf::Sprite> sps;

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
