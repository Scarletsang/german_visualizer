#include "Settings.hpp"

#include "libcpp/Maybe.hpp"

JsonEncoderSettings::JsonEncoderSettings(\
  int level, int indentation, libcpp::Maybe<char> indentation_character)
  : level_(level),
    indentation_(indentation),
    indentation_character_(indentation_character) {}

JsonEncoderSettings::JsonEncoderSettings(const JsonEncoderSettings& object)
  : level_(object.level_),
    indentation_(object.indentation_),
    indentation_character_(object.indentation_character_) {}

JsonEncoderSettings::~JsonEncoderSettings() {}

JsonEncoderSettings& JsonEncoderSettings::operator=(const JsonEncoderSettings& other)
{
  if (this != &other)
  {
    level_ = other.level_;
    indentation_ = other.indentation_;
    indentation_character_ = other.indentation_character_;
  }
  return *this;
}

int JsonEncoderSettings::level() const
{
  return level_;
}

int JsonEncoderSettings::indentation() const
{
  return indentation_;
}

libcpp::Maybe<char> JsonEncoderSettings::indentation_character() const
{
  return indentation_character_;
}

bool  JsonEncoderSettings::is_compact() const
{
  return !indentation_character_.is_ok();
}

void JsonEncoderSettings::set_level(int level)
{
  level_ = level;
}

void JsonEncoderSettings::increase_level()
{
  ++level_;
}

void JsonEncoderSettings::decrease_level()
{
  --level_;
}

void JsonEncoderSettings::set_indentation(int indentation)
{
  indentation_ = indentation;
}

void JsonEncoderSettings::set_indentation_character(libcpp::Maybe<char> indentation_character)
{
  indentation_character_ = indentation_character;
}
