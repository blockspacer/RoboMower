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

#include <xygine/Component.hpp>
#include <xygine/MessageBus.hpp>

Component::Component(MessageBus& m)
    : m_messageBus  (m),
    m_destroyed     (false),
    m_parentUID     (0u)
{

}

Component::~Component()
{
    Message msg;
    msg.type = Message::Type::ComponentSystem;
    msg.component.action = Message::ComponentEvent::Deleted;
    msg.component.ptr = this;
    msg.component.entityId = m_parentUID;
    m_messageBus.post(msg);
}

//public
void Component::onStart(Entity&)
{

}

void Component::destroy()
{
    m_destroyed = true;
}

bool Component::destroyed() const
{
    return m_destroyed;
}

void Component::setParentUID(sf::Uint64 uid)
{
    m_parentUID = uid;
}

void Component::setName(const std::string& name)
{
    m_name = name;
}

const std::string& Component::getName() const
{
    return m_name;
}

sf::FloatRect Component::localBounds() const
{
    return sf::FloatRect();
}

sf::FloatRect Component::globalBounds() const
{
    return sf::FloatRect();
}

//protected
void Component::sendMessage(const Message& m)
{
    m_messageBus.post(m);
}

MessageBus& Component::getMessageBus() const
{
    return m_messageBus;
}

sf::Uint64 Component::getParentUID() const
{
    return m_parentUID;
}