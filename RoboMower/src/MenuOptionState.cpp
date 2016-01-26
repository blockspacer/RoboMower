//==============================================================================
// RoboMower - Copyright (C) Matt Marchant; All Rights Reserved
// Unauthorized copying of this file via any medium is strictly prohibited
// Proprietary and confidential
// Written by Matt Marchant (matty_styles@hotmail.com) 2015 - 2016
//==============================================================================

#include <MenuOptionState.hpp>

#include <xygine/App.hpp>
#include <xygine/Log.hpp>

#include <xygine/ui/Slider.hpp>
#include <xygine/ui/CheckBox.hpp>
#include <xygine/ui/Selection.hpp>
#include <xygine/ui/Button.hpp>
#include <xygine/ui/TextBox.hpp>

#include <SFML/Window/Event.hpp>

namespace
{
    
}

MenuOptionState::MenuOptionState(xy::StateStack& stateStack, Context context)
    : State     (stateStack, context),
    m_messageBus(context.appInstance.getMessageBus())
{
    //m_menuSprite.setTexture(context.appInstance.getTexture("assets/images/main_menu.png"));
    //m_menuSprite.setPosition(context.defaultView.getCenter());
    //Util::Position::centreOrigin(m_menuSprite);
    //m_menuSprite.move(0.f, -40.f);

    m_cursorSprite.setTexture(m_textureResource.get("assets/images/ui/cursor.png"));
    m_cursorSprite.setPosition(context.renderWindow.mapPixelToCoords(sf::Mouse::getPosition(context.renderWindow)));
    
    const auto& font = m_fontResource.get("assets/fonts/N_E_B.ttf");
    buildMenu(font);

    auto msg = m_messageBus.post<xy::Message::UIEvent>(xy::Message::UIMessage);
    msg->type = xy::Message::UIEvent::MenuOpened;
    msg->stateId = States::ID::MenuOptions;
}

//public
bool MenuOptionState::update(float dt)
{
    m_uiContainer.update(dt);
    return true;
}

void MenuOptionState::draw()
{
    auto& rw = getContext().renderWindow;
    rw.setView(getContext().defaultView);
    //rw.draw(m_menuSprite);

    rw.draw(m_uiContainer);

    for (const auto& t : m_texts)
    {
        rw.draw(t);
    }

    rw.draw(m_cursorSprite);
}

bool MenuOptionState::handleEvent(const sf::Event& evt)
{
    if (evt.type == sf::Event::KeyReleased)
    {
        switch (evt.key.code)
        {
        case sf::Keyboard::Tab:

            return false;
        case sf::Keyboard::Space:

            return false;
        default: break;
        }
    }
    else if (evt.type == sf::Event::JoystickButtonReleased)
    {
        switch (evt.joystickButton.button)
        {
        case 7: //start on xbox

            return false;
        default: break;
        }
    }
    
    //pass remaining events to menu
    const auto& rw = getContext().renderWindow;
    auto mousePos = rw.mapPixelToCoords(sf::Mouse::getPosition(rw));

    m_uiContainer.handleEvent(evt, mousePos);
    m_cursorSprite.setPosition(mousePos);

    return false; //consume events
}

void MenuOptionState::handleMessage(const xy::Message& msg)
{
    switch (msg.id)
    {
    case xy::Message::Type::UIMessage:
    {
        auto& msgData = msg.getData<xy::Message::UIEvent>();
        switch (msgData.type)
        {
        case xy::Message::UIEvent::MenuClosed:

            break;
        default: break;
        }
        break;
    }
    default: break;
    }
}

