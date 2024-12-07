

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