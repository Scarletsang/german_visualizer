#include "Markdown.hpp"

#include <istream>
#include <memory>

#include "Parser/Tokens.hpp"
#include "Parser/Tokenizer.hpp"

MarkdownTokenizer::MarkdownTokenizer(std::istream& input_stream, \
                                     const MarkdownTokenizerSettings& settings)
  : Tokenizer(input_stream), settings_(settings) {}

MarkdownTokenizer::~MarkdownTokenizer() {}

std::unique_ptr<TokenCharacter> MarkdownTokenizer::tokenize_character()
{
  char character;
  stream_.get(character);
  if (!stream_.good())
    return nullptr;
  return std::unique_ptr<TokenCharacter>(new TokenCharacter(character));
}

std::unique_ptr<TokenNumber> MarkdownTokenizer::tokenize_number()
{
  std::ios_base::iostate original_states = stream_.rdstate();
  int number;
  stream_ >> std::noskipws >> number;
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
  // TODO: Implement this
  if (!stream_.good())
  {
    stream_.clear(original_states);
    return nullptr;
  }
  return nullptr;
}

std::unique_ptr<TokenSymbol> MarkdownTokenizer::tokenize_symbol()
{
  std::ios_base::iostate original_states = stream_.rdstate();
  // TODO: Implement this
  if (!stream_.good())
  {
    stream_.clear(original_states);
    return nullptr;
  }
  return nullptr;
}

std::unique_ptr<TokenWord> MarkdownTokenizer::tokenize_word()
{
  std::ios_base::iostate original_states = stream_.rdstate();
  // TODO: Implement this
  if (!stream_.good())
  {
    stream_.clear(original_states);
    return nullptr;
  }
  return nullptr;
}

std::unique_ptr<Token> MarkdownTokenizer::tokenize()
{
  std::ios_base::iostate original_states = stream_.rdstate();
    if (!stream_.good())
  {
    stream_.clear(original_states);
    return nullptr;
  }
  return nullptr;
}
