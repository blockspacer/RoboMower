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

#include <xygine/MenuPauseState.hpp>
#include <xygine/App.hpp>
#include <xygine/Util.hpp>

#include <xygine/ui/Button.hpp>

#include <SFML/Window/Mouse.hpp>

MenuPauseState::MenuPauseState(StateStack& stack, Context context)
    : State     (stack, context),
    m_messageBus(context.appInstance.getMessageBus())
{
    const auto& font = context.appInstance.getFont("flaps");
    
    buildMenu(font);

    m_texts.emplace_back("PAUSED", font);
    Util::Position::centreOrigin(m_texts.back());
    m_texts.back().setPosition(960.f, 200.f);

    m_cursorSprite.setTexture(context.appInstance.getTexture("assets/images/ui/cursor.png"));
    m_cursorSprite.setPosition(context.renderWindow.mapPixelToCoords(sf::Mouse::getPosition(context.renderWindow)));

    Message msg;
    msg.type = Message::Type::UI;
    msg.ui.type = Message::UIEvent::MenuOpened;
    msg.ui.value = 0.f;
    msg.ui.stateId = States::ID::MenuPaused;
    m_messageBus.post(msg);

    //TODO check if hosting and send pause packet to server
}

//public
bool MenuPauseState::update(float dt)
{
    m_uiContainer.update(dt);
    return true;
}

void MenuPauseState::draw()
{
    auto& rw = getContext().renderWindow;
    rw.setView(getContext().defaultView);
    //rw.draw(m_menuSprite);

    rw.draw(m_uiContainer);
    rw.draw(m_cursorSprite);
}

bool MenuPauseState::handleEvent(const sf::Event& evt)
{
    const auto& rw = getContext().renderWindow;
    auto mousePos = rw.mapPixelToCoords(sf::Mouse::getPosition(rw));

    m_uiContainer.handleEvent(evt, mousePos);
    m_cursorSprite.setPosition(mousePos);

    return false;
}

void MenuPauseState::handleMessage(const Message&){}

//private
void MenuPauseState::buildMenu(const sf::Font& font)
{
    auto button = std::make_shared<ui::Button>(font, getContext().appInstance.getTexture("assets/images/ui/start_button.png"));
    button->setText("Continue");
    button->setAlignment(ui::Alignment::Centre);
    button->setPosition(960.f, 475.f);
    button->setCallback([this]()
    {
        requestStackPop();
        sendCloseMessage();
    });
    m_uiContainer.addControl(button);

    button = std::make_shared<ui::Button>(font, getContext().appInstance.getTexture("assets/images/ui/start_button.png"));
    button->setText("Options");
    button->setAlignment(ui::Alignment::Centre);
    button->setPosition(960.f, 575.f);
    button->setCallback([this]()
    {
        requestStackPop();
        requestStackPush(States::ID::MenuOptions);
        sendCloseMessage();

    });
    m_uiContainer.addControl(button);

    button = std::make_shared<ui::Button>(font, getContext().appInstance.getTexture("assets/images/ui/start_button.png"));
    button->setText("Quit");
    button->setAlignment(ui::Alignment::Centre);
    button->setPosition(960.f, 675.f);
    button->setCallback([this]()
    {
        requestStackClear();
        requestStackPush(States::ID::MenuBackground);
        requestStackPush(States::ID::MenuMain);

        sendCloseMessage();

        Message msg;
        msg.type = Message::Type::Network;
        //msg.network.stateID = States::ID::MenuPaused;
        msg.network.action = Message::NetworkEvent::RequestDisconnect;
        m_messageBus.post(msg);
    });
    m_uiContainer.addControl(button);
}

void MenuPauseState::sendCloseMessage()
{
    Message msg;
    msg.type = Message::Type::UI;
    msg.ui.stateId = States::ID::MenuPaused;
    msg.ui.type = Message::UIEvent::MenuClosed;
    m_messageBus.post(msg);
}