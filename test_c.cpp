

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

    // for (const auto &move : moves) {
    //     // std::cout << move << std::endl;
    //     // std::cout << uci::moveToUci(move) << std::endl;
    // }

    // std::uint64_t bitboard = board.pieces(piece_type, color).getBits();
    // std::cout << board.pieces(PieceType::KING, Color::underlying::WHITE).getBits() << std::endl;

    // std::cout << Color::underlying::WHITE << std::endl;



    // board.bits()
    // board.pieces(P ieceType::KING, 0) 

    // for (int i=0; i < 100; i++){
    //     std::cout << "Weight: " << i << "\n";
    //     std::bitset<64> x(EvalWeight(i));
    //     std::cout << x << "\n";
    // }

    // Evals base for each type. 
    for (int pt = 0; pt < 7; pt++){
        std::cout << "Piece type " << pt << ": " << (short)EvalWeight(112 + pt) << std::endl;
    }

    // Additional evals by square:
    for (int pt = 0; pt < 1; pt++) // or 7, depending on your code
    {
        for (int sq = 0; sq < 64; sq++)
        {
            // We want the midgame and endgame PST for "pt" on "sq" as White:
            long index  = (long)(pt * 64 + (sq >> 3) ^ 0);
            int  shift  = (int)((0x01455410 >> (sq * 4)) * 8); 
            long masked = (long)(packedData[index] >> shift) & 0xFF00FF;

            // Real code multiplies by 24 times some linear interpolation of early game and end game. 
            int mgWhite = (short)(masked);
            int egWhite = (masked / 0x10000);
            // std::cout << sq << " " << mgWhite << std::endl;
            std::cout << sq << " " << egWhite << std::endl;

            // std::cout << mgWhite << std::endl;
            // std::cout << egWhite << std::endl;

            // Similarly for Black:
            // index  = (long)(pt * 64 + (sq >> 3) ^ 0b111);
            // masked = (long)(packedData[index] >> shift) & 0xFF00FF;
            // int mgBlack = (int)(masked & 0xFF);
            // int egBlack = (int)((masked >> 8) & 0xFF);

            // Save mgWhite, egWhite, mgBlack, egBlack to your own arrays.
        }
    }


    return 0;
}