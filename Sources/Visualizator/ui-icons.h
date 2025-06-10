#ifndef UI_ICONS_H
#define UI_ICONS_H
///----------------------------------------------------------------------------|
/// "ui-icons.h"
///----------------------------------------------------------------------------:
#include "ui-base.h"

namespace vsl
{

    struct  FigureField;
    struct  HolderTextureFieldCash   : std::vector<const sf::Texture*>
    {       HolderTextureFieldCash()
            {
            }

        const sf::Texture* get(unsigned id) const { return (*this)[id]; }

        void init(const vsl::FigureField& ff); /// "fig-field.h"
    };
}

namespace uii
{
    ///------------------------------------------------------------------------|
    /// UIGameIcons.
    ///------------------------------------------------------------ UIGameIcons:
    struct  UIGameIcons :    UIBase
    {       UIGameIcons(const vsl::HolderTextureFieldCash& holderTFieldCash)
                        :          holderTFieldCash       (holderTFieldCash)
            {
                name = "Купленные иконки";

                //bool   ok = buffer.loadFromFile("res/snd/click-01.mp3");
                //ASSERT(ok)

                (*this) << "...";
            }

        const vsl::HolderTextureFieldCash& holderTFieldCash;

        std::set<unsigned> idCells;

        void insert(unsigned id){ idCells.insert(id); }
        void erase (unsigned id)
        {   if (const auto&   f = idCells.find(id); f != idCells.end())
            {   idCells.erase(f);
            }
            else ASSERT(false)
        }

        void clear ()
        {   doClose();
            *this << Clear();
        }

        inline void reStart()
        {   idCells.clear  ();
        }

        void subShow()
        {
            const ImVec2 WH{30, 30};

            ///----------------------------------------------------------------|
            if (ImGui::CollapsingHeader("СОБСТВЕННОСТЬ:",
                                             ImGuiTreeNodeFlags_DefaultOpen))
            {   ImGui::Text("%s", log.str().c_str());


                for(const auto& id : idCells)
                {
                    if (ImGui::ImageButton("...", getTexId(id), WH))
                    {   vsl::Sounds::p->play(1);

                    }
                    ImGui::SameLine();
                }
            }
        }

    private:
        sf::Texture buttonTexture;
        ImTextureID texId;
        void init()
        {   if (!buttonTexture.loadFromFile("res/img/button.png"))
            {   ASSERT(false)
            }
            texId = UIBase::convertSFMLTexture2Im(buttonTexture);
        }

        ImTextureID getTexId(unsigned id)
        {   const auto& texture = *holderTFieldCash.get(id);
            return UIBase::convertSFMLTexture2Im  (texture);
        }
    };
}


#endif // UI_ICONS_H
