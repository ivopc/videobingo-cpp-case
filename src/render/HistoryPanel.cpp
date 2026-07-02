#include "HistoryPanel.hpp"

#include <SFML/Graphics/RenderTarget.hpp>

#include <algorithm>

namespace bingo::render {

HistoryPanel::HistoryPanel(const sf::Font& font, sf::Vector2f position, float lineHeight, unsigned int fontSize,
                            std::size_t maxVisible)
    : header_(font, "Ultimos sorteios:", fontSize) {
    header_.setPosition(position);

    lineTexts_.reserve(maxVisible);
    for (std::size_t i = 0; i < maxVisible; ++i) {
        lineTexts_.emplace_back(font, "", fontSize);
        lineTexts_.back().setPosition({position.x, position.y + lineHeight * static_cast<float>(i + 1)});
    }
}

void HistoryPanel::setLines(const std::vector<std::string>& lines) {
    const std::size_t visibleCount = std::min(lines.size(), lineTexts_.size());

    for (std::size_t i = 0; i < lineTexts_.size(); ++i) {
        lineTexts_[i].setString(i < visibleCount ? lines[i] : "");
    }
}

void HistoryPanel::draw(sf::RenderTarget& target) const {
    target.draw(header_);
    for (const auto& text : lineTexts_) {
        target.draw(text);
    }
}

}
