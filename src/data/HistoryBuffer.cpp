#include "HistoryBuffer.hpp"

#include <utility>

namespace bingo::data {

HistoryBuffer::HistoryBuffer(std::size_t maxItems) : maxItems_(maxItems) {}

void HistoryBuffer::push(HistoryEntry entry) {
    items_.push_front(std::move(entry));
    while (items_.size() > maxItems_) {
        items_.pop_back();
    }
}

const std::deque<HistoryEntry>& HistoryBuffer::items() const {
    return items_;
}

}
