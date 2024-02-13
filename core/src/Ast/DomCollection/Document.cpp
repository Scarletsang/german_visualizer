#include "Document.hpp"

#include "Encoder.hpp"
#include "Ast/DomCollection.hpp"
#include "Ast/Element.hpp"

Document::Document()
  : DomCollection() {}

Document::Document(const Document& object)
  : DomCollection(object) {}

Document::~Document() {}

Document& Document::operator=(const Document& other)
{
  if (this != &other)
    DomCollection::operator=(other);
  return *this;
}

int Document::encode(Encoder& encoder) const
{
  return encoder.encode(*this);
}
