#include "Settings.hpp"

#include <string>

const std::string  MarkdownTokenizerSettings::kDefaultSpaceCharacters = "\t\n\r\f\v";
const std::string  MarkdownTokenizerSettings::kDefaultPunctuationCharacters = ".!?,;:";
const std::string  MarkdownTokenizerSettings::kDefaultSymbolCharacters = "#-";

MarkdownTokenizerSettings::MarkdownTokenizerSettings()
  : space_characters_(kDefaultSpaceCharacters),
    dominant_space_character_(kDefaultDominantSpaceCharacter),
    punctuation_characters_(kDefaultPunctuationCharacters),
    symbol_characters_(kDefaultSymbolCharacters) {}

MarkdownTokenizerSettings::MarkdownTokenizerSettings(const MarkdownTokenizerSettings& settings)
  : space_characters_(settings.space_characters_),
    dominant_space_character_(settings.dominant_space_character_),
    punctuation_characters_(settings.punctuation_characters_),
    symbol_characters_(settings.symbol_characters_) {}

MarkdownTokenizerSettings::~MarkdownTokenizerSettings() {}

MarkdownTokenizerSettings& MarkdownTokenizerSettings::operator=(const MarkdownTokenizerSettings& settings)
{
  if (this != &settings)
  {
    space_characters_ = settings.space_characters_;
    dominant_space_character_ = settings.dominant_space_character_;
    punctuation_characters_ = settings.punctuation_characters_;
    symbol_characters_ = settings.symbol_characters_;
  }
  return *this;
}

bool  MarkdownTokenizerSettings::is_space(char character) const
{
  return (character == dominant_space_character_) || (space_characters_.find(character) != std::string::npos);
}

bool  MarkdownTokenizerSettings::is_dominant_space(char character) const
{
  return character == dominant_space_character_;
}

bool  MarkdownTokenizerSettings::is_punctuation(char character) const
{
  return punctuation_characters_.find(character) != std::string::npos;
}

bool  MarkdownTokenizerSettings::is_symbol(char character) const
{
  return symbol_characters_.find(character) != std::string::npos;
}

bool  MarkdownTokenizerSettings::is_word_character(char character) const
{
  return !is_space(character) && !is_punctuation(character);
}

void  MarkdownTokenizerSettings::set_space_characters(const std::string& space_characters)
{
  space_characters_ = space_characters;
}

void  MarkdownTokenizerSettings::set_dominate_space_character(char character)
{
  dominant_space_character_ = character;
}

void  MarkdownTokenizerSettings::set_punctuation_characters(const std::string& punctuation_characters)
{
  punctuation_characters_ = punctuation_characters;
}

void  MarkdownTokenizerSettings::set_symbol_characters(const std::string& symbol_characters)
{
  symbol_characters_ = symbol_characters;
}
