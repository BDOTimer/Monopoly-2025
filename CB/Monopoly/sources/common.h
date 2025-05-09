#ifndef COMMON_H
#define COMMON_H
///----------------------------------------------------------------------------|
/// "common.h"
///----------------------------------------------------------------------------:
#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <map>

#include "debug.h"

///----------------------------------------------------------------------------|
/// Интерфейс объектов рендера.
///----------------------------------------------------------------------------:
namespace vsl
{
    ///-------------------------|
    /// Интерфейс объекта.      |--------------------------------------------!!!
    ///-------------------------:
    struct      IObject : sf::Drawable
    {   virtual~IObject(){}
        virtual void update   (                        ) = 0;
        virtual bool RPControl(std::string_view command,
                         const std::vector<int>&  args ) = 0;
        virtual void input(const sf::Event&       event) = 0;

        std::string_view name;

    private:
    };
}

namespace uii
{
    ///-------------------------|
    /// Интерфейс Gui.          |--------------------------------------------!!!
    ///-------------------------:
    struct      IGui
    {           IGui(){}
        virtual~IGui(){}

        virtual void show() = 0;

        std::string_view name;

    private:
    };
}

#define PLUG_IOBJECT ;\
    virtual void update   (                        ){}; \
    virtual bool RPControl(std::string_view command,    \
                     const std::vector<int>&   args)    \
    {   return true;                                 }  \
    virtual void input(const sf::Event&       event){};

namespace vsl
{
    struct  Config
    {       Config  ()
            {   init();
            }

        sf::Vector2u size;
        unsigned& SW{size.x};
        unsigned& SH{size.y};

        static sf::Font& getFont()
        {   static sf::Font font("consola.ttf");
            return font;
        }

        void init()
        {   sf::VideoMode dm = sf::VideoMode::getDesktopMode();
            size = dm.size;

            size.x = 0.8f * size.x;
            size.y = 0.8f * size.y;

            getFont().setSmooth(true);
        }

    private:

    }cfg;
}


///----------------------------------------------------------------------------|
/// Макет пакета данных для одного отображаемого объекта.
///----------------------------------------------------------------------- Data:
struct Data
{   std::string  filename;
    sf::Vector2f position;
    sf::Vector2f scale   ;
};


///----------------------------------------------------------------------------|
/// Чистые данные.
///--------------------------------------------------------------- Data4Sprites:
struct  Data4Sprites
{       Data4Sprites()
        {    loadFromFile("data4Sprites.txt");
        }

    static const std::vector<Data>& get()
    {   static Data4Sprites ds;
        return              ds.dats;
    }

private:
    const std::vector<Data> dats
    {
        {"res/logo.jpg" , {   0,   0 }, { 1   , 1    }},
        {"res/house.png", {  50, 300 }, { 0.4f, 0.4f }},
        {"res/house.png", { 450, 300 }, { 0.4f, 0.4f }},
        {"res/house.png", { 250, 350 }, { 0.4f, 0.4f }}
    };

    ///-----------------------------------|
    /// Фича: Загрузка данных из файла.   |
    ///-----------------------------------:
    void loadFromFile(const char* filename)
    {
        std::ifstream file(filename);
        if(file.is_open())
        {   auto& vd = const_cast<std::vector<Data>&>(dats);
                  vd.clear();

            std::string          s ; std::getline(file, s, '\f');
            std::stringstream ss(s);

            for(vd.emplace_back(Data()); ss >> vd.back().filename;
                vd.emplace_back(Data()))
            {
                if(! (ss >> vd.back().position.x)) break;
                if(! (ss >> vd.back().position.y)) break;
                if(! (ss >> vd.back().scale   .x)) break;
                if(! (ss >> vd.back().scale   .y)) break;
            }
            vd.pop_back( );
        }
    }
};


