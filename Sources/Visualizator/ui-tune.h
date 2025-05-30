#ifndef UI_TUNE_H
#define UI_TUNE_H
///----------------------------------------------------------------------------|
/// "ui-tune.h"
///----------------------------------------------------------------------------:
#include "ui-imgui.h"

namespace uii
{
    ///------------------------------------------------------------------------|
    /// UITuneBase базовое начальное меню ...
    ///------------------------------------------------------------- UITuneBase:
    struct  UITuneBase   : UIBase
    {       UITuneBase() : snd1(buf1)
            {
                name = "Настройки";

                bool   ok = buffer.loadFromFile("res/snd/click-01.mp3");
                ASSERT(ok)

                       ok = buf1.loadFromFile("res/snd/gudok-doplera.mp3");
                ASSERT(ok)

                ImGuiStyle&      style = ImGui::GetStyle();
                ColorBLog.m[0] = style.Colors[ImGuiCol_Button];
            }

        //ImVec4 buttonColor;

        sf::SoundBuffer  buf1;
        sf::Sound        snd1;

        Callback fooRestart   {[this](){}};
        Callback fooContinue  {[this](){}};
        Callback fooTuneTester{[this](){}};
        Callback fooTuneGamer {[this](){}};
        Callback fooExit      {[this](){}};
        Callback fooTest      {[this](){}};

        ImVec2 WH  {100, 40};
        ImVec2 WHx2;

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


                if(ImGui::Button("НОВАЯ ИГРА", WH))
                {   snd1.play ();
                    fooRestart();
                }

                if(ImGui::Button("ПРОДОЛЖИТЬ", WH))
                {   fooContinue();
                    sound.play ();
                }

                if(ImGui::Button("НАСТРОЙКИ ТЕСТИРОВЩИКА", WH))
                {   fooTuneTester();
                    sound.play   ();
                }

                if(ImGui::Button("НАСТРОйКИ ИГРОКА", WH))
                {   fooTuneGamer();
                    sound.play  ();
                }

            /// ImGui::PushStyleColor(ImGuiCol_Button, ColorBLog.get());
                if(ImGui::Button("ПРАВИЛА ИГРЫ", WH))
                {   fooTest   ();
                    sound.play();
                    ColorBLog.next();
                }
            /// ImGui::PopStyleColor();

                if(ImGui::Button("ВЫХОД", WH))
                {   fooExit   ();
                    sound.play();
                }

            ImGui::End();
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


    private:

    };

}


#endif // UI_TUNE_H
