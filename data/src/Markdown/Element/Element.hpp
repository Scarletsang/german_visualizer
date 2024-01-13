#pragma once

class Encoder;

/**
 * @brief This represents an object in the markdown document.
*/
class MarkdownElement
{
  public:
    virtual ~MarkdownElement() = 0;

    virtual int encode(Encoder& encoder) const = 0;
    virtual bool  is_atom() const = 0;
};
