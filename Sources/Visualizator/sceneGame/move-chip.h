#ifndef MOVE_CHIP_H
#define MOVE_CHIP_H
///----------------------------------------------------------------------------|
/// "move-chip.h"
///----------------------------------------------------------------------------:
#include "fig-field.h"
#include "fig-chips.h"

inline float sqLen(const sf::Vector2f a)
{   return a.x * a.x + a.y * a.y;
}

namespace vsl
{
    ///------------------------------------------------------------------------|
    /// MoveChip
    ///--------------------------------------------------------------- MoveChip:
    struct  MoveChip
    {       MoveChip(vsl::Config& cfg, const FigureField& ff)
                :   cfg(cfg)
                ,   ff (ff )
            {
            }

              vsl::Config& cfg;
        const FigureField&  ff;

        bool isMove    {false};

        ///-------------------------------|
        /// Строим маршрут.               |
        ///-------------------------------:
        void buildRoute(FigureChip* fc, unsigned idCellEnd)
        {
            const auto& idCellStart = fc->idCell;

            if(idCellStart == idCellEnd)  return;

            route.clear();

            for(unsigned i = idCellStart; i != idCellEnd; i = nextCell(i))
            {   route.push_back(i);
            }   route.push_back(idCellEnd);

            this->fc =   fc;
            step     =    0;

            initStep();
        }

        ///-------------------------------|
        /// Двигаем.                      |
        ///-------------------------------:
        void move()
        {
            float r = speed * cfg.deltaTime.asSeconds();

            if (distance  > r )
            {   fc->move  ( r * unitDir );
                distance -= r ;
            }
            else
            {   if(++step == route.size())
                {   fc->setPosition(b, route.back(), true);
                    vsl::Sounds::p->stop(7);
                    isMove = false;
                }
                else
                {   fc->setPosition(b);
                    initStep();
                }
            }
        }

    private:
        std::vector<unsigned> route;
        FigureChip*     fc{nullptr};
        const float    speed{500.f};
        unsigned               step;
        sf::Vector2f           a, b;
        sf::Vector2f            dir;
        float              distance;
        sf::Vector2f        unitDir;

        unsigned nextCell(unsigned idCell)
        {   if(++idCell == ff.getPSP().size()) return 0;
            return idCell;
        }

        void initStep()
        {
            a = fc->getPosition();
            b = ff.getPSP()[route[step]].getPosition();

            dir      = b - a;
            distance = std::sqrtf(sqLen(dir));

            if(distance   == 0.f)
            {   if(++step == route.size()) { return; }
                 initStep ();
            }

            unitDir = dir / distance;
            isMove  = true;

            vsl::Sounds::p->playLoop(7);
        }
    };

}

#endif // MOVE_CHIP_H
