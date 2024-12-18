#include <iostream>  // For input (std::cin), output (std::cout), and standard I/O operations
#include <string>    // For using std::string to store and manipulate strings
#include <vector>    // For using std::vector to store a list of moves
#include <sstream>   // For using std::istringstream to split space-separated strings
#include <cstdlib>   // For using std::srand and std::rand to generate random numbers
#include <ctime>     // For using std::time to seed the random number generator with the current time
#include "chess.hpp"

using namespace chess;


// Piece types
const std::array<PieceType::underlying, 6> piece_types = {
    PieceType::underlying::PAWN,
    PieceType::underlying::KNIGHT,
    PieceType::underlying::BISHOP,
    PieceType::underlying::ROOK,
    PieceType::underlying::QUEEN,
    PieceType::underlying::KING
};

// Colors
const std::array<Color::underlying, 2> colors = {
    Color::underlying::WHITE,
    Color::underlying::BLACK
};

const std::array<int, 64> white_pawn_rewards = {
    0, 0, 0, 0, 0, 0, 0, 0, 
    15, 15, 15, 15, 15, 15, 15, 15,
    14, 14, 14, 14, 14, 14, 14, 14,
    13, 13, 13, 13, 13, 13, 13, 13,
    12, 12, 12, 12, 12, 12, 12, 12,
    11, 11, 11, 11, 11, 11, 11, 11,
    10, 10, 10, 10, 10, 10, 10, 10,
    0, 0, 0, 0, 0, 0, 0, 0
};

const std::array<int, 64> black_pawn_rewards = {
    0, 0, 0, 0, 0, 0, 0, 0, 
    10, 10, 10, 10, 10, 10, 10, 10,
    11, 11, 11, 11, 11, 11, 11, 11,
    12, 12, 12, 12, 12, 12, 12, 12,
    13, 13, 13, 13, 13, 13, 13, 13,
    14, 14, 14, 14, 14, 14, 14, 14,
    15, 15, 15, 15, 15, 15, 15, 15,
    0, 0, 0, 0, 0, 0, 0, 0
};

// Knight rewards as 26 + number of squares knight can move to
const std::array<int, 64> knight_rewards = {
    28, 29, 30, 30, 30, 30, 29, 28,
    29, 30, 32, 32, 32, 32, 30, 29,
    30, 32, 34, 34, 34, 34, 32, 30,
    30, 32, 34, 34, 34, 34, 32, 30,
    30, 32, 34, 34, 34, 34, 32, 30,
    30, 32, 34, 34, 34, 34, 32, 30,
    29, 30, 32, 32, 32, 32, 30, 29,
    28, 29, 30, 30, 30, 30, 29, 28
};

// Bishop rewards as 24 + number of squares bishop can move to
const std::array<int, 64> bishop_rewards = {
    31, 31, 31, 31, 31, 31, 31, 31,
    31, 33, 33, 33, 33, 33, 33, 31,
    31, 33, 35, 35, 35, 35, 33, 31,
    31, 33, 35, 37, 37, 35, 33, 31,
    31, 33, 35, 37, 37, 35, 33, 31,
    31, 33, 35, 35, 35, 35, 33, 31,
    31, 33, 33, 33, 33, 33, 33, 31,
    31, 31, 31, 31, 31, 31, 31, 31
};

// Rook rewards as 50 ig??
const std::array<int, 64> rook_rewards = {
    50, 50, 50, 50, 50, 50, 50, 50,
    50, 50, 50, 50, 50, 50, 50, 50,
    50, 50, 50, 50, 50, 50, 50, 50,
    50, 50, 50, 50, 50, 50, 50, 50,
    50, 50, 50, 50, 50, 50, 50, 50,
    50, 50, 50, 50, 50, 50, 50, 50,
    50, 50, 50, 50, 50, 50, 50, 50,
    50, 50, 50, 50, 50, 50, 50, 50
};

// Queen rewards as 90 plug 1 for each square it can move beyond the minimum 21
const std::array<int, 64> queen_rewards = {
    90, 90, 90, 90, 90, 90, 90, 90,
    90, 92, 92, 92, 92, 92, 92, 90,
    90, 92, 92, 92, 92, 92, 92, 90,
    90, 92, 92, 92, 92, 92, 92, 90,
    90, 92, 92, 92, 92, 92, 92, 90,
    90, 92, 92, 92, 92, 92, 92, 90,
    90, 92, 92, 92, 92, 92, 92, 90,
    90, 90, 90, 90, 90, 90, 90, 90
};

// King rewards are 1 for now. 
const std::array<int, 64> king_rewards = {
    1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1
};

// std::unordered_map<std::uint8_t, std::array<int, 64>> rewardMatrixMap = {
//     { PieceType::underlying::PAWN, white_pawn_rewards },
//     { PieceType::underlying::KNIGHT, knight_rewards },
//     { PieceType::underlying::BISHOP, bishop_rewards },
//     { PieceType::underlying::ROOK, rook_rewards },
//     { PieceType::underlying::QUEEN, queen_rewards },
//     { PieceType::underlying::KING, king_rewards }
// };




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


    void evaluate_helper(const int& is_white, const PieceType& piece_type, const std::uint64_t bitboard, long long& score) {
        const std::array<int, 64>* rewardMatrix;

        // select reward matrix:::
        if (piece_type == PieceType::underlying::PAWN) {
            if (is_white == 1) {
                rewardMatrix = &white_pawn_rewards;
            } else {
                rewardMatrix = &black_pawn_rewards;
            }
        } else if (piece_type == PieceType::underlying::KNIGHT) {
            rewardMatrix = &knight_rewards;
        } else if (piece_type == PieceType::underlying::BISHOP) {
            rewardMatrix = &bishop_rewards;
        } else if (piece_type == PieceType::underlying::ROOK) {
            rewardMatrix = &rook_rewards;
        } else if (piece_type == PieceType::underlying::QUEEN) {
            rewardMatrix = &queen_rewards;
        } else if (piece_type == PieceType::underlying::KING) {
            rewardMatrix = &king_rewards;
        } else {
            throw std::runtime_error("Piece type not defined");
        }

        // Loop through each bit and check if its set. If it is, add that to the evaulation score
        for (int i = 0; i < 64; i++){
            // Add positive values if white, negative if black
            score = score + is_white*(*rewardMatrix)[i];
        }
    }

    // Function to evaluate the board position
    long long evaluate() {
        long long score = 0;

        // 1 or -1 depending on if white or black
        int is_white = -1;

        // // Loop through all colors and piece types
        for (Color color : colors) {
            // Set color as white or black
            if (color == Color::underlying::WHITE) {is_white = 1;} else {is_white = -1;}

            for (PieceType piece_type : piece_types) {
                // Get the bitboard for the current piece type and color
                std::uint64_t bitboard = board.pieces(piece_type, color).getBits();

                // Add the value to score
                evaluate_helper(is_white, piece_type, bitboard, score);
            }
        }
        return score;
    }



    // Function to do tree search




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