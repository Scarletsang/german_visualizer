#include "Parser.hpp"

#include <memory>

#include "Parser/Tokenizer.hpp"
#include "Elements.hpp"

MarkdownParser::MarkdownParser(Tokenizer& tokenizer)
  : tokenizer_(tokenizer) {}

MarkdownParser::~MarkdownParser() {}

std::shared_ptr<Document> MarkdownParser::parse_document()
{
  std::shared_ptr<Document> document = CreateDocument();
  std::shared_ptr<MarkdownElement>  element;
  while (!is_eof())
  {
    // std::shared_ptr<MarkdownElement> section = parse_section();
    // if (section)
    // {
    //   document->add_element(section);
    //   continue;
    // }
    element = parse_element_delimiter();
    if (element)
    {
      document->add_element(element);
      continue;
    }
    break;
  }
  return document;
}

std::shared_ptr<Section> MarkdownParser::parse_section()
{
  std::istream::pos_type original_state = tokenizer_.snapshot();
  tokenizer_.debug();
  std::shared_ptr<Title> title = parse_title();
  tokenizer_.debug();
  if (!title)
  {
    return nullptr;
  }
  MarkdownParser::MarkdownElements body = parse_body(title->level());
  if (body.empty())
  {
    tokenizer_.rollback(original_state);
    return nullptr;
  }
  std::shared_ptr<Section> section = CreateSection(title);
  section->add_elements(body);
  return section;
}

MarkdownParser::MarkdownElements MarkdownParser::parse_body(int level)
{
  MarkdownElements  elements;
  while (!is_eof())
  {
    // std::cout << "level: " << level << ' ' << elements.size() << std::endl;
    std::istream::pos_type original_state = tokenizer_.snapshot();
    std::shared_ptr<MarkdownElement> element = parse_element_delimiter();
    if (element)
    {
      std::shared_ptr<Section> section = std::dynamic_pointer_cast<Section>(element);
      if (section && section->title()->level() <= level)
      {
        tokenizer_.rollback(original_state);
        return elements;
      }
      elements.push_back(element);
    }
    else
      break;
  }
  return elements;
}

std::shared_ptr<Title>  MarkdownParser::parse_title()
{
  std::istream::pos_type original_state = tokenizer_.snapshot();
  std::unique_ptr<TokenSymbol> hashes = tokenizer_.tokenize_symbol();
  if (!hashes || hashes->data() != '#')
  {
    tokenizer_.rollback(original_state);
    return nullptr;
  }
  if (!parse_space())
  {
    tokenizer_.rollback(original_state);
    return nullptr;
  }
  std::shared_ptr<Paragraph> paragraph = parse_paragraph();
  if (!paragraph)
  {
    tokenizer_.rollback(original_state);
    return nullptr;
  }
  if (ignore_element_end() == EXIT_FAILURE)
  {
    tokenizer_.rollback(original_state);
    return nullptr;
  }
  return CreateTitle(hashes->level(), paragraph);
}

std::shared_ptr<Paragraph>  MarkdownParser::parse_paragraph()
{
  std::shared_ptr<Paragraph>  paragraph = CreateParagraph();
  while (!is_eof())
  {
    std::shared_ptr<Sentence> sentence = parse_sentence();
    if (sentence)
      paragraph->add_element(sentence);
    else
      break;
  }
  if (paragraph->data().empty())
    return nullptr;
  return paragraph;
}

std::shared_ptr<MarkdownElement> MarkdownParser::parse_element()
{
  {
    std::shared_ptr<Section> section = parse_section();
    if (section)
      return section;
  }
  {
    std::shared_ptr<Paragraph> paragraph = parse_paragraph();
    if (paragraph)
    {
      if (paragraph->data().size() == 1)
        return paragraph->data()[0];
      return paragraph;
    }
  }
  {
    std::shared_ptr<Word> word = parse_word();
    if (word)
    {
      if (word->data().size() == 1)
        return std::make_shared<Character>(word->data()[0]);
      return word;
    }
  }
  {
    std::shared_ptr<Number> number = parse_number();
    if (number)
      return number;
  }
  return nullptr;
}

std::shared_ptr<MarkdownElement> MarkdownParser::parse_element_delimiter()
{
  std::istream::pos_type original_state = tokenizer_.snapshot();
  std::shared_ptr<MarkdownElement> element = parse_element();
  if (!element)
    return nullptr;
  if (ignore_element_end() == EXIT_FAILURE)
  {
    tokenizer_.rollback(original_state);
    return nullptr;
  }
  return element;
}

