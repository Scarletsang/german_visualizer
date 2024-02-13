#include "Paragraph.hpp"

#include "Encoder.hpp"
#include "Ast/DomCollection.hpp"
#include "Ast/DomCollection/Sentence.hpp"

Paragraph::Paragraph()
  : DomCollection() {}

Paragraph::Paragraph(const Paragraph& object)
  : DomCollection(object) {}

Paragraph::~Paragraph() {}

Paragraph& Paragraph::operator=(const Paragraph& other)
{
  if (this != &other)
    DomCollection::operator=(other);
  return *this;
}

int Paragraph::encode(Encoder& encoder) const
{
  return encoder.encode(*this);
}

const std::vector<std::shared_ptr<Sentence>>& Paragraph::data() const
{
  return reinterpret_cast<const std::vector<std::shared_ptr<Sentence>>&>(data_);
}
