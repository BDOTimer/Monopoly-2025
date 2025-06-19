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
    {       UISellPanel(const vsl::HolderTextureFieldCash& holderTFieldCash)
                :
                    holderTFieldCash(holderTFieldCash)
            {
                name = "ПРОДАТЬ ЯЧЕЙКУ";

                ImGuiStyle&      style = ImGui::GetStyle();
                ColorBLog.m[0] = style.Colors[ImGuiCol_Button];

                (*this) << uii::Clear() << "...(инфа)...\n";
                isOpen  =  false;
            }

    /// ImVec4 buttonColor;

        const vsl::HolderTextureFieldCash& holderTFieldCash;

        UIGameIcons* pIcons;
        IconIt       idCell;

        std::function<void(unsigned/*idCell*/)> fooSell{[this](unsigned id){}};
        std::function<void(unsigned/*idCell*/)> fooNext{[this](unsigned id){}};

        bool isBot{true};

        ImVec2 WH;
        ImVec2 WHicon;

        ImVec2 WHPanelL;
        ImVec2 WHPanelR;

        void doOpen2()
        {   UIBase::doOpen();
        }

        void doOpen(IconIt idCell)
        {   UIBase::doOpen();
            this->idCell = idCell;
        }

        IconIt next()
        {   if(++idCell == pIcons->idCells.end  ())
            {    idCell =  pIcons->idCells.begin();
            }
            return idCell;
        }

        void xloadInfo()
        {   (*this) << uii::Clear() << "ЯЧЕЙКА: " << idCell->first;
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

        ///////////////////////////////////////////////////////////////////////:
        ImGui::BeginChild("Left Panel", WHPanelL, true);
        const auto id{idCell->first};
            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(8, 4));
            ImGui::PushStyleColor(
                        ImGuiCol_Button, uii::style::colBorderStatus[id % 3]);
            if (ImGui::ImageButton("idCellSell", getTexId(idCell->first), WHicon))
            {   vsl::Sounds::p->play(5);
            }
            ImGui::PopStyleColor();
            ImGui::PopStyleVar  ();
        ImGui::EndChild();
        ImGui::SameLine();
        ImGui::BeginChild("Right Panel", WHPanelR, true);

            ImGui::Text("%s", log.str().c_str());

        ImGui::EndChild();
        ///////////////////////////////////////////////////////////////////////.

                auto nt = isBot ? "...(bot)..." : "ПРОДАТЬ";

            /// ImGui::PushStyleColor(ImGuiCol_Button, ColorBLog.get());
                if(ImGui::Button(nt, WH))
                {
                    if(!isBot)
                    {
                        const unsigned id = idCell->first;
                        fooSell       (id);
                        soundClick    (  );
                ///     ColorBLog.next(  );

                        if(pIcons->idCells.empty())
                        {   doClose();
                        }
                    }
                    else vsl::Sounds::p->play(2);
                }
            /// ImGui::PopStyleColor

                ImGui::SameLine();

                if(ImGui::Button("СЛЕДУЮЩАЯ", WH))
                {   next      ();
                /// loadInfo  ();
                    soundClick();

                    fooNext(idCell->first);
                }

                ImGui::SameLine();

                if(ImGui::Button("ЗАКРЫТЬ", WH))
                {   doClose   ();
                    soundClick();
                }

            ImGui::End();
        }

        void setGeometry(ImVec2 sz, ImVec2 ps)
        {   UIBase::setGeometry(sz,        ps);

            WH       = { size.x / 3.2f, size.y / 7.9f };

            const auto H = (sz.y - WH.y) / 1.24f;

            WHPanelL = { H + 20, H  };
            WHPanelR = { 0     , H  };

            WHicon   = { H / 1.25f, H / 1.25f };
        }

        myl::SwitcherData<ImVec4, 2> ColorBLog
        {   ImVec4{0.2f, 0.7f, 0.2f, 1.0f},
            ImVec4{0.7f, 0.2f, 0.2f, 1.0f}
        };

    private:
        ImTextureID getTexId(unsigned id)
        {   const auto& texture = *holderTFieldCash.get(id);
            return UIBase::convertSFMLTexture2Im  (texture);
        }
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
