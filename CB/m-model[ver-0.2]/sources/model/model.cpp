///----------------------------------------------------------------------------|
/// "model.h"
///----------------------------------------------------------------------------:
#include "../user-model.h"
#include "config-model.h"
#include "!m-model.h"

namespace model
{
    ///----------------------------------|
    /// Чья ячейка?                      |
    ///----------------------------------:
    std::string Cell::getOwner() const
    {   std::stringstream ss;
        pers == nullptr
            ?   ss  << "Эта ячейка свободна для продажи!\n"
            :   ss  << "Эта ячейка принадлежит "
                    << pers->name << '\n';
        return  ss.str();
    }

    ///------------------------------------------------------------------------|
    /// Интерфейс модели.
    ///------------------------------------------------------------------------:
    struct  ModelGate :  model::Referee
    {       ModelGate      (Config* cfg)
                :   model::Referee(*cfg)
                ,   cfg            (cfg)
            {
            }

        Config* cfg;

        std::string doStep(unsigned idPlayer)
        {   return model::Referee::doStep(idPlayer);
        }
    };

    struct CG { Config* cfg; ModelGate* mdl; };

    struct  HolderGates : std::vector<CG>
    {       HolderGates ()
            {    reserve(1000);
            }
           ~HolderGates()
            {   for(auto   p :  *this)
                {   delete p.cfg;
                    delete p.mdl;
                }
            }

        unsigned/*id*/ create()
        {   auto p = new Config();
            emplace_back(p, new ModelGate(p));
            return (unsigned)size() - 1;
        }

        std::string doStep( unsigned idGame, unsigned idPlayer )
        {   return (*this)[idGame].mdl->doStep(idPlayer);
        }

    }holderGates;


    ///------------------------------------------------------------------------|
    /// Интерфейс модели.
    ///------------------------------------------------------------------------:
    const Config getConfig()
    {   unsigned id = holderGates.create();
        holderGates[id].cfg->idGame = id;
        return *holderGates[id].cfg;
    }

    std::string  getLogo(unsigned id)
    {   return holderGates[id].cfg->getLogo  ()
             + holderGates[id].mdl->infoField();
    }

    ///---------------------------|
    /// controller.               |
    ///---------------------------:
    std::string doStep(std::string_view command, const std::vector<int>& args)
    {
        if(command == "bot")
        {   return holderGates.doStep(args[0], args[1]);
        }
        return "error...\n";
    }
}





