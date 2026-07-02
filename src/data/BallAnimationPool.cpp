#include "BallAnimationPool.hpp"

#include <algorithm>

namespace bingo::data {

bool BallAnimationPool::allCompleted() const {
    return std::all_of(completed.begin(), completed.end(), [](bool done) { return done; });
}

}
