#include "Document.hpp"

#include "Encoder.hpp"
#include "Ast/DomCollection.hpp"
#include "Ast/Element.hpp"

Document::Document()
  : DomCollection<MarkdownElement>() {}

Document::Document(const Document& object)
  : DomCollection<MarkdownElement>(object) {}

Document::~Document() {}

Document& Document::operator=(const Document& other)
{
  if (this != &other)
    DomCollection<MarkdownElement>::operator=(other);
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
