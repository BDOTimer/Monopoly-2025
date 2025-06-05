#ifndef UI_TUNE_H
#define UI_TUNE_H
///----------------------------------------------------------------------------|
/// "ui-tune.h"
///----------------------------------------------------------------------------:
#include "ui-imgui.h"

namespace vsl
{
    struct Config;
}


namespace uii
{
    ///------------------------------------------------------------------------|
    /// UITuneBase базовое начальное меню ...
    ///------------------------------------------------------------- UITuneBase:
    struct  UITuneBase  : UIBase
    {       UITuneBase  (vsl::Config* cfg)
            {
                name = "Настройки";

                ImGuiStyle&      style = ImGui::GetStyle();
                ColorBLog.m[0] = style.Colors[ImGuiCol_Button];

                init();
            }

    /// ImVec4     buttonColor;
        vsl::Config*       cfg;

        void  doTuneAllClose();

        Callback fooRestart   {[this](){}};
        Callback fooContinue  {[this](){}};
        Callback fooTuneTester{[this](){}};
        Callback fooTuneGamer {[this](){}};
        Callback fooRules     {[this](){}};
        Callback fooExit      {[this](){}};

        ImVec2 WH  {100, 40};
        ImVec2 WHx2;

        void show()
        {   if(!isOpen) return;

            auto& color = ImGui::GetStyle().Colors[ImGuiCol_WindowBg];
                  color = ImColor(35,35,35,190);


            ///---------------------------------------|
            /// Позиция и размер окна.                |
            ///---------------------------------------:
            /// TODO: Окно на разных компах должно соответствовать ....

            ///
            ImGui::SetNextWindowSize(size);
            ///
            ImGui::SetNextWindowPos (position);

            ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 35.0f);

            ///---------------------------------------|
            /// Окно <name>.                          |
            ///---------------------------------------:
            ImGui::Begin( name.data(), nullptr, 0
                    /// | ImGuiWindowFlags_NoCollapse
                        | ImGuiWindowFlags_NoMove
                        | ImGuiWindowFlags_NoTitleBar
                        | ImGuiWindowFlags_NoScrollbar
                        | ImGuiWindowFlags_NoResize
                    /// | ImGuiWindowFlags_HorizontalScrollbar
                    /// | ImGuiWindowFlags_AlwaysVerticalScrollbar
                    /// | ImGuiWindowFlags_MenuBar
                    /// | ImGuiWindowFlags_NoBackground
                    /// | ImGuiWindowFlags_AlwaysAutoResize
            );

            ImGui::PushStyleColor(ImGuiCol_Button,       (ImVec4)colButtonB);
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered,(ImVec4)colButtonH);
            ImGui::PushStyleColor(ImGuiCol_ButtonActive ,(ImVec4)colButtonA);

                if(ImGui::Button("НОВАЯ ИГРА", WH))
                {   //snd1.play ();
                    fooRestart();
                }

                if(ImGui::Button("ПРОДОЛЖИТЬ", WH))
                {   fooContinue();
                    vsl::Sounds::p->play(0);
                }

                if(ImGui::Button("НАСТРОЙКИ ТЕСТИРОВЩИКА", WH))
                {   fooTuneTester();
                    vsl::Sounds::p->play(0);
                }

                if(ImGui::Button("НАСТРОйКИ ИГРОКА", WH))
                {   fooTuneGamer();
                    vsl::Sounds::p->play(0);
                }

                if(ImGui::Button("ПРАВИЛА ИГРЫ", WH))
                {   fooRules  ();
                    vsl::Sounds::p->play(0);
                }

                if(ImGui::Button("ВЫХОД", WH))
                {   fooExit   ();
                    vsl::Sounds::p->play(0);
                }

                if (ImGui::ImageButton("АВТОРЫ", texId, WH))
                {   vsl::Sounds::p->play(1);

#ifdef _WIN32
    #define OPEN_CMD "start"
#elif __APPLE__
    #define OPEN_CMD "open"
#else
    #define OPEN_CMD "xdg-open"
#endif
                    auto comm{std::string(OPEN_CMD)
                         + " https://gamedev.ru/projects/forum/?id=289951"};
                    std::system(comm.c_str());
                }

