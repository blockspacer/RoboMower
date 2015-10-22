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

#ifndef UI_BUTTON_HPP_
#define UI_BUTTON_HPP_

#include <xygine/ui/Control.hpp>
#include <xygine/Resource.hpp>

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

#include <vector>
#include <string>
#include <memory>
#include <functional>

namespace xy
{
    namespace ui
    {
        class Button final : public Control
        {
        public:
            using Ptr = std::shared_ptr<Button>;
            using Callback = std::function<void()>;

            Button(const sf::Font& font, const sf::Texture& texture);
            ~Button() = default;

            bool selectable() const override;
            void select() override;
            void deselect() override;

            void activate() override;
            void deactivate() override;

            void handleEvent(const sf::Event& e, const sf::Vector2f& mousePos) override;

            void setAlignment(Alignment a) override;
            bool contains(const sf::Vector2f& mousePos)const override;

            void setCallback(Callback c);
            void setText(const std::string& text);
            void setTextColour(const sf::Color& c);
            void setFontSize(sf::Uint16 size);
            void setTogglable(bool b);


        private:
            enum State
            {
                Normal = 0,
                Selected,
                Active,
                Count
            };

            Callback m_callback;
            const sf::Texture& m_texture;
            sf::Sprite m_sprite;
            sf::Text m_text;
            bool m_toggleButton;

            std::vector<sf::IntRect> m_subRects;

            void draw(sf::RenderTarget& rt, sf::RenderStates states) const override;
        };
    }
}
#endif //UI_BUTTON_HPP_