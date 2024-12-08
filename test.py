# first let's make sure you have internet enabled
from kaggle_environments import make
import argparse
import chess
import chess.svg
from importlib import import_module
import os
import sys
import inspect

def test_models(env, model_1, model_2, n_games):
    points = [0, 0]
    time = [0, 0]
    for i in range(0, n_games):
        # Run the game
        result = env.run([model_1, model_2])

        # Store the result of the game
        points[0] += result[-1][0]['reward']
        time[0] += (10-result[-1][0]['observation']['remainingOverageTime'])/n_games
        points[1] += result[-1][1]['reward']
        time[1] += (10-result[-1][1]['observation']['remainingOverageTime'])/n_games

    print(model_1, "points:", points[0])
    print(" ", model_1, "average used time:", time[0])
    print(model_2, "points:", points[1])
    print(" ", model_2, "average used time:", time[1])


def render_game(env, model_1, model_2):
    result = env.run([model_1, model_2])

    game_name = "chess_game.html"
    html_content = env.render(mode="html")
    with open(game_name, "w") as f:
        print("Saving game out as:", game_name)
        f.write(html_content)


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument('--model_1', type=str, default='random')
    parser.add_argument('--model_2', type=str, default='random')
    parser.add_argument('--n_games', type=int, default=10)
    parser.add_argument('--render_game', action='store_true')

    args = parser.parse_args()



    print("Model 1:", args.model_1)
    print("Model 2:", args.model_2)
    print("n_games:", args.n_games)

    env = make("chess", debug=True)

    # Test the model
    test_models(env, args.model_1, args.model_2, args.n_games)

    if args.render_game == True:
        render_game(env, args.model_1, args.model_2)


