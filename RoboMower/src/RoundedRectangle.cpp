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

#include <RoundedRectangle.hpp>

#include <xygine/MessageBus.hpp>
#include <xygine/Entity.hpp>
#include <xygine/Assert.hpp>

namespace
{
    const sf::Uint8 radiusPoints = 8u;
    const float pi = 3.141592654f;
    const float radian = pi / 180.f;
}


RoundedRectangle::RoundedRectangle(const sf::Vector2f& size, float radius)
    : m_size        (size),
    m_cornerRadius  (radius),
    m_minimumSize   (10.f, 10.f)
{
    XY_ASSERT(radius > 0 && size.x > 0 && size.y > 0, "rounded rectangle parameters must be greater than 0");
    
    clampSize();
    update();
}

//public
void RoundedRectangle::setSize(const sf::Vector2f& size)
{
    m_size = size;
    clampSize();
    update();
}

void RoundedRectangle::setRadius(float radius)
{
    m_cornerRadius = radius;
    update();
}

const sf::Vector2f& RoundedRectangle::getSize() const
{
    return m_size;
}

std::size_t RoundedRectangle::getPointCount() const
{
    return radiusPoints * 4u;
}

sf::Vector2f RoundedRectangle::getPoint(std::size_t index) const
{
    if (index >= getPointCount()) return sf::Vector2f();

    float deltaAngle = 90.f / (radiusPoints - 1);
    sf::Vector2f center;
    sf::Uint32 centerIndex = index / radiusPoints;

    switch (centerIndex)
    {
    case 0:
        center.x = m_size.x - m_cornerRadius;
        center.y = m_cornerRadius;
        break;
    case 1:
        center.x = m_cornerRadius;
        center.y = m_cornerRadius;
        break;
    case 2:
        center.x = m_cornerRadius;
        center.y = m_size.y - m_cornerRadius;
        break;
    case 3:
        center.x = m_size.x - m_cornerRadius;
        center.y = m_size.y - m_cornerRadius;
        break;
    default: break;
    }

    return
    {
        m_cornerRadius * std::cos(deltaAngle * (index - centerIndex) * radian) + center.x,
        -m_cornerRadius * std::sin(deltaAngle * (index - centerIndex) * radian) + center.y
    };
}


//private
void RoundedRectangle::clampSize()
{
    m_size.x = std::max(m_size.x, m_minimumSize.x);
    m_size.y = std::max(m_size.y, m_minimumSize.y);
}
