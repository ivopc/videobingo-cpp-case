#pragma once

#include <string>

namespace bingo::infra {

class TimeService {
public:
    // '2026-06-30 11:35:00'
    std::string nowDateTime() const;

    // '11:35:00'
    std::string nowTime() const;
};

}