//private
void MenuOptionState::buildMenu(const sf::Font& font)
{
    auto soundSlider = std::make_shared<xy::UI::Slider>(font, m_textureResource.get("assets/images/ui/slider_handle.png"), 375.f);
    soundSlider->setPosition(600.f, 470.f);
    soundSlider->setText("Volume");
    soundSlider->setMaxValue(1.f);
    soundSlider->addCallback([this](const xy::UI::Slider* slider)
    {
        //send volume setting command
        auto msg = m_messageBus.post<xy::Message::UIEvent>(xy::Message::UIMessage);
        msg->type = xy::Message::UIEvent::RequestVolumeChange;
        msg->value = slider->getValue();

    }, xy::UI::Slider::Event::ValueChanged);
    soundSlider->setValue(getContext().appInstance.getAudioSettings().volume); //set this *after* callback is set
    m_uiContainer.addControl(soundSlider);

    auto muteCheckbox = std::make_shared<xy::UI::CheckBox>(font, m_textureResource.get("assets/images/ui/checkbox.png"));
    muteCheckbox->setPosition(1070.f, 430.f);
    muteCheckbox->setText("Mute");
    muteCheckbox->addCallback([this](const xy::UI::CheckBox* checkBox)
    {
        auto msg = m_messageBus.post<xy::Message::UIEvent>(xy::Message::UIMessage);
        msg->type = (checkBox->checked()) ? xy::Message::UIEvent::RequestAudioMute : xy::Message::UIEvent::RequestAudioUnmute;
    }, xy::UI::CheckBox::Event::CheckChanged);
    muteCheckbox->check(getContext().appInstance.getAudioSettings().muted);
    m_uiContainer.addControl(muteCheckbox);


    auto resolutionBox = std::make_shared<xy::UI::Selection>(font, m_textureResource.get("assets/images/ui/scroll_arrow.png"), 375.f);
    resolutionBox->setPosition(600.f, 510.f);

    const auto& modes = getContext().appInstance.getVideoSettings().AvailableVideoModes;
    auto i = 0u;
    auto j = 0u;
    for (const auto& m : modes)
    {
        std::string name = std::to_string(m.width) + " x " + std::to_string(m.height);
        sf::Int32 val = (m.width << 16) | m.height;
        resolutionBox->addItem(name, val);
        //select currently active mode
        if (getContext().appInstance.getVideoSettings().VideoMode != m)
            i++;
        else
            j = i;
    }
    if (i < modes.size()) resolutionBox->setSelectedIndex(j);

    m_uiContainer.addControl(resolutionBox);

    auto fullscreenCheckbox = std::make_shared<xy::UI::CheckBox>(font, m_textureResource.get("assets/images/ui/checkbox.png"));
    fullscreenCheckbox->setPosition(1070.f, 510.f);
    fullscreenCheckbox->setText("Full Screen");
    fullscreenCheckbox->addCallback([this](const xy::UI::CheckBox*)
    {

    }, xy::UI::CheckBox::Event::CheckChanged);
    fullscreenCheckbox->check((getContext().appInstance.getVideoSettings().WindowStyle & sf::Style::Fullscreen) != 0);
    m_uiContainer.addControl(fullscreenCheckbox);

    auto difficultySelection = std::make_shared<xy::UI::Selection>(font, m_textureResource.get("assets/images/ui/scroll_arrow.png"), 375.f);
    difficultySelection->setPosition(600.f, 590.f);
    difficultySelection->addItem("Easy", static_cast<int>(xy::Difficulty::Easy));
    difficultySelection->addItem("Medium", static_cast<int>(xy::Difficulty::Medium));
    difficultySelection->addItem("Hard", static_cast<int>(xy::Difficulty::Hard));
    difficultySelection->selectItem(0);
    difficultySelection->setCallback([this](const xy::UI::Selection* s)
    {
        //send message with new difficulty
        auto msg = m_messageBus.post<xy::Message::UIEvent>(xy::Message::UIMessage);
        msg->type = xy::Message::UIEvent::RequestDifficultyChange;
        msg->difficulty = static_cast<xy::Difficulty>(s->getSelectedValue());
    });
    difficultySelection->selectItem(static_cast<int>(getContext().appInstance.getGameSettings().difficulty));
    m_uiContainer.addControl(difficultySelection);

    auto controllerCheckbox = std::make_shared<xy::UI::CheckBox>(font, m_textureResource.get("assets/images/ui/checkbox.png"));
    controllerCheckbox->setPosition(1070.f, 590.f);
    controllerCheckbox->setText("Enable Controller");
    controllerCheckbox->addCallback([this](const xy::UI::CheckBox* checkBox)
    {
        auto msg = m_messageBus.post<xy::Message::UIEvent>(xy::Message::UIMessage);
        msg->type = (checkBox->checked()) ? xy::Message::UIEvent::RequestControllerEnable : xy::Message::UIEvent::RequestControllerDisable;

    }, xy::UI::CheckBox::Event::CheckChanged);
    controllerCheckbox->check(getContext().appInstance.getGameSettings().controllerEnabled);
    m_uiContainer.addControl(controllerCheckbox);

    auto applyButton = std::make_shared<xy::UI::Button>(font, m_textureResource.get("assets/images/ui/button.png"));
    applyButton->setText("Apply");
    applyButton->setAlignment(xy::UI::Alignment::Centre);
    applyButton->setPosition(840.f, 770.f);
    applyButton->addCallback([fullscreenCheckbox, resolutionBox, this]()
    {
        auto res = resolutionBox->getSelectedValue();

        xy::App::VideoSettings settings;
        settings.VideoMode.width = res >> 16;
        settings.VideoMode.height = res & 0xFFFF;
        settings.WindowStyle = (fullscreenCheckbox->checked()) ? sf::Style::Fullscreen : sf::Style::Close;
        getContext().appInstance.applyVideoSettings(settings);

        auto msg = m_messageBus.post<xy::Message::UIEvent>(xy::Message::UIMessage);
        msg->type = xy::Message::UIEvent::ResizedWindow;
    });
    m_uiContainer.addControl(applyButton);

    auto backButton = std::make_shared<xy::UI::Button>(font, m_textureResource.get("assets/images/ui/button.png"));
    backButton->setText("Back");
    backButton->setAlignment(xy::UI::Alignment::Centre);
    backButton->setPosition(1080.f, 770.f);
    backButton->addCallback([this]()
    {
        close();
        requestStackPush((getContext().appInstance.connected()) ? States::ID::MenuPaused : States::ID::MenuMain);
    });
    m_uiContainer.addControl(backButton);
}

void MenuOptionState::close()
{
    requestStackPop();

    auto msg = m_messageBus.post<xy::Message::UIEvent>(xy::Message::UIMessage);
    msg->type = xy::Message::UIEvent::MenuClosed;
    msg->stateId = States::ID::MenuOptions;
}