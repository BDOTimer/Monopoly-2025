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
                ,   fon    (cfg.szfWin)
            {
                fon.setTexture(&HolderTexture::get(nameTx));

                pr::setOrigin(fon);

                cfg.info_01(++cnt);

                cfg.uiUpLog.fooTune = [this]()
                {   this->winGame.quickFinished();
                    goTune();
                };

            //  cfg.uiUpLog.fooMusic = [this]()
            //  {
            //  };

                cfg.uiUpLog.fooDice2 = [this]()
                {   clickUpDice();
                };

                cfg.uiUpLog.fooLog = [this]()
                {      this->isLog = !this->isLog;
                };

                cfg.uiUpLog.fooFieldGeom = [this]()
                {   this->cfg.cfgModel.nextWorldGeometry();
                    this->winGame.figField.reGeometry();
                };

                cfg.uiCellInfo.fooNext = [this]()
                {   clickUpDice();
                };

                cfg.uiCellInfo.fooBuy = [this]()
                {   this->doBuy();
                };

                for(auto& e : cfg.uiPlayers)
                {   e.uiGameIcons.fooSellCell = [this](uii::UIGameIcons* p,
                                                       uii::IconIt idCell)
                    {   this->cfg.uiSellPanel.doOpen2();
                        this->cfg.uiSellPanel.idCell = idCell;
                        this->cfg.uiSellPanel.pIcons = p;

                        this->infoCellSell(idCell->first);
                    };
                    e.fooInfoColor = [this](unsigned idPlayer)
                    {   this->set2uiPlayers_Color(idPlayer);
                    };
                }

                cfg.uiSellPanel.fooSell = [this](unsigned idCell)
                {   this->doSell(idCell);
                };

                cfg.uiSellPanel.fooNext = [this](unsigned idCell)
                {   this->infoCellSell(idCell);
                };

                cfg.uiCellInfo.fooInfoColor = [this]()
                {   this->set2uiCellInfoColor();
                };

                cfg.uiUpLog.fooTestGOver = [this]()
                {    this->winGame.figGOver.isGameOver =
                    !this->winGame.figGOver.isGameOver ;

                    if(this->winGame.figGOver.isGameOver)
                    {   this->winGame.figGOver.onGameOver(
                            "test::Петя Череззаборногузадерищенко");
                    }
                };

                timerNext = cfg.timers.add();

                timerNext->foo = [this]()
                {   if(this->cfg.uiCellInfo.isOpen)
                    {
                    }
                    this->upDice();
                };
            }

        vsl::Config&  cfg;

        PLUG_IOBJECT2

        unsigned   idUI{};
        unsigned      cnt;

        ITimer  timerNext;

        using ED = model::StateGame::eSTATE;
        using ES = model::StateGame::eSTATESTR;

    /// bool pressEnter{false};

        void goTune()
        {   using E = vsl::ScenesSwitcher;
            cfg.scenesSwitcher.doSwitcher(E::E_TUNE);
            cfg.musics.stop();

            cfg.timers.clearITimer();
            cfg.isAutoClickNext = false;
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

            if (ISKEYPRESSED(Space))
            {   clickUpDice();
            }

            if (auto p = event->getIf<sf::Event::MouseButtonPressed>())
            {   if ( p->button  == sf::Mouse::Button::Left)
                {
                }
            }
        }

        unsigned  nStep     {};
        unsigned  nClickDice{};

        void upDice()
        {
            if(winGame.moveChip.isMove || winGame.figGOver.isGameOver)
            {   vsl::Sounds::p->play(6);
                return;
            }

            cfg.uiSellPanel.doClose();

            switch(nClickDice)
            {
                case 0:
                {   auto& o = this->winGame;

                    o.doDice();

                    this->winGame.isUiCellInfo = false;
                    this->cfg.sounds.play(3);

                    break;
                }
                case 1:
                {   auto& o = this->winGame;

                    o.stopDice();

                    this->doStep();

                    break;
                }
                case 2:
                {
                    if( winGame.isUiCellInfo)
                    {   winGame.isUiCellInfo = false;
                        setCellColor    ();

                        updateEndStep   ();
                    /// set2uiPlayers   ();

                        updateInfoPlayer();
                    }

                    break;
                }
                default: ASSERT(false)
            }

            nClickDice = (nClickDice + 1) % 3;
        }

        pr::InsexCircle iWin{3};

        ///-----------------------------------|
        /// Сделать ход к модели.             |
        ///-----------------------------------:
        void doStep()
        {
            const auto&     mdl    {cfg.cfgModel};

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

            ASSERT(cfg._3player[idUI].idTune == idM)
            
            std::string name{"\""};
                        name += cfg._3player[idUI].name;
                        name += "\"";

            ASSERT(sg.str[ES::E_NAME] == name)

            cfg._3player[idUI].stateGame = sg;

        /// set2uiPlayers ();
        /// set2uiCellInfo();

            if(isGameOver = sg.dat[ED::E_GAMEOVER] >= 0; isGameOver)
            {   cfg.uiDownMessage << uii::Clear()
                    << "ИГРА ЗАКОНЧЕНА! Победитель: "
                    << cfg.cfgModel.players[sg.dat[ED::E_GAMEOVER]].name;

                this->winGame.figGOver.onGameOver(
                    cfg.cfgModel.players[sg.dat[ED::E_GAMEOVER]].name.c_str());
            }
            else
            {
                winGame.movePositionChip(idUI, sg.dat[ED::E_POSITION], true);
                ///////////////////////////////////////////////////

                winGame.isUiCellInfo = true;
            }
            cfg.cfgModel.moneyBank = sg.dat[model::StateGame::E_BANK1];

            setCellColorSell();
        }

        bool isGameOver{false};

        void updateInfoPlayer()
        {
            /// TODO ...
            nextPlayer();
        }

        void nextPlayer()
        {
            cfg.uiPlayers[idUI].setFocus(false);

            idUI = (idUI + 1) % cfg._3player.size();

            cfg.info_01(++cnt);

            cfg.uiDownMessage << uii::Clear() << "Ход ИГРОКА: "
                              << (idUI + 1)   << ": \""
                              << cfg._3player[idUI].name << ": \""
                              << mess[rand()%mess.size()];
            ++nStep;

            cfg.uiPlayers[idUI].setFocus(true);

            cfg.uiCellInfo .isBot = cfg.uiPlayers[idUI].isBot();
            cfg.uiSellPanel.isBot = cfg.uiPlayers[idUI].isBot();

            this->autoClickNext();
        }

        ///-----------------------------------|
        /// Имя загруженной текстуры.         |
        ///-----------------------------------:
        std::string     nameTx{"res/game.jpg"};
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
            isGameOver = false;

            cfg.uiPlayers[idUI].setFocus(false);

            idUI       = 0;
            cnt        = 0;
            nClickDice = 0;
            nStep      = 0;

            cfg.uiPlayers[idUI].setFocus(true);

            winGame.isUiCellInfo = false;

            cfg.uiDownMessage << uii::Clear() << "НОВАЯ ИГРА! Ход ИГРОКА: "
                              << (idUI + 1) << ": \""
                              << cfg._3player[idUI].name << "\"";
            isLog = false;

            winGame.reStart ();
            winGame.dice.resetDice();

            for(auto& e : cfg.uiPlayers) e.reStart();

            cfg.uiCellInfo .isBot = cfg.uiPlayers[idUI].isBot();
            cfg.uiSellPanel.isBot = cfg.uiPlayers[idUI].isBot();

            winGame.figField.setGeometryRand();

            autoClickNext();
        }

        ///-----------------------------------|
        /// Покупка ячейки.                   |
        ///-----------------------------------:
        void doBuy()
        {
            if(cfg._3player[idUI].isBot) return;

                  auto& sg4S{cfg._3player[idUI].stateGame4S};
            const auto& mdl {cfg.cfgModel};

            sg4S.resert();

            sg4S.isBuy  = true;

            cfg._3player[idUI].stateGame =
                  model::sendStateGame("4server",
                                      {(int)mdl.idGame, (int)idUI}, sg4S);

            ///-------------------------------|
            /// Получить стейт.               |
            ///-------------------------------:
            //cfg._3player[idUI].stateGame = sg;

            if(cfg._3player[idUI].stateGame.dat[ED::E_ISBYU])
            {   cfg.uiDownMessage << uii::Clear()
                    << " Товар был куплен! Проверьте его наличие "
                       "в вашей собственности!";
            }

        /// set2uiCellInfo();
            setCellColor  ();

            updateEndStep ();
        /// set2uiPlayers ();
        }

        ///-----------------------------------|
        /// Продажа ячейки.                   |
        ///-----------------------------------:
        void doSell(unsigned idCell)
        {
            if(cfg._3player[idUI].isBot) return;

            if(nClickDice == 0)
            {   cfg.uiDownMessage << uii::Clear() << cfg._3player[idUI].name
                                  << ", cначала киньте кубик!";
                return;
            }

                  auto& sg4S{cfg._3player[idUI].stateGame4S};
            const auto& mdl {cfg.cfgModel};

            sg4S.resert();

            sg4S.isSellIds.push_back(idCell);

            cfg._3player[idUI].stateGame =
                  model::sendStateGame("4server",
                                      {(int)mdl.idGame, (int)idUI}, sg4S);

            if(cfg._3player[idUI].stateGame.dat[ED::E_ISSELL])
            {
                cfg.uiPlayers[this->idUI].uiGameIcons.erase(idCell);
                cfg.uiSellPanel.next();

                winGame.setCellColor(idCell, false);
            }

        /// set2uiCellInfo();
            setCellColor  ();

            updateEndStep ();
        /// set2uiPlayers ();

            if(!cfg._3player[idUI].stateGame.str[ES::E_REFEREE].empty())
            {
                cfg.uiDownMessage << uii::Clear()
                    << cfg._3player[idUI].stateGame.str[ES::E_REFEREE];
            }
        }

        void set2uiPlayers_Color(unsigned idPlayer)
        {
                  auto& sg = cfg._3player[idPlayer].stateGame;
            const auto& mdl{ cfg.cfgModel};

            const
            unsigned I{static_cast<unsigned>(sg.dat[ED::E_STATUS_PERS])
                     % static_cast<unsigned>(uii::style::colTxtStatus.size())};

            ImGui::Text("  КОШЕЛЁК:  %d\n", sg.dat[ED::E_MONEY1]);
            ImGui::Text("  КУБИК  :  %d\n", sg.dat[ED::E_NDICE ]);
            ImGui::Text("  СТАТУС : ");
            ImGui::SameLine ();
            ImGui::TextColored(
                uii::style::colTxtStatus[I], "%u ---> %s\n",
                sg.dat[ED::E_STATUS_PERS]+1,
                mdl.decode2Str.getPlayer(sg.dat[ED::E_STATUS_PERS]).data());
        }

        void set2uiCellInfoColor()
        {
            const auto& sg = this->cfg._3player[idUI].stateGame;
            const auto& mdl{ this->cfg.cfgModel};

            const
            unsigned I{static_cast<unsigned>(sg.dat[ED::E_STATUS_CELL])
                     % static_cast<unsigned>(uii::style::colTxtStatus.size())};

            ImGui::Text("  ИГРОК    : %s\n", sg.str[ES::E_NAME].c_str());
            ImGui::Text("  ЯЧЕЙКА   : ");
            ImGui::SameLine ();
            ImGui::TextColored(
                uii::style::colTxtStatus[I],"%s\n", sg.str[ES::E_CELL].c_str());

            ImGui::Text("  ПОЗИЦИЯ  :  %d\n", sg.dat[ED::E_POSITION]);
            ImGui::Text("  СТАТУС   : ");
            ImGui::SameLine ();
            ImGui::TextColored(
                uii::style::colTxtStatus[I], "%u ---> %s\n",
                sg.dat[ED::E_STATUS_CELL]+1,
                mdl.decode2Str.getCell(sg.dat[ED::E_STATUS_CELL]).data());

            ImGui::Text("  ПРОДАЁТСЯ:  %d\n", sg.dat[ED::E_SELL]);
            ImGui::Text("  СКУПКА   :  %d\n", sg.dat[ED::E_BYU ]);
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
            {   insertIcon          (idUI   , idCell);
                winGame.setCellColor(idCell, isBusy);

                vsl::Sounds::p->play(11);
                cfg.uiDownMessage << uii::Clear()
                    << " Товар \""
                    << cfg.cfgModel.cells[idCell].name
                    << "\" был куплен!";

                cfg._3player[this->idUI].statistic.nAllByu++;
            }
        }

        ///-----------------------------------|
        /// Цвет проданной ячейки.            |
        ///-----------------------------------:
        void setCellColorSell()
        {
            const auto& sg     = cfg._3player[idUI].stateGame;
            const auto& isSell =  sg.dat[ED::E_ISSELL];

            if(isSell)
            {   for(const auto idCellSell : sg.idCellSells)
                {   //cfg.uiPlayers[this->idUI].eraseIcon(idCellSell);

                    cfg.uiPlayers[this->idUI].uiGameIcons.erase(idCellSell);
                    winGame.setCellColor(idCellSell, false);

                    cfg.uiDownMessage << uii::Clear()
                        << " Товар \""  
                        << cfg.cfgModel.cells[idCellSell].name 
                        << "\" был продан!";

                    cfg._3player[this->idUI].statistic.nAllSell++;
                }
                const_cast<model::StateGame&>(sg).idCellSells.clear();

                cfg.uiPlayers[this->idUI] << uii::Clear()
                    << "Всего продано: " 
                    << cfg._3player[this->idUI].statistic.nAllSell;

                vsl::Sounds::p->play(11);
            }
        }

        void infoCellSell(const unsigned idCell)
        {   const auto& cellUser
            {   this->cfg.cfgModel.cells[idCell]
            };

            /// bankSell - Банк продаёт.
            /// bankBuy  - Банк покупает.

            this->cfg.uiSellPanel << uii::Clear()
                << "ЯЧЕЙКА: " << idCell << "\n\""
                << cellUser.name        << "\"\n\n"
                << "ЦЕНА         :  " 
                << model::getPriseBankBuy(
                        cfg.cfgModel.idGame, 0, idCell)        << "\n"
                << "Банк ПРОДАЁТ : ["  << cellUser.bankSell[0] << ", "
                                       << cellUser.bankSell[1] << ", "
                                       << cellUser.bankSell[2] << "]\n"
                << "Банк ПОКУПАЕТ: ["  << cellUser.bankBuy [0] << ", "
                                       << cellUser.bankBuy [1] << ", "
                                       << cellUser.bankBuy [2] << "]\n"
                ;
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

        void autoClickNext(uint16_t sec)
        {   cfg.timers.setPeriod(timerNext, sec);
        }

        void autoClickNext()
        {   if(cfg.isAutoClickNext && cfg._3player[idUI].isBot)
            {   this->autoClickNext(1);
                this->autoClickNext(4);
                this->autoClickNext(7);
            }
            else
            {   vsl::Sounds::p->play(10);
            }
        }

        void clickUpDice()
        {   if(  this->cfg.isAutoClickNext && 
                 this->cfg._3player[idUI].isBot ) vsl::Sounds::p->play(6);
            else this->upDice();           
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
