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
                :   cfg   (cfg)
                ,   fon   (cfg.szfWin)
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
                };

                cfg.uiTuneBase.fooContinue = [this]()
                {   if ( this->isGameRun)
                         this->cfg.scenesSwitcher.next();
                    else this->cfg.sounds.play(2);
                };

                cfg.uiTuneBase.fooExit = [this]()
                {   this->cfg.pwin->close();
                };

                cfg.uiTuneBase.fooTuneGamer = [this]()
                {   const bool b = this->cfg.uiTuneGamer.isOpen;
                    this->cfg.doTuneAllClose();
                    this->cfg.uiTuneGamer.isOpen = !b;
                };

                cfg.uiTuneBase.fooTuneTester = [this]()
                {   const bool b = this->cfg.uiTuneBackDoor.isOpen;
                    this->cfg.doTuneAllClose();
                    this->cfg.uiTuneBackDoor.isOpen = !b;
                };

                cfg.uiTuneBase.fooRules = [this]()
                {   this->cfg.doTuneAllClose();
                    this->cfg.uiTuneRulesInfo.doOpen ();
                    this->cfg.uiTuneBase     .doClose();
                };
            }

        bool isGameRun{false};

        vsl::Config& cfg;

        PLUG_IOBJECT2

        void input(const std::optional<sf::Event>&  event) override
        {
            if (event->is<sf::Event::KeyPressed>())
            {   if (ISKEYPRESSED(F5))
                {   this->startModel();
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
        unsigned cntStart{};

        void startModel()
        {
            if(cntStart++ % 3) vsl::Sounds::p->play(1);
            else               vsl::Musics::p->play(3);

            //cfg.cfgModel = *model::getConfig  ();
            cfg.setConfigModel(*model::getConfig(cfg.backDoor,
                                                 cfg.userInit4Model));

            cfg.uiInit << model::getLogo(cfg.cfgModel.idGame) << "\n";

            cfg.uiGameLog << "///-----------------------------------|\n"
                             "///         ИГРА НАЧАЛАСЬ!            |\n"
                             "///-----------------------------------:\n" <<'\n';

            auto p = (*cfg.scenesSwitcher.scenes)[ScenesSwitcher::E_GAME];
            auto P = dynamic_cast<SceneGame*>(p);
                 P->reStart();

            this->cfg.scenesSwitcher.next();
            this->isGameRun = true;
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
            cfg.uiTuneGamer    .show();
        }
    };
}

#endif // SCENE_TUNE_H
