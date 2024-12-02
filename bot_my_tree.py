from Chessnut import Game
import random




def eval_piece(piece, row, col):





def eval_game():
    game = Game()
    fen = str(game.board)

    # Positive is good for white, negative is good for black
    evaluation = 0

    # Start at the top left of the board
    row = 1
    col = 1
    for i in range(0, len(fen)):
        if fen[i] == '/':
            # Move down one row and reset the column
            row += 1
            col = 1
        elif fen[i].isnumeric():
            col += int(fen[i])
        else:
            evaluation += eval_piece(fen[i], row, col)