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

#ifndef STATE_IDS_HPP_
#define STATE_IDS_HPP_

namespace States
{
    enum class ID
    {
        None,
        Game,
        MenuBackground,
        MenuMain,
        MenuLobby,
        MenuJoin,
        MenuOptions,
        MenuPaused,
        Score,
        Stats,
        Title
    };
}

//TODO there should be a more intuitive place to put these
enum class Difficulty
{
    Easy,
    Medium,
    Hard
};

#endif //STATE_IDS_HPP_