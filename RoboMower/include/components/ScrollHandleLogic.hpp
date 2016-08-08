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

//logic for scroll bar handle

#ifndef SCROLL_HANDLE_LOGIC_HPP_
#define SCROLL_HANDLE_LOGIC_HPP_

#include <xygine/components/Component.hpp>

class ScrollHandleLogic final : public xy::Component
{
public:
    explicit ScrollHandleLogic(xy::MessageBus&);
    ~ScrollHandleLogic() = default;
    ScrollHandleLogic(const ScrollHandleLogic&) = delete;
    ScrollHandleLogic& operator = (const ScrollHandleLogic&) = delete;

    xy::Component::Type type() const override { return xy::Component::Type::Script; }
    void entityUpdate(xy::Entity&, float) override;

    void setCarried(bool);
    bool carried() const;

    void setLength(float);
    //returns position as normalised value of length
    float getPosition() const;

private:

    bool m_carried;
    float m_length;
    float m_position;

    bool m_update;

    void handleMessage(xy::Component*, const xy::Message&);
};

#endif //SCROLL_HANDLE_LOGIC_HPP_