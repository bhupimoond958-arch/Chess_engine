#include <iostream>
#include <unistd.h>
#include <cstring>
#include <stdexcept>

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
