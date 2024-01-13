#include "Document.hpp"

#include "Encoder.hpp"
#include "Element/Collection.hpp"
#include "Element/Element.hpp"

Document::Document()
  : Collection<MarkdownElement>() {}

Document::Document(const Document& object)
  : Collection<MarkdownElement>(object) {}

Document::~Document() {}

Document& Document::operator=(const Document& other)
{
  if (this != &other)
    Collection<MarkdownElement>::operator=(other);
  return *this;
}

int Document::encode(Encoder& encoder) const
{
  return encoder.encode(*this);
}

bool  Document::is_atom() const
{
  return false;
}
