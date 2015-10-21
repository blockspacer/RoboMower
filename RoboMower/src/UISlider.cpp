//==============================================================================
// RoboMower - Copyright (C) Matt Marchant; All Rights Reserved
// Unauthorized copying of this file via any medium is strictly prohibited
// Proprietary and confidential
// Written by Matt Marchant (matty_styles@hotmail.com) 2015
//==============================================================================

#include <UISlider.hpp>
#include <Util.hpp>

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Window/Event.hpp>

#include <cassert>

using namespace ui;

namespace
{
    const float thickness = 4.5f;
    const sf::Color fillColour(255u, 255u, 255u, 70u);
    const sf::Color borderColour(250u, 250u, 250u, 100u);
    const float deadzone = 40.f;
}

Slider::Slider(const sf::Font& font, const sf::Texture& texture, float length, float maxValue)
    : m_maxValue    (maxValue),
    m_length        (length),
    m_direction     (Direction::Horizontal),
    m_handleSprite  (texture),
    m_slotShape     ({ length, thickness }),
    m_text          ("", font, 36u),
    m_valueText     ("0", font, 36u),
    m_borderColour  (borderColour),
    m_activeColour  (255u, 255u, 255u, 120u)
{
    sf::IntRect subrect(0, 0, texture.getSize().x, texture.getSize().y / 2u);
    m_subRects.push_back(subrect);
    subrect.top += subrect.height;
    m_subRects.push_back(subrect);

    m_handleSprite.setTextureRect(m_subRects[State::Normal]);

    Util::Position::centreOrigin(m_handleSprite);
    m_slotShape.setOrigin(0.f, thickness / 2.f);
    m_slotShape.setFillColor(fillColour);
    m_slotShape.setOutlineColor(m_borderColour);
    m_slotShape.setOutlineThickness(thickness / 2);

    m_valueChanged.push_back(std::bind(&Slider::valueChanged, this, std::placeholders::_1));
}

//public
bool Slider::selectable() const
{
    return true;
}

void Slider::select()
{
    Control::select();
    m_handleSprite.setTextureRect(m_subRects[State::Selected]);
}

void Slider::deselect()
{
    Control::deselect();
    m_handleSprite.setTextureRect(m_subRects[State::Normal]);
}

void Slider::activate()
{
    Control::activate();
    m_slotShape.setOutlineColor(m_activeColour);
    if (m_setActive) m_setActive(this);
}

void Slider::deactivate()
{
    Control::deactivate();
    m_slotShape.setOutlineColor(m_borderColour);
    if (m_setInactive) m_setInactive(this);
}

void Slider::handleEvent(const sf::Event& e, const sf::Vector2f& mousePos)
{
    //keyboard
    if (e.type == sf::Event::KeyPressed)
    {
        if (e.key.code == sf::Keyboard::Left)
        {
            decrease();
        }
        else if (e.key.code == sf::Keyboard::Right)
        {
            increase();
        }           
    }
    else if (e.type == sf::Event::KeyReleased)
    {
        if (e.key.code == sf::Keyboard::Return)
        {
            deactivate();
        }
    }
    //controller
    else if (e.type == sf::Event::JoystickMoved)
    {
        if (e.joystickMove.axis == sf::Joystick::PovX)
        {
            if (e.joystickMove.position > deadzone)
            {
                increase();
            }
            else if (e.joystickMove.position < -deadzone)
            {
                decrease();
            }
        }
    }
    else if (e.type == sf::Event::JoystickButtonReleased)
    {
        if (e.joystickButton.button == 1)
        {
            deactivate();
        }
    }
    //mouse
    else if (e.type == sf::Event::MouseButtonReleased)
    {
        deactivate();
    }
    else if (e.type == sf::Event::MouseMoved)
    {
        auto localPos = getInverseTransform().transformPoint(mousePos);
        if (m_direction == Direction::Horizontal)
        {
            localPos.x = std::min(localPos.x, m_length);
            localPos.x = std::max(localPos.x, 0.f);
            m_handleSprite.setPosition(localPos.x, m_handleSprite.getPosition().y);
        }
        else
        {
            localPos.y = std::min(localPos.y, m_length);
            localPos.y = std::max(localPos.y, 0.f);
            m_handleSprite.setPosition(m_handleSprite.getPosition().x, localPos.y);
        }
        for (auto& c : m_valueChanged) c(this);
    }
}

void Slider::setAlignment(Alignment a)
{
    switch (a)
    {
    case Alignment::TopLeft:
        setOrigin(0.f, 0.f);
        break;
    case Alignment::BottomLeft:
        setOrigin(0.f, m_slotShape.getSize().y);
        break;
    case Alignment::Centre:
        setOrigin(m_slotShape.getSize() / 2.f);
        break;
    case Alignment::TopRight:
        setOrigin(m_slotShape.getSize().x, 0.f);
        break;
    case Alignment::BottomRight:
        setOrigin(m_slotShape.getSize());
        break;
    default:break;
    }
}

