#include "Markdown.hpp"

#include <istream>
#include <memory>
#include <cstdio>

#include "Parser/Tokens.hpp"
#include "Parser/Tokenizer.hpp"

MarkdownTokenizer::MarkdownTokenizer(std::istream& input_stream, \
                                     const MarkdownTokenizerSettings& settings)
  : Tokenizer(input_stream), settings_(settings)
{
  stream_ >> std::noskipws;
}

MarkdownTokenizer::~MarkdownTokenizer()
{
  stream_ >> std::skipws;
}

std::unique_ptr<TokenCharacter> MarkdownTokenizer::tokenize_character()
{
  char character;
  stream_.get(character);
  if (!stream_.good())
  {
    stream_.clear();
    stream_.unget();
    return nullptr;
  }
  return std::unique_ptr<TokenCharacter>(new TokenCharacter(character));
}

std::unique_ptr<TokenNumber> MarkdownTokenizer::tokenize_number()
{
  std::ios_base::iostate original_states = stream_.rdstate();
  int number;
  stream_ >> number;
  if (!stream_.good())
  {
    stream_.clear(original_states);
    return nullptr;
  }
  return std::unique_ptr<TokenNumber>(new TokenNumber(number));
}

std::unique_ptr<TokenSpace> MarkdownTokenizer::tokenize_space()
{
  std::ios_base::iostate original_states = stream_.rdstate();
  if (ignore_single_domainant_space() == EXIT_SUCCESS)
    return std::unique_ptr<TokenSpace>(new TokenSpace());
  std::string spaces;
  char character;
  while (stream_.good() && settings_.is_space(character))
  {
    stream_.get(character);
    spaces += character;
  }
  if (spaces.empty())
  {
    stream_.clear(original_states);
    return nullptr;
  }
  return std::unique_ptr<TokenSpace>(new TokenSpace(spaces));
}

std::unique_ptr<TokenSymbol> MarkdownTokenizer::tokenize_symbol()
{
  if (!settings_.is_symbol(stream_.peek()))
  {
    stream_.clear();
    return nullptr;
  }
  int level = 0;
  char  symbol_character;
  stream_.get(symbol_character);
  while (stream_.good())
  {
    if (stream_.peek() != symbol_character)
      break;
    stream_.ignore();
    ++level;
  }
  return std::unique_ptr<TokenSymbol>(new TokenSymbol(symbol_character, level));
}

std::unique_ptr<TokenWord> MarkdownTokenizer::tokenize_word()
{
  std::string word;
  char  character;
  while (stream_.good() && settings_.is_word_character(character))
  {
    stream_.get(character);
    word += character;
  }
  if (word.empty())
  {
    stream_.clear();
    return nullptr;
  }
  return std::unique_ptr<TokenWord>(new TokenWord(word));
}

int MarkdownTokenizer::ignore_single_domainant_space()
{
  std::ios_base::iostate original_states = stream_.rdstate();
  char character;
  stream_.get(character);
  if (!stream_.good() || !settings_.is_dominant_space(character))
  {
    stream_.clear(original_states);
    return EXIT_FAILURE;
  }
  character = stream_.peek();
  if (!stream_.good() || settings_.is_space(character))
  {
    stream_.clear(original_states);
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
