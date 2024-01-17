#include "Tokenizer.hpp"

#include <iostream>
#include <iomanip>
#include <memory>
#include <string>

#include <cstdio>

#include "Parser/Tokens.hpp"
#include "Parser/Tokenizer.hpp"

MarkdownTokenizer::MarkdownTokenizer(std::istream& input_stream, \
                                     MarkdownTokenizerSettings& settings)
  : Tokenizer(input_stream), settings_(settings)
{
  stream_.unsetf(std::ios::skipws);
}

MarkdownTokenizer::~MarkdownTokenizer()
{
  stream_.setf(std::ios::skipws);
}

std::unique_ptr<TokenCharacter> MarkdownTokenizer::tokenize_character(char c)
{
  stream_.clear();
  if (c != kNoCriteria && stream_.peek() != c)
    return nullptr;
  stream_.clear();
  char character;
  stream_ >> character;
  if (stream_.eof())
    return nullptr;
  return std::unique_ptr<TokenCharacter>(new TokenCharacter(character));
}

std::unique_ptr<TokenCharacter> MarkdownTokenizer::tokenize_punctuation(char c)
{
  stream_.clear();
  if (!settings_.is_punctuation(c))
    return nullptr;
  else if (c != kNoCriteria && stream_.peek() != c)
    return nullptr;
  stream_.clear();
  char character;
  stream_.get(character);
  if (stream_.eof())
    return nullptr;
  return std::unique_ptr<TokenCharacter>(new TokenCharacter(character));
}

std::unique_ptr<TokenCharacter> MarkdownTokenizer::tokenize_sentence_end()
{
  stream_.clear();
  if (!settings_.is_sentence_end_character(stream_.peek()))
    return nullptr;
  stream_.clear();
  char character;
  stream_.get(character);
  if (stream_.eof())
    return nullptr;
  return std::unique_ptr<TokenCharacter>(new TokenCharacter(character));
}

std::unique_ptr<TokenNumber> MarkdownTokenizer::tokenize_number()
{
  std::istream::pos_type original_state = snapshot();
  stream_.clear();
  int number;
  stream_ >> number;
  if (!stream_.good())
  {
    rollback(original_state);
    return nullptr;
  }
  return std::unique_ptr<TokenNumber>(new TokenNumber(number));
}

std::unique_ptr<TokenSpace> MarkdownTokenizer::tokenize_space()
{
  std::istream::pos_type original_state = snapshot();
  if (ignore_dominant_space() == EXIT_SUCCESS)
    return std::unique_ptr<TokenSpace>(new TokenSpace());
  std::string spaces;
  char character;
  while (stream_.good())
  {
    stream_.clear();
    stream_.get(character);
    if (!settings_.is_space(character))
    {
      stream_.clear();
      stream_.unget();
      break;
    }
    spaces += character;
  }
  if (spaces.empty())
  {
    rollback(original_state);
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
  int level = 1;
  char  symbol_character;
  char  c;
  stream_.get(symbol_character);
  while (stream_.good())
  {
    stream_.clear();
    if (stream_.peek() != symbol_character)
      break;
    stream_.clear();
    stream_.get(c);
    ++level;
  }
  return std::unique_ptr<TokenSymbol>(new TokenSymbol(symbol_character, level));
}

std::unique_ptr<TokenWord> MarkdownTokenizer::tokenize_word()
{
  std::string word;
  char  character;
  while (stream_.good())
  {
    stream_.clear();
    stream_.get(character);
    if (!settings_.is_word_character(character))
    {
      stream_.clear();
      stream_.unget();
      break;
    }
    if (stream_.good())
      word += character;
  }
  if (word.empty())
    return nullptr;
  return std::unique_ptr<TokenWord>(new TokenWord(word));
}

int MarkdownTokenizer::ignore_dominant_space()
{
  char character;
  stream_.clear();
  stream_.get(character);
  if (stream_.eof() || !settings_.is_dominant_space(character))
  {
    stream_.clear();
    stream_.unget();
    return EXIT_FAILURE;
  }
  character = stream_.peek();
  if (stream_.eof() || settings_.is_space(character))
  {
    stream_.clear();
    stream_.unget();
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

int MarkdownTokenizer::ignore_newline()
{
  char character;
  stream_.clear();
  stream_.get(character);
  if (stream_.eof())
    return EXIT_SUCCESS;
  else if (!settings_.is_newline_character(character))
  {
    stream_.clear();
    stream_.unget();
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

const std::istream& MarkdownTokenizer::stream() const
{
  return stream_;
}

void  MarkdownTokenizer::debug()
{
  static int count = 0;
  std::istream::pos_type original_state = snapshot();
  stream_.clear();
  std::string s;
  std::getline(stream_, s);
  std::cout << count << ": '" << s << "'" << std::endl;
  count++;
  rollback(original_state);
}