            ImGui::PopStyleColor();
            ImGui::PopStyleColor();
            ImGui::PopStyleColor();

            ImGui::End();

        //  ImGui::Begin("TEST");
        //  ImGui:(buttonTexture);
        //  ImGui::End();

            ImGui::PopStyleVar();
        }

        void setGeometry(ImVec2 sz, ImVec2 ps)
        {   UIBase::setGeometry(sz,        ps);
            WH   = {sz.x - 17.0f, sz.y / 6.5f};
            WHx2 = {WH.x + WH.x, WH.y};
        }

        myl::SwitcherData<ImVec4, 2> ColorBLog
        {   ImVec4{0.2f, 0.7f, 0.2f, 1.0f},
            ImVec4{0.7f, 0.2f, 0.2f, 1.0f}
        };

        static ImTextureID convertSFMLTexture2Im(const sf::Texture& tx)
        {   return (ImTextureID)(size_t)tx.getNativeHandle();
        }

        sf::Texture buttonTexture;
        ImTextureID texId;
        void init()
        {   if (!buttonTexture.loadFromFile("res/img/button.png"))
            {   ASSERT(false)
            }
            texId = convertSFMLTexture2Im(buttonTexture);
        }

    private:

    };

    extern const char* const strRules;
    ///------------------------------------------------------------------------|
    /// UITuneRulesInfo базовое начальное меню ...
    ///-------------------------------------------------------- UITuneRulesInfo:
    struct  UITuneRulesInfo   : UIBase
    {       UITuneRulesInfo(UITuneBase& uiTuneBase)
                :   uiTuneBase( uiTuneBase)
            {
                name = "ПРАВИЛА ИГРЫ";

                ImGuiStyle&      style = ImGui::GetStyle();
                ColorBLog.m[0] = style.Colors[ImGuiCol_Button];

                init();

                (*this) << strRules;

                doClose();
            }

        UITuneBase& uiTuneBase;

        //ImVec4 buttonColor;

        Callback fooEmpty   {[this](){}};

        ImVec2 WH  {100, 40};
        ImVec2 WHx2;

        void doOpen()
        {   vsl::Musics::p->play(2);
            isOpen = true;
        }

        void show()
        {
            if(!isOpen) return;

            auto& color = ImGui::GetStyle().Colors[ImGuiCol_WindowBg];
                  color = ImColor(35,35,35,190);

            ///---------------------------------------|
            /// Позиция и размер окна.                |
            ///---------------------------------------:
            /// TODO: Окно на разных компах должно соответствовать ....

            ///
            ImGui::SetNextWindowSize(size);
            ///
            ImGui::SetNextWindowPos (position);

            ///---------------------------------------|
            /// Окно <name>.                          |
            ///---------------------------------------:
            ImGui::Begin( name.data(), &isOpen, 0
                        | ImGuiWindowFlags_NoCollapse
                        | ImGuiWindowFlags_NoMove
                    /// | ImGuiWindowFlags_NoTitleBar
                        | ImGuiWindowFlags_NoScrollbar
                        | ImGuiWindowFlags_NoResize
                    /// | ImGuiWindowFlags_HorizontalScrollbar
                    /// | ImGuiWindowFlags_AlwaysVerticalScrollbar
                    /// | ImGuiWindowFlags_MenuBar
                    /// | ImGuiWindowFlags_NoBackground
                    /// | ImGuiWindowFlags_AlwaysAutoResize
            );

            ImGui::PushStyleColor(ImGuiCol_Button,       (ImVec4)colButtonB);
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered,(ImVec4)colButtonH);
            ImGui::PushStyleColor(ImGuiCol_ButtonActive ,(ImVec4)colButtonA);

            {   showButtonClose("Закрыть");
                
                if(ImGui::SliderFloat("Громкость музыки:", 
                                   vsl::Musics::p->getPVol(), 0, 100, "%.f"))
                {   vsl::Musics::p->setVolume();
                }

                ImGui::Text("%s", log.str().c_str());
                showButtonClose("Закрыть ");
            }

            ImGui::PopStyleColor();
            ImGui::PopStyleColor();
            ImGui::PopStyleColor();

            ImGui::End();

        //  ImGui::Begin("TEST");
        //  ImGui:(buttonTexture);
        //  ImGui::End();

            if(!isOpen)
            {   vsl::Sounds::p->play(0);
                uiTuneBase.doOpen();
                vsl::Musics::p->stop();
            }
        }

        void setGeometry(ImVec2 sz, ImVec2 ps)
        {   UIBase::setGeometry(sz,        ps);
            WH   = {sz.x - 17.0f, sz.y / 20.f};
            WHx2 = {WH.x + WH.x, WH.y};
        }

        myl::SwitcherData<ImVec4, 2> ColorBLog
        {   ImVec4{0.2f, 0.7f, 0.2f, 1.0f},
            ImVec4{0.7f, 0.2f, 0.2f, 1.0f}
        };

        static ImTextureID convertSFMLTexture2Im(const sf::Texture& tx)
        {   return (ImTextureID)(size_t)tx.getNativeHandle();
        }

        sf::Texture buttonTexture;
        ImTextureID texId;
        void init()
        {   if (!buttonTexture.loadFromFile("res/img/button.png"))
            {   ASSERT(false)
            }
            texId = convertSFMLTexture2Im(buttonTexture);
        }

    private:

        void showButtonClose(const char* mess)
        {   if(ImGui::Button(mess, WH))
            {   
                vsl::Sounds::p->play(0);
                isOpen     = false;
                uiTuneBase .doOpen();

                vsl::Musics::p->stop();
            }
        }
    };

    const char* const strRules
    {
R"(
--------------------------------------------------------------------------------
ПРАВИЛА ИГРЫ: ПРОМЫШЛЕННАЯ МОНОПОЛИЯ.
--------------------------------------------------------------------------------

Подготовка.
    Игроки выбирают фишки . Далее определяют, кто будет ходить первым.
    Это делают броском кубика: чем больше число выпало, тем раньше ходит игрок.
    Банкиром в ире будет вычислительная машина.
    Каждый игрок получает 300 монет на свой счёт.
--------------------------------------------------------------------------------
Порядок ходов.
    Ходы отределяет стандартный кубик 1-6 граней .
    Игроки по очереди бросают кубик и делают соответствующее количество ходов.
    Порядок покупки и продажи недвижимости.
    Встав на поле, игрок либо приобретает актив либо отказывается от сделки.
    Стоимость актива указана на игровой ячейке
    и рассчитана согласно статусу игрока.
    В игре установлены три вида промышленности:
        - Производство военных товаров.
        - Производство продуктов питания.
        - Детская промышленность.
    Игрок может быть в статусе менеджера
        - по продаже детских товаров,
        - военным инженером или
        - менеджером по продажам продуктов питания.
    Каждый статус даёт преимущества при управлении своим вида промышленности
    и недостатки при управлении чужим. Игрок проходит переобучение при
    прохождении точки старта и меняет статус позволяющий извлекать прибыль из
    другого вида промышленности.
    Подробнее указано на информационных табло предприятий.
    Продажи собственности между игроками запрещены.
    Продавать можно в банк.
    Стоимость недвижимости изменяется при изменении статуса.
    Это даёт возможность для биржиых торгов. Раз за круг игрок может купить
    одну любую общественную собственность, без занятия этого поля.
--------------------------------------------------------------------------------
Аренда.
    Если игрок встал на уже купленное поле,
    то он обязан оплатить аренду (стоимость предприятия).
    Сумма высчитывается от умения управлять данным видом предприятий.
    На аренду влияет, купил ли собственник все поля этого цвета
    или только часть этих полей,
    то есть собрал монополию. С ней арендный платеж будет максимальным.
    Монополия начинаеся с 4 предприятий одного вида промышленности.
    Аренда повышается с увелечением предприятий этого вида.
    Если игрок встал на своё поле получает из банка дивиденды в размере умения
    управлять данным видом промышленности.
--------------------------------------------------------------------------------
Порядок изъятия собственности при минусовом балансе.
    До минуса 200 монет собственность не изымается.
    Аренда другому игроку платится из банка,
    но все последующие доходы автоматически уходят в банк.
    Игрок не может покупать собственность в минус.
    Если долг составит больше 200 золотых монет,
    собственность продадут автоматически (банкротство).
--------------------------------------------------------------------------------
События.
    Первые 6 кругов игроки получают бонус в размере 100 монет
    за прохождение начального поля.
    События определяются автоматически. Открывается карточка шанс.
    Какими бывают события:
    получить деньги из банка;
    отдать часть денег в банк;
    получить право сделать на выбор от 1 до 3 ходов .
    получить право сделать на выбор от 3 до 6 ходов .
    сменить статус или продлить текущий статус на следующий круг.
    получить дополнительный ход и так далее.
    имет право приобрести собственность по карточке шанс.
--------------------------------------------------------------------------------
Победа условия.
    Исходом игры станет капитализация рынка одним игроком
    ( стоимость активов по номиналу + оборотные средства на счету)
    в размере 51% процента при игре трёх игроков.
    Оба других игрока окажутся в минусе  больше чем на 100 монет и один из них
    не будет иметь собственность.
    Один игрок занял 10 ячеек одной отрасли промышленности и две другой.
--------------------------------------------------------------------------------
)"
    };


    ///------------------------------------------------------------------------|
    /// UITuneBackDoor черный вход на сервер.
    ///--------------------------------------------------------- UITuneBackDoor:
    struct  UITuneBackDoor   : UIBase
    {       UITuneBackDoor(UITuneBase& uiTuneBase, vsl::Config* cfg)
                :   cfg       (cfg)
                ,   uiTuneBase( uiTuneBase)
            {
                name = "Хак-Тюнинг";

                ImGuiStyle&      style = ImGui::GetStyle();
                ColorBLog.m[0] = style.Colors[ImGuiCol_Button];

                init();

                (*this) << "Только в новой игре!";

                doClose();
            }

        vsl::Config*       cfg;
        UITuneBase& uiTuneBase;

        //ImVec4 buttonColor;

        Callback fooEmpty   {[this](){}};

        ImVec2 WH  {100, 40};
        ImVec2 WHx2;

        void show()
        {
            if(!isOpen) return;

            auto& color = ImGui::GetStyle().Colors[ImGuiCol_WindowBg];
                  color = ImColor(35,35,35,190);

            ///---------------------------------------|
            /// Позиция и размер окна.                |
            ///---------------------------------------:
            /// TODO: Окно на разных компах должно соответствовать ....

            ///
            ImGui::SetNextWindowSize(size);
            ///
            ImGui::SetNextWindowPos (position);

            ///---------------------------------------|
            /// Окно <name>.                          |
            ///---------------------------------------:
            ImGui::Begin( name.data(), &isOpen, 0
                        | ImGuiWindowFlags_NoCollapse
                        | ImGuiWindowFlags_NoMove
                    /// | ImGuiWindowFlags_NoTitleBar
                        | ImGuiWindowFlags_NoScrollbar
                        | ImGuiWindowFlags_NoResize
                    /// | ImGuiWindowFlags_HorizontalScrollbar
                    /// | ImGuiWindowFlags_AlwaysVerticalScrollbar
                    /// | ImGuiWindowFlags_MenuBar
                    /// | ImGuiWindowFlags_NoBackground
                    /// | ImGuiWindowFlags_AlwaysAutoResize
            );

            ImGui::PushStyleColor(ImGuiCol_Button,       (ImVec4)colButtonB);
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered,(ImVec4)colButtonH);
            ImGui::PushStyleColor(ImGuiCol_ButtonActive ,(ImVec4)colButtonA);

            {   showButtonClose("Закрыть");
                ImGui::Text("%s", log.str().c_str());
                ImGui::DragInt ("isSeed##3a", getIsSeed());
            }

            ImGui::PopStyleColor();
            ImGui::PopStyleColor();
            ImGui::PopStyleColor();

            ImGui::End();

        //  ImGui::Begin("TEST");
        //  ImGui:(buttonTexture);
        //  ImGui::End();

            if(!isOpen)
            {   vsl::Sounds::p->play(0);
                uiTuneBase.doOpen();
            }
        }

        void setGeometry(ImVec2 sz, ImVec2 ps)
        {   UIBase::setGeometry(sz,        ps);
            WH   = {sz.x - 17.0f, sz.y / 20.f};
            WHx2 = {WH.x + WH.x, WH.y};
        }

        myl::SwitcherData<ImVec4, 2> ColorBLog
        {   ImVec4{0.2f, 0.7f, 0.2f, 1.0f},
            ImVec4{0.7f, 0.2f, 0.2f, 1.0f}
        };

        static ImTextureID convertSFMLTexture2Im(const sf::Texture& tx)
        {   return (ImTextureID)(size_t)tx.getNativeHandle();
        }

        sf::Texture buttonTexture;
        ImTextureID texId;
        void init()
        {   if (!buttonTexture.loadFromFile("res/img/button.png"))
            {   ASSERT(false)
            }
            texId = convertSFMLTexture2Im(buttonTexture);
        }

    private:

        void showButtonClose(const char* mess)
        {   if(ImGui::Button(mess, WH))
            {   /// snd1.play();
                /// fooEmpty ();
                    vsl::Sounds::p->play(0);
                    isOpen = false;
            }
        }

        int* getIsSeed();
    };


    ///------------------------------------------------------------------------|
    /// UITuneGamer черный вход на сервер.
    ///------------------------------------------------------------ UITuneGamer:
    struct  UITuneGamer   : UIBase
    {       UITuneGamer(UITuneBase&     uiTuneBase,
                        vsl  ::Config*         cfg,
                        model::UserInit4Model* userInit4Model)
                :   uiTuneBase    ( uiTuneBase )
                ,   cfg           ( cfg )
                ,   userInit4Model( userInit4Model )
            {
                name = "Игрок ...";

                ImGuiStyle&      style = ImGui::GetStyle();
                ColorBLog.m[0] = style.Colors[ImGuiCol_Button];

                init();

            /// (*this) << "Только в новой игре!";

                doClose();
            }

        UITuneBase&                uiTuneBase;
        vsl::Config*                      cfg;
        model::UserInit4Model* userInit4Model;

        //ImVec4 buttonColor;

        Callback fooEmpty   {[this](){}};

        ImVec2 WH  {100, 40};
        ImVec2 WHx2;

        std::string  strTest {"...пусто..."};

        void show()
        {
            std::array<model::Player, 3>& pl{userInit4Model->players};

            if(!isOpen) return;

            auto& color = ImGui::GetStyle().Colors[ImGuiCol_WindowBg];
                  color = ImColor(35,35,35,190);

            ///---------------------------------------|
            /// Позиция и размер окна.                |
            ///---------------------------------------:
            /// TODO: Окно на разных компах должно соответствовать ....

            ///
            ImGui::SetNextWindowSize(size);
            ///
            ImGui::SetNextWindowPos (position);

            ///---------------------------------------|
            /// Окно <name>.                          |
            ///---------------------------------------:
            ImGui::Begin( name.data(), &isOpen, 0
                        | ImGuiWindowFlags_NoCollapse
                        | ImGuiWindowFlags_NoMove
                    /// | ImGuiWindowFlags_NoTitleBar
                        | ImGuiWindowFlags_NoScrollbar
                        | ImGuiWindowFlags_NoResize
                    /// | ImGuiWindowFlags_HorizontalScrollbar
                    /// | ImGuiWindowFlags_AlwaysVerticalScrollbar
                    /// | ImGuiWindowFlags_MenuBar
                    /// | ImGuiWindowFlags_NoBackground
                    /// | ImGuiWindowFlags_AlwaysAutoResize
            );

            ImGui::PushStyleColor(ImGuiCol_Button,       (ImVec4)colButtonB);
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered,(ImVec4)colButtonH);
            ImGui::PushStyleColor(ImGuiCol_ButtonActive ,(ImVec4)colButtonA);

            {   showButtonClose("Закрыть");
                ImGui::Text("%s", "Кто в игре?");

        ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.8f, 0.5f, 0.2f, 1.0f));
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5.0f);

        const char* items[] = { "человек", "бот"};

    ImGui::BeginChild("Left Panel", ImVec2(150, 99), true);
        static int currItem0 = 1;
        if(ImGui::Combo("?##0", &currItem0, items, IM_ARRAYSIZE(items)))
        {   vsl::Sounds::p->play(0);
            //pl[0].isBot = currItem0;
        }
        
        static int currItem1 = 1;
        if(ImGui::Combo("?##1", &currItem1, items, IM_ARRAYSIZE(items)))
        {   vsl::Sounds::p->play(0);
            //pl[1].isBot = currItem1;
        }
        
        static int currItem2 = 1;
        if(ImGui::Combo("?##2", &currItem2, items, IM_ARRAYSIZE(items)))
        {   vsl::Sounds::p->play(0);
            //pl[2].isBot = currItem2;
        }
    ImGui::EndChild();
    ImGui::SameLine();
    ImGui::BeginChild("Right Panel", ImVec2(0, 99), true);
        ImGui::InputText("Игрок-1", &pl[0].nameInput, sizeof pl[0].nameInput);
        ImGui::InputText("Игрок-2", &pl[1].nameInput, sizeof pl[1].nameInput);
        ImGui::InputText("Игрок-3", &pl[2].nameInput, sizeof pl[2].nameInput);
    ImGui::EndChild();

    /// ImGui::DragInt ("...##3a", getIsSeed());
    /// ImGui::SameLine();

                if(ImGui::SliderFloat(  "Громкость музыки:", 
                    vsl::Musics::p->getPVol(), 0, 100, "%.f"))
                {   vsl::Musics::p->setVolume();
                }

                if(ImGui::SliderFloat(  "Громкость звуков:", 
                    vsl::Sounds::p->getPVol(), 0, 100, "%.f"))
                {   vsl::Sounds::p->play(0);
                }

                if(ImGui::Button("Test", WH))
                {   vsl::Musics::p->play(0);
                }
            }
        
        ImGui::PopStyleVar();
        ImGui::PopStyleColor();

            ImGui::PopStyleColor();
            ImGui::PopStyleColor();
            ImGui::PopStyleColor();

            ImGui::End();

        //  ImGui::Begin("TEST");
        //  ImGui:(buttonTexture);
        //  ImGui::End();

            if(!isOpen)
            {   vsl::Sounds::p->play(0);
                uiTuneBase.doOpen();
            }
        }

        void setGeometry(ImVec2 sz, ImVec2 ps)
        {   sz.x += 200;
            UIBase::setGeometry(sz,        ps);
            WH   = {sz.x - 17.0f, sz.y / 20.f};
            WHx2 = {WH.x + WH.x, WH.y};
        }

        myl::SwitcherData<ImVec4, 2> ColorBLog
        {   ImVec4{0.2f, 0.7f, 0.2f, 1.0f},
            ImVec4{0.7f, 0.2f, 0.2f, 1.0f}
        };

        static ImTextureID convertSFMLTexture2Im(const sf::Texture& tx)
        {   return (ImTextureID)(size_t)tx.getNativeHandle();
        }

        sf::Texture buttonTexture;
        ImTextureID texId;
        void init()
        {   if (!buttonTexture.loadFromFile("res/img/button.png"))
            {   ASSERT(false)
            }
            texId = convertSFMLTexture2Im(buttonTexture);
        }

    private:

        void showButtonClose(const char* mess)
        {   if(ImGui::Button(mess, WH))
            {   /// snd1.play();
                /// fooEmpty ();
                    vsl::Sounds::p->play(0);
                    isOpen = false;
            }
        }
    };
}


#endif // UI_TUNE_H
