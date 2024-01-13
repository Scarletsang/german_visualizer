#pragma once

#include "Encoder.hpp"

/**
 * @brief This represents an object in the markdown document.
*/
class MarkdownElement
{
  public:
    virtual ~MarkdownElement() {};

    virtual int encode(Encoder& encoder) const = 0;
    virtual bool  is_atom() const = 0;
};
