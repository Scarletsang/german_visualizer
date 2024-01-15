#include "Markdown.hpp"

#include <iostream>

#include "Encoder.hpp"
#include "Encoder/Markdown/Settings.hpp"
#include "Elements.hpp"

MarkdownEncoder::MarkdownEncoder(std::ostream& stream, const MarkdownEncoderSettings& settings)
  : output_(stream), settings_(settings) {}

MarkdownEncoder::MarkdownEncoder(const MarkdownEncoder& object)
  : output_(object.output_), settings_(object.settings_) {}

MarkdownEncoder::~MarkdownEncoder() {}

int MarkdownEncoder::encode(const Character& character)
{
  output_ << character.data();
  return EXIT_SUCCESS;
}

int MarkdownEncoder::encode(const Word& word)
{
  output_ << word.data();
  return EXIT_SUCCESS;
}

int MarkdownEncoder::encode(const Number& number)
{
  output_ << number.data();
  return EXIT_SUCCESS;
}

int MarkdownEncoder::encode(const Sentence& sentence)
{
  for (const auto& element : sentence.data())
  {
    element->encode(*this);
  }
  output_ << " ";
  return EXIT_SUCCESS;
}
