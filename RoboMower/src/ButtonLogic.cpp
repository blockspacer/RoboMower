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

#include <components/ButtonLogic.hpp>
#include <Messages.hpp>

#include <xygine/Log.hpp>

ButtonLogicScript::ButtonLogicScript(xy::MessageBus& mb, Instruction inst)
    : Component(mb, this),
    m_instruction(inst){}

//public
void ButtonLogicScript::entityUpdate(xy::Entity& ent, float)
{

}

Instruction ButtonLogicScript::getInstruction() const
{
    return m_instruction;
}

void ButtonLogicScript::doClick(const sf::Vector2f& abs, const sf::Vector2f& rel)
{
    auto msg = sendMessage<TrayIconEvent>(MessageId::TrayIconMessage);
    msg->action = TrayIconEvent::Clicked;
    msg->instruction = m_instruction;
    msg->absX = abs.x;
    msg->absY = abs.y;
    msg->relX = rel.x;
    msg->relY = rel.y;
}