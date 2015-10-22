//==============================================================================
// RoboMower - Copyright (C) Matt Marchant; All Rights Reserved
// Unauthorized copying of this file via any medium is strictly prohibited
// Proprietary and confidential
// Written by Matt Marchant (matty_styles@hotmail.com) 2015
//==============================================================================

#include <xygine/ui/UIScoreList.hpp>
#include <xygine/Util.hpp>

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

#include <cassert>

using namespace ui;

namespace
{
    const float scrollSpeed = 350.f;
    const float verticalSpace = 50.f;
}

ScoreList::ScoreList(const sf::Font& font)
    : m_bounds              ({}, { 920.f, 420.f }),
    m_font                  (font),
    m_scrollTargetDistance  (0.f),
    m_scrollCurrentDistance (0.f),
    m_scrollSpeed           (0.f),
    m_doScroll              (false)
{

}

//public
bool ScoreList::selectable() const
{
    return false;
}

void ScoreList::select(){}
void ScoreList::deselect(){}
void ScoreList::activate(){}
void ScoreList::deactivate(){}
void ScoreList::handleEvent(const sf::Event&, const sf::Vector2f&){}

void ScoreList::update(float dt)
{
    if (m_doScroll)
    {
        const float centre = m_bounds.height / 2.f;
        const float scrollAmount = (m_scrollSpeed * (m_scrollCurrentDistance / m_scrollTargetDistance)) * dt;

        if (m_texts[0].getPosition().y + scrollAmount > centre)
        {
            m_doScroll = false;
            return;
        }
        if (m_texts.back().getPosition().y + scrollAmount < centre)
        {
            m_doScroll = false;
            return;
        }

        updateTexts(scrollAmount);
        
        m_scrollCurrentDistance -= scrollAmount;
    }
}

void ScoreList::setAlignment(Alignment a)
{
    switch (a)
    {
    case Alignment::BottomLeft:
        setOrigin(m_bounds.left, m_bounds.top + m_bounds.height);
        break;
    case Alignment::BottomRight:
        setOrigin(m_bounds.left + m_bounds.width, m_bounds.top + m_bounds.height);
        break;
    case Alignment::Centre:
        setOrigin((m_bounds.left + m_bounds.width) / 2.f, (m_bounds.top + m_bounds.height) / 2.f);
        break;
    case Alignment::TopLeft:
        setOrigin(m_bounds.left, m_bounds.top);
        break;
    case Alignment::TopRight:
        setOrigin(m_bounds.left + m_bounds.width, m_bounds.top);
        break;
    default: break;
    }
}

bool ScoreList::contains(const sf::Vector2f& mousePos) const
{
    return getTransform().transformRect(m_bounds).contains(mousePos);
}

void ScoreList::scroll(float amount)
{
    assert(amount != 0);
    m_scrollTargetDistance = amount;
    m_scrollCurrentDistance = m_scrollTargetDistance;
    m_scrollSpeed = (amount > 0) ? scrollSpeed : -scrollSpeed;
    m_doScroll = true;
}

void ScoreList::setList(const std::vector<Scores::Item>& list)
{
    auto centre = sf::Vector2f(m_bounds.width / 2.f, m_bounds.height / 2.f);
    for (auto i = 0u; i < list.size(); ++i)
    {
        m_texts.emplace_back(std::to_string(i + 1) + ".    " + std::string(list[i].name) + " - " + std::to_string(list[i].score),m_font, 42u);
        auto& text = m_texts.back();
        Util::Position::centreOrigin(text);
        text.setPosition(centre);
        text.move(0.f, (verticalSpace) * i);
    }

    updateTexts(0.f);
}

void ScoreList::setIndex(sf::Uint32 index)
{
    if (index < m_texts.size()) //texts might be empty
    {
        const float centre = m_bounds.height / 2.f;
        const float diff = centre - m_texts[index].getPosition().y;
        for (auto& text : m_texts)
        {
            text.move(0.f, diff);
            text.setStyle(sf::Text::Regular);
        }
        m_texts[index].setStyle(sf::Text::Bold | sf::Text::Italic);
        m_texts[index].setColor({ 255u, 240u, 200u });

        const float dist = centre - m_texts[index].getPosition().y;
        updateTexts(dist);
    }
}

float ScoreList::getVerticalSpacing() const
{
    return verticalSpace;
}

//private
void ScoreList::draw(sf::RenderTarget& rt, sf::RenderStates states) const
{
    states.transform *= getTransform();
    for (const auto& text : m_texts)
    {
        if (text.getGlobalBounds().intersects(m_bounds))
        {
            rt.draw(text, states);
        }
    }
}

void ScoreList::updateTexts(float scrollAmount)
{
    const float centre = m_bounds.height / 2.f;

    for (auto& text : m_texts)
    {
        text.move(0.f, scrollAmount);
        const float diff = std::abs(centre - text.getPosition().y);
        const float ratio = 1.f - (diff / m_bounds.height);
        sf::Color colour = text.getColor();
        colour.a = static_cast<sf::Uint8>(ratio * 255.f);
        text.setColor(colour);
        text.setScale(ratio, ratio);
    }
}