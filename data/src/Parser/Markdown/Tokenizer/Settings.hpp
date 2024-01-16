#pragma once

#include <string>

class MarkdownTokenizerSettings
{
  public:
    static const std::string  kDefaultSpaceCharacters;
    static const char kDefaultDominantSpaceCharacter = ' ';
    static const char kDefaultSentenceEndCharacter = '.';
    static const std::string  kDefaultPunctuationCharacters;
    static const std::string  kDefaultSymbolCharacters;
    MarkdownTokenizerSettings();
    MarkdownTokenizerSettings(const MarkdownTokenizerSettings& settings);
    ~MarkdownTokenizerSettings();

    MarkdownTokenizerSettings& operator=(const MarkdownTokenizerSettings& settings);

    bool  is_space(char character) const;
    bool  is_dominant_space(char character) const;
    bool  is_sentence_end_character(char character) const;
    bool  is_punctuation(char character) const;
    bool  is_symbol(char character) const;
    bool  is_word_character(char character) const;

    void  set_space_characters(const std::string& space_characters);
    void  set_dominate_space_character(char character);
    void  set_sentence_end_character(char character);
    void  set_punctuation_characters(const std::string& punctuation_characters);
    void  set_symbol_characters(const std::string& symbol_characters);

  private:
    std::string space_characters_;
    char  dominant_space_character_;
    char  sentence_end_character_;
    std::string punctuation_characters_;
    std::string symbol_characters_;
};
