/*********************************************************************
Matt Marchant 2014 - 2015
http://trederia.blogspot.com

xygine - Zlib license.

This software is provided 'as-is', without any express or
implied warranty. In no event will the authors be held
liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute
it freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented;
you must not claim that you wrote the original software.
If you use this software in a product, an acknowledgment
in the product documentation would be appreciated but
is not required.

2. Altered source versions must be plainly marked as such,
and must not be misrepresented as being the original software.

3. This notice may not be removed or altered from any
source distribution.
*********************************************************************/

//utility functions for working with picojson and sfml

#ifndef JSON_UTIL_H_
#define JSON_UTIL_H_

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Rect.hpp>

#include <xygine/picojson.h>

#include <fstream>

namespace Util
{
    namespace Json
    {

        static inline void parseJsonObject(const picojson::object& o, sf::FloatRect& rect)
        {
            for (const auto& p : o)
            {
                if (p.first == "x")
                {
                    rect.left = (p.second.is<double>()) ?
                        static_cast<float>(p.second.get<double>()) : 0;
                }
                else if (p.first == "y")
                {
                    rect.top = (p.second.is<double>()) ?
                        static_cast<float>(p.second.get<double>()) : 0;
                }
                else if (p.first == "w")
                {
                    rect.width = (p.second.is<double>()) ?
                        static_cast<float>(p.second.get<double>()) : 0;
                }
                else if (p.first == "h")
                {
                    rect.height = (p.second.is<double>()) ?
                        static_cast<float>(p.second.get<double>()) : 0;
                }
            }
        }

        static inline void parseJsonObject(const picojson::object& o, sf::Vector2f& v)
        {
            for (const auto& p : o)
            {
                if (p.first == "x" || p.first == "w")
                {
                    v.x = (p.second.is<double>()) ?
                        static_cast<float>(p.second.get<double>()) : 0;
                }
                else if (p.first == "y" || p.first == "h")
                {
                    v.y = (p.second.is<double>()) ?
                        static_cast<float>(p.second.get<double>()) : 0;
                }
            }
        }
    }

    namespace File
    {
        static inline bool validLength(std::ifstream& file)
        {
            file.seekg(0, file.end);
            sf::Int32 fileLength = static_cast<sf::Int32>(file.tellg());
            file.seekg(0, file.beg);
            return (fileLength > 0);
        }
    }
}

#endif //JSON_UTIL_H_