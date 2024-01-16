#include "Tokenizer.hpp"

#include <iostream>

Tokenizer::Tokenizer(std::istream& stream)
  : stream_(stream) {}

const std::istream& Tokenizer::stream() const
{
  return stream_;
}

std::streampos Tokenizer::snapshot() const
{
  return stream_.tellg();
}

void  Tokenizer::rollback(std::streampos position)
{
  stream_.clear();
  stream_.seekg(position);
}

void  Tokenizer::debug() {}