///----------------------------------------------------------------------------|
/// Коллекция текстур.
///-------------------------------------------------------------- HolderTexture:
struct  HolderTexture : private std::list<sf::Texture>
{
    ///-----------------------------------|
    /// Окошко раздачи текстур.           |
    ///-----------------------------------:
    static const sf::Texture& get(std::string_view filename)
    {
        static HolderTexture ht;

        if(const auto p = ht.m.find(filename); p != ht.m.end())
        {   return  *(p->second);
        }

        ht.emplace_back(sf::Texture());

        if(!ht.back().loadFromFile(filename))
        {   std::cout << "ERROR: loadFromFile: " << filename << '\n';
        }

        ht.m[filename] = &ht.back();
        return            ht.back();
    }

private:
    std::map<std::string_view, sf::Texture*> m;
};


///----------------------------------------------------------------------------|
/// Отображаемый одиночный объект.
///--------------------------------------------------------------------- Object:
struct  Object : vsl::IObject
{       Object(const Data& dat)
            :   sp    (HolderTexture::get(dat.filename))
            ,   nameTx(                   dat.filename)
        {
            sp.setPosition(dat.position);
            sp.setScale   (dat.scale   );
        }

    PLUG_IOBJECT

    ///-----------------------------------|
    /// Имя загруженной текстуры.         |
    ///-----------------------------------:
    sf::Sprite           sp;
    std::string_view nameTx;

    ///-----------------------------------|
    /// Дебаг.                            |
    ///-----------------------------------:
    void debug() const
    {
        l(nameTx)
        l(sp.getTexture().getSize().x)
        l(sp.getTexture().getSize().y)
    }

    ///------------------------------------|
    /// На рендер.                         |
    ///------------------------------------:
    virtual void draw(sf::RenderTarget& target,
                      sf::RenderStates  states) const
    {   target.draw(sp, states);
    }
};


///----------------------------------------------------------------------------|
/// Отображаемое множество графических объектов.
///-------------------------------------------------------------------- Objects:
struct  xObjects   : private std::vector<Object>, vsl::IObject
{       xObjects() : tmess1(vsl::Config::getFont())
        {
            const auto&         dats = Data4Sprites::get();
            reserve(            dats.size());
            for(const auto& d : dats) emplace_back(Object(d));

            tmess1.setString           (mess1);
            tmess1.setCharacterSize       (18);
            tmess1.setFillColor({128, 64, 32});
        }

    PLUG_IOBJECT

    std::string mess1{"PRESS ENTER ..."};
    sf::Text   tmess1;

    ///-----------------------------------|
    /// Дебаг.                            |
    ///-----------------------------------:
    void debug() const
    {
        l(size())
        if(empty()) return;
        front().debug();
    }

private:

    ///------------------------------------|
    /// На рендер.                         |
    ///------------------------------------:
    virtual void draw(sf::RenderTarget& target,
                      sf::RenderStates  states) const
    {
        for(const auto& sp : *this) target.draw(sp, states);
        target.draw(tmess1, states);
    }
};


///----------------------------------------------------------------------------|
/// Снипетc-процедуры...
///------------------------------------------------------------------------ Foo:
struct Foo
{
    static void findErrorSymbol(std::string_view fname)
    {   if(std::ifstream f(fname.data()); f.is_open())
        {   for(std::string s; std::getline(f, s); )
            {   for(size_t i = 0; i < s.size(); ++i)
                {   if(unsigned(s[i]) > 127)
                    {   std::cout << "findErrorSymbol(): '"
                                  << s[i]
                                  << "' position:" << i << '\n';
                    }
                }
            }
        }
        else std::cout << "ERROR: \"" << fname << "\" failed ...\n";
    }
};

namespace vsl
{
    struct  TextStyleA    : sf::Text
    {       TextStyleA()  : sf::Text(Config::getFont())
            {   setCharacterSize         (24);
                setFillColor({128, 196, 255});
            }
    };
}


#endif // COMMON_H
