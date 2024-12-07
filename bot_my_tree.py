from Chessnut import Game
import random


# PIECE_VALUES = {'p': 100, 'n': 320, 'b': 330, 'r': 500, 'q': 900, 'k': 20000}


# 8x8 array which represents the number of spaces a knight can move to
knight_space = [
    [2, 3, 4, 4, 4, 4, 3, 2],
    [3, 4, 6, 6, 6, 6, 4, 3],
    [4, 6, 8, 8, 8, 8, 6, 4],
    [4, 6, 8, 8, 8, 8, 6, 4],
    [4, 6, 8, 8, 8, 8, 6, 4],
    [4, 6, 8, 8, 8, 8, 6, 4],
    [3, 4, 6, 6, 6, 6, 4, 3],
    [2, 3, 4, 4, 4, 4, 3, 2]
]

def eval_knight(row, col):
    starting_value = 250
    # Give a +10 bonus for each available move the knight has

    return reward = starting_value + knight_space[row-1][col-1] * 10
    # Give a +10 bonus for each available move the knight has
    # row_space = knight_space[row - 1]
    # col_space = knight_space[col - 1]

    # # The number of available moves the knight has
    # # (1, 1) -> 2
    # # (1, 3) -> 4
    # # (2, 2) -> 4
    # # (2, 3) -> 6
    # # (3, 3) -> 8
    # # If one is >=2 and the other is 3, we have extra moves
    # available_moves = row_space + col_space
    # if row_space >= 2 and col_space >= 2 and row_space != col_space:
    #     # One is two one is three, thus we have 6 total space
    #     available_moves = 6
    # elif row_space == 3 and col_space == 3:
    #     # Have 3 spaces on both sides, thus we have 8 total spaces
    #     available_moves = 8

    # return reward = starting_value + available_moves * 10


def eval_pawn(row, col):
    starting_value = 100
    
    return starting_value 




def eval_piece(piece, row, col):
    # Print if is white
    is_white = piece.isupper()

    if not is_white:
        # Flip the board so that we can evaluate everything as if we were white
        row = 9 - row

    if piece.lower() == "n":
        return eval_knight(row, col)
    elif piece.lower() == "p":
        return eval_pawn(row, col)
    elif piece.lower() == "b":
    
    elif piece.lower() == "r":

    elif piece.lower() == "q":

    elif piece.lower() == "k":
    
    else:
        raise ValueError("Invalid piece: ", piece)



def eval_game():
    game = Game()
    fen = str(game.board)

    # Positive is good for white, negative is good for black
    evaluation = 0

    # Start at the top left of the board
    row = 8
    col = 1
    for i in range(0, len(fen)):
        if fen[i] == '/':
            # Move down one row and reset the column
            row -= 1
            col = 1
        elif fen[i].isdigit():
            col += int(fen[i])
        else:
            evaluation += eval_piece(fen[i], row, col)