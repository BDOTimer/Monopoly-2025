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

        unsigned idCellPos{};
    };


    ///------------------------------------------------------------------------|
    /// FigureField
    ///------------------------------------------------------------ FigureField:
    struct  FigureField : vsl::IObject
    {       FigureField(vsl::Config&  cfg)
                :   cfg              (cfg)
                ,   cfgModel(cfg.cfgModel)
            {
                sps.resize(cfgModel.amountCells);

                for    (unsigned i  = 0; i < cfgModel.amountCells; ++i)
                {
                    std::string name  { "res/img/field/" };
                                name += std::to_string(i);
                                name += ".jpeg";

                    auto& e{sps[i]};

                    e.nameTxtr = name;
                    e.id       = i;
                    e.setTexture(&HolderTexture::get(name));
                    e.setSize     ({255, 255});
                    e.setOutlineThickness(4.f);
                    e.setFillColor  (color[0]);

                    pr::setOrigin (e);

                    switch(i %  3)
                    {   case 0: e.setOutlineColor({0,  128,0}); break;
                        case 1: e.setOutlineColor({128,  0,0}); break;
                        case 2: e.setOutlineColor({128,128,0}); break;
                    }
                }

                setGeomPos();

                figPos.setSize  ({szCell, szCell});
                setFigurePos2Pos(0);
                pr::setOrigin   (figPos);

                cfg.holderTFieldCash.init(*this);
            }

        vsl  ::Config&      cfg;
        model::ConfigShare& cfgModel;

        FigurePosition      figPos;

        float szCell{300};

        std::array<sf::Color, 2> color
        {   sf::Color{180,180,180},
            sf::Color{ 40, 40, 80}
        };

        std::function<void()> fooRePosition{[](){}};

        PLUG_IOBJECT

        sf::Vector2f getCenter() const
        {   const auto& m{cfgModel.getWorldGeometry()};
            const auto szCell2 = szCell / 2;
            return { szCell * m[0].size() / 2 - szCell2 ,
                     szCell * m   .size() / 2 - szCell2 };
        }

        sf::Vector2f getSize() const
        {   const auto& m{cfgModel.getWorldGeometry()};
            const auto szCell2 = szCell / 2;
            return { szCell *( m[0].size()) + szCell2,
                     szCell *( m   .size()) + szCell2};
        }

        const PFS& operator[](unsigned i) const
        {   return sps[i];
        }

        void setFigurePos2Pos(unsigned id)
        {   figPos.setPosition(sps[id].getPosition());
            figPos.idCellPos     = id;
        }

        void updateFigurePos2Pos()
        {   figPos.setPosition(sps[figPos.idCellPos].getPosition());
        }

        void setColor(unsigned idCell, unsigned idColor)
        {   sps[idCell].setFillColor    (color[idColor]);
        }

        void clear()
        {   for(auto p : sps) p.setFillColor(color[0]);
        }

        const  std::vector<PFS>& getPSP() const { return sps; }

        void reGeometry()
        {   setGeomPos         ();
            updateFigurePos2Pos();
            fooRePosition      ();
            return;
        }

    private:
        std::vector<PFS > sps;

        void setGeomPos()
        {   std::vector<PFS>& r{sps};

            const auto& m{cfgModel.getWorldGeometry()};
            for    (unsigned y  = 0; y < m   .size(); ++y)
            {   for(unsigned x  = 0; x < m[y].size(); ++x)
                {
                    if(const int&     ID = m[y][x]; ID >= 0)
                    {   ASSERT(size_t(ID) < r.size())
                        r[ID].setPosition({ x * szCell, y * szCell });
                    }
                }
            }
        }

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


    ///------------------------------------------------------------------------|
    /// Грабим текстуры у ячеек в кэш.
    /// (нужно для uii::UIGameIcons)
    ///------------------------------------------------------------------------:
    void HolderTextureFieldCash::init(const vsl::FigureField& ff)
    {   clear  ();
        reserve(ff.getPSP().size());
        for(const auto& o : ff.getPSP())
        {   push_back(  o .getTexture());
        }
    }
}


#endif // FIELD_H
