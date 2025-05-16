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

                tmess1.setString(L"ИГРА.\nВыход: ESCAPE");

                dice.init();

                cfg.info_01(++cnt);

                cfg.uiGameLog.fooDice = [this]()
                {   if(!this->isDiceHide)
                        this->dice.isRot = !dice.isRot;
                };

                cfg.uiGameLog.fooMusic = [this]()
                {   using E = sf::SoundSource::Status;
					const bool
                    b{    this->cfg.musicGame.getStatus() == E::Playing };
                    b   ? this->cfg.musicGame.pause()
                        : this->cfg.musicGame.play ();
                };

                cfg.uiGameLog.fooDiceHide = [this]()
                {   this->isDiceHide = !this->isDiceHide;
                };
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

        void doStep()
        {
            unsigned& idPlayer = cfg.players[IDPLAYER].id;

            cfg.uiGameLog << model::doStep
            (   "bot", { (int)cfg.idGame,
                         (int)idPlayer }
            );

            if(++IDPLAYER == cfg.players.size()) IDPLAYER = 0;

            cfg.info_01(++cnt);
        }

        ///-----------------------------------|
        /// Имя загруженной текстуры.         |
        ///-----------------------------------:
        std::string     nameTx;
        sf::RectangleShape fon;
        TextStyleA      tmess1;

        ShaderDice        dice;
        WinGame        winGame{cfg};
        WinUp            winUp{cfg};
        WinDown        winDown{cfg};

        WinPlayer winPlayers[3]
        {   {cfg, 0},
            {cfg, 1},
            {cfg, 2}
        };

        bool isDiceHide{true};

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

            target.draw   (winPlayers[0], states);
            target.draw   (winPlayers[1], states);
            target.draw   (winPlayers[2], states);
            target.draw   (winDown  , states);
            target.draw   (winUp    , states);
            target.draw   (winGame  , states);

            target.setView(*cfg.camGui   );
            target.draw   (tmess1, states);

            if(!isDiceHide)
            {   target.setView(*cfg.camFon );
                target.draw   (dice, states);
            }

            cfg.uiGameLog.show();
        }
    };
}

#endif // SCENE_GAME_H
