//
// Created by Christian Alexander Marrero on 4/15/25.
//

#ifndef SELECTABLE_H
#define SELECTABLE_H

#include <queue>

#include "SFML/Graphics/Rect.hpp"

enum SelectableEvent {
    SELECTED,
    CLICKED,
    UNSELECTED,
};

class SelectableRegion : sf::Rect<int> {
    std::queue<SelectableEvent> events;
    public:
    SelectableRegion(
        const sf::Vector2f& position,
        const sf::Vector2f& size
    );
    bool contains(const sf::Vector2f& position) const;
};



#endif //SELECTABLE_H
