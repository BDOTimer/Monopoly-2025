#ifndef SCENE_TUNE_H
#define SCENE_TUNE_H
///----------------------------------------------------------------------------|
/// "scene-tune.h"
///----------------------------------------------------------------------------:
#include "common.h"
#include "scene-game.h"

namespace vsl
{
    ///------------------------------------------------------------------------|
    /// SceneTune.
    ///-------------------------------------------------------------- SceneTune:
    struct  SceneTune   :  vsl::IObject
    {       SceneTune     (vsl::Config& cfg)
				:	cfg   (cfg)
				,	fon   (cfg.szfWin)
            {
                fon.setTexture(&HolderTexture::get("res/tune.jpg"));
                pr::setOrigin(fon);

                tmess1.setString(L"ТЮНИНГ.\nДля игры: ЖМИ \"1\"");

                const auto r = tmess1.getGlobalBounds();

                tmess1.setPosition
                ({  cfg.szfWin.x - r.size.x - 30,
                    r.position.y
                });

                cfg.uiTuneBase.fooRestart = [this]()
                {   this->startModel();
                    this->cfg.scenesSwitcher.next();
                    this->isGameRun = true;
                };

                cfg.uiTuneBase.fooContinue = [this]()
                {   if ( this->isGameRun)
                         this->cfg.scenesSwitcher.next();
                    else this->cfg.mp3no.play();
                };

                cfg.uiTuneBase.fooExit = [this]()
                {   this->cfg.pwin->close();
                };

                cfg.uiTuneBase.fooRules = [this]()
                {   this->cfg.uiTuneRulesInfo.doOpen ();
                    this->cfg.uiTuneBase     .doClose();
                };

                cfg.uiTuneBase.fooTuneTester = [this]()
                {   this->cfg.uiTuneBackDoor.isOpen =
                   !this->cfg.uiTuneBackDoor.isOpen ;
                };
            }

        bool isGameRun{false};

		vsl::Config& cfg;

        PLUG_IOBJECT2

		void input(const std::optional<sf::Event>&  event) override
		{
			if (event->is<sf::Event::KeyPressed>())
            {   if (ISKEYPRESSED(Num1))
                {   this->startModel();
                    cfg.scenesSwitcher.next();
                }
            }
		}

        ///-----------------------------------|
        /// ...                               |
        ///-----------------------------------:
        sf::RectangleShape fon;
        pr::TextStyleA  tmess1;

        ///-----------------------------------|
        /// Дебаг.                            |
        ///-----------------------------------:
        void debug() const
        {   l(fon.getTexture()->getSize().x)
            l(fon.getTexture()->getSize().y)
        }

    private:
        ///////////////////////////////////////////////////////////////////////:
        void startModel()
        {   //cfg.cfgModel = *model::getConfig ();
            cfg.setConfigModel(*model::getConfig (cfg.backDoor));
            cfg.uiTune << model::getLogo(cfg.cfgModel.idGame) << "\n";

            cfg.uiGameLog << "///-----------------------------------|\n"
                             "///         ИГРА НАЧАЛАСЬ!            |\n"
                             "///-----------------------------------:\n" <<'\n';

            auto p = (*cfg.scenesSwitcher.scenes)[ScenesSwitcher::E_GAME];
            auto P = dynamic_cast<SceneGame*>(p);
                 P->reStart();
        }
        ///////////////////////////////////////////////////////////////////////.

        ///------------------------------------|
        /// На рендер.                         |
        ///------------------------------------:
        virtual void draw(sf::RenderTarget& target,
                          sf::RenderStates  states) const
        {
            target.setView(*cfg  . camFon);
            target.draw   ( fon  , states);

            target.setView(*cfg  . camGui);
            target.draw   (tmess1, states);

        /// cfg.uiTune    .show();
            cfg.uiTuneBase     .show();
            cfg.uiTuneRulesInfo.show();
            cfg.uiTuneBackDoor .show();
        }
    };
}

#endif // SCENE_TUNE_H
