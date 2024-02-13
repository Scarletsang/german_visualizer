#include "Word.hpp"

Word::Word()
  : data_() {}

Word::Word(const std::string& word)
  : data_(word) {}

Word::Word(const Word& object)
  : data_(object.data_) {}

Word::~Word() {}

Word& Word::operator=(const Word& other)
{
  if (this != &other)
    data_ = other.data_;
  return *this;
}

int Word::encode(Encoder& encoder) const
{
  return encoder.encode(*this);
}

ElementType Word::type() const
{
  return ElementType::kAtom;
}

void  Word::set_data(const std::string& word)
{
  data_ = word;
}

const std::string& Word::data() const
{
  return data_;
}
