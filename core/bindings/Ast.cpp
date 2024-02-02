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

using StandardDomCollection = DomCollection<MarkdownElement>;

class StandardDomCollectionWrapper : public wrapper<StandardDomCollection>
{
  public:
    EMSCRIPTEN_WRAPPER(StandardDomCollectionWrapper);

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
        .function("data", select_overload<const char&() const>(&Character::data))
        .function("writable_data", select_overload<char&()>(&Character::data));
    
    class_<Delimiter, base<MarkdownElement>>("AstDelimiter")
        .constructor<>()
        .constructor<const std::string&>()
        .function("encode", &Delimiter::encode)
        .function("type", &Delimiter::type)
        .function("data", select_const(&Delimiter::data), allow_raw_pointers())
        .function("writable_data", select_overload<std::string&()>(&Delimiter::data), allow_raw_pointers());

    class_<Word, base<MarkdownElement>>("AstWord")
        .constructor<>()
        .constructor<const std::string&>()
        .function("encode", &Word::encode)
        .function("type", &Word::type)
        .function("data", select_const(&Word::data), allow_raw_pointers())
        .function("writable_data", select_overload<std::string&()>(&Word::data), allow_raw_pointers());

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
        .function("data", select_const(&Number::data), allow_raw_pointers())
        .function("writable_data", select_overload<atom::Number&()>(&Number::data), allow_raw_pointers());

    class_<Title, base<MarkdownElement>>("AstTitle")
        .constructor<>()
        .constructor<int>()
        .constructor<int, std::shared_ptr<Paragraph>>()
        .function("encode", &Title::encode)
        .function("type", &Title::type)
        .property("data", &Title::data, &Title::set_data)
        .property("level", &Title::level, &Title::set_level);

    class_<Paragraph, base<MarkdownElement>>("AstParagraph")
        .constructor<>()
        .function("encode", &Paragraph::encode)
        .function("type", &Paragraph::type)
        .function("add_element", select_overload<int(std::shared_ptr<Sentence>)>(&Paragraph::add_element))
        .function("add_elements", &Paragraph::add_elements)
        .function("data", select_const(&Paragraph::data), allow_raw_pointers())
        .function("writable_data", select_overload<std::vector<std::shared_ptr<Sentence>>&()>(&Paragraph::data), allow_raw_pointers());

    class_<StandardDomCollection, base<MarkdownElement>>("AstDomCollection")
        .constructor<>()
        .function("encode", &StandardDomCollection::encode)
        .function("type", &StandardDomCollection::type)
        .function("add_element", select_overload<int(std::shared_ptr<MarkdownElement>)>(&StandardDomCollection::add_element))
        .function("add_elements", &StandardDomCollection::add_elements)
        .function("data", select_const(&StandardDomCollection::data), allow_raw_pointers())
        .function("writable_data", select_overload<std::vector<std::shared_ptr<MarkdownElement>>&()>(&StandardDomCollection::data), allow_raw_pointers())
        .allow_subclass<StandardDomCollectionWrapper>("StandardDomCollectionWrapper");

    class_<Sentence, base<StandardDomCollection>>("AstSentence")
        .constructor<>()
        .function("encode", &Sentence::encode)
        .function("type", &Sentence::type)
        .function("add_element", select_overload<int(std::shared_ptr<MarkdownElement>)>(&Sentence::add_element));

    class_<Section, base<StandardDomCollection>>("AstSection")
        .constructor<>()
        .constructor<std::shared_ptr<Title>>()
        .function("encode", &Section::encode)
        .function("type", &Section::type)
        .property("title", &Section::title, &Section::set_title);

    class_<Document, base<StandardDomCollection>>("AstDocument")
        .constructor<>()
        .function("encode", &Document::encode)
        .function("type", &Document::type);
}

