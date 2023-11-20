#ifndef MARKDOWN_OBJECTS_TYPES_HPP
# define MARKDOWN_OBJECTS_TYPES_HPP

enum  MarkdownType
{
  kInvalid = -1,
  kCharacter,
  kWord,
  kTitle,
  kWordLinked,
  kTitleLinked,
  kParagraph,
  kParagraphLinked,
  kLine,
  kDocument
};

#endif