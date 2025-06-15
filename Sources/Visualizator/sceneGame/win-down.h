#ifndef WIN_DOWN_H
#define WIN_DOWN_H
///----------------------------------------------------------------------------|
/// "win-down.h"
///----------------------------------------------------------------------------:
#include "../common.h"


namespace vsl
{
    struct  WinDown : vsl::IObject
    {       WinDown(vsl::Config& cfg) : cfg(cfg)
            {
                const auto& rect    = cfg.markupSG.winDown;
                const auto& border  = cfg.markupSG.border ;
                const auto& border2 = border + border;

                const float x = rect.size.x * cfg.szfWin.x;
                const float y = rect.size.y * cfg.szfWin.y;

                cam.setViewport(rect);
                cam.setSize  ({x, y});
                cam.setCenter({0, 0});  camUI = cam;

                camUI.setCenter({x / 2, y / 2}); 

                fon.setSize  ({x-border2, y-border2});
              //fon.setPosition({border, border});
                pr::setOrigin(fon);
                fon.setFillColor({  0,0,200,40});
                fon.setOutlineColor({64,64,128});
                fon.setOutlineThickness (border);

                tmess1.setString(L"ИГРА.\nВыход: ESCAPE");
            }

        vsl::Config& cfg;

        PLUG_IOBJECT

    private:
        sf::View         camUI;
        sf::View           cam;
        sf::RectangleShape fon;
        pr::TextStyleA  tmess1;

        ///------------------------------------|
        /// На рендер.                         |
        ///------------------------------------:
        virtual void draw(sf::RenderTarget& target,
                          sf::RenderStates  states) const
        {
            target.setView(cam);
            target.draw   (fon, states);
        /// target.draw   (objectTest2, states);

            target.setView(camUI);
            target.draw   (tmess1, states);

            cfg.uiDownMessage.show();
        }
    };
}

#endif // WIN_DOWN_H
