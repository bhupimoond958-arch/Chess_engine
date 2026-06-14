# Chess Engine with Stockfish

An advanced chess engine built with C++ that integrates Stockfish.

## Setup Instructions

### Prerequisites
- C++ compiler (g++, clang, or MSVC)
- CMake (optional, for building)
- Stockfish binary

### Installation

#### 1. Clone the Repository
```bash
git clone https://github.com/bhupimoond958-arch/Chess_engine.git
cd Chess_engine
```

#### 2. Download Stockfish

Download the precompiled Stockfish binary from [https://stockfishchess.org/download/](https://stockfishchess.org/download/)

Extract it and place it in the `bin/` directory:
```
Chess_engine/
├── bin/
│   └── stockfish (or stockfish.exe on Windows)
├── src/
├── include/
└── CMakeLists.txt
```

Or use a submodule to build Stockfish from source:
```bash
git submodule add https://github.com/official-stockfish/Stockfish.git external/stockfish
cd external/stockfish/src
make build ARCH=x86-64
```

### Building Your Project

#### Option A: With CMake
```bash
mkdir build
cd build
cmake ..
make
```

#### Option B: Manual G++ Compilation
```bash
g++ -std=c++17 -O3 src/main.cpp -o chess_engine
```

### Integrating with Stockfish in C++

Create a simple wrapper to communicate with Stockfish via UCI protocol:

```cpp
#include <iostream>
#include <unistd.h>
#include <cstring>

class StockfishEngine {
private:
    FILE* pipe;
    
public:
    StockfishEngine(const char* path) {
        pipe = popen(path, "r+");
        if (!pipe) throw std::runtime_error("Failed to start Stockfish");
        setbuf(pipe, NULL);
    }
    
    void sendCommand(const char* cmd) {
        fprintf(pipe, "%s\n", cmd);
    }
    
    std::string readOutput() {
        char buffer[256];
        std::string result;
        while(fgets(buffer, sizeof(buffer), pipe)) {
            result += buffer;
            if(strstr(buffer, "bestmove")) break;
        }
        return result;
    }
    
    ~StockfishEngine() {
        if(pipe) pclose(pipe);
    }
};
```

### Usage Example
```cpp
int main() {
    StockfishEngine engine("./bin/stockfish");
    
    engine.sendCommand("uci");
    std::cout << engine.readOutput();
    
    engine.sendCommand("setoption name Hash value 256");
    engine.sendCommand("position startpos moves e2e4 c7c5");
    engine.sendCommand("go depth 20");
    std::cout << engine.readOutput();
    
    return 0;
}
```

## Project Structure
```
Chess_engine/
├── bin/                    # Stockfish binary goes here
├── src/                    # Your C++ source files
├── include/                # Header files
├── external/               # External dependencies (Stockfish submodule)
├── CMakeLists.txt
└── README.md
```

## Resources
- [Stockfish Official](https://stockfishchess.org/)
- [UCI Protocol Documentation](http://wbec-ridderkerk.nl/html/UCIProtocol.html)

## Contributing

Feel free to open issues and pull requests!
