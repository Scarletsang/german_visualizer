#include "Line.hpp"

#include <iostream>
#include <vector>
#include <memory>

#include "Encoder.hpp"
#include "Atom.hpp"

Line::Line() {}

Line::Line(const Line& object)
  : data_(object.data_) {}

Line::~Line() {}

Line& Line::operator=(const Line& other)
{
  if (this != &other)
    data_ = other.data_;
  return *this;
}

int Line::encode(Encoder& encoder, std::istream& input) const
{
  return encoder.encode(*this, input);
}

bool  Line::is_atom() const
{
  return false;
}

void  Line::add_sentence(std::shared_ptr<Sentence> sentence)
{
  data_.push_back(sentence);
}

const std::vector<Line::element>& Line::data() const
{
  return data_;
}

std::vector<Line::element>& Line::data()
{
  return data_;
}
