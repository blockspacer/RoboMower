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

#include <MenuMainState.hpp>
#include <UIControlIDs.hpp>

#include <xygine/App.hpp>
#include <xygine/ui/Button.hpp>

#include <SFML/Window/Mouse.hpp>

MenuMainState::MenuMainState(xy::StateStack& stack, Context context)
    : State     (stack, context),
    m_messageBus(context.appInstance.getMessageBus()),
    m_uiContainer(m_messageBus)
{
    context.appInstance.setMouseCursorVisible(false);
    m_cursorSprite.setTexture(m_textureResource.get("assets/images/ui/cursor.png"));
    m_cursorSprite.setPosition(context.renderWindow.mapPixelToCoords(sf::Mouse::getPosition(context.renderWindow)));

    buildMenu();

    auto msg = m_messageBus.post<xy::Message::UIEvent>(xy::Message::UIMessage);
    msg->type = xy::Message::UIEvent::MenuOpened;
    msg->stateID = States::ID::MenuMain;
}

//public
bool MenuMainState::update(float dt)
{
    m_uiContainer.update(dt);
    return false;
}

void MenuMainState::draw()
{
    auto& rw = getContext().renderWindow;
    rw.setView(getContext().defaultView);
    //rw.draw(m_menuSprite);

    rw.draw(m_uiContainer);
    rw.draw(m_cursorSprite);
}

bool MenuMainState::handleEvent(const sf::Event& evt)
{
    const auto& rw = getContext().renderWindow;
    auto mousePos = rw.mapPixelToCoords(sf::Mouse::getPosition(rw));

    m_uiContainer.handleEvent(evt, mousePos);
    m_cursorSprite.setPosition(mousePos);

    return false;
}

void MenuMainState::handleMessage(const xy::Message& msg)
{

}

//private
void MenuMainState::buildMenu()
{
    const auto& font = m_fontResource.get("assets/fonts/VeraMono.ttf");
    
    auto button = std::make_shared<xy::UI::Button>(font, m_textureResource.get("assets/images/ui/start_button.png"));
    button->setText("Single Player");
    button->setAlignment(xy::UI::Alignment::Centre);
    button->setPosition(960.f, 475.f);
    button->addCallback([this]()
    {
        //raise message with button ID that called close event
        auto msg = m_messageBus.post<xy::Message::UIEvent>(xy::Message::UIMessage);
        msg->controlID = ControlId::SinglePlayerButton;
        msg->type = xy::Message::UIEvent::ButtonPressed;
        msg->stateID = States::Game;

        close();
        requestStackClear();
    });
    m_uiContainer.addControl(button);

    button = std::make_shared<xy::UI::Button>(font, m_textureResource.get("assets/images/ui/start_button.png"));
    button->setText("Host Multiplayer");
    button->setAlignment(xy::UI::Alignment::Centre);
    button->setPosition(960.f, 575.f);
    button->addCallback([this]()
    {

    });
    m_uiContainer.addControl(button);

    button = std::make_shared<xy::UI::Button>(font, m_textureResource.get("assets/images/ui/start_button.png"));
    button->setText("Join Multiplayer");
    button->setAlignment(xy::UI::Alignment::Centre);
    button->setPosition(960.f, 675.f);
    button->addCallback([this]()
    {

    });
    m_uiContainer.addControl(button);

    button = std::make_shared<xy::UI::Button>(font, m_textureResource.get("assets/images/ui/start_button.png"));
    button->setText("Options");
    button->setAlignment(xy::UI::Alignment::Centre);
    button->setPosition(960.f, 775.f);
    button->addCallback([this]()
    {
        close();
        requestStackPush(States::ID::MenuOptions);
    });
    m_uiContainer.addControl(button);

    button = std::make_shared<xy::UI::Button>(font, m_textureResource.get("assets/images/ui/start_button.png"));
    button->setText("Quit");
    button->setAlignment(xy::UI::Alignment::Centre);
    button->setPosition(960.f, 875.f);
    button->addCallback([this]()
    {
        getContext().renderWindow.close();
    });
    m_uiContainer.addControl(button);
}

void MenuMainState::close()
{
    requestStackPop();

    auto msg = m_messageBus.post<xy::Message::UIEvent>(xy::Message::UIMessage);
    msg->type = xy::Message::UIEvent::MenuClosed;
    msg->stateID = States::ID::MenuMain;
}