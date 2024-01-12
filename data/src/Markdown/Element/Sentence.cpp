#include "Sentence.hpp"

#include <iostream>
#include <vector>
#include <memory>

#include "Encoder.hpp"
#include "Atom.hpp"

Sentence::Sentence() {}

Sentence::Sentence(const Sentence& object)
  : data_(object.data_) {}

Sentence::~Sentence() {}

Sentence& Sentence::operator=(const Sentence& other)
{
  if (this != &other)
    data_ = other.data_;
  return *this;
}

int Sentence::encode(Encoder& encoder, std::istream& input) const
{
  return encoder.encode(*this, input);
}

bool  Sentence::is_atom() const
{
  return false;
}

void  Sentence::add_word(std::weak_ptr<Word> word)
{
  data_.push_back(word);
}

void  Sentence::add_number(std::weak_ptr<Number> number)
{
  data_.push_back(number);
}

void  Sentence::add_character(std::weak_ptr<Character> character)
{
  data_.push_back(character);
}

const std::vector<Sentence::element>& Sentence::data() const
{
  return data_;
}

std::vector<Sentence::element>& Sentence::data()
{
  return data_;
}
