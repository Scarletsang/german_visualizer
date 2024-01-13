#include "Json.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <memory>

#include "Elements.hpp"

JsonEncoder::JsonEncoder(std::ostream& output, const JsonEncoderSettings& settings)
  : output_(output), settings_(settings) {}

JsonEncoder::JsonEncoder(const JsonEncoder& object)
  : output_(object.output_), settings_(object.settings_) {}

JsonEncoder::~JsonEncoder() {}

int JsonEncoder::encode(const Character& character)
{
  output_ << '{';
  {
    settings_.increase_level();
    write_newline_indent();
    write_pair("type", "character", true);
    write_newline_indent();
    write_pair("value", character.data(), false);
    settings_.decrease_level();
  }
  write_newline_indent();
  output_ << '}';
  return EXIT_SUCCESS;
}

int JsonEncoder::encode(const Word& word)
{
  output_ << '{';
  {
    settings_.increase_level();
    write_newline_indent();
    write_pair("type", "word", true);
    write_newline_indent();
    write_pair("value", word.data(), false);
    settings_.decrease_level();
  }
  write_newline_indent();
  output_ << '}';
  return EXIT_SUCCESS;
}

int JsonEncoder::encode(const Number& number)
{
  output_ << '{';
  {
    settings_.increase_level();
    write_newline_indent();
    write_pair("type", "number", true);
    write_newline_indent();
    write_pair("value", number.data(), false);
    settings_.decrease_level();
  }
  write_newline_indent();
  output_ << '}';
  return EXIT_SUCCESS;
}

int JsonEncoder::encode(const Sentence& sentence)
{
  output_ << '{';
  {
    settings_.increase_level();
    write_newline_indent();
    write_pair("type", "sentence", true);
    write_newline_indent();
    write_key("data");
    encode(sentence.data());
    settings_.decrease_level();
  }
  write_newline_indent();
  output_ << '}';
  return EXIT_SUCCESS;
}

int JsonEncoder::encode(const Title& title)
{
  output_ << '{';
  {
    settings_.increase_level();
    write_newline_indent();
    write_pair("type", "title", true);
    write_newline_indent();
    write_pair("level", title.level(), true);
    write_newline_indent();
    write_key("data");
    title.data()->encode(*this);
    settings_.decrease_level();
  }
  write_newline_indent();
  output_ << '}';
  return EXIT_SUCCESS;
}

int JsonEncoder::encode(const Paragraph& paragraph)
{
  output_ << '{';
  {
    settings_.increase_level();
    write_newline_indent();
    write_pair("type", "paragraph", true);
    write_newline_indent();
    write_key("data");
    encode(paragraph.data());
    settings_.decrease_level();
  }
  write_newline_indent();
  output_ << '}';
  return EXIT_SUCCESS;
}

int JsonEncoder::encode(const Section& section)
{
  output_ << '{';
  {
    settings_.increase_level();
    write_newline_indent();
    write_pair("type", "section", true);
    write_newline_indent();
    write_key("title");
    section.title()->encode(*this);
    output_ << ',';
    write_newline_indent();
    write_key("data");
    encode(section.data());
    settings_.decrease_level();
  }
  write_newline_indent();
  output_ << '}';
  return EXIT_SUCCESS;
}

int JsonEncoder::encode(const Document& document)
{
  output_ << '{';
  {
    settings_.increase_level();
    write_newline_indent();
    write_pair("type", "document", true);
    write_newline_indent();
    write_key("data");
    encode(document.data());
    settings_.decrease_level();
  }
  write_newline_indent();
  output_ << '}';
  return EXIT_SUCCESS;
}

int JsonEncoder::encode(const MarkdownElement& element)
{
  return element.encode(*this);
}

void  JsonEncoder::write_indent()
{
  if (settings_.is_compact())
    return;
  for (int i = 0; i < settings_.level(); ++i)
  {
    for (int j = 0; j < settings_.indentation(); ++j)
    {
      output_ << settings_.indentation_character();
    }
  }
}

void  JsonEncoder::write_newline_indent()
{
  if (settings_.is_compact())
    return;
  output_ << std::endl;
  write_indent();
}

void  JsonEncoder::write_key(const std::string& key)
{
  output_ << '"';
  write_sanitize(key);
  output_ << "\": ";
}

void  JsonEncoder::write_pair(const std::string& key,
                              const std::string& value,
                              bool comma)
{
  write_key(key);
  output_ << '"';
  write_sanitize(value);
  output_ << '"';
  if (comma)
    output_ << ',';
}

void  JsonEncoder::write_pair(const std::string& key,
                              char value,
                              bool comma)
{
  write_key(key);
  output_ << '"';
  write_sanitize(value);
  output_ << '"';
  if (comma)
    output_ << ',';
}

void  JsonEncoder::write_pair(const std::string& key,
                              const atom::Number& value,
                              bool comma)
{
  write_key(key);
  output_ << value;
  if (comma)
    output_ << ',';
}

void  JsonEncoder::write_pair(const std::string& key,
                              int value,
                              bool comma)
{
  write_key(key);
  output_ << value;
  if (comma)
    output_ << ',';
}

void JsonEncoder::write_sanitize(char c)
{
  switch (c)
  {
    case '\n':
      output_ << "\\n";
      break;
    case '\t':
      output_ << "\\t";
      break;
    case '\r':
      output_ << "\\r";
      break;
    case '\b':
      output_ << "\\b";
      break;
    case '\f':
      output_ << "\\f";
      break;
    case '\\':
      output_ << "\\\\";
      break;
    case '"':
      output_ << "\\\"";
      break;
    default:
      output_ << c;
      break;
  }
}

void JsonEncoder::write_sanitize(const std::string& string)
{
  for (char c : string)
    write_sanitize(c);
}
