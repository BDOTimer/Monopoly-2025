#ifndef SCENE_GAME_H
#define SCENE_GAME_H
///----------------------------------------------------------------------------|
/// "scene-game.h"
///----------------------------------------------------------------------------:
#include "common.h"
#include "shader-dice.h"
#include "sceneGame/win-game.h"
#include "sceneGame/win-up.h"
#include "sceneGame/win-down.h"
#include "sceneGame/win-player.h"

namespace vsl
{

    ///------------------------------------------------------------------------|
    /// Отображаемый одиночный объект.
    ///-------------------------------------------------------------- SceneGame:
    struct  SceneGame   :   vsl::IObject
    {       SceneGame      (vsl::Config& cfg)
				:	cfg    (cfg)
				,	nameTx ("res/game.jpg")
                ,	fon    (cfg.szfWin)
                ,	dice   (cfg)
            {
                fon.setTexture(&HolderTexture::get(nameTx));

                vsl::Config::setOrigin(fon);

                dice.init();

                cfg.info_01(++cnt);

                cfg.uiUpLog.fooDice2 = [this]()
                {   this->dice.isRot = !this->dice.isRot;
                    this->isDiceHide = !this->isDiceHide;

                    this->dice.resetDice();

                    if(!this->dice.isRot)
                    {   this->doStep();
                    }
                };

                cfg.uiUpLog.fooMusic = [this]()
                {   using E = sf::SoundSource::Status;
					const bool
                    b{    this->cfg.musicGame.getStatus() == E::Playing };
                    b   ? this->cfg.musicGame.pause()
                        : this->cfg.musicGame.play ();
                };

                cfg.uiUpLog.fooLog = [this]()
                {   this->isLog = !this->isLog;
                };

                cfg.uiDownMessage << uii::Clear() << "Ход ИГРОКА: " 
                                  << (IDPLAYER + 1);
            }

		vsl::Config&  cfg;

        PLUG_IOBJECT2

        unsigned IDPLAYER{0};
        unsigned cnt     {0};

    /// bool pressEnter{false};

		void input(const std::optional<sf::Event>&  event) override
		{
			if (event->is<sf::Event::KeyPressed>())
            {   if (ISKEYPRESSED(Escape))
                {   using E = vsl::ScenesSwitcher;
					cfg.scenesSwitcher.doSwitcher(E::E_TUNE);
					cfg.musicGame.stop();
                }
                else
                {   winGame.input(event);
                }
            }


            if (ISKEYPRESSED(Enter))
            {
                doStep();
            }

            if (auto p = event->getIf<sf::Event::MouseButtonPressed>())
            {   if ( p->button  == sf::Mouse::Button::Left)
                {

                }
		    }
		}

		unsigned nStep{};

        void doStep()
        {
            unsigned& idPlayer = cfg.players[IDPLAYER].id;

            cfg.uiGameLog << model::doStep
            (   "bot", { (int)cfg.idGame,
                         (int)idPlayer }
            );

/*
            ///////////////////////////////////////////////////
            cfg.uiPlayers[idPlayer] << uii::Clear() <<
            model::doStep
            (   "bot", { (int)cfg.idGame,
                         (int)idPlayer  }
            );
*/

            const model::StateGame sg = model::getStateGame
            (   "get", {(int)cfg.idGame, (int)idPlayer}
            );

            ASSERT((unsigned)sg[model::StateGame::E_SIZE == sg.size()])

            const auto&   ID = (unsigned)sg[model::StateGame::E_IDPLAYER];

            cfg.players  [ID].stateGame = sg;

            cfg.uiPlayers[ID] << uii::Clear()
                << "DICE    : " << sg[model::StateGame::E_NDICE   ] << '\n'
                << "POSITION: " << sg[model::StateGame::E_POSITION] << '\n';

            winGame.setPositionChip(ID, sg[model::StateGame::E_POSITION]);
            ///////////////////////////////////////////////////

            if(++IDPLAYER == cfg.players.size()) IDPLAYER = 0;

            cfg.info_01(++cnt);

            cfg.uiDownMessage << uii::Clear() << "Ход ИГРОКА: " 
                              << IDPLAYER+1   << mess[rand()%mess.size()];
        }

        ///-----------------------------------|
        /// Имя загруженной текстуры.         |
        ///-----------------------------------:
        std::string     nameTx;
        sf::RectangleShape fon;

        ShaderDice        dice;
        WinGame        winGame{cfg};
        WinUp            winUp{cfg};
        WinDown        winDown{cfg};

        WinPlayer winPlayers[3]
        {   {cfg, 0},
            {cfg, 1},
            {cfg, 2}
        };

        bool isDiceHide{true };
        bool isLog     {false};

        ///-----------------------------------|
        /// Дебаг.                            |
        ///-----------------------------------:
        void debug() const
        {   l(nameTx)
            l(fon.getTexture()->getSize().x)
            l(fon.getTexture()->getSize().y)
        }

        ///------------------------------------|
        /// На рендер.                         |
        ///------------------------------------:
        virtual void draw(sf::RenderTarget& target,
                          sf::RenderStates  states) const
        {   target.setView(*cfg.camFon);
            target.draw   (fon, states);

            if(!isLog)
            {   target.draw   (winPlayers[0], states);
                target.draw   (winPlayers[1], states);
                target.draw   (winPlayers[2], states);
            }

            target.draw   (winDown  , states);
            target.draw   (winUp    , states);
            target.draw   (winGame  , states);

            target.setView(*cfg.camGui);

            if(!isDiceHide)
            {   target.setView(*cfg.camFon );
                target.draw   (dice, states);
            }

            if(isLog)   cfg.uiGameLog.show();

        }

        ///------------------------------------|
        /// 🔥
        ///------------------------------------:
        std::array<const char* const, 6> mess
        {   " Эй, не спи! Пора делать ход!",
            " Эй, проснись! Твой ход ждёт!",
            " Сон подождёт - сейчас твой черёд!",
            " Встряхнись! Игра не закончится без тебя!",
            " Хватит мечтать! Пора делать ход!",
            " Время идёт, а ты всё ещё не сделал ход!"
        };
    };
}

#endif // SCENE_GAME_H
