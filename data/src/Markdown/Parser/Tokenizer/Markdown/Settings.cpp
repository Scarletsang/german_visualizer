#include "Settings.hpp"

#include <string>

const std::string  MarkdownTokenizerSettings::kDefaultSpaceCharacters = " \t\n\r\f\v";
const std::string  MarkdownTokenizerSettings::kDefaultPunctuationCharacters = ".!?,;:()[]{}<>\"'`~@#$%^&*\\|/-_=+";

MarkdownTokenizerSettings::MarkdownTokenizerSettings()
  : space_characters_(kDefaultSpaceCharacters), punctuation_characters_(kDefaultPunctuationCharacters) {}

MarkdownTokenizerSettings::MarkdownTokenizerSettings(const MarkdownTokenizerSettings& settings)
  : space_characters_(settings.space_characters_),
    punctuation_characters_(settings.punctuation_characters_) {}

MarkdownTokenizerSettings::~MarkdownTokenizerSettings() {}

MarkdownTokenizerSettings& MarkdownTokenizerSettings::operator=(const MarkdownTokenizerSettings& settings)
{
  if (this != &settings)
  {
    space_characters_ = settings.space_characters_;
    punctuation_characters_ = settings.punctuation_characters_;
  }
  return *this;
}

bool  MarkdownTokenizerSettings::is_space(char character) const
{
  return space_characters_.find(character) != std::string::npos;
}

bool  MarkdownTokenizerSettings::is_punctuation(char character) const
{
  return punctuation_characters_.find(character) != std::string::npos;
}

void  MarkdownTokenizerSettings::set_space_characters(const std::string& space_characters)
{
  space_characters_ = space_characters;
}

void  MarkdownTokenizerSettings::set_punctuation_characters(const std::string& punctuation_characters)
{
  punctuation_characters_ = punctuation_characters;
}
