#include "Object.hpp"

#include <stdexcept>

////////////////////////////////////////////
////////////   MarkdownObject   ////////////
////////////////////////////////////////////

MarkdownObject::MarkdownObject() {}

MarkdownObject::MarkdownObject(const MarkdownObject& other) {}

MarkdownObject::~MarkdownObject() {}

MarkdownObject* MarkdownObject::view(Perspective& perspective) const
{
  return perspective.apply(*this);
}

MarkdownType MarkdownObject::get_type() const
{
  throw std::runtime_error("MarkdownObject: get_type method not implemented");
}

////////////////////////////////////////////
////////////   Perspective   ///////////////
////////////////////////////////////////////

Perspective::Perspective() {}

Perspective::Perspective(const Perspective& other) {}

Perspective::~Perspective() {}

MarkdownObject* Perspective::apply(const MarkdownObject& object) const
{
  throw std::runtime_error("Perspective: apply method not implemented");
}
