#pragma once

#include <string>

class MarkdownTokenizerSettings
{
  public:
    static const std::string  kDefaultSpaceCharacters;
    static const std::string  kDefaultPunctuationCharacters;
    MarkdownTokenizerSettings();
    MarkdownTokenizerSettings(const MarkdownTokenizerSettings& settings);
    ~MarkdownTokenizerSettings();

    MarkdownTokenizerSettings& operator=(const MarkdownTokenizerSettings& settings);

    bool  is_space(char character) const;
    bool  is_punctuation(char character) const;

    void  set_space_characters(const std::string& space_characters);
    void  set_punctuation_characters(const std::string& punctuation_characters);

  private:
    std::string space_characters_;
    std::string punctuation_characters_;
};
