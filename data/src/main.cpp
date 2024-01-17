#include <fstream>
#include <iostream>
#include <sstream>
#include <memory>
#include <string>
#include <random>

#include <ctime>

#include "Elements.hpp"
#include "Parser/Markdown/Tokenizer.hpp"
#include "Parser/Markdown/Parser.hpp"
#include "Encoder/Json.hpp"

int GenerateRandomNumber(int minValue, int maxValue)
{
  std::random_device rd;  // A hardware entropy source, if available
  std::mt19937 generator(rd());  // Standard mersenne_twister_engine seeded with rd()
  std::uniform_int_distribution<int> distribution(minValue, maxValue);

  return distribution(generator);
}

std::string GenerateRandomString(std::size_t length)
{
  const std::string characters = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
  std::mt19937 generator(static_cast<unsigned int>(std::time(0))); // Seed the random number generator
  std::uniform_int_distribution<std::size_t> distribution(0, characters.size() - 1);

  std::string randomString;
  randomString.reserve(length);

  for (std::size_t i = 0; i < length; ++i) {
      randomString.push_back(characters[distribution(generator)]);
  }

  return randomString;
}

std::shared_ptr<Sentence> CreateRandomSentence(int amount_of_words)
{
  std::shared_ptr<Sentence> sentence = std::make_shared<Sentence>();
  for (int i = 0; i < amount_of_words; ++i)
  {
    std::shared_ptr<Word> word = CreateWord(\
      GenerateRandomString(GenerateRandomNumber(1, 10)));
    sentence->add_element(word);
  }
  return sentence;
}

std::shared_ptr<Paragraph> CreateRandomParagraph(int amount_of_sentence)
{
  std::shared_ptr<Paragraph> paragraph = std::make_shared<Paragraph>();
  for (int i = 0; i < amount_of_sentence; ++i)
  {
    std::shared_ptr<Sentence> sentence = CreateRandomSentence(GenerateRandomNumber(1, 10));
    paragraph->add_element(sentence);
  }
  return paragraph;
}

std::shared_ptr<Title> CreateRandomTitle(int level, int amount_of_words)
{
  std::shared_ptr<Title> title = std::make_shared<Title>(level);
  std::shared_ptr<Paragraph> paragraph = CreateParagraph();
  paragraph->add_element(CreateRandomSentence(amount_of_words));
  title->set_data(paragraph);
  return title;
}

std::shared_ptr<Section> CreateRandomSection(int level, int amount_of_paragraphs)
{
  std::shared_ptr<Section> section = std::make_shared<Section>();
  std::shared_ptr<Title> title = CreateRandomTitle(level, GenerateRandomNumber(1, 10));
  section->set_title(title);
  for (int i = 0; i < amount_of_paragraphs; ++i)
  {
    std::shared_ptr<Paragraph> paragraph = CreateRandomParagraph(GenerateRandomNumber(1, 10));
    section->add_element(paragraph);
  }
  return section;
}

std::shared_ptr<Document> CreateRandomDocument()
{
  std::shared_ptr<Document> document = CreateDocument();
  document->add_element(CreateRandomSection(1, 3));
  // document->add_element(CreateRandomSentence(3));
  document->add_element(CreateRandomSection(2, 1));
  return document;
}

std::shared_ptr<Document> CreateDocumentFromFile(char* filename)
{
  std::ifstream input_file(filename);
  MarkdownTokenizerSettings parser_settings;
  std::stringstream ss;
  MarkdownTokenizer tokenizer = MarkdownTokenizer(input_file, parser_settings);
  MarkdownParser parser = MarkdownParser(tokenizer);
  std::shared_ptr<Document> document = parser.parse_document();
  input_file.close();
  return document;
}

int main(int argc, char** argv)
{
  std::shared_ptr<Document> document;
  if (argc != 2)
    document = CreateRandomDocument();
  else
    document = CreateDocumentFromFile(argv[1]);
  JsonEncoderSettings settings;
  JsonEncoder encoder(std::cout, settings);
  encoder.encode(*document);
  std::cout << std::endl;
  return EXIT_SUCCESS;
}
