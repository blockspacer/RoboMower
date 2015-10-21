//==============================================================================
// RoboMower - Copyright (C) Matt Marchant; All Rights Reserved
// Unauthorized copying of this file via any medium is strictly prohibited
// Proprietary and confidential
// Written by Matt Marchant (matty_styles@hotmail.com) 2015
//==============================================================================

#include <UIButton.hpp>
#include <Util.hpp>

#include <SFML/Graphics/RenderTarget.hpp>

using namespace ui;

Button::Button(const sf::Font& font, const sf::Texture& texture)
    : m_texture     (texture),
    m_text          ("", font, 30u),
    m_toggleButton  (false)
{
    sf::IntRect subrect({ 0, 0 }, sf::Vector2i(texture.getSize()));
    subrect.height /= State::Count;

    for (auto i = 0; i < State::Count; ++i)
    {
        m_subRects.push_back(subrect);
        subrect.top += subrect.height;
    }

    m_sprite.setTexture(m_texture);
    m_sprite.setTextureRect(m_subRects[State::Normal]);
    
    auto bounds = m_sprite.getLocalBounds();
    m_text.setPosition(bounds.width / 2.f, bounds.height / 2.f);
}

//public
bool Button::selectable() const
{
    return true;
}

void Button::select()
{
    Control::select();
    m_sprite.setTextureRect(m_subRects[State::Selected]);
}

void Button::deselect()
{
    Control::deselect();
    m_sprite.setTextureRect(m_subRects[State::Normal]);
}

void Button::activate()
{
    Control::activate();
    if (m_toggleButton)
        m_sprite.setTextureRect(m_subRects[State::Active]);

    if (m_callback) m_callback();

    if (!m_toggleButton)
        deactivate();
}

void Button::deactivate()
{
    Control::deactivate();
    if (m_toggleButton)
    {
        if (selected())
        {
            m_sprite.setTextureRect(m_subRects[State::Selected]);
        }
        else
        {
            m_sprite.setTextureRect(m_subRects[State::Normal]);
        }
    }
}

void Button::handleEvent(const sf::Event& e, const sf::Vector2f& mousePos)
{

}

void Button::setAlignment(Alignment a)
{
    switch (a)
    {
    case Alignment::TopLeft:
        setOrigin(0.f, 0.f);
        break;
    case Alignment::BottomLeft:
        setOrigin(0.f, static_cast<float>(m_subRects[0].height));
        break;
    case Alignment::Centre:
        setOrigin(static_cast<float>(m_subRects[0].width / 2), static_cast<float>(m_subRects[0].height / 2));
        break;
    case Alignment::TopRight:
        setOrigin(static_cast<float>(m_subRects[0].width), 0.f);
        break;
    case Alignment::BottomRight:
        setOrigin(static_cast<float>(m_subRects[0].width), static_cast<float>(m_subRects[0].height));
        break;
    default:break;
    }
}

bool Button::contains(const sf::Vector2f& mousePos) const
{
    return getTransform().transformRect(m_sprite.getGlobalBounds()).contains(mousePos);
}

void Button::setCallback(Callback cb)
{
    m_callback = std::move(cb);
}

void Button::setText(const std::string& text)
{
    m_text.setString(text);
    Util::Position::centreOrigin(m_text);
}

void Button::setTextColour(const sf::Color& colour)
{
    m_text.setColor(colour);
}

void Button::setFontSize(sf::Uint16 size)
{
    m_text.setCharacterSize(size);
    Util::Position::centreOrigin(m_text);
}

void Button::setTogglable(bool b)
{
    m_toggleButton = b;
}

//private
void Button::draw(sf::RenderTarget& rt, sf::RenderStates states) const
{
    states.transform *= getTransform();
    rt.draw(m_sprite, states);
    rt.draw(m_text, states);
}