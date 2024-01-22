#pragma once

#include "Encoder.hpp"
#include "Ast/Type.hpp"

/**
 * @brief This represents an object in the markdown document.
*/
class MarkdownElement
{
  public:
    virtual ~MarkdownElement() {};

    virtual int encode(Encoder& encoder) const = 0;
    virtual ElementType type() const = 0;
};
