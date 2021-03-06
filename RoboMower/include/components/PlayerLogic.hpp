/*-----------------------------------------------------------------------

Matt Marchant 2015 - 2016
http://trederia.blogspot.com

Robomower - Zlib license.

This software is provided 'as-is', without any express or
implied warranty.In no event will the authors be held
liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute
it freely, subject to the following restrictions :

1. The origin of this software must not be misrepresented;
you must not claim that you wrote the original software.
If you use this software in a product, an acknowledgment
in the product documentation would be appreciated but
is not required.

2. Altered source versions must be plainly marked as such,
and must not be misrepresented as being the original software.

3. This notice may not be removed or altered from any
source distribution.

-----------------------------------------------------------------------*/

#ifndef RM_PLAYER_LOGIC_HPP_
#define RM_PLAYER_LOGIC_HPP_

#include <PacketEnums.hpp>

#include <xygine/components/Component.hpp>
#include <xygine/network/Config.hpp>

#include <SFML/System/Clock.hpp>

class PlayerLogic final : public xy::Component
{
public:
    PlayerLogic(xy::MessageBus&, const sf::Vector2f&);
    ~PlayerLogic() = default;

    xy::Component::Type type() const override { return xy::Component::Type::Script; }
    void entityUpdate(xy::Entity&, float) override;
    void onDelayedStart(xy::Entity&) override;

    void setClientID(xy::ClientID);
    void setProgram(const std::vector<sf::Uint8>& program) { m_program = program; }

    void start();
    void pause();
    void rewind();

private:
    xy::Entity* m_entity;
    sf::Vector2f m_spawnPosition;
    xy::ClientID m_clientID;
    Direction m_currentDirection;
    sf::Vector2f m_target;
    sf::Clock m_rotationTimer;

    TransportStatus m_transportStatus;
    std::vector<sf::Uint8> m_program;
    std::size_t m_programCounter;
    std::size_t m_loopDestination;
    sf::Int8 m_loopCounter;

    sf::Uint8 m_currentParameter;
    std::function<bool(xy::Entity&, float)> m_currentAction;

    void stop();
};

#endif //RM_PLAYER_LOGIC_HPP_