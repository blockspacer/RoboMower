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

//stack of game states

#ifndef STATESTACK_HPP_
#define STATESTACK_HPP_

#include <xygine/State.hpp>

#include <map>
#include <functional>
#include <vector>

namespace sf
{
    class RenderWindow;
    class Event;
}

class StateStack final
{
public:
    enum class Action
    {
        Push,
        Pop,
        Clear
    };

    explicit StateStack(State::Context context);
    ~StateStack() = default;
    StateStack(const StateStack&) = delete;
    const StateStack& operator = (const StateStack&) = delete;

    template <typename T>
    void registerState(States::ID id)
    {
        m_factories[id] = [this]()
        {
            return std::make_unique<T>(*this, m_context);
        };
    }

    void update(float dt);
    void draw();
    void handleEvent(const sf::Event& evt);
    void handleMessage(const Message&);

    void pushState(States::ID id);
    void popState();
    void clearStates();

    bool empty() const;

    sf::View updateView();

    void applyPendingChanges();

private:

    struct Pendingchange
    {
        explicit Pendingchange(Action, States::ID id = States::ID::None);
        Action action;
        States::ID id;
    };

    std::vector<State::Ptr> m_stack;
    std::vector<Pendingchange> m_pendingChanges;
    State::Context m_context;
    std::map<States::ID, std::function<State::Ptr()>> m_factories;
    State::Ptr createState(States::ID id);

};

#endif //STATESTACK_HPP_