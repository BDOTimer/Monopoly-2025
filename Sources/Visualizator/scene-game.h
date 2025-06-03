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

#include "config-vsl.h"

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
            {
                fon.setTexture(&HolderTexture::get(nameTx));

                pr::setOrigin(fon);

                cfg.info_01(++cnt);

                cfg.uiUpLog.fooTune = [this]()
                {   goTune();
                };

                cfg.uiUpLog.fooMusic = [this]()
                {   using E = sf::SoundSource::Status;
					const bool
                    b{    this->cfg.musicGame.getStatus() == E::Playing };
                    b   ? this->cfg.musicGame.pause()
                        : this->cfg.musicGame.play ();
                };

                cfg.uiUpLog.fooDice2 = [this]()
                {   this->upDice();
                };

                cfg.uiUpLog.fooLog = [this]()
                {      this->isLog = !this->isLog;
                };

                cfg.uiCellInfo.fooNext = [this]()
                {   this->winGame.isUiCellInfo = false;
                    this->updateInfoPlayer();
                };
            }

		vsl::Config&  cfg;

        PLUG_IOBJECT2

        unsigned IDPLAYER;
        unsigned cnt     ;

    /// bool pressEnter{false};

        void goTune()
        {   using E = vsl::ScenesSwitcher;
			cfg.scenesSwitcher.doSwitcher(E::E_TUNE);
			cfg.musicGame.stop();
        }

		void input(const std::optional<sf::Event>&  event) override
		{
			if (event->is<sf::Event::KeyPressed>())
            {   if (ISKEYPRESSED(Escape))
                {   goTune();
                }
                else
                {   winGame.input(event);
                }
            }

            if (ISKEYPRESSED(Enter))
            {   this->upDice();
            }

            if (auto p = event->getIf<sf::Event::MouseButtonPressed>())
            {   if ( p->button  == sf::Mouse::Button::Left)
                {
                }
		    }
		}

		unsigned nStep{};

        void upDice()
        {   ///if(isGameOver) return; TODO ...

            if( winGame.isUiCellInfo)
            {   winGame.isUiCellInfo = false;
                updateInfoPlayer();
                return;
            }
            
            auto& o = this->winGame;

            o.dice.isRot = !o.dice.isRot;
            o.isDiceHide = !o.isDiceHide;

            o.dice.resetDice();

            if( !o.dice.isRot)
            {   this->doStep();
            }
            else
            {   this->winGame.isUiCellInfo = false;
                this->cfg.mp3dice1.play();
            }
        }

        pr::InsexCircle iWin{3};

        using    E  = model::StateGame;
        using    ED = model::StateGame::eSTATE;
        using    ES = model::StateGame::eSTATESTR;

        void doStep()
        {
            const auto& mdl{cfg.cfgModel};

            unsigned& idPlayer = cfg.players[IDPLAYER].id;

            cfg.uiCellInfo.isBot = mdl.players[idPlayer].isBot;

            cfg.uiGameLog << model::doStep
            (   "bot", { (int)mdl.idGame,
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
            (   "get", {(int)mdl.idGame, (int)idPlayer}
            );

            ASSERT((unsigned)sg.dat[E::E_SIZE == sg.dat.size()])

            const auto&   ID = (unsigned)sg.dat[E::E_IDPLAYER];

            cfg.players  [ID].stateGame = sg;

            cfg.uiPlayers[IDPLAYER]<< uii::Clear()
                << "  ИГРОК  : " << sg.str[ES::E_NAME  ]        << '\n'
                << "  КОШЕЛЁК: " << sg.dat[ED::E_MONEY1]        << '\n'
                << "  КУБИК  : " << sg.dat[ED::E_NDICE ]        << '\n'
                << "  СТАТУС : " << sg.dat[ED::E_STATUS_PERS]+1 << " ---> "      
                << mdl.decode2Str.getPlayer(sg.dat[ED::E_STATUS_PERS]).data()
                << '\n'
                ;

            //model::Field& field1 = *(cfg.cfgModel.pfield);
            //model::Cell& cell1 = field1[0];

            ///----------------------|
            /// TODO: ...            |
            ///----------------------:
            //model::Field field1(cfg.cfgModel);

            cfg.uiCellInfo       << uii::Clear()
                << "  ИГРОК    : " << sg.str[ES::E_NAME]             << '\n'
                << "  ЯЧЕЙКА   : " << sg.str[ES::E_CELL]             << '\n'
                << "  ПОЗИЦИЯ  : " << sg.dat[ED::E_POSITION]         << '\n'
                << "  СТАТУС   : " << sg.dat[ED::E_STATUS_CELL]+1<< " ---> "      
                << mdl.decode2Str.getCell(sg.dat[ED::E_STATUS_CELL]) << '\n'
                << "  ПРОДАЁТСЯ: " << sg.dat[ED::E_SELL]             << '\n'
                << "  СКУПКА   : " << sg.dat[ED::E_BYU]              << '\n'
                ;

            if(isGameOver = sg.dat[ED::E_GAMEOVER] >= 0; isGameOver)
            {   cfg.uiDownMessage << uii::Clear()
                    << "ИГРА ЗАКОНЧЕНА! Победитель: "
                    << cfg.players[sg.dat[ED::E_GAMEOVER]].name
                    ;
            }

            winGame.setPositionChip(ID, sg.dat[ED::E_POSITION]);
            ///////////////////////////////////////////////////

            winGame.isUiCellInfo = true;
        }

        bool isGameOver{false};

        void updateInfoPlayer()
        {   
            /// TODO ...
            nextPlayer();
        }

        void nextPlayer()
        {
            if(++IDPLAYER == cfg.players.size()) IDPLAYER = 0;

            cfg.info_01(++cnt);

            cfg.uiDownMessage << uii::Clear() << "Ход ИГРОКА: "
                              << (IDPLAYER + 1) << ": \""
                              << cfg.cfgModel.players[IDPLAYER].name << ": \""
                              << mess[rand()%mess.size()];
        }

        ///-----------------------------------|
        /// Имя загруженной текстуры.         |
        ///-----------------------------------:
        std::string     nameTx;
        sf::RectangleShape fon;

        WinGame        winGame{cfg};
        WinUp            winUp{cfg};
        WinDown        winDown{cfg};

        std::array<WinPlayer, 3> winPlayers
        {   WinPlayer{cfg, 0},
            WinPlayer{cfg, 1},
            WinPlayer{cfg, 2}
        };

        bool isLog;

        ///-----------------------------------|
        /// Новая игра.                       |
        ///-----------------------------------:
        void reStart()
        {
            IDPLAYER = 0;
            cnt      = 0;

            cfg.uiDownMessage << uii::Clear() << "НОВАЯ ИГРА! Ход ИГРОКА: "
                              << (IDPLAYER + 1) << ": \""
                              << cfg.cfgModel.players[IDPLAYER].name << "\"";

            isLog     = false;

            winGame.reStart();
            cfg    .reStart();
        }

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

        friend struct SceneTune;
    };
}

#endif // SCENE_GAME_H
