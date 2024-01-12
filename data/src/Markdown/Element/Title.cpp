#include "Title.hpp"

Title::Title() : level_(), line_() {}

Title::Title(int level) : level_(level), line_() {}

Title::Title(int level, std::shared_ptr<Line> line)
  : level_(level), line_(line) {}

Title::Title(const Title& object)
  : level_(object.level_), line_(object.line_) {}

Title::~Title() {}

Title& Title::operator=(const Title& other)
{
  if (this != &other)
  {
    level_ = other.level_;
    line_ = other.line_;
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

std::shared_ptr<Line> Title::line() const
{
  return line_;
}

void Title::set_level(int level)
{
  level_ = level;
}

void Title::set_line(std::shared_ptr<Line> line)
{
  line_ = line;
}
