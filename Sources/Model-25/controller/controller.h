#ifndef CONTROLLER_H
#define CONTROLLER_H
///----------------------------------------------------------------------------|
/// "controller.h"
///----------------------------------------------------------------------------:
#include "../debug.h"
#include "../user-model.h"

namespace controller
{
    ///------------------------------------------------------------------------|
    /// Игрок и его стейт.
    ///----------------------------------------------------------------- Player:
    struct  Player
    {       Player() = default;
            Player( bool  isBot, unsigned id, std::string_view name)
                :   isBot(isBot)
                ,   idTune      (id)
                ,   name  (name)
            {
            }

        bool       isBot;
        unsigned      idTune;
        std::string name;

        ///---------------------------|
        /// Что должен знать игрок.   |
        ///---------------------------:
        model::StateGame     stateGame;

        ///---------------------------|
        /// Данные для сервера.       |
        ///---------------------------:
        model::StateGame4Server stateGame4S;

        std::string input()
        {   std::string s; std::getline(std::cin, s); return s;
        }
    };

    ///------------------------------------------------------------------------|
    /// Все игроки игры.
    ///----------------------------------------------------------------- Players:
    struct  Players : std::vector<Player>
    {

    };
}

#endif // X_H
