#ifndef X_H
#define X_H
///----------------------------------------------------------------------------|
/// "config-vsl.h"
///----------------------------------------------------------------------------:
#include "debug.h"

#include "controller/controller.h"
#include "ui-tune.h"
#include "ui-win-game.h"
#include "user-model.h"
#include "sceneGame/markup.h"


namespace vsl
{
    struct  Config
    {       Config (sf::RenderWindow& window)
                :   pwin            (&window)
                ,   uiTune          ( window, "Настройки ...")
            {   init_();
                resizeFormImgui();

                Musics::p = &musics;
                Sounds::p = &sounds;
            }

        ///-----------------------------------|
        /// Профиль игрока.                   |
        ///-----------------------------------:
        model::UserInit4Model   userInit4Model;

        ///-----------------------------------|
        /// Конфиг модели.                    |
        ///-----------------------------------:
        model::ConfigShare            cfgModel;

        ///-----------------------------------|
        /// Черная дверь на сервер.           |
        ///-----------------------------------:
        BackDoor::Data                 backDoor;

        void setConfigModel(model::ConfigShare& mdl)
        {   cfgModel = mdl;
            initPlayers_();
            reStart_    ();
        }

        controller::Players _3player;

        void initPlayers_()
        {   const auto&         P = cfgModel.players;
            _3player .clear    ();
            _3player .reserve  (P.size());
            for(unsigned i = 0; P.size() > i; ++i)
            {   auto&    b =    P[(unsigned)cfgModel.order[i]];

                _3player .emplace_back(controller::Player(b.isBot, i, b.name));
                uiPlayers[i].setPlayer(_3player.back());
            }
        }

        sf::RenderWindow*        pwin;

        MarkupSceneGame      markupSG;

        sf::Vector2u         szuWin  ;
        sf::Vector2f         szfWin  ;

        sf::View*     camFon{nullptr};
        sf::View*     camGui{nullptr};

        ScenesSwitcher scenesSwitcher;

        sf::Time           deltaTime;

        Musics             musics;
        Sounds             sounds;

        uii::UITest        uiTune       ;
        uii::UIUpLog       uiUpLog      ;
        uii::UIGame        uiGameLog    ;
        uii::UIDownMessage uiDownMessage;

        uii::UITuneBase    uiTuneBase{this};

        std::array<uii::UIScnGamePlayer, 3> uiPlayers;

        uii::UIWinGameCellInfo uiCellInfo;

        uii::UITuneRulesInfo uiTuneRulesInfo{ uiTuneBase       };
        uii::UITuneGamer     uiTuneGamer    { uiTuneBase, this,&userInit4Model};
        uii::UITuneBackDoor  uiTuneBackDoor { uiTuneBase, this };

        uii::UIUpBankInfo    uiUpBankInfo   {&cfgModel.moneyBank};

        void doTuneAllClose()
        {   uiTuneGamer   .doClose();
            uiTuneBackDoor.doClose();
        }

        ///-----------------------------------|
        /// Новая игра.                       |
        ///-----------------------------------:
        void reStart_()
        {    for(auto&  e : uiPlayers) e << uii::Clear();
             uiGameLog.clear();
             uiUpLog  .clear();
        }

        static sf::Font& getFont()
        {///static sf::Font font("consola.ttf");
            static sf::Font font("res/JetBrainsMono-Regular.ttf");
            return font;
        }

        void init_()
        {   szuWin = initWinSize();
            szfWin = {float(szuWin.x), float(szuWin.y)};

            getFont().setSmooth(true);
        }

        inline static constexpr float SCALE_WIN{0.95f};

        static sf::Vector2u initWinSize(sf::Vector2u szuWin = {0,0})
        {
            if(szuWin.x == 0)
            {   sf::VideoMode  dm = sf::VideoMode::getDesktopMode();
                    szuWin   = dm.size;
                    szuWin.x = unsigned(            szuWin.x);
                    szuWin.y = unsigned(SCALE_WIN * szuWin.y);
            }
            return  szuWin;
        }

        void info_01(unsigned cnt)
        {
            uiGameLog << "ПАУЗА::\nНажмите ENTER, чтобы сделать "
                      << cnt << " шаг ...\n"
                      << "------------------------------"
                        "-------------------------...\n";
        }

        void      setFramerateLimit(unsigned fps = 10000)
        {   pwin->setFramerateLimit(fps);
        }

        const float dt() const { return deltaTime.asSeconds(); }

        static vsl::Config&    get()
        {   sf::RenderWindow window(sf::VideoMode({ 1344, 768 }),
                                   "SFML::Test::2", sf::State::Windowed);
            static vsl::Config cfg (window);
            return             cfg;
        }

