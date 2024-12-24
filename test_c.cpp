

#include "chess.hpp"

using namespace chess;


constexpr std::array<std::uint64_t, 63> packedData = {
    0x0000000000000000, 0x2328170f2d2a1401, 0x1f1f221929211507, 0x18202a1c2d261507,
    0x252e3022373a230f, 0x585b47456d65321c, 0x8d986f66a5a85f50, 0x0002000300070005,
    0xfffdfffd00060001, 0x2b1f011d20162306, 0x221c0b171f15220d, 0x1b1b131b271c1507,
    0x232d212439321f0b, 0x5b623342826c2812, 0x8db65b45c8c01014, 0x0000000000000000,
    0x615a413e423a382e, 0x6f684f506059413c, 0x82776159705a5543, 0x8b8968657a6a6150,
    0x948c7479826c6361, 0x7e81988f73648160, 0x766f7a7e70585c4e, 0x6c7956116e100000,
    0x3a3d2d2840362f31, 0x3c372a343b3a3838, 0x403e2e343c433934, 0x373e3b2e423b2f37,
    0x383b433c45433634, 0x353d4b4943494b41, 0x46432e354640342b, 0x55560000504f0511,
    0x878f635c8f915856, 0x8a8b5959898e5345, 0x8f9054518f8e514c, 0x96985a539a974a4c,
    0x9a9c67659e9d5f59, 0x989c807a9b9c7a6a, 0xa09f898ba59c6f73, 0xa1a18386a09b7e84,
    0xbcac7774b8c9736a, 0xbab17b7caebd7976, 0xc9ce7376cac57878, 0xe4de6f70dcd87577,
    0xf4ef7175eedc7582, 0xf9fa8383dfe3908e, 0xfffe7a81f4ec707f, 0xdfe79b94e1ee836c,
    0x2027252418003d38, 0x4c42091d31193035, 0x5e560001422c180a, 0x6e6200004d320200,
    0x756c000e5f3c1001, 0x6f6c333f663e3f1d, 0x535b55395c293c1b, 0x2f1e3d5e22005300,
    0x004c0037004b001f, 0x00e000ca00be00ad, 0x02e30266018800eb, 0xffdcffeeffddfff3,
    0xfff9000700010007, 0xffe90003ffeefff4, 0x00000000fff5000d,
};

// Define the EvalWeight function
int EvalWeight(int item) {
    return static_cast<int>(packedData[item >> 1] >> ((item & 1) * 32));
}



int main () {

    Board board = Board("r1bqkb1r/pppppppp/8/8/2B1n3/5Q2/PPPP1PPP/RNB1K1NR w KQkq - 0 1");

    Movelist moves;
    movegen::legalmoves(moves, board);

    std::uint64_t bitboard = board.pieces(PieceType::underlying::PAWN, Color::underlying::WHITE).getBits();

    std::cout << bitboard << std::endl;
    // for (const auto &move : moves) {
    //     // std::cout << move << std::endl;
    //     // std::cout << uci::moveToUci(move) << std::endl;
    // }

    // std::uint64_t bitboard = board.pieces(piece_type, color).getBits();
    // std::cout << board.pieces(PieceType::KING, Color::underlying::WHITE).getBits() << std::endl;

    // std::cout << Color::underlying::WHITE << std::endl;

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            std::cout << ((bitboard >> (i * 8 + j)) & 1) << " ";
        }
        std::cout << std::endl;
    }

    // board.bits()
    // board.pieces(P ieceType::KING, 0) 

    // for (int i=0; i < 100; i++){
    //     std::cout << "Weight: " << i << "\n";
    //     std::bitset<64> x(EvalWeight(i));
    //     std::cout << x << "\n";
    // }

    // Piece is half 
    // Evals base for each type. 
    for (int pt = 0; pt < 7; pt++){
        std::cout << "Piece type " << pt << ": " << (short)EvalWeight(112 + pt) << std::endl;
        std::cout << "EG: " << pt << ": " << (EvalWeight(112 + pt)/ 0x10000) << std::endl;
    }

    // Typically 6 piece types: 0..5 (Pawn, Knight, Bishop, Rook, Queen, King).
    // If your engine uses 0..6 for some reason, adjust accordingly.
    for (int pieceType = 0; pieceType < 6; pieceType++)
    {
        // Toggle to 'false' if you want to see black’s mirrored PST
        bool pieceIsWhite = true;
        
        // Print row by row. If you prefer rank 8 -> rank 1 top-down, loop rank = 7 down to 0.
        for (int rank = 7; rank >= 0; rank--)
        {
            for (int file = 0; file < 8; file++)
            {
                int sqIndex = rank * 8 + file;

                // This matches: pieceType * 64 + (sqIndex >> 3) ^ (pieceIsWhite ? 0 : 0b111)
                int arrayIndex = pieceType * 64 + ((sqIndex >> 3) ^ (pieceIsWhite ? 0 : 0b111));

                // Next, compute the shift:
                // (0x01455410 >> (sqIndex * 4)) & 0xF  gives us a nibble, then multiply by 8
                int nibble = (int)((0x01455410UL >> (sqIndex * 4)) & 0xF);
                int shiftAmount = nibble * 8;


                // Extract mg/eg from packedData:
                uint64_t chunk = packedData[arrayIndex];
                uint val   = (uint)((chunk >> shiftAmount) & 0xFF00FF);
                
                int mg = (int)(val & 0xFF);         // middle-game
                int eg = (int)((val >> 16) & 0xFF); // endgame

                // Print mg, eg pair
                std::cout << "({" << mg<< ","<<eg<<"}) ";
            }
            std::cout<<std::endl;
        }
        std::cout<<std::endl;
    }


    return 0;
}