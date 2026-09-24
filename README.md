# Calculator Based on a Custom Lexer and State Machine Parser

A flexible parser and calculator engine developed using pure **C++17** and the raw **Win32 API**. This project demonstrates compiler front-end mechanisms without relying on any external libraries (such as Qt, MFC, C#, etc.).

---

## 🎯 Key Features & Design Decisions

### 1. Keyboard-Friendly & Parenthesis-Free Design (UX Decision)
To eliminate the time-consuming use of complex parentheses (e.g., `Shift+8/9`), this calculator is designed for **parenthesis-free (linear input)** entry. Users input expressions in a single line, and the system automatically manages operator logic in the background.

### 2. Custom `p` (Percentage) Operator
In addition to standard operators (`+`, `-`, `*`, `/`), a custom `p` (Percentage) operator has been added to facilitate financial calculations and speed up workflows.
* **Formula:** `a p b` → `a * (b / 100)`
* **Example:** The expression `1000 + 50 p 10` automatically calculates `50 p 10 = 5` first (due to operator precedence) and then produces the result `1000 + 5 = 1005`.

### 3. Dynamic Win32 GUI Layout Engine
Instead of using hardcoded pixel values ​​for the interface, a `VerticalLayout` engine is implemented; it tracks element positions in memory and arranges them dynamically. This ensures that all elements automatically realign if the window size changes. ---

## 🧠 Technical Architecture

The project is structured into three main modules in accordance with software engineering principles: `main.cpp` (Interface & Event Loop), `parser.h` (Declarations), and `parser.cpp` (Implementation).

* **Lexer (Scanner):** Scans the source text character-by-character using `std::wstring_view` with zero-allocation semantics. It employs an **LL(1) / One-Character Lookahead** architecture based on `peek()` and `advance()` methods, requiring no backtracking.
* **Parser (Interface / State Machine):** Validates inputs using the `ParserState` enumeration (e.g., `EXPECT_OPERAND`, `EXPECT_OPERATOR`). It resolves token precedence using `std::variant` and a type-safe `std::stack` approach (**Shunting-Yard-style State-Machine Parser**).

### 🔮 Vision for x64 Opcode and Compiler Extensibility
In the current architecture, the `eval()` function operates like an interpreter, calculating results immediately. However, the front-end is designed to be fully modular. The infrastructure is ready to evolve into a compiler; by integrating a code generator into the `eval()` function in future stages, these simplified expressions can be directly converted into **raw x64 Assembly or Opcode (Machine Code)**.

---

## 🛠️ How to Compile?

1. Open any C++17-compatible compiler (Visual Studio MSVC is recommended). 2. Include `main.cpp`, `parser.cpp`, and `parser.h` in your project.
3. Compile by selecting the **Windows (`/SUBSYSTEM:WINDOWS`)** subsystem setting.
