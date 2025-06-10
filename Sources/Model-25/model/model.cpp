///----------------------------------------------------------------------------|
/// "model.h"
///----------------------------------------------------------------------------:
#include "../user-model.h"
#include "config-model.h"
#include "!m-model.h"


namespace BackDoor
{
    void loadBackDoor2Config(const BackDoor::Data& bd, model::Config* cfg)
    {
        if(!cfg->isActiveBackDoor) return;

        cfg->isSeed = (unsigned)bd.isSeed;
    }
}


namespace model
{
    ///----------------------------------|
    /// Чья ячейка?                      |
    ///----------------------------------:
    std::string Cell::getOwner() const
    {   std::stringstream ss;
        pers == nullptr
            ?   ss  << "Эта ячейка свободна для продажи!"
            :   ss  << "Эта ячейка принадлежит " << pers->name;
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

        size_t whoVictor() const
        {  return model::Referee::whoVictor();
        }

        const StateGame getStateGame(unsigned idPlayer)
        {   return model::Referee::getStateGame(idPlayer);
        }

        void sendStateGame(const unsigned           idPlayer,
                           const StateGame4Server&  stateGame4S)
        {   model::Referee::sendStateGame(idPlayer, stateGame4S);
        }
    };

    struct CG
    {   Config*    cfg{nullptr};
        ModelGate* mdl{nullptr};

        void clear()
        {   if(nullptr != cfg) delete cfg;
            if(nullptr != mdl) delete mdl;
        }

        void reCreate(const BackDoor::Data& data,
                      const UserInit4Model& userInit4Mode)
        {    clear();
             cfg = new Config();
             cfg->_init();

             auto p = const_cast<UserInit4Model*>(&userInit4Mode);
                  p-> initNames();

             cfg->players = userInit4Mode.players; /// TODO ...

        ///  l(userInit4Mode.players[0].name)
        ///  l(cfg->players[0].name)

             BackDoor::loadBackDoor2Config(data, cfg);

             mdl = new ModelGate(cfg);
        }
    };

    struct  HolderGates : std::vector<CG>
    {       HolderGates ()
            {    reserve(1000);
            }
           ~HolderGates()
            {   for(auto p : *this) p.clear();
            }

        unsigned/*id*/ reCreate(const std::string&   login,
                                const BackDoor::Data& data,
                                const UserInit4Model& userInit4Mode)
        {   if(const auto p = logins.find(login); p != logins.end())
            {   const unsigned&    id = p->second;
                auto&  o = (*this)[id];
                       o.reCreate   (data, userInit4Mode);
                return id;
            }
            else
            {   emplace_back(CG());
                back().reCreate(data, userInit4Mode);
                return (unsigned)size() - 1;
            }
        }

        std::string doStep( unsigned idGame, unsigned idPlayer )
        {   return (*this)[idGame].mdl->doStep(idPlayer);
        }

        size_t whoVictor(unsigned idGame) const
        {   return (*this)[idGame].mdl->whoVictor();
        }

        const StateGame getStateGame( unsigned idGame, unsigned idPlayer)
        {   return (*this)[idGame].mdl->getStateGame(idPlayer);
        }

        void sendStateGame(unsigned idGame, unsigned idPlayer, 
                           const StateGame4Server& stateGame4S)
        {
            (*this)[idGame].mdl->sendStateGame(idPlayer, stateGame4S);
        }

    private:
        std::map<std::string/*login*/, unsigned/*id*/> logins;

    }holderGates;


    ///------------------------------------------------------------------------|
    /// Интерфейс модели.
    ///------------------------------------------------------------------------:
    ConfigShare* getConfig(const BackDoor::Data& data,
                           const UserInit4Model& userInit4Model)
    {
        /// TODO ...

        unsigned    id = holderGates.reCreate("desktop", data, userInit4Model);
        holderGates[id].cfg->idGame = id;
        return holderGates[id].cfg;
    }

    std::string   getLogo(unsigned id)
    {   return holderGates[id].cfg->getLogo  ()
             + holderGates[id].mdl->infoField();
    }

    ///---------------------------|
    /// controller.               |
    ///---------------------------:
    std::string doStep(std::string_view command, const std::vector<int>& args)
    {
        if(command == "start")
        {   return holderGates.doStep(args[0], args[1]);
        }

        std::cout << "ERROR: model::doStep(..." << command << "...);\n";
        return "error...\n";
    }

    const Cell& Config::getCell(const unsigned position) const
    {   return (*pfield)[position];
    }

    Cell& Config::getCell(const unsigned position)
    {   return (*pfield)[position];
    }

    size_t whoVictor(unsigned idGame)
    {   return holderGates.whoVictor(idGame);
    }

    const StateGame getStateGame(std::string_view command,
                                  const std::vector<int>& args)
    {   if(command == "get")
        {   return holderGates.getStateGame(args[0], args[1]);
        }
        return {{999}};
    }

    const StateGame sendStateGame( std::string_view        command,
                                   const std::vector<int>& args,
                                   const StateGame4Server& stateGame4S )
    {   if(command == "4server")
        {          holderGates.sendStateGame(args[0], args[1],stateGame4S);
            return holderGates.getStateGame (args[0], args[1]);
        }
        return {{999}};
    }

}





