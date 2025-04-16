#pragma once

#include "json.hpp"
using json = nlohmann::json;

struct Grid {
    int size_grid;
    std::vector<int> origin;
    std::vector<std::vector<int>> path;
};

inline void to_json(json& j, const Grid& grid) {
    j = json{ {"size_grid", grid.size_grid}, {"origin", grid.origin}, {"path", grid.path} };
}

inline void from_json(const json& j, Grid& grid) {
    j.at("size_grid").get_to(grid.size_grid);
    j.at("origin").get_to(grid.origin);
    j.at("path").get_to(grid.path);
}
    
    

