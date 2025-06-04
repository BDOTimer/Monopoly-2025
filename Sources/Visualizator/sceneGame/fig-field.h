#ifndef FIELD_H
#define FIELD_H
///----------------------------------------------------------------------------|
/// "fig-field.h"
///----------------------------------------------------------------------------:
#include "../common.h"

namespace vsl
{
    ///------------------------------------------------------------------------|
    /// PrimFigure
    ///------------------------------------------------------------- PrimFigure:
    template<typename    T>
    struct  PrimFigure : T
    {       PrimFigure(const sf::Texture& t) : T(t) {}
            PrimFigure(){}

        std::string nameTxtr;
        unsigned          id;
    };

    using PFS = PrimFigure<sf::RectangleShape>;


    ///------------------------------------------------------------------------|
    /// FigureField
    ///------------------------------------------------------------ FigureField:
    struct  FigurePosition  : sf::RectangleShape
    {       FigurePosition  ()
            {   setFillColor( sf::Color{127,127,180,180} );
            }
    };


    ///------------------------------------------------------------------------|
    /// FigureField
    ///------------------------------------------------------------ FigureField:
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
                            std::string name  { "res/img/field/" };
                                        name += std::to_string(ID);
                                        name += ".jpeg";

                            sps.emplace_back
                            (   PFS()
                            );

                            sps.back().setTexture(&HolderTexture::get(name));

                            sps.back().nameTxtr = name;
                            sps.back().id       = ID;
                            sps.back().setSize    ({255, 255});
                            sps.back().setPosition({x * szCell, y * szCell});
                            sps.back().setOutlineThickness(4.f);
                            sps.back().setFillColor(color[0]);

                            pr::setOrigin(sps.back());

                            switch(ID % 3)
                            {
                                case 0:
                                {   sps.back().setOutlineColor({0,128,0});
                                    break;
                                }
                                case 1:
                                {   sps.back().setOutlineColor({128,0,0});
                                    break;
                                }
                                case 2:
                                {   sps.back().setOutlineColor({128,128,0});
                                    break;
                                }
                            }

                            psp.push_back(&sps.back());
                        }
                    }
                }

                std::sort(psp.begin(), psp.end(),
                    [](const PFS* a, const PFS* b)
                    {   return    a->id < b->id;
                    }
                );

                figPos.setSize({szCell, szCell});
                setFigurePos2Pos(0);
                pr::setOrigin     (figPos);
            }

        vsl  ::Config&      cfg;
        model::ConfigShare& cfgModel;

        FigurePosition      figPos;

        float szCell{300};

        std::array<sf::Color, 2> color
        {   sf::Color{180,180,180},
            sf::Color{ 40, 40, 80}
        };

        PLUG_IOBJECT

        sf::Vector2f getCenter() const
        {   const auto& m{cfgModel.worldGeometry};
            const auto szCell2 = szCell / 2;
            return { szCell * m[0].size() / 2 - szCell2 ,
                     szCell * m   .size() / 2 - szCell2 };
        }

        const PFS& operator[](unsigned i) const
        {   return *psp[i];
        }

        void setFigurePos2Pos(unsigned id)
        {   figPos.setPosition(psp[id]->getPosition());
        }

        void setColor(unsigned idCell, unsigned idColor)
        {   psp[idCell]->setFillColor    (color[idColor]);
        }

        void clear()
        {   for(auto p : psp) p->setFillColor(color[0]);
        }

    private:
        std::vector<PFS > sps;
        std::vector<PFS*> psp;

        ///------------------------------------|
        ///  На рендер.                        |
        ///------------------------------------:
        virtual void draw(sf::RenderTarget& target,
                          sf::RenderStates  states) const
        {/// auto p = const_cast<FigureField*>(this);

            target.draw( figPos, states);

            for(const  auto& sp : sps)
            {   target.draw( sp , states);
            }
        }
    };
}


#endif // FIELD_H
