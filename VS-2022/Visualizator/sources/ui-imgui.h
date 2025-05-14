#ifndef UI_IMGUI_H
#define UI_IMGUI_H
/// "ui-imgui.h"
///----------------------------------------------------------------------------|
/// ...
///----------------------------------------------------------------------------:

#include "imgui.h"
#include "imgui-SFML.h"
#include "misc/cpp/imgui_stdlib.h"

//#include "myl.h"

///---------|
/// ImGui   |
///---------:
namespace uii
{
    using Callback = std::function<void()>;

    struct ImGuiDemoWindowData
    {   bool MainMenuBar = false;
        bool Help        = false;
        bool About       = false;
        bool Demo        = false;
        bool Log         = false;
    };

    struct  TextField
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
    /// UI
    ///--------------------------------------------------------------------- UI:
    struct  UITest
    {       UITest() = delete;
            UITest(sf::RenderWindow& w) : window(w)
            {   initImgui();
                log.reserve(0xFFFFFF);

                callbacks.reserve(10);
            }


        Callback foo;

        void pushCallback(const std::pair<const char*, Callback> cb)
        {   callbacks.emplace_back(cb);
        }

        void add(const TextField& tf)
        {   textFields.push_back (tf);
        }

        void show()
        {
            auto& color = ImGui::GetStyle().Colors[ImGuiCol_WindowBg];

            color = ImColor(0.f, 0.1f, 0.f, 255.0f);
            if (isShow.About) showAbout();

            color = ImColor(0.f, 0.f, 0.4f, 0.8f);
            if (isShow.Log  ) showLog           (log);
            if (isShow.Demo ) ImGui::ShowDemoWindow();

            color = ImColor(35,35,35,190);
            showMain();
        }

        inline static bool autoScroll{false};

        UITest& operator<<(const char c)
        {   log += std::string(1, c);
            return *this;
        }

        UITest& operator<<(const int n)
        {   log += std::to_string   (n);
            return *this;
        }

        UITest& operator<<(const unsigned n)
        {   log += std::to_string        (n);
            return *this;
        }

        UITest& operator<<(std::string_view s)
        {   log += s;
            autoScroll = true;
            return *this;
        }

        UITest& operator<<(const Clear)
        {   log.clear();
            return *this;
        }

        bool isAnyFocused{false};

    private:
        sf::RenderWindow&    window       ;
        std::list<TextField> textFields   ;

        std::string  str {"...пусто..."};
        std::string  log ;
        std::string  help{"KEYBOARD:\n"
                          "  ...\n "};

        float f{};

        ImGuiDemoWindowData isShow;

        ///--------------------------------------|
        /// initImgui                            |
        ///--------------------------------------:
        void initImgui()
        {   bool
            isGood = ImGui::SFML::Init(window);
            if (!isGood)
            {   ASSERTM(false, "ImGui::SFML::Init() is failed ...")
            }

			const char* fontname = "res/JetBrainsMono-Regular.ttf";

            ImGuiIO& io = ImGui::GetIO();

            io.Fonts->Clear();
            isGood = io.Fonts->AddFontFromFileTTF(fontname, 18.f, NULL,
                     io.Fonts->GetGlyphRangesCyrillic());
            if (!isGood)
            {   ASSERTM(false, "io.Fonts->AddFontFromFileTTF() is failed ...")
            }

            isGood = ImGui::SFML::UpdateFontTexture();
            if (!isGood)
            {   ASSERTM(false, "ImGui::SFML::UpdateFontTexture()is failed ...")
            }
        }