int MarkdownParser::ignore_eof()
{
  tokenizer_.tokenize_character();
  if (is_eof())
    return EXIT_SUCCESS;
  return EXIT_FAILURE;
}

int  MarkdownParser::ignore_element_end()
{
  if (ignore_eof() || (ignore_newline() == EXIT_SUCCESS))
    return EXIT_SUCCESS;
  return EXIT_FAILURE;
}

std::shared_ptr<Sentence>   MarkdownParser::parse_sentence()
{
  std::istream::pos_type original_state = tokenizer_.snapshot();
  std::shared_ptr<MarkdownElement>  element = parse_sentence_element();
  if (!element)
    return nullptr;
  std::shared_ptr<Sentence> sentence = CreateSentence();
  sentence->add_element(element);
  while (is_healthy())
  {
    std::istream::pos_type second_state = tokenizer_.snapshot();
    std::shared_ptr<Delimiter>  space = parse_space();
    if (!space)
      break;
    element = parse_sentence_element();
    if (!element)
    {
      tokenizer_.rollback(second_state);
      break;
    }
    if (!space->data().empty())
      sentence->add_element(space);
    sentence->add_element(element);
  }
  std::shared_ptr<Character> fullstop = parse_fullstop();
  if (fullstop)
  {
    sentence->add_element(fullstop);
    return sentence;
  }
  std::istream::pos_type second_state = tokenizer_.snapshot();
  if (ignore_element_end() == EXIT_SUCCESS)
  {
    tokenizer_.rollback(second_state);
    return sentence;
  }
  tokenizer_.rollback(original_state);
  return nullptr;
}

std::shared_ptr<MarkdownElement>   MarkdownParser::parse_sentence_element()
{
  {
    std::shared_ptr<Word> word = parse_word();
    if (word)
      return word;
  }
  {
    std::shared_ptr<Number> number = parse_number();
    if (number)
      return number;
  }
  {
    std::shared_ptr<Character> punctuation = parse_punctuation();
    if (punctuation)
      return punctuation;
  }
  return nullptr;
}

std::shared_ptr<Word> MarkdownParser::parse_word()
{
  std::unique_ptr<TokenWord> token_word = tokenizer_.tokenize_word();
  if (token_word)
    return CreateWord(token_word->data());
  return nullptr;
}

std::shared_ptr<Number> MarkdownParser::parse_number()
{
  std::unique_ptr<TokenNumber> token_number = tokenizer_.tokenize_number();
  if (token_number)
    return CreateNumber(token_number->data());
  return nullptr;
}

std::shared_ptr<Delimiter> MarkdownParser::parse_space()
{
  std::unique_ptr<TokenSpace> token_space = tokenizer_.tokenize_space();
  if (token_space)
  {
    if (token_space->data().empty())
      return std::make_shared<Delimiter>();
    return CreateDelimiter(token_space->data());
  }
  return nullptr;
}

int MarkdownParser::ignore_newline()
{
  std::unique_ptr<TokenCharacter> token_newline = tokenizer_.tokenize_character('\n');
  if (token_newline)
    return EXIT_SUCCESS;
  return EXIT_FAILURE;
}

std::shared_ptr<Character>  MarkdownParser::parse_punctuation()
{
  std::unique_ptr<TokenCharacter> token_punctuation = tokenizer_.tokenize_punctuation();
  if (token_punctuation)
    return CreateCharacter(token_punctuation->data());
  return nullptr;
}

std::shared_ptr<Character>  MarkdownParser::parse_fullstop()
{
  std::unique_ptr<TokenCharacter> token_punctuation = tokenizer_.tokenize_sentence_end();
  if (token_punctuation)
    return CreateCharacter(token_punctuation->data());
  return nullptr;
}

std::shared_ptr<Character>  MarkdownParser::parse_character()
{
  std::unique_ptr<TokenCharacter> token_character = tokenizer_.tokenize_character();
  if (token_character)
    return CreateCharacter(token_character->data());
  return nullptr;
}

bool  MarkdownParser::is_healthy() const
{
  return tokenizer_.stream().good();
}

bool  MarkdownParser::is_eof() const
{
  return tokenizer_.stream().eof();
}