        ///-----------------------------------|
        /// UI геометрия.                     |
        ///-----------------------------------:
        void resizeFormImgui()
        {
            ///-------------------|
            /// uiGameLog         |
            ///-------------------:
            {   const auto  px = 0;
                const auto& py = markupSG.winUp.size.y;
                const auto& sx = markupSG.winPlayer[0].size.x;
                const auto& sy = markupSG.getWinBase().size.y;

                uiGameLog.setGeometry( {sx * szfWin.x, sy * szfWin.y},
                                       {px           , py * szfWin.y}
                );
            }

            const auto& G = markupSG.winPlayer;

            for(size_t i{}; i < uiPlayers.size(); ++i)
            {
                const float szX = G[i].size    .x * szfWin.x;
                const float szY = G[i].size    .y * szfWin.y;
                const float psX = G[i].position.x * szfWin.x;
                const float psY = G[i].position.y * szfWin.y;

                uiPlayers[i].setGeometry({szX, szY}, {psX, psY});
            }

            const float Y = 6.0f;
            const float Y2 = Y+Y;

            ///-------------------|
            /// uiUpLog           |
            ///-------------------:
            {   const auto& H = markupSG.winUp;
                const float Y = 6.0f;

                const float szX = G[0].size .x * szfWin.x - Y2;
                const float szY = H.size    .y * szfWin.y - Y2;
                const float psX = H.position.x * szfWin.x + Y;
                const float psY = H.position.y * szfWin.y + Y;

                uiUpLog.setGeometry({szX, szY}, {psX, psY});
            }

            ///-------------------|
            /// uiDownMessage     |
            ///-------------------:
            {   const auto& H = markupSG.winDown;
                const float szX =(H.size    .x - G[0].size .x) * szfWin.x - Y2;
                const float szY = H.size    .y * szfWin.y - Y2;
                const float psX = G[0].size .x * szfWin.x + Y;
                const float psY = H.position.y * szfWin.y + Y;

                uiDownMessage.setGeometry({szX, szY}, {psX, psY});
            }

            ///-------------------|
            /// uiTuneBase        |
            ///-------------------:
            {   const float szX = szfWin.x / 5.f;
                const float szY = szfWin.y / 2.f;
                const float psX = szfWin.x / 2.f - szX / 2.f;
                const float psY = szfWin.y / 2.f - szY / 2.f;

                uiTuneBase.setGeometry({szX, szY}, {psX, psY});
            }

            ///-------------------|
            /// uiCellInfo        |
            ///-------------------:
            {   const auto& G = markupSG.getWinBase();

                const float szX = G.size.x / 2 * szfWin.x;
                const float szY = G.size.y / 4 * szfWin.y;
                const float psX =(G.position.x + G.size.x     / 4) * szfWin.x;
                const float psY =(G.position.y + G.size.y * 3 / 8) * szfWin.y;

                uiCellInfo.setGeometry({szX, szY}, {psX, psY});
            }

            ///-------------------|
            /// uiTuneRulesInfo   |
            ///-------------------:
            {   const float PAD  = 30.f;
                const float W    = szfWin.x / 2.3f;
                const float PAD2 = PAD + PAD;
                const float szX  = W;
                const float szY  = szfWin.y - PAD2;
                const float psX  = (szfWin.x - W) / 2;
                const float psY  = PAD;

                uiTuneRulesInfo.setGeometry({szX, szY}, {psX, psY});
            }

            ///-------------------|
            /// uiTuneBackDoor    |
            ///-------------------:
            {   const auto& sz = uiTuneBase.size;
                const auto& ps = uiTuneBase.position;

                const float psX = ps.x + sz.x + 10.f;
                const float psY = ps.y;

                uiTuneBackDoor.setGeometry(sz, {psX, psY});
            }

            ///-------------------|
            /// uiTuneGamer       |
            ///-------------------:
            {   const auto& sz = uiTuneBase.size;
                const auto& ps = uiTuneBase.position;

                const float psX = ps.x + sz.x + 10.f;
                const float psY = ps.y;

                uiTuneGamer.setGeometry(sz, {psX, psY});
            }

            ///-------------------|
            /// uiUpBankInfo      |
            ///-------------------:
            {
                const auto& G = markupSG.getWinBase();

                const auto  px = uiUpLog.size.x + 10.f;
                const auto& py = uiUpLog.position.y;
                const auto& sx = G.size.x * szfWin.x * 0.2f;
                const auto& sy = uiUpLog.size.y;

                uiUpBankInfo.setGeometry({sx, sy}, {px, py});
            }

            uiUpLog.messDown = &uiDownMessage;
        }
    };
}

namespace uii
{
    int* UITuneBackDoor::getIsSeed(){ return &cfg->backDoor.isSeed; }
}

#endif // X_H
