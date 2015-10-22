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

//flexible logging class

#ifndef LOGGER_HPP_
#define LOGGER_HPP_

#include <SFML/System/Lock.hpp>
#include <SFML/System/Mutex.hpp>

#include <string>
#include <iostream>
#include <fstream>
#include <list>

#ifdef _MSC_VER
#define NOMINMAX
#include <Windows.h>
#endif //_MSC_VER

namespace xy
{
    class Logger final
    {
    public:
        enum class Output
        {
            Console,
            File,
            All
        };

        enum class Type
        {
            Info,
            Warning,
            Error
        };

        //logs a message to a given destination.
        //message: message to log
        //type: whether this message gets tagged as information, a warning or an error
        //output: can be the console via cout, a log file on disk, or both
        static void log(const std::string& message, Type type = Type::Info, Output output = Output::Console)
        {
            std::string outstring;
            switch (type)
            {
            case Type::Info:
            default:
                outstring = "INFO: " + message;
                break;
            case Type::Error:
                outstring = "ERROR: " + message;
                break;
            case Type::Warning:
                outstring = "WARNING: " + message;
                break;
            }

            sf::Lock lock(mutex());
            if (output == Output::Console || output == Output::All)
            {
                (type == Type::Error) ?
                    std::cerr << outstring << std::endl
                    :
                    std::cout << outstring << std::endl;

                const std::size_t maxBuffer = 30;
                buffer().push_back(outstring);
                if (buffer().size() > maxBuffer)buffer().pop_front(); //no majick here pl0x
                updateOutString(maxBuffer);

#ifdef _MSC_VER
                outstring += "\n";
                OutputDebugString(outstring.c_str());
#endif //_MSC_VER
            }
            if (output == Output::File || output == Output::All)
            {
                //output to a log file
                std::ofstream file("output.log", std::ios::app);
                if (file.good())
                {
                    file << outstring << std::endl;
                    file.close();
                }
                else
                {
                    log(message, type, Output::Console);
                    log("Above message was intended for log file. Opening file probably failed.", Type::Warning, Output::Console);
                }
            }
        }

        static const std::string& bufferString(){ return stringOutput(); }

    private:
        static sf::Mutex& mutex(){ static sf::Mutex m; return m; }
        static std::list<std::string>& buffer(){ static std::list<std::string> buffer; return buffer; }
        static std::string& stringOutput() { static std::string output; return output; }
        static void updateOutString(std::size_t maxBuffer)
        {
            static size_t count = 0;
            stringOutput().append(buffer().back());
            stringOutput().append("\n");
            count++;

            if (count > maxBuffer)
            {
                stringOutput() = stringOutput().substr(stringOutput().find_first_of('\n') + 1, stringOutput().size());
                count--;
            }
        }
    };
}
#ifndef _DEBUG_
#define LOG(message, type)
#else
#define LOG(message, type) xy::Logger::log(message, type);
#endif //_DEBUG_

#endif //LOGGER_HPP_