#ifndef SCENE_TUNE_H
#define SCENE_TUNE_H
///----------------------------------------------------------------------------|
/// "scene-tune.h"
///----------------------------------------------------------------------------:
#include "common.h"

namespace vsl
{
    ///------------------------------------------------------------------------|
    /// SceneTune.
    ///-------------------------------------------------------------- SceneTune:
    struct  SceneTune   : vsl::IObject
    {       SceneTune()
                        :   nameTx(            "res/tune.jpg")
                        ,   sp    (HolderTexture::get(nameTx))
            {
                tmess1.setString(L"Настройки...");
            }

        PLUG_IOBJECT

        ///-----------------------------------|
        /// Имя загруженной текстуры.         |
        ///-----------------------------------:
        std::string nameTx;
        sf::Sprite      sp;
        TextStyleA  tmess1;

        ///-----------------------------------|
        /// Дебаг.                            |
        ///-----------------------------------:
        void debug() const
        {   l(nameTx)
            l(sp.getTexture().getSize().x)
            l(sp.getTexture().getSize().y)
        }

        ///------------------------------------|
        /// На рендер.                         |
        ///------------------------------------:
        virtual void draw(sf::RenderTarget& target,
                          sf::RenderStates  states) const
        {   target.draw(sp    , states);
            target.draw(tmess1, states);
        }
    };
}

#endif // SCENE_TUNE_H
