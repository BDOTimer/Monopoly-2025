#ifndef UII_BASE_H
#define UII_BASE_H
///----------------------------------------------------------------------------|
/// "ui-base.h"
///----------------------------------------------------------------------------:
#include <SFML/Audio/Sound.hpp>
#include "imgui.h"
#include "imgui-SFML.h"
#include "misc/cpp/imgui_stdlib.h"

#include "common.h"

namespace uii
{
    using Callback = std::function<void()>;

    constexpr ImColor colButtonA{ 6,15,14,120}; /// Нажата.
    constexpr ImColor colButtonB{ 6,35,34,170}; /// Спокойная.
    constexpr ImColor colButtonH{26,55,54,170}; /// Наведенная.

    struct ImGuiDemoWindowData
    {   bool MainMenuBar = false;
        bool Help        = false;
        bool About       = false;
        bool Demo        = false;
        bool Log         = false;
    };

    struct TextField
    {   std::string_view name;
        std::string*        s;

        void bind(std::string_view nm, std::string* str)
        {   name = nm ;
            s    = str;
        }

        void prn() const
        {   ImGui::Text("%s: %s", name.data(), s->c_str());
        }
    };

    struct Clear {};


    ///------------------------------------------------------------------------|
    /// MyHover.
    ///---------------------------------------------------------------- MyHover:
    struct  MyHover
    {       MyHover()
                :   txtr1("res/sys/cursor32-1.png")
                ,   txtr2("res/sys/cursor32-2.png")
                ,   cursor(txtr1)
            {   
                win::noShowCursor();
            }

        void reset()
        {   cursor.setTexture(txtr1);
        }

        void test()
        {   if (ImGui::IsItemHovered())
            {   cursor.setTexture(txtr2);
            }
        }

    //private:
        const sf::Texture txtr1;
        const sf::Texture txtr2;
        sf::Sprite       cursor;
    };


    struct  UIDownMessage;
    ///------------------------------------------------------------------------|
    /// UIBase.
    ///----------------------------------------------------------------- UIBase:
    ///static bool autoScroll{false};
    struct  UIBase
    {       UIBase() //(vsl::Config cfg) : cfg(cfg)
                :   soundx(buffer)
            {

            }

    // vsl::Config& cfg;

        Callback       fooHover{[](){}};

        sf::SoundBuffer  buffer;
        sf::Sound        soundx;

        bool autoScroll {false};

        std::stringstream   log;

        UIDownMessage* messDown{nullptr};

        bool isOpen    { true };
        void doClose() { isOpen = false; }
        void doOpen () { isOpen = true ; }

        void doFooEmpty();
        std::function<void()> fooEmpty
        {   [this](){ this->doFooEmpty(); }
        };

        UIBase& operator<<(std::string_view s)
        {   log << s;
            autoScroll = true;
            return *this;
        }

        UIBase& operator<<(const char c)
        {   log << std::string(1, c);
            return *this;
        }

        UIBase& operator<<(const int n)
        {   log << std::to_string   (n);
            return *this;
        }

        UIBase& operator<<(const unsigned n)
        {   log << std::to_string        (n);
            return *this;
        }

        UIBase& operator<<(const Clear)
        {   log.str("");
            return *this;
        }

        void show()
        {
        }

        void setGeometry(ImVec2 sz, ImVec2 ps)
        {   size     = sz;
            position = ps;
        }

        ImVec2 size    {200,200};
        ImVec2 position{  0,  0};

        static ImTextureID convertSFMLTexture2Im(const sf::Texture& tx)
        {   return (ImTextureID)(size_t)tx.getNativeHandle();
        }

        void soundClick(){ vsl::Sounds::p->play(0); }

    protected:
        std::string_view name;
    };
}


#endif // UII_BASE_H
