#include "Object.hpp"

#include <stdexcept>
#include <string>

////////////////////////////////////////////
////////////   MarkdownObject   ////////////
////////////////////////////////////////////

MarkdownObject::MarkdownObject() {}

MarkdownObject::MarkdownObject(const MarkdownObject& other) {}

MarkdownObject::~MarkdownObject() {}

MarkdownType MarkdownObject::get_type() const
{
  throw std::runtime_error("MarkdownObject: get_type method not implemented");
}
