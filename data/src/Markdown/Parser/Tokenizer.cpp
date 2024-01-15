#include "Tokenizer.hpp"

#include <iostream>

Tokenizer::Tokenizer(std::istream& stream)
  : stream_(stream) {}

const std::istream& Tokenizer::stream() const
{
  return stream_;
}
