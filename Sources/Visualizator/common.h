#ifndef COMMON_H
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

    struct  Sound
    {       Sound(std::string_view fn) : snd(buf)
            {   bool   ok = buf.loadFromFile(fn.data());
                ASSERT(ok)
            }

        void              play()     {        snd.play     (); }
        sf::Sound::Status getStatus(){ return snd.getStatus(); }

    private:
        sf::SoundBuffer buf;
        sf::Sound       snd;
    };


    struct  Music : sf ::Music
    {       Music ( std::string_view name)
            {
                if (!openFromFile( name.data()))
                {   std::cout << "ERROR: \"" << name << "\"";
                }
            }

    private:
    };

    ///------------------------------------------------------------------------|
    /// Мюзикс-контрол.
    /// vsl::Musics::p->play(3);
    ///----------------------------------------------------------------- Musics:
    #define MAY(a) if(a == nullptr) return
    struct  Musics   : std::vector<sf::Music*>
    {       Musics() : std::vector<sf::Music*>(fn.size(), nullptr)
            {
            }

        enum eMusic
        {    E_Acid,
             E_Life,
             E_musicRule,
             E_
        };

        std::array<const char*, 4> fn
        {   "res/snd/gaming.mp3",    /// 0
            "res/snd/logo.mp3",      /// 1
            "res/snd/musicRule.mp3", /// 2
            "res/muz/kirk.mp3"       /// 3
        };

        void play(unsigned i)
        {   if(i >= fn.size()) return;
            else if( auto& p = (*this)[i]; p != nullptr )
            {
                if( pnow              != nullptr &&
                    pnow->getStatus() == sf::SoundSource::Status::Playing &&
                    pnow              == p)
                {   pause();
                }
                else
                {   //stop();
                   (pnow = p)->play();
                    pnow ->setVolume (volume);
                }
            }
            else
            {   stop();
                m.emplace_back( sf::Music(fn[i]) );
                (*this)[i] = &m.back();

                (pnow = &m.back())->play();
                (p = pnow)->setVolume(volume);
            }
        }

        void stop     (          ){ MAY(pnow); pnow->stop     (      ); }
        void pause    (          ){ MAY(pnow); pnow->pause    (      ); }
        void setVolume(          ){ MAY(pnow); pnow->setVolume(volume); }
        void setVolume(float  val)
        {   if(0.f   > val || val > 100.f) return;
            volume   = val ;
            MAY(pnow); pnow->setVolume(val);
        }
        sf::SoundSource::Status     getStatus() const
        {   if    (pnow == nullptr) return sf::SoundSource::Status::Stopped;
            return pnow->getStatus();
        }

        bool isPlaying() const
        {   return  pnow != nullptr
                &&  pnow->getStatus() == sf::SoundSource::Status::Playing;
        }

        float* getPVol() { return &volume; }

        static inline Musics* p{nullptr};

    private:
        std::list<sf ::Music>           m;
        sf ::Music*         pnow{nullptr};
        float volume                 {60};
    };


    ///------------------------------------------------------------------------|
    /// Саунд-контрол.
    /// vsl::Sounds::p->play(5);
    ///----------------------------------------------------------------- Sounds:
    struct  Sounds   : std::vector<sf::Sound*>
    {       Sounds() : std::vector<sf::Sound*>(60, nullptr)
            {
            }

        enum eMusic
        {    E_click_01
        };

        std::array<const char*, 6> fn
        {   "res/snd/click-01.mp3",      // 0
            "res/snd/gudok-doplera.mp3", // 1
            "res/snd/no.mp3",            // 2
            "res/snd/dice1.mp3",         // 3 - бросок кубика.
            "res/snd/field-fly.mp3",     // 4 - клик поле.
            "res/snd/tum.mp3"            // 5 - клик иконка.
        };

        //Sound mp3no   {"res/snd/no.mp3"};
        //Sound mp3dice1{"res/snd/dice1.mp3"};


        void play(unsigned i)
        {   if(i >= fn.size())  return;
            if(auto& e =  (*this )[i]; e == nullptr)
            {
                b.emplace_back(sf::SoundBuffer(fn[i]));
                s.emplace_back(sf::Sound   (b.back()));
                s.back().setVolume(volume);

                e = &s.back();
                e ->   play();
            }
            else
            {   e->setVolume(volume);
                e->play     ();
            }
        }

        void setVolume(float  val)
        {   if(0.f   > val || val > 100.f) return;
            volume   = val ;
        }

        float* getPVol() { return &volume; }

        static inline Sounds*  p{nullptr};

    private:
        std::list<sf::SoundBuffer>      b;
        std::list<sf::Sound>            s;
        float volume                 {60};
    };
    #undef MAY


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

    struct  ScenesSwitcher
    {       ScenesSwitcher()
            {
            }

        enum eSCENE
        {   E_LOGO,
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
        {   ASSERT(scenes != nullptr)
            nowScene = (*scenes)[id];
            nScene   = id;
        }

        void next()
        {   ASSERT(scenes != nullptr)
            nScene =  (nScene + 1) % scenes->size();
            nowScene = (*scenes)[nScene];
        }

        void init(ScenesAll* sns)
        {   scenes = sns;
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
            sp.setOrigin({(float)sp.getTexture().getSize().x/2,
                          (float)sp.getTexture().getSize().y/2});
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

namespace primitive
{
    template<typename T>
    static void setOrigin(T& o)
    {   o.setOrigin({ o.getSize().x / 2, o.getSize().y / 2 });
    }

    sf::Vector2f cv(const sf::Vector2u u){return {(float)   u.x,(float)  u.y}; }
    sf::Vector2u cv(const sf::Vector2f f){return {(unsigned)f.x,(unsigned)f.y};}

    struct  TextStyleA    : sf::Text
    {       TextStyleA()  : sf::Text(vsl::Config::getFont())
            {   setCharacterSize         (18);
                setFillColor({127, 196, 127});
            }
    };

    struct  FigRectTest : sf::RectangleShape
    {       FigRectTest ()
            {       init();
            }

    protected:
        sf::Color colBorder{  0,127,   0     };
        sf::Color colFon1  {  0,  0, 128,    };
        sf::Color colFon2  {  0,  0, 128, 254};

        void init()
        {   setSize        ({500, 500});
            setPosition    ({  0,   0});
            setFillColor      (colFon2);
            setOutlineColor (colBorder);
            setOutlineThickness    (10);
            primitive::setOrigin(*this);
        }
    };

    struct InsexCircle
    {
        unsigned n;
        unsigned i{};

        void operator++()
        {   ++i; if(i >= n) i = 0;
        }
        void operator--()
        {   --i; if(i >= n) i = n - 1;
        }

        unsigned operator()() const { return i; }
    };
}
namespace pr = primitive;


#endif // COMMON_H
