#include "Tokenizer.hpp"

#include <iostream>

Tokenizer::Tokenizer(std::istream& stream)
  : stream_(stream) {}

const std::istream& Tokenizer::stream() const
{
  return stream_;
}

std::istream::pos_type Tokenizer::snapshot() const
{
  return stream_.tellg();
}

void  Tokenizer::rollback(std::istream::pos_type position)
{
  stream_.clear();
  stream_.seekg(position);
}
