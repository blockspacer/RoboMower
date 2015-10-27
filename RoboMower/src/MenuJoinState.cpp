//==============================================================================
// RoboMower - Copyright (C) Matt Marchant; All Rights Reserved
// Unauthorized copying of this file via any medium is strictly prohibited
// Proprietary and confidential
// Written by Matt Marchant (matty_styles@hotmail.com) 2015
//==============================================================================

#include <MenuJoinState.hpp>

#include <xygine/App.hpp>
#include <xygine/Log.hpp>

#include <xygine/ui/Button.hpp>
#include <xygine/ui/TextBox.hpp>

#include <SFML/Window/Mouse.hpp>


MenuJoinState::MenuJoinState(xy::StateStack& stack, Context context)
    : State         (stack, context),
    m_messageBus    (context.appInstance.getMessageBus())
{
    m_cursorSprite.setTexture(context.appInstance.getTexture("assets/images/ui/cursor.png"));
    m_cursorSprite.setPosition(context.renderWindow.mapPixelToCoords(sf::Mouse::getPosition(context.renderWindow)));
    buildMenu();

    auto msg = m_messageBus.post<xy::Message::UIEvent>(xy::Message::UIMessage);
    msg->type = xy::Message::UIEvent::MenuOpened;
    msg->stateId = States::ID::MenuJoin;
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
    if (msg.id == xy::Message::Type::NetworkMessage)
    {
        auto& msgData = msg.getData<xy::Message::NetworkEvent>();
        switch (msgData.action)
        {
        case xy::Message::NetworkEvent::ConnectSuccess:
            requestStackPop();
            requestStackPush(States::ID::MenuLobby);
            sendCloseMessage();
            break;
        case xy::Message::NetworkEvent::ConnectFail:
            m_statusLabel->setString("Connection Failed.");
            break;
        default: break;
        }
    }
}

//private
void MenuJoinState::buildMenu()
{
    const auto& font = getContext().appInstance.getFont("flaps");

    auto textbox = std::make_shared<xy::ui::TextBox>(font);
    textbox->setLabelText("IP Address:");
    textbox->setPosition(960.f, 500.f);
    textbox->setAlignment(xy::ui::Alignment::Centre);
    textbox->setText("127.0.0.1");
    m_uiContainer.addControl(textbox);

    m_statusLabel = std::make_shared<xy::ui::Label>(font);
    m_statusLabel->setAlignment(xy::ui::Alignment::Centre);
    m_statusLabel->setPosition(960.f, 590.f);
    m_uiContainer.addControl(m_statusLabel);

    auto joinButton = std::make_shared<xy::ui::Button>(font, getContext().appInstance.getTexture("assets/images/ui/button.png"));
    joinButton->setText("Join");
    joinButton->setAlignment(xy::ui::Alignment::Centre);
    joinButton->setPosition(840.f, 770.f);
    joinButton->setCallback([textbox, this]()
    {
        getContext().appInstance.setDestinationIP(textbox->getText());
        m_statusLabel->setString("Connecting...");

        auto msg = m_messageBus.post<xy::Message::NetworkEvent>(xy::Message::NetworkMessage);
        msg->action = xy::Message::NetworkEvent::RequestJoinServer;

    });
    m_uiContainer.addControl(joinButton);

    auto backButton = std::make_shared<xy::ui::Button>(font, getContext().appInstance.getTexture("assets/images/ui/button.png"));
    backButton->setText("Back");
    backButton->setAlignment(xy::ui::Alignment::Centre);
    backButton->setPosition(1080.f, 770.f);
    backButton->setCallback([this]()
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
    msg->stateId = States::ID::MenuJoin;
}