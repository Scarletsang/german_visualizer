#include "Encoder.hpp"

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
#include "Ast/Graph.hpp"

using namespace emscripten;

class EncoderWrapper : public wrapper<Encoder>
{
  public:
    EMSCRIPTEN_WRAPPER(EncoderWrapper);

    int encode(const Character& character) override
    {
      return call<int>("encode", character);
    }

    int encode(const Word& word) override
    {
      return call<int>("encode", word);
    }

    int encode(const Delimiter& delimiter) override
    {
      return call<int>("encode", delimiter);
    }

    int encode(const Number& number) override
    {
      return call<int>("encode", number);
    }

    int encode(const Title& title) override
    {
      return call<int>("encode", title);
    }

    int encode(const Sentence& sentence) override
    {
      return call<int>("encode", sentence);
    }

    int encode(const Paragraph& paragraph) override
    {
      return call<int>("encode", paragraph);
    }

    int encode(const Section& section) override
    {
      return call<int>("encode", section);
    }

    int encode(const Document& document) override
    {
      return call<int>("encode", document);
    }

    int encode(const DirectedEdge& directed_edge) override
    {
      return call<int>("encode", directed_edge);
    }

    int encode(const UndirectedEdge& undirected_edge) override
    {
      return call<int>("encode", undirected_edge);
    }

    int encode(const WeightedEdge& weighted_edge) override
    {
      return call<int>("encode", weighted_edge);
    }

    int encode(const Graph& graph) override
    {
      return call<int>("encode", graph);
    }

    int encode(const MarkdownElement&) override
    {
      return EXIT_FAILURE;
    }
};

EMSCRIPTEN_BINDINGS(encoder)
{
    class_<Encoder>("Encoder")
        .function("encode_character", select_overload<int(const Character&)>(&Encoder::encode), pure_virtual())
        .function("encode_word", select_overload<int(const Word&)>(&Encoder::encode), pure_virtual())
        .function("encode_delimiter", select_overload<int(const Delimiter&)>(&Encoder::encode), pure_virtual())
        .function("encode_number", select_overload<int(const Number&)>(&Encoder::encode), pure_virtual())
        .function("encode_title", select_overload<int(const Title&)>(&Encoder::encode), pure_virtual())
        .function("encode_sentence", select_overload<int(const Sentence&)>(&Encoder::encode), pure_virtual())
        .function("encode_paragraph", select_overload<int(const Paragraph&)>(&Encoder::encode), pure_virtual())
        .function("encode_section", select_overload<int(const Section&)>(&Encoder::encode), pure_virtual())
        .function("encode_Document", select_overload<int(const Document&)>(&Encoder::encode), pure_virtual())
        .function("encode_directed_edge", select_overload<int(const DirectedEdge&)>(&Encoder::encode), pure_virtual())
        .function("encode_undirected_edge", select_overload<int(const UndirectedEdge&)>(&Encoder::encode), pure_virtual())
        .function("encode_weighted_edge", select_overload<int(const WeightedEdge&)>(&Encoder::encode), pure_virtual())
        .function("encode_graph", select_overload<int(const Graph&)>(&Encoder::encode), pure_virtual())
        .function("encode_element", select_overload<int(const MarkdownElement&)>(&Encoder::encode), pure_virtual())
        .allow_subclass<EncoderWrapper>("EncoderWrapper");
}
