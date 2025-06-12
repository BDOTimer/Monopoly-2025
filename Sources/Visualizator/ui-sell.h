#ifndef UI_SELL_H
#define UI_SELL_H
///----------------------------------------------------------------------------|
/// "ui-sell.h"
///----------------------------------------------------------------------------:
#include "ui-base.h"
#include "ui-icons.h"

namespace uii
{
    ///------------------------------------------------------------------------|
    /// UISellPanel базовое начальное меню ...
    /// -   объект существует в единственном экземпляре.
    /// -   объект находится в vsl::Config
    ///------------------------------------------------------------ UISellPanel:
    struct  UISellPanel : UIBase
    {       UISellPanel()
            {
                name = "ПРОДАТЬ ЯЧЕЙКУ";

                ImGuiStyle&      style = ImGui::GetStyle();
                ColorBLog.m[0] = style.Colors[ImGuiCol_Button];

                (*this) << uii::Clear() << "...(инфа)...\n";
            }

        //ImVec4 buttonColor;

        Callback fooSell {[this](){}};
        Callback fooNext {[this](){}};
        Callback fooClose{[this](){ this->doClose(); }};

        bool isBot{true};

        ImVec2 WH;

        UIGameIcons*   pUiGameIcons{nullptr};

        using idIter = std::map<unsigned, std::string>::iterator;
        idIter idNow ;

        void doOpen2()
        {   UIBase::doOpen();
        }

        void doOpen(std::map<unsigned, std::string>::iterator idNow)
        {   UIBase::doOpen();
            this->idNow = idNow;
        }

        idIter next()
        {   if(++idNow == pUiGameIcons->idCells.end  ())
            {    idNow =  pUiGameIcons->idCells.begin();
            }
            return idNow;
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
                if(ImGui::Button("ПРОДАТЬ", WH))
                {   fooSell   ();
                    vsl::Sounds::p->play(0);
            ///     ColorBLog.next();
                }
            /// ImGui::PopStyleColor

                ImGui::SameLine();

                if(ImGui::Button("СЛЕДУЮЩАЯ", WH))
                {   fooNext   ();
                    vsl::Sounds::p->play(0);
                }

                ImGui::SameLine();

                if(ImGui::Button("ЗАКРЫТЬ", WH))
                {   fooClose  ();
                    vsl::Sounds::p->play(0);
                }

            ImGui::End();
        }

        void setGeometry(ImVec2 sz, ImVec2 ps)
        {   UIBase::setGeometry(sz,        ps);
            WH = {size.x / 3.1f, size.y / 8.f};
        }

        myl::SwitcherData<ImVec4, 2> ColorBLog
        {   ImVec4{0.2f, 0.7f, 0.2f, 1.0f},
            ImVec4{0.7f, 0.2f, 0.2f, 1.0f}
        };

    private:

    };


    ///------------------------------------------------------------------------|
    /// SFML панель под ui.
    ///-------------------------------------------------------------- PanelSell:
    struct  xPanelSell : vsl::IObject
    {       xPanelSell ( )
            {
            }


        PLUG_IOBJECT

    private:

        ///------------------------------------|
        ///  На рендер.                         |
        ///------------------------------------:
        virtual void draw(sf::RenderTarget& target,
                          sf::RenderStates  states) const
        {
        /// auto p = const_cast<xPanelSell*>(this);
        /// target.draw(sp, states);
        }
    };
}


#endif // UI_SELL_H
