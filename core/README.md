# Core Engine

The core engine is built with a simple structure:

Different parsers can decode different kind of input into my AST. My AST is able to encoded into different format for easy data transfer.

Currently there is only a markdown parser and a JSON encoder.

## AST

There are 3 kinds of AST Element:

1. Atom - Nodes that hold data
2. DomCollection - Nodes that represents structure in a DOM tree
3. GraphCollection - Nodes that represents edges and relations in a more general graph

Normally, relationships are considered a layer above normal element. However, the GraphCollection nodes are all just a normal AST element, which means one can make relation to a relation node. This is the harder part of the data structure. I want my graph to be quite general to encompass different kinds of graphs. I am still thinking about how I should store the edges and relations of my graph.