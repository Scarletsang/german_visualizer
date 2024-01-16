#pragma once

#include <iostream>

#include "Encoder.hpp"
#include "Encoder/Markdown/Settings.hpp"
#include "Elements.hpp"

class MarkdownEncoder : public Encoder
{
  public:
    MarkdownEncoder(std::ostream& stream, const MarkdownEncoderSettings& settings = {});
    MarkdownEncoder(const MarkdownEncoder& object);
    virtual ~MarkdownEncoder();

    virtual int encode(const Character& character) override;
    virtual int encode(const Word& word) override;
    virtual int encode(const Number& number) override;

    // group of atoms
    virtual int encode(const Sentence& sentence) override;
    // level and one line
    virtual int encode(const Title& title) override;

    // group of lines
    virtual int encode(const Paragraph& paragraph) override;
    // group of a title, and a group of elements with the same level
    virtual int encode(const Section& section) override;
    // group of sections
    virtual int encode(const Document& document) override;

    virtual int encode(const MarkdownElement& element) override;

  protected:
    std::ostream& output_;
    MarkdownEncoderSettings settings_;
};
