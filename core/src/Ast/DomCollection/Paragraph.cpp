#include "Paragraph.hpp"

#include "Encoder.hpp"
#include "Ast/DomCollection.hpp"
#include "Ast/DomCollection/Sentence.hpp"

Paragraph::Paragraph()
  : DomCollection<Sentence>() {}

Paragraph::Paragraph(const Paragraph& object)
  : DomCollection<Sentence>(object) {}

Paragraph::~Paragraph() {}

Paragraph& Paragraph::operator=(const Paragraph& other)
{
  if (this != &other)
    DomCollection<Sentence>::operator=(other);
  return *this;
}

int Paragraph::encode(Encoder& encoder) const
{
  return encoder.encode(*this);
}
