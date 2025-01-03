#include <iostream>  // For input (std::cin), output (std::cout), and standard I/O operations
#include <string>    // For using std::string to store and manipulate strings
#include <vector>    // For using std::vector to store a list of moves
#include <sstream>   // For using std::istringstream to split space-separated strings
#include <cstdlib>   // For using std::srand and std::rand to generate random numbers
#include <ctime>     // For using std::time to seed the random number generator with the current time
#include <chrono>
#include <fstream>
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


const std::array<int, 64> black_pawn_rewards = {
    0, 0, 0, 0, 0, 0, 0, 0, 
    15, 15, 15, 15, 15, 15, 15, 15,
    14, 14, 14, 14, 14, 14, 14, 14,
    13, 13, 13, 13, 13, 13, 13, 13,
    12, 12, 12, 12, 12, 12, 12, 12,
    11, 11, 11, 11, 11, 11, 11, 11,
    10, 10, 10, 10, 10, 10, 10, 10,
    0, 0, 0, 0, 0, 0, 0, 0
};

const std::array<int, 64> white_pawn_rewards = {
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

// Queen rewards as 90 plus 1 for each square it can move beyond the minimum 21
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



class Chess {
private:
    int current_player_color;      // 0 for white, 1 for black
    std::string current_fen;       // Current board position in FEN format
    std::string available_moves; //SHOULDN"T NEED
    std::string chosen_move;       // Chosen move which will be printed to output
    std::string best_move_temp;

    struct KillerMove {
        Move move;
        long long score = -1000000000;  // Initialize with a very low score
    };
    KillerMove killer_moves[2][20]; // Keeps track of previously chosen moves and looks at those first

    Board board;  // Board object to store the current board position
    const std::string logFile = "logs/log.txt";


    // Example input:::
    // input_to_cpp_agent:  
    // current_player_color: 0
    // current_fen: rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1
    void reset_killer_moves() {
        std::fill(&killer_moves[0][0], &killer_moves[0][0] + 2 * 20, KillerMove{Move(), -1000000000});
    }

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
            if (bitboard & (1ULL << i)) {
                // Add positive values if white, negative if black
                score = score + is_white*(*rewardMatrix)[i];
            }
        }
    }

    // Function to evaluate the board position
    // Returns positive values if the input color is winning and negative if the input color is losing
    long long evaluate(int color) {
        long long score = 0;

        // Check if game is over and why
        auto [reason, result] = board.isGameOver();

        if (result == GameResult::NONE){
            // 1 or -1 depending on if white or black
            int is_white = -1;
            // Loop through all colors and piece types
            for (Color color_2 : colors) {
                // Set color as white or black
                if (color_2 == Color::underlying::WHITE) {is_white = 1;} else {is_white = -1;}

                for (PieceType piece_type : piece_types) {
                    // Get the bitboard for the current piece type and color
                    std::uint64_t bitboard = board.pieces(piece_type, color_2).getBits();

                    // Add the value to score
                    evaluate_helper(is_white, piece_type, bitboard, score);
                }
            }
            // Score is always positive if the input color is winning and negative if losing
            return color*score;
        }else{
            if (reason == GameResultReason::CHECKMATE){
                // You just got mated!
                return -1000000;
            }else{
                // Otherwise it is a draw
                return 0;
            }
        }
    }

    // Helper function for sorting moves
    uint8_t get_piece_value(const PieceType& piece_type){
        if (piece_type == PieceType::underlying::NONE){
            return 1;
        }else if (piece_type == PieceType::underlying::PAWN){
            return 1;
        }else if (piece_type == PieceType::underlying::KNIGHT){
            return 3;
        }else if (piece_type == PieceType::underlying::BISHOP){
            return 4;
        }else if (piece_type == PieceType::underlying::ROOK){
            return 5;
        }else if (piece_type == PieceType::underlying::QUEEN){
            return 9;
        }else{
            return 1;
        }
    }

    bool move_is_check(const Move& move){
        // Make the move
        board.makeMove(move);
        // Check if the move puts the king in check
        bool is_check = board.inCheck();
        // Unmake the move
        board.unmakeMove(move);
        return is_check;
    }

    // Sort the moves with moves that are likely to be better first
    // That way we can prune off other moves quicker since we are using alpha-beta pruning
    void sort_moves(Movelist& moves, int& current_depth) {
        // loop through all moves
        for (auto &move : moves) {
            // Look at checks first
            if (move == killer_moves[0][current_depth].move){
                move.setScore(30000);
            }
            else if (move == killer_moves[1][current_depth].move){
                move.setScore(20000);
            }
            else if (move_is_check(move)){
                move.setScore(10000);
            }
            // Check for captures
            // A capture is better if it is a lower value piece capturing a higher value piece
            else if (board.isCapture(move)){
                move.setScore(1000);
                PieceType piece_captured = board.at(move.to()).type();
                PieceType piece_attacker = board.at(move.from()).type();
                move.setScore(1000 - get_piece_value(piece_attacker) + get_piece_value(piece_captured));
            }
            // Check for promotions
            else if (move.typeOf() == Move::PROMOTION){
                move.setScore(100);
            }
            // Check for castling
            else if (move.typeOf() == Move::CASTLING){
                move.setScore(50);
            }
            // All other moves just look at randomly
            else{
                move.setScore(0);
            }
        }

        // Sort the moves based on the score
        std::sort(moves.begin(), moves.end(), [](const Move& a, const Move& b) {
            return a.score() > b.score();
        });
    }

    long long negamax(const std::chrono::time_point<std::chrono::steady_clock>& start_time, 
                     const double& time_limit, 
                     const int depth,
                     const int max_depth,
                     long long alpha, 
                     long long beta, 
                     const int color,
                     const bool is_root = true) {

        auto current_time = std::chrono::steady_clock::now();
        std::chrono::duration<double> elapsed_time = current_time - start_time;
        // Check time limit 
        if (elapsed_time.count() > time_limit) {
            throw std::runtime_error("Time limit exceeded");
        }

        int current_depth = max_depth - depth;

        // Initialize variables
        Movelist moves;
        movegen::legalmoves(moves, board);

        // If reached depth, game is over, or time is up, return the evaluation
        if (depth == 0 || moves.size() == 0){// || elapsed_time.count() > time_limit) {
            return evaluate(color);
        }

        // Sort the moves
        sort_moves(moves, current_depth);

        long long best_score = -1000000000; // Best move starts at worst possible outcome

        // Loop through all moves
        for (const auto &move : moves) {
            // Make the move
            board.makeMove(move);

            // Recursively call negamax
            long long score;
            score = -negamax(start_time, time_limit, depth - 1, max_depth, -beta, -alpha, -color, false);

            // Unmake the move
            board.unmakeMove(move);

            // If the score is better than the best score, update the best score and best move
            if (score > best_score) {
                best_score = score;
                if (is_root) {
                    best_move_temp = uci::moveToUci(move);
                }
            }

            // Update the killer move list based on the score
            if (score > killer_moves[0][current_depth].score) {
                killer_moves[1][current_depth] = killer_moves[0][current_depth];  // Shift the lower move down
                killer_moves[0][current_depth] = {move, score};           // Store the better move
            }
            else if (score > killer_moves[1][current_depth].score) {
                killer_moves[1][current_depth] = {move, score};           // Store the second-best move
            }

            // if (is_root){
            //     std::ofstream logStream(logFile, std::ios::app);
            //     logStream << "MOVE: " << move << " Score: " << score << " alpha: " << alpha << std::endl;
            //     logStream.close();
            // }

            alpha = std::max(alpha, score);
            if (alpha >= beta) {
                break; // Alpha-beta pruning
            }
        }

        return best_score;
    }

    void iterativeDeepening(const double time_limit, Color current_player_color) {
        using clock = std::chrono::steady_clock;
        auto start_time = clock::now();
        int max_depth = 2;

        // Reset killer moves
        reset_killer_moves();

        while (true) {
            // Check elapsed time
            auto current_time = clock::now();
            std::chrono::duration<double> elapsed_time = current_time - start_time;
            if (elapsed_time.count() > time_limit) {
                break; // Stop searching if time is up
            }

            try {
                // Call negamax with increasing depth
                negamax(start_time, time_limit, max_depth, max_depth, 
                        -1000000000,
                         1000000000, 
                        current_player_color, true);
                chosen_move = best_move_temp; // Store the best move from the latest completed search
                // std::ofstream logStream(logFile, std::ios::app);
                // logStream << max_depth << " " << chosen_move << std::endl;
            } catch (const std::exception& e) {
                // Handle exceptions if needed (e.g., time limit exceeded inside negamax)
                break;
            }

            max_depth = max_depth + 2; // Increase the search depth
        }
    }

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
        // Set current player color to be 1 if white, and -1 if black
        if (current_player_color == 0) {current_player_color = 1;}else{current_player_color = -1;}
        std::cin.ignore(); // To consume the newline character

        // Read current FEN string
        std::getline(std::cin, current_fen);

        // Read available moves (space-separated)
        std::getline(std::cin, available_moves);

        // Update the board object with the current FEN string
        board = Board(current_fen);

    }

    void think(){
        std::chrono::time_point<std::chrono::steady_clock> start_time = std::chrono::steady_clock::now();
        double thinking_time = 0.1;
        Movelist moves;
        movegen::legalmoves(moves, board);

        // Start by choosing some random move
        if (moves.size() > 0) {
            chosen_move = uci::moveToUci(moves[0]);
        } else {
            // I don't think this should ever happen
            chosen_move = "No move available";
        }
        
        // Search tree search through moves until time is out
        try{
            iterativeDeepening(thinking_time, current_player_color);
        }
        catch (...){
            // Run out of time
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
