# first let's make sure you have internet enabled
from kaggle_environments import make
import argparse
import chess
import chess.svg
from importlib import import_module
import os
import sys
import inspect
import pygame


# Constants for the chessboard
WIDTH, HEIGHT = 600, 600
SQUARE_SIZE = WIDTH // 8
WHITE = (255, 255, 255)
BLACK = (100, 100, 100)

# Load piece images
def load_images():
    pieces = {}
    for piece in ["p", "n", "b", "r", "q", "k", "WP", "WN", "WB", "WR", "WQ", "WK"]:
        pieces[piece] = pygame.image.load(os.path.join("assets", f"{piece}.png"))
        pieces[piece] = pygame.transform.scale(pieces[piece], (SQUARE_SIZE, SQUARE_SIZE))
    return pieces

# Draw the board
def draw_board(screen, board, images):
    for row in range(8):
        for col in range(8):
            color = WHITE if (row + col) % 2 == 0 else BLACK
            pygame.draw.rect(screen, color, pygame.Rect(col * SQUARE_SIZE, row * SQUARE_SIZE, SQUARE_SIZE, SQUARE_SIZE))
            piece = board.piece_at(chess.square(col, 7 - row))
            if piece:
                if piece.symbol().isupper():
                    image = "W" + piece.symbol()
                else:
                    image = piece.symbol()
                screen.blit(images[image], (col * SQUARE_SIZE, row * SQUARE_SIZE))

# Convert mouse position to chess move
def get_square_from_mouse(pos):
    x, y = pos
    col = x // SQUARE_SIZE
    row = 7 - (y // SQUARE_SIZE)
    return chess.square(col, row)


class Observation:
    """A class to encapsulate the board and moves for the bot."""
    def __init__(self, board, moves, player_color):
        self.board = board
        self.moves = moves
        self.player_color = player_color # 0 for white, 1 for black
# Game loop
def play_human_vs_bot(bot_module):
    pygame.init()
    screen = pygame.display.set_mode((WIDTH, HEIGHT))
    pygame.display.set_caption("Human vs Bot Chess")
    clock = pygame.time.Clock()

    # Load images
    images = load_images()

    # Initialize the board
    board = chess.Board()

    # Load bot
    def load_last_function_from_module(module_name):
        module = import_module(module_name)
        functions = [func for name, func in inspect.getmembers(module, inspect.isfunction)]
        return functions[-1]

    def convert_file_to_module(file_path):
        file_path = file_path[:-3]
        file_path = file_path.replace(os.sep, ".")
        directory = os.path.dirname(file_path)
        if directory and directory not in sys.path:
            sys.path.append(directory)
        return file_path

    module_name = convert_file_to_module(bot_module)
    bot_function = load_last_function_from_module(module_name)

    running = True
    selected_square = None

    while running:
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                running = False

            elif event.type == pygame.MOUSEBUTTONDOWN:
                pos = pygame.mouse.get_pos()
                selected_square = get_square_from_mouse(pos)

            elif event.type == pygame.MOUSEBUTTONUP:
                pos = pygame.mouse.get_pos()
                destination_square = get_square_from_mouse(pos)
                move = chess.Move(selected_square, destination_square)

                if move in board.legal_moves:
                    board.push(move)
                    selected_square = None

                    # Check if the game is over
                    if board.is_game_over():
                        running = False
                        break

                    # Bot's move
                    observation = Observation(board.fen(), [move.uci() for move in board.legal_moves], 1)
                    bot_move_uci = bot_function(observation)
                    bot_move = chess.Move.from_uci(bot_move_uci)
                    if bot_move in board.legal_moves:
                        board.push(bot_move)

        # Draw everything
        draw_board(screen, board, images)
        pygame.display.flip()
        clock.tick(30)

    pygame.quit()
    print("Game Over!")
    print(board.result())
    print(board)

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument('--model', type=str, default='INPUT A MODEL')
    args = parser.parse_args()



    print("Playing human vs bot")
    print("Model:", args.model)
    play_human_vs_bot(args.model)
        


