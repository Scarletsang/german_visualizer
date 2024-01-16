#pragma once

#include <string>

class MarkdownTokenizerSettings
{
  public:
    MarkdownTokenizerSettings();
    MarkdownTokenizerSettings(const MarkdownTokenizerSettings& settings);
    ~MarkdownTokenizerSettings();

    MarkdownTokenizerSettings& operator=(const MarkdownTokenizerSettings& settings);

    bool  is_space(char character) const;

  private:
    std::string space_characters_;
};
