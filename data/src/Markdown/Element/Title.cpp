#include "Title.hpp"

#include <iostream>
#include <memory>

#include "Element.hpp"
#include "Encoder.hpp"
#include "Collection.hpp"

Title::Title() : level_(), data_() {}

Title::Title(int level) : level_(level), data_() {}

Title::Title(int level, std::shared_ptr<Paragraph> line)
  : level_(level), data_(line) {}

Title::Title(const Title& object)
  : level_(object.level_), data_(object.data_) {}

Title::~Title() {}

Title& Title::operator=(const Title& other)
{
  if (this != &other)
  {
    level_ = other.level_;
    data_ = other.data_;
  }
  return *this;
}

int Title::encode(Encoder& encoder, std::istream& input) const
{
  return encoder.encode(*this, input);
}

bool Title::is_atom() const
{
  return false;
}

int Title::level() const
{
  return level_;
}

std::shared_ptr<Paragraph> Title::data() const
{
  return data_;
}

void Title::set_level(int level)
{
  level_ = level;
}

void Title::set_data(std::shared_ptr<Paragraph> paragraph)
{
  data_ = paragraph;
}
