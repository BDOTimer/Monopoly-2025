///----------------------------------------------------------------------------|
/// C++20
/// ...
///-------------------------------------------------------------- sfml-Bogradan:
#include <SFML/Graphics.hpp>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <list>
#include <map>

#define  l(v)  std::cout << #v << " = " << (v) << std::endl;

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

    static const std::list<Data>& get()
    {   static Data4Sprites ds;
        return              ds.dats;
    }

private:
    const std::list<Data> dats
    {
        {"res/wallpaper.jpg", {   0,   0 }, { 1   , 1    }},
        {"res/house.png"    , {  50, 300 }, { 0.4f, 0.4f }},
        {"res/house.png"    , { 450, 300 }, { 0.4f, 0.4f }},
        {"res/house.png"    , { 250, 350 }, { 0.4f, 0.4f }}
    };

    ///-----------------------------------|
    /// Фича: Загрузка данных из файла.   |
    ///-----------------------------------:
    void loadFromFile(const char* filename)
    {
        std::ifstream file(filename);
        if(file.is_open())
        {   auto& vd = const_cast<std::list<Data>&>(dats);
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
struct  Object : sf::Sprite
{       Object(const Data& dat)
            :   sf::Sprite(HolderTexture::get(dat.filename))
            ,   nameTx    (                   dat.filename)
        {
            setPosition(dat.position);
            setScale   (dat.scale   );
        }

    ///-----------------------------------|
    /// Имя загруженной текстуры.         |
    ///-----------------------------------:
    std::string_view nameTx;

    ///-----------------------------------|
    /// Дебаг.                            |
    ///-----------------------------------:
    void debug() const
    {
        l(nameTx)
        l(getTexture().getSize().x)
        l(getTexture().getSize().y)
    }
};


///----------------------------------------------------------------------------|
/// Отображаемое множество графических объектов.
///-------------------------------------------------------------------- Objects:
struct  Objects : private std::vector<Object>, sf::Drawable
{       Objects()
        {
            const auto&         dats = Data4Sprites::get();
            reserve(            dats.size());
            for(const auto& d : dats) emplace_back(Object(d));
        }

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
    }
};


///----------------------------------------------------------------------------|
/// Окно рендера.
///----------------------------------------------------------------- showWindow:
template<typename T> void showWindow(T&  objects)
{
    sf::RenderWindow window(sf::VideoMode({ 1344, 768 }), "Affected Zone");

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>()) window.close();
        }

    //  window.clear      ();
        window.draw(objects);
        window.display    ();
    }
}


///----------------------------------------------------------------------------|
/// Старт.
///----------------------------------------------------------------------- main:
int main()
{
    std::system ("chcp 65001");
    std::cout << "Старт ...\n";

    Objects  objects                            ; showWindow(objects);
    Object   object(Data4Sprites::get().front()); showWindow(object );
}
