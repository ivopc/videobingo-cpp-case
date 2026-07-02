#include "data/HistoryBuffer.hpp"
#include "domain/DrawResult.hpp"

#include <cassert>
#include <cstdio>

using bingo::data::HistoryBuffer;
using bingo::data::HistoryEntry;
using bingo::domain::DrawResult;

namespace {

void testNewestEntryComesFirst() {
    HistoryBuffer buffer(5);

    buffer.push(HistoryEntry{DrawResult{{1, 2, 3, 4}}, "10:00:00"});
    buffer.push(HistoryEntry{DrawResult{{5, 6, 7, 8}}, "10:00:05"});

    const auto& items = buffer.items();
    assert(items.size() == 2);
    assert(items[0].timestamp == "10:00:05" && "most recently pushed entry must be first");
    assert(items[1].timestamp == "10:00:00");
}

void testOldestEntryDropsOnceOverCapacity() {
    HistoryBuffer buffer(2);

    buffer.push(HistoryEntry{DrawResult{{0, 1, 2, 3}}, "a"});
    buffer.push(HistoryEntry{DrawResult{{1, 2, 3, 4}}, "b"});
    buffer.push(HistoryEntry{DrawResult{{2, 3, 4, 5}}, "c"});

    const auto& items = buffer.items();
    assert(items.size() == 2 && "buffer must never exceed maxItems");
    assert(items[0].timestamp == "c");
    assert(items[1].timestamp == "b");
}

void testEmptyBufferHasNoItems() {
    const HistoryBuffer buffer(5);
    assert(buffer.items().empty());
}

}

int main() {
    testNewestEntryComesFirst();
    testOldestEntryDropsOnceOverCapacity();
    testEmptyBufferHasNoItems();

    std::printf("HistoryBufferTests: all tests passed\n");
    return 0;
}
