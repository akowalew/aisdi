#pragma once

#include <iosfwd>

#include "aisdi/Graph.hpp"

namespace aisdi {

aisdi::Graph load_graph(std::istream& is);

} // aisdi
