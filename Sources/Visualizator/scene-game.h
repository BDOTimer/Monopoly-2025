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
                :   cfg    (cfg)
                ,   nameTx ("res/game.jpg")
                ,   fon    (cfg.szfWin)
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
                    b{    this->cfg.musics.getStatus() == E::Playing };
                    b   ? this->cfg.musics.pause()
                        : this->cfg.musics.play(0);
                };

                cfg.uiUpLog.fooDice2 = [this]()
                {   this->upDice();
                };

                cfg.uiUpLog.fooLog = [this]()
                {      this->isLog = !this->isLog;
                };

                cfg.uiUpLog.fooFieldGeom = [this]()
                {   this->cfg.cfgModel.nextWorldGeometry();
                    this->winGame.figField.reGeometry();
                };

                cfg.uiCellInfo.fooNext = [this]()
                {   this->upDice();
                };

                cfg.uiCellInfo.fooBuy = [this]()
                {   this->doBuy();
                };
            }

        vsl::Config&  cfg;

        PLUG_IOBJECT2

        unsigned idUI;
        unsigned  cnt;

        using ED = model::StateGame::eSTATE;
        using ES = model::StateGame::eSTATESTR;

    /// bool pressEnter{false};

        void goTune()
        {   using E = vsl::ScenesSwitcher;
            cfg.scenesSwitcher.doSwitcher(E::E_TUNE);
            cfg.musics.stop();
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

        unsigned nStep     {};
        unsigned nClickDice{};

        void upDice()
        {   ///if(isGameOver) return; TODO ...

            switch(nClickDice)
            {
                case 0:
                {   auto& o = this->winGame;

                    o.dice.isRot = true ;
                    o.isDiceHide = false;

                    this->winGame.isUiCellInfo = false;
                    this->cfg.sounds.play(3);
                    break;
                }
                case 1:
                {   auto& o = this->winGame;

                    o.dice.isRot = false;
                    o.isDiceHide = true ;

                    o.dice.resetDice();

                    this->doStep();
                    break;
                }
                case 2:
                {
                    if( winGame.isUiCellInfo)
                    {   winGame.isUiCellInfo = false;
                        setCellColor    ();

                        updateEndStep   ();
                        set2uiPlayers   ();

                        updateInfoPlayer();
                    }
                    break;
                }
                default: ASSERT(false)
            }

            nClickDice = (nClickDice + 1) % 3;
        }

        pr::InsexCircle iWin{3};

        void doStep()
        {
            const auto&    mdl    {cfg.cfgModel};
            cfg.uiCellInfo.isBot = cfg._3player[idUI].isBot;

            cfg.uiGameLog << model::doStep
            (   "start", { (int)mdl.idGame,
                           (int)idUI }
            );
/*
            ///////////////////////////////////////////////////
            cfg.uiPlayers[idUI] << uii::Clear() <<
            model::doStep
            (   "start", { (int)cfg.idGame,
                         (int)idUI  }
            );
*/

            const model::StateGame sg = model::getStateGame
            (   "get", {(int)mdl.idGame, (int)idUI}
            );

            ASSERT((unsigned)sg.dat[ED::E_SIZE == sg.dat.size()])

            unsigned idM = (unsigned)sg.dat[ED::E_IDPLAYER];

            ASSERT(cfg._3player[idUI].id == idM)

            cfg._3player[idUI].stateGame = sg;

            set2uiPlayers ();
            set2uiCellInfo();

            if(isGameOver = sg.dat[ED::E_GAMEOVER] >= 0; isGameOver)
            {   cfg.uiDownMessage << uii::Clear()
                    << "ИГРА ЗАКОНЧЕНА! Победитель: "
                    << cfg._3player[sg.dat[ED::E_GAMEOVER]].name
                    ;
            }

            winGame.setPositionChip(idUI, sg.dat[ED::E_POSITION], true);
            ///////////////////////////////////////////////////

            winGame.isUiCellInfo = true;

            cfg.cfgModel.moneyBank = sg.dat[model::StateGame::E_BANK1];
        }

        bool isGameOver{false};

        void updateInfoPlayer()
        {
            /// TODO ...
            nextPlayer();
        }

        void nextPlayer()
        {
            idUI = (idUI + 1) % cfg._3player.size();

            cfg.info_01(++cnt);

            cfg.uiDownMessage << uii::Clear()   << "Ход ИГРОКА: "
                              << (idUI + 1) << ": \""
                              << cfg._3player[idUI].name << ": \""
                              << mess[rand()%mess.size()];
            ++nStep;
        }

        ///-----------------------------------|
        /// Имя загруженной текстуры.         |
        ///-----------------------------------:
        std::string     nameTx;
        sf::RectangleShape fon;

        WinGame   winGame{cfg};
        WinUp       winUp{cfg};
        WinDown   winDown{cfg};

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
            idUI       = 0;
            cnt        = 0;
            nClickDice = 0;
            nStep      = 0;

            winGame.isUiCellInfo = false;

            cfg.uiDownMessage << uii::Clear() << "НОВАЯ ИГРА! Ход ИГРОКА: "
                              << (idUI + 1) << ": \""
                              << cfg._3player[idUI].name << "\"";
            isLog = false;

            winGame.reStart ();
            winGame.dice.resetDice();

            for(auto& e : cfg.uiPlayers) e.reStart();
        }

        ///-----------------------------------|
        /// Покупка ячейки.                   |
        ///-----------------------------------:
        void doBuy()
        {
            ASSERT(!cfg._3player[idUI].isBot) /// Запрет для ботов.

                  auto& sg4S{cfg._3player[idUI].stateGame4S};
            const auto& mdl {cfg.cfgModel};

            sg4S.isBuy  = true;

            cfg._3player[idUI].stateGame =
                  model::sendStateGame("4server",
                                      {(int)mdl.idGame, (int)idUI}, sg4S);

            ///-------------------------------|
            /// Получить стейт.               |
            ///-------------------------------:
            //cfg._3player[idUI].stateGame = sg;

            set2uiCellInfo();
            setCellColor  ();

            updateEndStep ();
            set2uiPlayers ();
        }

        void set2uiPlayers()
        {
                  auto& sg = cfg._3player[idUI].stateGame;
            const auto& mdl{ cfg.cfgModel};

            cfg.uiPlayers[idUI]  << uii::Clear()
                << "  ИГРОК  : " << sg.str[ES::E_NAME  ]        << '\n'
                << "  КОШЕЛЁК: " << sg.dat[ED::E_MONEY1]        << '\n'
                << "  КУБИК  : " << sg.dat[ED::E_NDICE ]        << '\n'
                << "  СТАТУС : " << sg.dat[ED::E_STATUS_PERS]+1 << " ---> "
                << mdl.decode2Str.getPlayer(sg.dat[ED::E_STATUS_PERS]).data()
                << '\n'
                ;
        }

        void set2uiCellInfo()
        {
            const auto& sg = cfg._3player[idUI].stateGame;
            const auto& mdl{cfg.cfgModel};

            cfg.uiCellInfo         << uii::Clear()
                << "  ИГРОК    : " << sg.str[ES::E_NAME]             << '\n'
                << "  ЯЧЕЙКА   : " << sg.str[ES::E_CELL]             << '\n'
                << "  ПОЗИЦИЯ  : " << sg.dat[ED::E_POSITION]         << '\n'
                << "  СТАТУС   : " << sg.dat[ED::E_STATUS_CELL]+1<< " ---> "
                << mdl.decode2Str.getCell(sg.dat[ED::E_STATUS_CELL]) << '\n'
                << "  ПРОДАЁТСЯ: " << sg.dat[ED::E_SELL]             << '\n'
                << "  СКУПКА   : " << sg.dat[ED::E_BYU]              << '\n'
                ;
        }

        ///-----------------------------------|
        /// Цвет купленной ячейки.            |
        ///-----------------------------------:
        void setCellColor()
        {   const auto& sg     = cfg._3player[idUI].stateGame;
            const auto& idCell =  sg.dat[ED::E_POSITION   ];
            const auto& isBusy =  sg.dat[ED::E_ISBUSYCELL ];
            const auto& isByu  =  sg.dat[ED::E_ISBYU      ];

            if(isByu)
            {   insertIcon          (idUI  , idCell); /// TODO: отладка ...
                winGame.setCellColor(idCell, isBusy);
            }
        }

        void updateEndStep()
        {   auto& sg = cfg._3player[idUI].stateGame;

            cfg.cfgModel.moneyBank = sg.dat[ED::E_BANK2 ];
            sg.dat[ED::E_MONEY1]   = sg.dat[ED::E_MONEY2];
        }

        void insertIcon(unsigned idPlayer, unsigned idCell)
        {   cfg.uiPlayers[idPlayer].insertIcon(idCell);
        }

        void eraseIcon(unsigned idPlayer, unsigned idCell)
        {   cfg.uiPlayers[idPlayer].eraseIcon(idCell);
        }

        ///-----------------------------------|
        /// Дебаг.                            |
        ///-----------------------------------:
        void debug( ) const
        {   l(cfg.cfgModel.order)
            l(cfg.cfgModel.order[idUI])

            const auto& sg = cfg._3player[idUI].stateGame;
            l(sg.dat[ED::E_IDPLAYER])
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
