#ifndef MARKDOWN_OBJECTS_TYPES_HPP
# define MARKDOWN_OBJECTS_TYPES_HPP

enum  MarkdownType
{
  kInvalid = -1,
  // Concret types:
  kCharacter,
  kWord,
  kNumber,
  // Compose types:
  kTitle,
  kWordLinked,
  kTitleLinked,
  kParagraph,
  kParagraphLinked,
  kLine,
  kDocument
};

#endif