#include "Sentence.hpp"

#include <vector>
#include <memory>

#include "Encoder.hpp"
#include "Element/Element.hpp"
#include "Element/Atom/Word.hpp"
#include "Element/Atom/Number.hpp"
#include "Element/Atom/Character.hpp"

Sentence::Sentence()
  : Collection<MarkdownElement>() {}

Sentence::Sentence(const Sentence& object)
  : Collection<MarkdownElement>(object) {}

Sentence::~Sentence() {}

Sentence& Sentence::operator=(const Sentence& other)
{
  if (this != &other)
    Collection<MarkdownElement>::operator=(other);
  return *this;
}

int Sentence::encode(Encoder& encoder) const
{
  return encoder.encode(*this);
}

bool  Sentence::is_atom() const
{
  return false;
}

void  Sentence::add_word(std::shared_ptr<Word> word)
{
  data_.push_back(word);
}

void  Sentence::add_number(std::shared_ptr<Number> number)
{
  data_.push_back(number);
}

void  Sentence::add_character(std::shared_ptr<Character> character)
{
  data_.push_back(character);
}
