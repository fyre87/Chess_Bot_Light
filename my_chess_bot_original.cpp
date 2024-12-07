

#include <iostream>  // For input (std::cin), output (std::cout), and standard I/O operations
#include <string>    // For using std::string to store and manipulate strings
#include <vector>    // For using std::vector to store a list of moves
#include <sstream>   // For using std::istringstream to split space-separated strings
#include <cstdlib>   // For using std::srand and std::rand to generate random numbers
#include <ctime>     // For using std::time to seed the random number generator with the current time


class Chess {
private:
    int current_player_color;      // 0 for white, 1 for black
    std::string current_fen;       // Current board position in FEN format
    std::string available_moves;   // String containing all available moves (space separated)
    std::string chosen_move;       // Chosen move which will be printed to output

    // Example input:::
    // input_to_cpp_agent:  
    // current_player_color: 0
    // current_fen: rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1
    // available_moves: a2a3 a2a4 b2b3 b2b4 c2c3 c2c4 d2d3 d2d4 e2e3 e2e4 f2f3 f2f4 g2g3 g2g4 h2h3 h2h4 b1c3 b1a3 g1h3 g1f3



    

public:
    // Default constructor with initialization inside the body
    Chess() {
        current_player_color = 0;
        current_fen = "";
        available_moves = "";
        chosen_move = "";
    }

    // Function to read input
    void read_input() {
        // Read current player color (it will be 0 for white, 1 for black)
        std::cin >> current_player_color;
        std::cin.ignore(); // To consume the newline character

        // Read current FEN string
        std::getline(std::cin, current_fen);

        // Read available moves (space-separated)
        std::getline(std::cin, available_moves);
    }


    // Function to think and set the chosen move randomly
    void think() {
        if (!available_moves.empty()) {
            // Split the available_moves into a vector of moves
            std::vector<std::string> moves;
            std::istringstream iss(available_moves);
            std::string move;
            while (iss >> move) {
                moves.push_back(move);
            }

            // Pick a random move
            if (!moves.empty()) {
                std::srand(std::time(nullptr)); // Seed the random number generator
                int random_index = std::rand() % moves.size();
                chosen_move = moves[random_index];
            } else {
                chosen_move = "No valid moves available";
            }
        } else {
            chosen_move = "No move available";
        }
    }

    // Function to print the chosen move
    void print_output() const {
        std::cout << chosen_move << std::endl;
    }
};

int main() {
    Chess game;

    while (true) { // Infinite loop
        game.read_input();
        game.think();
        game.print_output();
    }

    return 0;
}