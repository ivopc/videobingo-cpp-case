#pragma once

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Vector2.hpp>

#include <string>
#include <vector>

namespace sf {
class RenderTarget;
}

namespace bingo::render {
class HistoryPanel {
public:
    HistoryPanel(const sf::Font& font, sf::Vector2f position, float lineHeight, unsigned int fontSize,
                 std::size_t maxVisible);

    void setLines(const std::vector<std::string>& lines);

    void draw(sf::RenderTarget& target) const;

private:
    sf::Text header_;
    std::vector<sf::Text> lineTexts_;
};

}
