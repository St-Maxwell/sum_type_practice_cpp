# A Practice of Sum-type in Cpp
I write this calculator application, in which the parsing of arithmetic expression is involved, to learn the sum-type paradigm in modern Cpp.

The facilities of sum-type in Cpp may include `std::variant`, `std::optional`, and `std::expected`. They can corresponde to `enum`, `Option`, and `Result` in Rust language.

I find the sum-type paradigm is elegant in a sense. But due to the limit of language itself, using sum-type in Cpp is not as convenient in Rust. Hope this issuse can be solved in the future standard of Cpp.

## Grammar
```
E --> T {( "+" | "-" ) T}
T --> F {( "*" | "/" ) F}
F --> P ["^" F]
P --> v | "(" E ")" | "-" T
```

## Process
```mermaid
flowchart TD
    A[get_line] --> |Arithmetic Expression| B[Tokenize]
    B --> |Tokens| C(Parse)
    C --> |Expression Tree| D(Translate)
    D --> |RPN| E(Evaluate)
```

## Dependences
* Meson
* fmtlib

## References
* https://devblogs.microsoft.com/cppblog/cpp23s-optional-and-expected/
* https://www.engr.mun.ca/~theo/Misc/exp_parsing.htm
* https://www.foonathan.net/2022/05/recursive-variant-box/
* https://adriann.github.io/rust_parser.html

