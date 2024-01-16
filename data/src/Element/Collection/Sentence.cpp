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

int  Sentence::add_element(std::shared_ptr<Word> word)
{
  data_.push_back(word);
  return EXIT_SUCCESS;
}

int  Sentence::add_element(std::shared_ptr<Number> number)
{
  data_.push_back(number);
  return EXIT_SUCCESS;
}

int  Sentence::add_element(std::shared_ptr<Character> character)
{
  data_.push_back(character);
  return EXIT_SUCCESS;
}

int  Sentence::add_element(std::shared_ptr<Delimiter> delimiter)
{
  data_.push_back(delimiter);
  return EXIT_SUCCESS;
}

int  Sentence::add_element(std::shared_ptr<MarkdownElement> element)
{
  if (dynamic_cast<Word*>(element.get()))
    add_element(std::static_pointer_cast<Word>(element));
  else if (dynamic_cast<Number*>(element.get()))
    add_element(std::static_pointer_cast<Number>(element));
  else if (dynamic_cast<Character*>(element.get()))
    add_element(std::static_pointer_cast<Character>(element));
  else if (dynamic_cast<Delimiter*>(element.get()))
    add_element(std::static_pointer_cast<Delimiter>(element));
  else
    return EXIT_FAILURE;
  return EXIT_SUCCESS;
}
