#include "Object.hpp"

#include <stdexcept>
#include <string>
#include <memory>

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

//////////////////////////////////
////////////   View   ////////////
//////////////////////////////////

MarkdownObject::View::View(MarkdownObject& object)
  : object_(object) {}

MarkdownObject::View::~View() {}
