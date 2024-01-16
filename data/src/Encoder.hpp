#pragma once

#include <iostream>
#include <vector>
#include <memory>

class MarkdownElement;

// atom
class Character;
class Delimiter;
class Word;
class Number;

// composite
class Sentence;
class Title;

// collection
class Paragraph;
class Section;
class Document;

class Encoder
{
  public:
    Encoder& operator=(const Encoder& encoder) = delete;
    virtual ~Encoder() {};

    virtual int encode(const Character& character) = 0;
    virtual int encode(const Word& word) = 0;
    virtual int encode(const Delimiter& delimiter) = 0;
    virtual int encode(const Number& number) = 0;

    // group of atoms
    virtual int encode(const Sentence& sentence) = 0;
    // level and one line
    virtual int encode(const Title& title) = 0;

    // group of lines
    virtual int encode(const Paragraph& paragraph) = 0;
    // group of a title, and a group of elements with the same level
    virtual int encode(const Section& section) = 0;
    // group of sections
    virtual int encode(const Document& document) = 0;

    virtual int encode(const MarkdownElement& document) = 0;
};
