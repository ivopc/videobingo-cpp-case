#pragma once

#include "domain/DrawResult.hpp"

#include <cstddef>
#include <deque>
#include <string>

namespace bingo::data {
struct HistoryEntry {
    domain::DrawResult result;
    std::string timestamp;
};

class HistoryBuffer {
public:
    explicit HistoryBuffer(std::size_t maxItems);

    void push(HistoryEntry entry);

    const std::deque<HistoryEntry>& items() const;

private:
    std::size_t maxItems_;
    std::deque<HistoryEntry> items_;
};

}
