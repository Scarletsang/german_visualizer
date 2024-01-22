#include "Title.hpp"

#include <memory>

#include "Encoder.hpp"
#include "Ast/Element.hpp"
#include "Ast/DomCollection.hpp"

Title::Title() : level_(), data_() {}

Title::Title(int level) : level_(level), data_() {}

Title::Title(int level, std::shared_ptr<Paragraph> paragraph)
  : level_(level), data_(paragraph) {}

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

int Title::encode(Encoder& encoder) const
{
  return encoder.encode(*this);
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
