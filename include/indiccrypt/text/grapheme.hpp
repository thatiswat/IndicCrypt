#pragma once

#include <cstddef>
#include <string>
#include <string_view>
#include <vector>

namespace indiccrypt::text {

struct GraphemeCluster {
    std::size_t utf160ffset;
    std::size_t utf16Length;
    std::string utf8;
};

class GraphemeEngine {
public:
    [[nodiscard]]
    static std::vector<GraphemeCluster>
    segment(std::string_view utf8);
};

} // namespace indiccrypt::text