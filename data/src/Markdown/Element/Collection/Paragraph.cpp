#include "Paragraph.hpp"

#include "Encoder.hpp"
#include "Element/Collection.hpp"
#include "Element/Collection/Sentence.hpp"

Paragraph::Paragraph()
  : Collection<Sentence>() {}

Paragraph::Paragraph(const Paragraph& object)
  : Collection<Sentence>(object) {}

Paragraph::~Paragraph() {}

Paragraph& Paragraph::operator=(const Paragraph& other)
{
  if (this != &other)
    Collection<Sentence>::operator=(other);
  return *this;
}

int Paragraph::encode(Encoder& encoder) const
{
  return encoder.encode(*this);
}

bool  Paragraph::is_atom() const
{
  return false;
}
