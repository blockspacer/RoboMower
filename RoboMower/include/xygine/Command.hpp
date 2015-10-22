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

//command class for targetting entities within a scene graph

#ifndef COMMAND_HPP_
#define COMMAND_HPP_

#include <SFML/Config.hpp>

#include <functional>
#include <queue>

namespace xy
{
    class Entity;
    struct Command final
    {
        std::function<void(Entity&, float)> action;
        sf::Uint64 entityID = 0u;
        enum
        {
            None = 0,
            Player = 0x1,
            Alien = 0x2,
            Particle = 0x4,
            GameRules = 0x8,
            Human = 0x10,
            All = Player | Alien | Particle | GameRules | Human
        }category = None;
    };

    class CommandQueue final
    {
    public:
        CommandQueue() = default;
        ~CommandQueue() = default;
        CommandQueue(const CommandQueue&) = delete;
        CommandQueue& operator = (const CommandQueue&) = delete;

        void push(const Command&);
        Command pop();
        bool empty() const;

    private:
        std::queue<Command> m_queue;
    };
}
#endif //COMMAND_HPP_