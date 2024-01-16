#include "Settings.hpp"

#include <string>

MarkdownTokenizerSettings::MarkdownTokenizerSettings()
  : space_characters_(" \t\n\r\f\v") {}

MarkdownTokenizerSettings::MarkdownTokenizerSettings(const MarkdownTokenizerSettings& settings)
  : space_characters_(settings.space_characters_) {}

MarkdownTokenizerSettings::~MarkdownTokenizerSettings() {}

MarkdownTokenizerSettings& MarkdownTokenizerSettings::operator=(const MarkdownTokenizerSettings& settings)
{
  if (this != &settings)
    space_characters_ = settings.space_characters_;
  return *this;
}

bool  MarkdownTokenizerSettings::is_space(char character) const
{
  return space_characters_.find(character) != std::string::npos;
}