bool Slider::contains(const sf::Vector2f& mousePos) const
{
    return getTransform().transformRect(m_handleSprite.getGlobalBounds()).contains(mousePos);
}

void Slider::setMaxValue(float value)
{
    assert(value > 0);
    m_maxValue = value;
}

void Slider::setDirection(Direction direction)
{
    if (direction != m_direction)
    {
        m_direction = direction;

        float newX = m_slotShape.getSize().y;
        float newY = m_slotShape.getSize().x;
        m_slotShape.setSize({ newX, newY });

        newX = m_handleSprite.getPosition().y;
        newY = m_handleSprite.getPosition().x;
        m_handleSprite.setPosition(newX, newY);

        newX = m_localBounds.height;
        newY = m_localBounds.width;
        m_localBounds.width = newX;
        m_localBounds.height = newY;

        newX = m_localBounds.top;
        newY = m_localBounds.left;
        m_localBounds.left = newX;
        m_localBounds.top = newY;
    }
}

void Slider::setLength(float length)
{
    auto currentSize = m_slotShape.getSize();
    if (m_direction == Direction::Horizontal)
    {
        float pos = m_handleSprite.getPosition().x / currentSize.x;
        pos *= length;
        m_handleSprite.setPosition(pos, m_handleSprite.getPosition().y);

        currentSize.x = length;
        m_slotShape.setSize(currentSize);
        m_localBounds.width = length;
    }
    else
    {
        float pos = m_handleSprite.getPosition().y / currentSize.y;
        pos *= length;
        m_handleSprite.setPosition(m_handleSprite.getPosition().x, pos);

        currentSize.y = length;
        m_slotShape.setSize(currentSize);
        m_localBounds.height = length;
    }
    m_length = length;
}

void Slider::setValue(float value)
{
    assert(value <= m_maxValue && value >= 0);
    auto pos = m_handleSprite.getPosition();
    if (m_direction == Direction::Horizontal)
    {
        pos.x = (m_length / m_maxValue) * value;
    }
    else
    {
        pos.y = (m_length / m_maxValue) * value;
    }
    m_handleSprite.setPosition(pos);
    for (auto& c : m_valueChanged) c(this);
}

float Slider::getValue() const
{
    if (m_direction == Direction::Horizontal)
    {
        return m_handleSprite.getPosition().x / m_length * m_maxValue;
    }
    else
    {
        return m_handleSprite.getPosition().y / m_length * m_maxValue;
    }
}

float Slider::getLength() const
{
    return m_length;
}

void Slider::setText(const std::string& text)
{
    m_text.setString(text);
    updateText();
}

void Slider::setTextColour(const sf::Color& colour)
{
    m_text.setColor(colour);
}

void Slider::setFontSize(sf::Uint16 size)
{
    m_text.setCharacterSize(size);
}

void Slider::setCallback(Slider::Callback c, Event e)
{
    switch (e)
    {
    case Event::SetActive:
        m_setActive = std::move(c);
        break;
    case Event::SetInactive:
        m_setInactive = std::move(c);
        break;
    case Event::ValueChanged:
        m_valueChanged.push_back(std::move(c));
        break;
    default: break;
    }
}

//private
void Slider::draw(sf::RenderTarget& rt, sf::RenderStates states)const
{
    states.transform *= getTransform();
    //states.blendMode = sf::BlendAdd;
    rt.draw(m_slotShape, states);
    states.blendMode = sf::BlendAlpha;
    rt.draw(m_handleSprite, states);
    rt.draw(m_text, states);
    rt.draw(m_valueText, states);
}

void Slider::updateText()
{
    Util::Position::centreOrigin(m_text);
    m_text.setPosition(m_text.getOrigin().x, -m_text.getLocalBounds().height/* * 2.f*/);

    Util::Position::centreOrigin(m_valueText);
    m_valueText.setPosition(m_text.getPosition());
    m_valueText.move(m_length - ((m_text.getLocalBounds().width + m_valueText.getLocalBounds().width) / 2.f), -10.f);
}

void Slider::increase()
{
    auto pos = m_handleSprite.getPosition();    
    if (m_direction == Direction::Horizontal)
    {
        pos.x = std::min(pos.x + (m_length / m_maxValue), m_length);
    }
    else
    {
        pos.y = std::min(pos.y + (m_length / m_maxValue), m_length);
    }
    m_handleSprite.setPosition(pos);

    for (auto& c : m_valueChanged) c(this);
}

void Slider::decrease()
{
    auto pos = m_handleSprite.getPosition();
    if (m_direction == Direction::Horizontal)
    {
        pos.x = std::max(pos.x - (m_length / m_maxValue), 0.f);
    }
    else
    {
        pos.y = std::max(pos.y - (m_length / m_maxValue), 0.f);
    }
    m_handleSprite.setPosition(pos);

    for (auto& c : m_valueChanged) c(this);
}

void Slider::valueChanged(const Slider*)
{
    m_valueText.setString(std::to_string(static_cast<int>(getValue() * (100.f / m_maxValue))));
    updateText();
}