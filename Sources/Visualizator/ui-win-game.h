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

                bool   ok = buffer.loadFromFile("res/snd/click-01.mp3");
                ASSERT(ok)

                ImGuiStyle&      style = ImGui::GetStyle();
                ColorBLog.m[0] = style.Colors[ImGuiCol_Button];
            }

        //ImVec4 buttonColor;

        Callback fooGo{[this](){}};

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

            /// ImGui::PushStyleColor(ImGuiCol_Button, ColorBLog.get());
                if(ImGui::Button("КУПИТЬ", WH))
                {   fooGo     ();
                    sound.play();
            ///     ColorBLog.next();
                }
            /// ImGui::PopStyleColor

                ImGui::SameLine();

                if(ImGui::Button("ДАЛЬШЕ", WH))
                {   fooGo     ();
                    sound.play();
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

}


#endif // UI_WIN_GAME_H
