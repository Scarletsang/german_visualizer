#pragma once

#include <string>
#include <memory>

#include "Element/Atom/Character.hpp"
#include "Element/Atom/Delimiter.hpp"
#include "Element/Atom/Word.hpp"
#include "Element/Atom/Number.hpp"
#include "Element/Collection/Paragraph.hpp"
#include "Element/Collection/Document.hpp"
#include "Element/Collection/Section.hpp"
#include "Element/Collection/Sentence.hpp"
#include "Element/Title.hpp"

// atom
std::shared_ptr<Character>  CreateCharacter(char c);
std::shared_ptr<Word>       CreateWord(const std::string& word);
std::shared_ptr<Delimiter>  CreateDelimiter(const std::string& word);
std::shared_ptr<Number>     CreateNumber(int number);
std::shared_ptr<Number>     CreateNumber(float number);

// // composite
std::shared_ptr<Sentence>   CreateSentence();
std::shared_ptr<Title>      CreateTitle(int level, std::shared_ptr<Paragraph> paragraph);

// // collection
std::shared_ptr<Paragraph>  CreateParagraph();
std::shared_ptr<Section>    CreateSection(std::shared_ptr<Title> title);
std::shared_ptr<Document>   CreateDocument();
