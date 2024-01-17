#include "Error.h"

#include <stdlib.h>

MarkdownParserError::MarkdownParserError()
  : type_(kOk) {}

MarkdownParserError::MarkdownParserError(Type type)
  : type_(type) {}

MarkdownParserError::MarkdownParserError(const MarkdownParserError& other)
  : type_(other.type_) {}

MarkdownParserError::~MarkdownParserError() {}

MarkdownParserError& MarkdownParserError::operator=(const MarkdownParserError& other)
{
  type_ = other.type_;
  return *this;
}

bool  MarkdownParserError::operator==(enum Type type) const
{
  return type_ == type;
}
