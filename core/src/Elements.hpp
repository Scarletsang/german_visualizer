#pragma once

#include <string>
#include <memory>

#include "Ast/Atom/Character.hpp"
#include "Ast/Atom/Delimiter.hpp"
#include "Ast/Atom/Word.hpp"
#include "Ast/Atom/Number.hpp"
#include "Ast/DomCollection/Title.hpp"
#include "Ast/DomCollection/Sentence.hpp"
#include "Ast/DomCollection/Paragraph.hpp"
#include "Ast/DomCollection/Section.hpp"
#include "Ast/DomCollection/Document.hpp"
#include "Ast/GraphCollection/Edge.hpp"
#include "Ast/GraphCollection/Edge/Directed.hpp"
#include "Ast/GraphCollection/Edge/Undirected.hpp"
#include "Ast/GraphCollection/Tagged/Weighted.hpp"
#include "Ast/Graph.hpp"

std::shared_ptr<Character>  CreateCharacter(char c);
std::shared_ptr<Word>       CreateWord(const std::string& word);
std::shared_ptr<Delimiter>  CreateDelimiter(const std::string& word);
std::shared_ptr<Number>     CreateNumber(int number);
std::shared_ptr<Number>     CreateNumber(float number);

std::shared_ptr<Title>      CreateTitle(int level, std::shared_ptr<Paragraph> paragraph);
std::shared_ptr<Sentence>   CreateSentence();
std::shared_ptr<Paragraph>  CreateParagraph();
std::shared_ptr<Section>    CreateSection(std::shared_ptr<Title> title);
std::shared_ptr<Document>   CreateDocument();
