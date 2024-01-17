# German learning visualizer

> A Website powered by C++

The goal is let Web Assembly handle the actual logic of the website, and the website is merely a visualization of the data structure. I always wanted to store my ideas in a really flexible way, so I decided to write the core data structure, including a Markdown parser and a JSON encoder in C++. This project is merely a project where I can write a lot of parser (I love it) and a investigation into WebAssembly, also a project that I do when I procrastinate.

What I have currently in mind is the following:

1. The data stored on the frontend is managed by the C++ core.
2. User can write Markdown on the website, and the C++ core can real time parse the user input and change the data structure. The C++ code is designed to be flexible, so in theory one can also write a different parser to parse their language into my data structure.
3. Different encoders (currently only JSON) can allow the data being transferred to the backend or other places.
4. One of the encoder is to encode the data structure back into some kind of visualization in the frontend.

