#ifndef UI_WIN_GAME_H
#define UI_WIN_GAME_H
///----------------------------------------------------------------------------|
/// "ui-win-game.h"
///----------------------------------------------------------------------------:
#include "ui-imgui.h"

namespace uii
{
    ///------------------------------------------------------------------------|
    /// UIWinGameCellInfo базовое начальное меню ...
    ///------------------------------------------------------ UIWinGameCellInfo:
    struct  UIWinGameCellInfo   : UIBase
    {       UIWinGameCellInfo()
            {
                name = "Инфа о ячейке";

                ImGuiStyle&      style = ImGui::GetStyle();
                ColorBLog.m[0] = style.Colors[ImGuiCol_Button];
            }

        //ImVec4 buttonColor;

        Callback fooBuy {[this](){}};
        Callback fooNext{[this](){}};

        bool isBot{true};

        ImVec2 WH;

        void show()
        {
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
            ImGui::Begin (name.data(), nullptr, 0
                    /// | ImGuiWindowFlags_NoCollapse
                        | ImGuiWindowFlags_NoMove
                        | ImGuiWindowFlags_NoTitleBar
                    /// | ImGuiWindowFlags_HorizontalScrollbar
                    /// | ImGuiWindowFlags_AlwaysVerticalScrollbar
                        | ImGuiWindowFlags_NoScrollbar
                    /// | ImGuiWindowFlags_MenuBar
                    /// | ImGuiWindowFlags_NoBackground
                        | ImGuiWindowFlags_NoResize
                    /// | ImGuiWindowFlags_AlwaysAutoResize
            );

                ImGui::Text("%s", log.str().c_str());

                auto nt = isBot ? "...(bot)..." : "КУПИТЬ";

            /// ImGui::PushStyleColor(ImGuiCol_Button, ColorBLog.get());
                if(ImGui::Button(nt, WH))
                {   fooBuy    ();
                    vsl::Sounds::p->play(0);
            ///     ColorBLog.next();
                }
            /// ImGui::PopStyleColor

                ImGui::SameLine();

                if(ImGui::Button("ДАЛЬШЕ", WH))
                {   fooNext   ();
                    vsl::Sounds::p->play(0);
            ///     ColorBLog.next();
                }

            ImGui::End();
        }

        void setGeometry(ImVec2 sz, ImVec2 ps)
        {   UIBase::setGeometry(sz,        ps);
            WH = {size.x / 2.1f, size.y / 8.f};
        }

        myl::SwitcherData<ImVec4, 2> ColorBLog
        {   ImVec4{0.2f, 0.7f, 0.2f, 1.0f},
            ImVec4{0.7f, 0.2f, 0.2f, 1.0f}
        };


    private:

    };


    ///------------------------------------------------------------------------|
    /// UIUpBankInfo.
    ///------------------------------------------------------------ UIUpBankInfo:
    struct  UIUpBankInfo    : UIBase
    {       UIUpBankInfo(const uii::UITest& uiInit, int* bankMoney)
                :   uiInit(uiInit)
                ,   bankMoney ( bankMoney)
            {
                name = "Банк";

                ImGuiStyle&      style = ImGui::GetStyle();
                ColorBLog.m[0] = style.Colors[ImGuiCol_Button];

                init();

                (*this) << strRules;

                isOpen = true;
            }

        //ImVec4 buttonColor;

        const uii::UITest& uiInit;

        Callback fooEmpty   {[this](){}};

        ImVec2   WH  {100, 40};
        ImVec2   WHx2;

        void doOpen()
        {   vsl::Musics::p->play(2);
            isOpen = true;
        }

        int* bankMoney;

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

            ImGui::PushStyleColor(ImGuiCol_Border, IM_COL32(  0, 127, 127, 180));
            ImGui::PushStyleVar  (ImGuiStyleVar_WindowBorderSize, 3.0f);

            ///---------------------------------------|
            /// Окно <name>.                          |
            ///---------------------------------------:
            ImGui::Begin( name.data(), &isOpen, 0
                        | ImGuiWindowFlags_NoCollapse
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

            {   ImGui::SetWindowFontScale(1.6f);

                const char* text = "  БАНК: ";
                // Получаем доступную ширину внутри окна
                auto st   = ImGui::CalcTextSize(text);
                auto sw   = ImGui::GetContentRegionAvail();
                     sw.x =(sw.x - st.x) * 0.5f;
                auto py   = sw.y * 0.5f - st.y * 0.25f;
                // Вычисляем отступ слева для центрирования
                //ImGui::SetCursorPosX(sx);
                ImGui::SetCursorPosY(py);

                ImGui::PushFont(uiInit.second_font);
                ImGui::Text("%s", text);
                

                ImGui::SameLine();
            /// ImGui::SetCursorPosY(py);

                ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "%d", *bankMoney);

                ImGui::SetWindowFontScale(1.0f);

                ImGui::PopFont();
            }

            ImGui::PopStyleColor();
            ImGui::PopStyleColor();
            ImGui::PopStyleColor();

            ImGui::End();

            ImGui::PopStyleVar  ();
            ImGui::PopStyleColor();

        //  ImGui::Begin("TEST");
        //  ImGui:(buttonTexture);
        //  ImGui::End();
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
    };

}


#endif // UI_WIN_GAME_H
