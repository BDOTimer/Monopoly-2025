#ifndef VSL_H
#define VSL_H
///----------------------------------------------------------------------------|
/// "vsl.h"
///----------------------------------------------------------------------------:
#include <iostream>
#include "../debug.h"


namespace visual
{
    struct endl{};

    struct  Console
    {       Console()
            {
            }

        Console& operator <<(endl e)
        {   std::cout << std::endl;
            return *this;
        }

        Console& operator<<(std::string_view s)
        {   std::cout << s;
            return *this;
        }

        Console& operator<<(char c)
        {   std::cout << c;
            return *this;
        }

        Console& operator<<(unsigned n)
        {   std::cout << n;
            return *this;
        }

    private:

    };
}


#endif // VSL_H
