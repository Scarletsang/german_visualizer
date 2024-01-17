#pragma once

class MarkdownParserError
{
  public:
    enum Type
    {
      kOk,
      kSyntaxError,
      kTooLowLevel
    };

    MarkdownParserError();
    MarkdownParserError(Type type);
    MarkdownParserError(const MarkdownParserError& other);
    virtual ~MarkdownParserError();
    MarkdownParserError& operator=(const MarkdownParserError& other);

    bool  operator==(enum Type type) const;

  private:
    Type  type_;
};
