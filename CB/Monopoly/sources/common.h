﻿#ifndef COMMON_H
#define COMMON_H
///----------------------------------------------------------------------------|
/// "common.h"
///----------------------------------------------------------------------------:
#include <SFML/System/Angle.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <map>

#include "debug.h"

///---------|
/// my lib  |
///---------:
namespace myl
{
    template<typename T, unsigned N>
    struct SwitcherData
    {
        const T& get () const { return m[n]; }
        const T& next()       { return m[n = (n +  1) %  N               ]; }
        const T& back()       { return m[n = (n == 0) ? (N - 1) : (n - 1)]; }

        std::array<T, N> m;
        unsigned         n{};
    };
}

#define ISKEYPRESSED(a) sf::Keyboard::isKeyPressed(sf::Keyboard::Key::a)

template<typename T>
std::ostream& operator<<(std::ostream& o, const sf::Vector2<T>& a)
{          o << "{ " << a.x << ", " << a.y << " }";
    return o;
}

///----------------------------------------------------------------------------|
/// Интерфейс объектов рендера.
///----------------------------------------------------------------------------:
namespace vsl
{

	struct	Music : sf::Music
	{		Music(std::string_view name)
			{	if (!openFromFile( name.data()))
				{	std::cout << "ERROR: \"" << name << "\"";
				}
			}

		/// "res/snd/Maddix - Acid Soul.mp3"
		/// "res/snd/Maddix - Receive Life.mp3"
	};

    ///-------------------------|
    /// Интерфейс объекта.      |--------------------------------------------!!!
    ///-------------------------:
    struct      IObject : sf::Drawable
    {   virtual~IObject(){}
        virtual void update   (                        ) = 0;
        virtual bool RPControl(std::string_view command,
                         const std::vector<int>&  args ) = 0;
        virtual void input(const std::optional<sf::Event>& event) = 0;

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
    virtual void input(const std::optional<sf::Event>&       event){};

#define PLUG_IOBJECT2 ;\
    virtual void update   (                        ){}; \
    virtual bool RPControl(std::string_view command,    \
                     const std::vector<int>&   args)    \
    {   return true;                                 }

namespace vsl
{
	using ScenesAll = std::array<vsl::IObject*, 3>;

	struct	ScenesSwitcher
	{		ScenesSwitcher()
			{
			}

		enum eSCENE
		{	E_LOGO,
			E_TUNE,
			E_GAME
		};

		///---------------------|
		/// Номер сцены.        |
		///---------------------:
		unsigned      nScene   {E_LOGO};
		vsl::IObject* nowScene{nullptr};
		ScenesAll*    scenes;

		void doSwitcher(eSCENE id = E_TUNE)
		{	ASSERT(scenes != nullptr)
		    nowScene = (*scenes)[id];
			nScene   = id;
		}

		void next()
		{	ASSERT(scenes != nullptr)
		    nScene =  (nScene + 1) % scenes->size();
			nowScene = (*scenes)[nScene];
		}

		void init(ScenesAll* sns)
		{	scenes = sns;
			doSwitcher(E_LOGO);
		}
	};
}


///----------------------------------------------------------------------------|
/// "config-vsl.h"
///------------------------------------------------------------- "config-vsl.h":
#include "config-vsl.h"


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
        {"res/house.png", {-200, -200}, { 0.4f, 0.4f }},
        {"res/money.png", {-100, -100}, { 0.4f, 0.4f }},
        {"res/money.png", {-100, -250}, { 0.4f, 0.4f }}
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
    static const sf::Texture& get(std::string filename)
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
    std::map<std::string, sf::Texture*> m;
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

///----------------------------------------------------------------------------|
/// Тестовый одиночный объект.
///--------------------------------------------------------------------- Object:
struct  ObjectTest : vsl::IObject
{       ObjectTest(const Data& dat, float speed)
            :   sp    (HolderTexture::get(dat.filename))
            ,   nameTx(                   dat.filename)
            ,   speed (speed)
        {
            sp.setPosition(dat.position);
            sp.setScale   (dat.scale   );
        }

    PLUG_IOBJECT

    void update(float dt)
    {   a = speed * dt;
    }

    ///-----------------------------------|
    /// Имя загруженной текстуры.         |
    ///-----------------------------------:
    sf::Sprite           sp;
    std::string_view nameTx;
    float             speed;
    float                 a;

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
    {   auto p = const_cast<ObjectTest*>(this);
        target.draw(sp, states);

        p->sp.rotate(sf::degrees(a));
    }
}

objectTest4({"res/money.png", {0, 0}, { 0.9f, 0.9f }}, -60.f);

namespace vsl
{
    struct  TextStyleA    : sf::Text
    {       TextStyleA()  : sf::Text(Config::getFont())
            {   setCharacterSize         (18);
                setFillColor({127, 196, 127});
            }
    };
}


#endif // COMMON_H
