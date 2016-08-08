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

//renders the background under any active menu state

#ifndef MENU_BACKGROUND_STATE_HPP_
#define MENU_BACKGROUND_STATE_HPP_

#include <StateIds.hpp>

#include <xygine/State.hpp>
#include <xygine/Resource.hpp>
#include <xygine/ui/Container.hpp>

#include <SFML/Graphics/Text.hpp>

#include <vector>

namespace xy
{
    class MessageBus;
}
class MenuBackgroundState final : public xy::State
{
public:
    MenuBackgroundState(xy::StateStack&, Context);
    ~MenuBackgroundState() = default;

    bool update(float) override;
    void draw() override;
    bool handleEvent(const sf::Event&) override;
    void handleMessage(const xy::Message&) override;
    xy::StateID stateID() const override
    {
        return States::ID::MenuBackground;
    }
private:
    xy::MessageBus& m_messageBus;
    xy::UI::Container m_uiContainer;
    xy::FontResource m_fontResource;

    std::vector<sf::Text> m_texts;
};

#endif //MENU_BACKGROUND_STATE_HPP_