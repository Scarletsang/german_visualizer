#include "Encoder.hpp"

#include <emscripten/bind.h>

#include "Ast.hpp"

using namespace emscripten;

EMSCRIPTEN_BINDINGS(encoder) {
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
        .function("encode_element", select_overload<int(const MarkdownElement&)>(&Encoder::encode), pure_virtual()); 
}
