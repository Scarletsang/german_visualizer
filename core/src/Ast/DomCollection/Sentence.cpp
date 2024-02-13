#include "Sentence.hpp"

#include <vector>
#include <memory>

#include "Encoder.hpp"
#include "Ast/Element.hpp"
#include "Ast/Atom/Word.hpp"
#include "Ast/Atom/Number.hpp"
#include "Ast/Atom/Character.hpp"

Sentence::Sentence()
  : DomCollection() {}

Sentence::Sentence(const Sentence& object)
  : DomCollection(object) {}

Sentence::~Sentence() {}

Sentence& Sentence::operator=(const Sentence& other)
{
  if (this != &other)
    DomCollection::operator=(other);
  return *this;
}

int Sentence::encode(Encoder& encoder) const
{
  return encoder.encode(*this);
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
  else if (dynamic_cast<Delimiter*>(element.get()))
    add_element(std::static_pointer_cast<Delimiter>(element));
  else if (dynamic_cast<Character*>(element.get()))
    add_element(std::static_pointer_cast<Character>(element));
  else if (dynamic_cast<Number*>(element.get()))
    add_element(std::static_pointer_cast<Number>(element));
  else
    return EXIT_FAILURE;
  return EXIT_SUCCESS;
}
