#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

#include <iostream>
#include <list>
#include <vector>
#include <string>
#include <sstream>

#define DEBUG   true
#define HEIGHT  600
#define WIDTH   1200
#define SPEED   10

namespace patch
{
    template < typename T > std::string to_string( const T& n )
    {
        std::ostringstream stm ;
        stm << n ;
        return stm.str() ;
    }
}

enum    CHARTYPE{
        DEFAUTL,
        HULK,
        RABBIT,
        LITTLE
};
