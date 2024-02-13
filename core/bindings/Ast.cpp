#include <emscripten/bind.h>

#include "Ast/Type.hpp"
#include "Ast/Element.hpp"
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

using namespace emscripten;

class MarkdownElementWrapper : public wrapper<MarkdownElement>
{
  public:
    EMSCRIPTEN_WRAPPER(MarkdownElementWrapper);

    virtual int encode(Encoder&) const override
    {
      return EXIT_FAILURE;
    }

    virtual ElementType type() const override
    {
      return call<ElementType>("type");
    }
};

class DomCollectionWrapper : public wrapper<DomCollection>
{
  public:
    EMSCRIPTEN_WRAPPER(DomCollectionWrapper);

    virtual int encode(Encoder&) const override
    {
      return EXIT_FAILURE;
    }

    virtual ElementType type() const override
    {
      return call<ElementType>("type");
    }
};

EMSCRIPTEN_BINDINGS(ast)
{
    enum_<ElementType>("AstType")
        .value("kAtom", ElementType::kAtom)
        .value("kDomCollection", ElementType::kDomCollection)
        .value("kGraphCollection", ElementType::kGraphCollection);

    class_<MarkdownElement>("AstElement")
        .function("encode", &MarkdownElement::encode, pure_virtual())
        .function("type", &MarkdownElement::type, pure_virtual())
        .allow_subclass<MarkdownElementWrapper>("MarkdownElementWrapper");

    class_<Character, base<MarkdownElement>>("AstCharacter")
        .constructor<>()
        .constructor<char>()
        .function("encode", &Character::encode)
        .function("type", &Character::type)
        .property("data", &Character::data, &Character::set_data);
    
    class_<Delimiter, base<MarkdownElement>>("AstDelimiter")
        .constructor<>()
        .constructor<const std::string&>()
        .function("encode", &Delimiter::encode)
        .function("type", &Delimiter::type)
        .property("data", &Delimiter::data, &Delimiter::set_data);

    class_<Word, base<MarkdownElement>>("AstWord")
        .constructor<>()
        .constructor<const std::string&>()
        .function("encode", &Word::encode)
        .function("type", &Word::type)
        .property("data", &Word::data, &Word::set_data);

    class_<atom::Number>("Number")
        .constructor<>()
        .function("set_integer", &atom::Number::set_integer)
        .function("set_float", &atom::Number::set_float)
        .function("as_integer", &atom::Number::get_integer)
        .function("as_float", &atom::Number::get_float);

    class_<Number, base<MarkdownElement>>("AstNumber")
        .constructor<>()
        .constructor<atom::Number>()
        .function("encode", &Number::encode)
        .function("type", &Number::type)
        .property("data", &Number::data, &Number::set_data);

    class_<Title, base<MarkdownElement>>("AstTitle")
        .constructor<>()
        .constructor<int>()
        .constructor<int, std::shared_ptr<Paragraph>>()
        .function("encode", &Title::encode)
        .function("type", &Title::type)
        .property("data", &Title::data, &Title::set_data)
        .property("level", &Title::level, &Title::set_level);

    class_<DomCollection, base<MarkdownElement>>("AstDomCollection")
        .constructor<>()
        .function("encode", &DomCollection::encode)
        .function("type", &DomCollection::type)
        .function("add_element", select_overload<int(std::shared_ptr<MarkdownElement>)>(&DomCollection::add_element))
        .function("add_elements", &DomCollection::add_elements)
        .property("data", &DomCollection::data, &DomCollection::set_data)
        .allow_subclass<DomCollectionWrapper>("StandardDomCollectionWrapper");

    class_<Sentence, base<DomCollection>>("AstSentence")
        .constructor<>()
        .function("encode", &Sentence::encode)
        .function("type", &Sentence::type)
        .function("add_element", select_overload<int(std::shared_ptr<MarkdownElement>)>(&Sentence::add_element));

    class_<Paragraph, base<DomCollection>>("AstParagraph")
        .constructor<>()
        .function("encode", &Paragraph::encode)
        .function("type", &Paragraph::type);

    class_<Section, base<DomCollection>>("AstSection")
        .constructor<>()
        .constructor<std::shared_ptr<Title>>()
        .function("encode", &Section::encode)
        .function("type", &Section::type)
        .property("title", &Section::title, &Section::set_title);

    class_<Document, base<DomCollection>>("AstDocument")
        .constructor<>()
        .function("encode", &Document::encode)
        .function("type", &Document::type);
}

