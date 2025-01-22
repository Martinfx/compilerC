# CompilerC

CompilerC is a modular compiler written in C++ designed for parsing, analyzing, and generating assembly code from custom programming languages. 
his project includes a lexer, an Abstract Syntax Tree (AST) parser, and an assembler code generator.

## Features
- **Lexer**: Tokenizes input source code into meaningful lexical tokens using regular expressions.
- **AST Parser**: Constructs an Abstract Syntax Tree (AST) to represent the syntactic structure of the code.
- **Assembler Generator**: Produces assembly code from the AST, enabling low-level execution.

## Project Structure
- `lexer.hpp`: Header file implementing the lexer for tokenizing source code.
- `compiler.hpp`: Includes core compiler logic, including AST parsing.
- `main.cpp`: The main entry point of the application.
- `test/`: Contains unit and integration tests for the lexer, parser, and assembler generator.

## How It Works
1. **Lexical Analysis**: 
   The lexer reads the input code and produces a stream of tokens such as keywords, literals, operators, and delimiters.
   
2. **Syntax Analysis (AST Parser)**: 
   The parser processes tokens and generates an Abstract Syntax Tree (AST) that represents the syntactic structure of the code.

3. **Code Generation**: 
   The assembler generator translates the AST into assembly code for execution on the target architecture.

## Prerequisites
- **C++17 or later**: Required for modern C++ features.
- **CMake**: To configure and build the project.
- **A C++ Compiler**: GCC, Clang, or MSVC.

## Getting Started
### Build Instructions
1. Clone the repository:
   ```bash
   git clone <repository-url>
   cd compilerC
   ```

2. Create and navigate to the build directory:
   ```bash
   mkdir build
   cd build
   ```

3. Configure and build the project:
   ```bash
   cmake ..
   make
   ```

### Run the Compiler
After building, execute the program:
```bash
./compilerC <source_file>
```
Replace `<source_file>` with the path to your source code.

## Testing
To ensure the components are functioning correctly, the `test/` directory includes unit tests. Run tests using CTest:
```bash
cd build
ctest
```

## Example
Sample input code:
```c
int main() {
    return 42;
}
```

Generated assembly code:
```asm
.section .text
.global _start
_start:
    mov $42, %eax
    ret
```

## License
This project is licensed under the terms specified in the `LICENSE` file.

## Contributing
Contributions are welcome! Feel free to fork the repository and submit pull requests.

## Contact
For any questions or further details, please contact the maintainers.