        ///--------------------------------------|
        /// Hello, Informer!                     |
        ///--------------------------------------:
        void showMain()
        {
        /// ImGui::SetNextWindowSize({500,500});
            ImGui::Begin ("По идее тут должен быть конфиг!", nullptr, 0
                      ///   ImGuiWindowFlags_NoCollapse
                          | ImGuiWindowFlags_HorizontalScrollbar
                          | ImGuiWindowFlags_AlwaysVerticalScrollbar
                          | ImGuiWindowFlags_MenuBar
                      /// | ImGuiWindowFlags_NoBackground
                      /// | ImGuiWindowFlags_NoResize
                      /// | ImGuiWindowFlags_AlwaysAutoResize
            );

            isAnyFocused = ImGui::IsAnyItemActive();
            ///          = ImGui::IsWindowFocused();

            InformerMenuBar(isShow);

            ///-------------------------------|
            /// Батоны.                       |
            ///-------------------------------:
            const unsigned W = 80;
            const unsigned H = 30;

            if(ImGui::Button("Demo", {W, H}))
            {   isShow.Demo = !isShow.Demo;
            }

            ImGui::SameLine ();

            if(ImGui::Button("Log", {W, H}))
            {   isShow.Log = !isShow.Log;
            }

            ImGui::SameLine ();

            if(ImGui::Button("Exit", {W, H}))
            {   window.close();
            }

            ImGui::SameLine ();

            if(ImGui::Button("Test", {W, H}))
            {   foo();
            }

            controlButton();

            ///-------------------------------|
            /// Инфо о внешних полях.         |
            ///-------------------------------:
            for(const auto& t : textFields) t.prn();

            ImGui::Text ("isAnyFocused : %d", isAnyFocused);////////////////////

            if (isShow.Help && ImGui::CollapsingHeader("Help..."))
            {   ImGui::Text("%s", help.c_str());
            }

            ImGui::InputText("InputText", &str, sizeof str);

            ImGui::DragFloat("float##3a", &f);

            if (ImGui::CollapsingHeader("Лог игры... (СДЕЛАТЬ ХОД: ENTER)",
                                        ImGuiTreeNodeFlags_DefaultOpen))
            {   ImGui::Text("%s", log .c_str());
            /// ImGui::SetScrollHereY(1.0f);

                if (autoScroll)
                {   ImGui::SetScrollHereY(1.0f);
                    autoScroll = false;
                }
            }

            //ImGui::PushItemWidth(10);
            //ImGui::Text("%s", igHelp.c_str());
            //ImGui::PopItemWidth();

            ImGui::End();

            /*
            ImGui::SetNextWindowSize({50,50});
            ImGui::BeginTooltip            ();
            ImGui::Text("%s", igHelp.c_str());
            ImGui::EndTooltip              ();
            */
        }

        ///--------------------------------------|
        /// initImgui                            |
        ///--------------------------------------:
        static void InformerMenuBar(ImGuiDemoWindowData& isShow)
        {   if (ImGui::BeginMenuBar())
            {   if (ImGui::BeginMenu("Menu"))
                {   ImGui::MenuItem("Item1", NULL, &isShow.MainMenuBar);
                    ImGui::MenuItem("Item2", NULL, &isShow.MainMenuBar);
                    ImGui::SeparatorText("-----");
                    ImGui::MenuItem("Item3", NULL, &isShow.MainMenuBar);
                    ImGui::MenuItem("Item4", NULL, &isShow.MainMenuBar);
                    ImGui::EndMenu();
                }

                if (ImGui::BeginMenu("Tools"))
                {   ImGui::MenuItem("Item5", NULL, &isShow.MainMenuBar);
                    ImGui::MenuItem("Item6", NULL, &isShow.MainMenuBar);
                    ImGui::EndMenu();
                }

                if (ImGui::BeginMenu("?"))
                {   ImGui::MenuItem("Help" , NULL, &isShow.Help );
                    ImGui::MenuItem("About", NULL, &isShow.About);
                    ImGui::EndMenu();
                }

                ImGui::EndMenuBar();
            }
        }

        ///--------------------------------------|
        /// showAbout.                           |
        ///--------------------------------------:
        void showAbout()
        {
            const char* const about
            {   "\n"
                "\tТема  : Монополия-2025\n"
                "\tФорум : www.gamedev.ru\n"
                "\tАвторы: slava-rusi11, xlat-code\n"
            };

            ImGui::Begin("About", &isShow.About, ImGuiWindowFlags_NoCollapse);
            ImGui::Text ("%s", about);
            ImGui::End  ();
        }

        ///--------------------------------------|
        /// showLog.                             |
        ///--------------------------------------:
        void showLog(std::string_view str)
        {
            ImGui::Begin("Log", &isShow.Log, 0
            ///     ImGuiWindowFlags_NoCollapse
            /// |   ImGuiWindowFlags_NoBackground
                |   ImGuiWindowFlags_NoResize
            );
            ImGui::Text("%s", str.data());
            ImGui::End ();
        }

        std::vector<std::pair<const char*, Callback>> callbacks;

        ///--------------------------------------|
        /// controlButton.                       |
        ///--------------------------------------:
        void controlButton()
        {
            const unsigned W = 60;
            const unsigned H = 25;

            #define SHOWBUTTON if(ImGui::Button(it->first, {W,H})) it->second();

            ImGui::SeparatorText("\nControl Buttons:");

            if(callbacks.empty()) return;

            auto it = callbacks.cbegin();

            for(const auto E = it + 3; it != E; ++it )
            {   SHOWBUTTON ImGui::SameLine ();
            }   SHOWBUTTON

            for(const auto E = callbacks.cend() - 1; ++it != E;)
            {   SHOWBUTTON ImGui::SameLine ();
            }   SHOWBUTTON

            ImGui::SeparatorText("...");

            #undef SHOWBUTTON
        }
    };
};

#endif // UI_IMGUI_H
