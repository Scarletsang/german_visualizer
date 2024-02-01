#pragma once

#include <map>
#include <vector>

#include "Encoder.hpp"
#include "Ast/Type.hpp"
#include "Ast/Element.hpp"
#include "Ast/GraphCollection/Edge.hpp"

class Graph : public MarkdownElement
{
  public:
    using GraphElementId = int;
    Graph() = default;
    Graph(const Graph&) = default;
    ~Graph() = default;
    Graph& operator=(const Graph&) = default;

    virtual int encode(Encoder& encoder) const override;
    virtual ElementType type() const override;
  
  protected:
    std::map<GraphElementId, std::vector<GraphElementId> > edges_;
};
