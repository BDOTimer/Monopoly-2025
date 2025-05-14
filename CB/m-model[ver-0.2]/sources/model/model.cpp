///----------------------------------------------------------------------------|
/// "model.h"
///----------------------------------------------------------------------------:
#include "../user-model.h"
#include "config-model.h"
#include "!m-model.h"

namespace model
{
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
    std::string  doStep(std::string_view command, const std::vector<int>& args)
    {
        return "";
    }
}





