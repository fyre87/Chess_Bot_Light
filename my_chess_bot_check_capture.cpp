#include <iostream>  // For input (std::cin), output (std::cout), and standard I/O operations
#include <string>    // For using std::string to store and manipulate strings
#include <vector>    // For using std::vector to store a list of moves
#include <sstream>   // For using std::istringstream to split space-separated strings
#include <cstdlib>   // For using std::srand and std::rand to generate random numbers
#include <ctime>     // For using std::time to seed the random number generator with the current time
#include "chess.hpp"

using namespace chess;


class Chess {
private:
    int current_player_color;      // 0 for white, 1 for black
    std::string current_fen;       // Current board position in FEN format
    // std::string available_moves; //SHOULDN"T NEED
    std::string chosen_move;       // Chosen move which will be printed to output

    Board board;  // Board object to store the current board position

    // Example input:::
    // input_to_cpp_agent:  
    // current_player_color: 0
    // current_fen: rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1

public:
    // Default constructor with initialization inside the body
    Chess() {
        current_player_color = 0;
        current_fen = "";
        // available_moves = "";
        chosen_move = "";
    }

    // Function to read input
    void read_input() {
        // Read current player color (it will be 0 for white, 1 for black)
        std::cin >> current_player_color;
        std::cin.ignore(); // To consume the newline character

        // Read current FEN string
        std::getline(std::cin, current_fen);

        
        // std::getline(std::cin, available_moves); //SHOULDN"T NEED

        // Update the board object with the current FEN string
        board = Board(current_fen);

    }

    // Function to think and set the chosen move randomly
    void think() {
        Movelist moves;
        movegen::legalmoves(moves, board);
        
        if (moves.size() > 0) {
            // Check every move for checkmate
            for (const auto &move : moves) {
                board.makeMove(move);
                if (board.getHalfMoveDrawType().first == GameResultReason::CHECKMATE) {
                    chosen_move = uci::moveToUci(move);
                    return;
                }
                board.unmakeMove(move);
            }

            // Check every move for captures
            for (const auto &move : moves) {
                if (board.at(move.to()) != Piece::NONE && move.typeOf() != Move::CASTLING) {
                    chosen_move = uci::moveToUci(move);
                    return;
                }
            }

            // Pick a random move
            chosen_move = uci::moveToUci(moves[std::rand() % moves.size()]);

        } else {
            chosen_move = "No move available";
        }
    }

    // Function to print the chosen move
    void print_output() const {
        std::cout << chosen_move << std::endl;
    }


    // Function to evaluate the board position
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