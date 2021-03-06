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

#include <MenuJoinState.hpp>

#include <xygine/App.hpp>
#include <xygine/Log.hpp>

#include <xygine/ui/Button.hpp>
#include <xygine/ui/TextBox.hpp>

#include <SFML/Window/Mouse.hpp>


MenuJoinState::MenuJoinState(xy::StateStack& stack, Context context)
    : State         (stack, context),
    m_messageBus    (context.appInstance.getMessageBus()),
    m_uiContainer   (m_messageBus)
{
    m_cursorSprite.setTexture(m_textureResource.get("assets/images/ui/cursor.png"));
    m_cursorSprite.setPosition(context.renderWindow.mapPixelToCoords(sf::Mouse::getPosition(context.renderWindow)));
    buildMenu();

    auto msg = m_messageBus.post<xy::Message::UIEvent>(xy::Message::UIMessage);
    msg->type = xy::Message::UIEvent::MenuOpened;
    msg->stateID = States::ID::MenuJoin;
}

//public
bool MenuJoinState::update(float dt)
{
    m_uiContainer.update(dt);
    return true;
}

void MenuJoinState::draw()
{
    auto& rw = getContext().renderWindow;
    rw.setView(getContext().defaultView);

    rw.draw(m_uiContainer);
    rw.draw(m_cursorSprite);
}

bool MenuJoinState::handleEvent(const sf::Event& evt)
{
    const auto& rw = getContext().renderWindow;
    auto mousePos = rw.mapPixelToCoords(sf::Mouse::getPosition(rw));

    m_uiContainer.handleEvent(evt, mousePos);
    m_cursorSprite.setPosition(mousePos);
    return false;
}

void MenuJoinState::handleMessage(const xy::Message& msg)
{

}

//private
void MenuJoinState::buildMenu()
{
    const auto& font = m_fontResource.get("flaps");

    auto textbox = std::make_shared<xy::UI::TextBox>(font);
    textbox->setLabelText("IP Address:");
    textbox->setPosition(960.f, 500.f);
    textbox->setAlignment(xy::UI::Alignment::Centre);
    textbox->setText("127.0.0.1");
    m_uiContainer.addControl(textbox);

    m_statusLabel = std::make_shared<xy::UI::Label>(font);
    m_statusLabel->setAlignment(xy::UI::Alignment::Centre);
    m_statusLabel->setPosition(960.f, 590.f);
    m_uiContainer.addControl(m_statusLabel);

    auto joinButton = std::make_shared<xy::UI::Button>(font, m_textureResource.get("assets/images/ui/button.png"));
    joinButton->setText("Join");
    joinButton->setAlignment(xy::UI::Alignment::Centre);
    joinButton->setPosition(840.f, 770.f);
    joinButton->addCallback([textbox, this]()
    {

    });
    m_uiContainer.addControl(joinButton);

    auto backButton = std::make_shared<xy::UI::Button>(font, m_textureResource.get("assets/images/ui/button.png"));
    backButton->setText("Back");
    backButton->setAlignment(xy::UI::Alignment::Centre);
    backButton->setPosition(1080.f, 770.f);
    backButton->addCallback([this]()
    {
        requestStackPop();
        sendCloseMessage();
        requestStackPush(States::ID::MenuMain);
    });
    m_uiContainer.addControl(backButton);


}

void MenuJoinState::sendCloseMessage()
{
    auto msg = m_messageBus.post<xy::Message::UIEvent>(xy::Message::UIMessage);
    msg->type = xy::Message::UIEvent::MenuClosed;
    msg->stateID = States::ID::MenuJoin;
}