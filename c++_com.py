

"""
WP note:

Source: https://www.kaggle.com/code/ashketchum/an-example-c-bot-outputs-a-randomly-chosen-move

"""

from subprocess import Popen, PIPE
from threading import Thread
from queue import Queue, Empty

import atexit
import os
import sys
import inspect

from Chessnut import Game


my_agent_process = None
t = None
q = None


def cleanup_process():
    global my_agent_process
    if my_agent_process is not None:
        my_agent_process.kill()


def enqueue_output(out, queue):
    for line in iter(out.readline, b''):
        queue.put(line)
    out.close()


def z_cpp_agent(observation):
    """
    a wrapper around a c++ agent.

    Note:
        The last function (i.e. the last "def") in our main.py file is called by the kaggle game runner.
        So, it should be the one that accepts an observation as argument and returns an action.
        This function "cpp_agent" will be our last def in the file.

    What this function does:
        1. If the c++ agent is not yet spawned:

            1.1. We will spawn the c++ agent using "Popen" of subprocess module with
            the ability to write to stdin of the c++ agent and read from stdout and stderr of the c++ agent.

            1.2. We also start a thread using "Thread" of threading module to read from the stderr of the c++ agent.
            I believe that this was done because reading stderr should not be blocking, i.e., if it was read similar to
            stdout using "readline", then, that call would block if there were no statements printed
            to error stream by the c++ agent.

        2. By this point, we have our c++ agent spawned. We write the gameInfo to its stdin.
        Here, we can choose whatever format we want to write it in. Just remember that c++ agent should parse that.
        You can modify this to your liking.
        I am choosing the current player color as integer in one line, game-board-string in the next line, and,
        comma separated moves in the third line.

        3. We wait for our c++ bot to make an output. The "readline" function is blocking and will return whenever
        our c++ agent prints an action to its output stream.

        4. We read all the data (if any) printed to err stream of the c++ agent, and print it to the stderr of this
        python process.

        5. We return the move that we got in step-3.
        Remember that this function must accept an observation and return an action.
    """
    global my_agent_process, t, q

    agent_process = my_agent_process

    
    if agent_process is None:
        src_file_path = inspect.getfile(lambda: None)  # the path to this main.py file. https://stackoverflow.com/a/53293924
        cwd = os.getcwd()
        ##### IMPORTANT:::: Use this commented line when you submit to kaggle
        # cwd = os.path.split(src_file_path)[0]
        file_path = os.path.join(cwd, "my_chess_bot.out")
        file_size = os.path.getsize(file_path)
        print(f"The size of '{file_path}' is {file_size/1000} kilobytes.")
        agent_process = Popen(["./my_chess_bot.out"], stdin=PIPE, stdout=PIPE, stderr=PIPE, cwd=cwd)
        my_agent_process = agent_process
        atexit.register(cleanup_process)

        # following 4 lines from https://stackoverflow.com/questions/375427/a-non-blocking-read-on-a-subprocess-pipe-in-python
        q = Queue()
        t = Thread(target=enqueue_output, args=(agent_process.stderr, q))
        t.daemon = True  # thread dies with the program
        t.start()

    # read observation, and, send inputs to our cpp agent in required format
    # Read observation:
    game = Game(observation.board)
    moves = list(game.get_moves())
    
    # Our cpp bot expects:
    # first line: current player color (0 for white, 1 for black)
    # second line: board FEN string
    # third line: space separated available moves
    # Form the input to our cpp agent:
    input_to_cpp_agent = f"{0 if game.state.player == 'w' else 1}\n"  # first line
    input_to_cpp_agent += f"{game.get_fen()}\n"  # add second line
    input_to_cpp_agent += f"{' '.join(moves)}\n"  # add third line




    # send it to the cpp agent
    agent_process.stdin.write(input_to_cpp_agent.encode())
    agent_process.stdin.flush()

    # wait for data written to stdout
    agent_res = (agent_process.stdout.readline()).decode()
    agent_res = agent_res.strip()  # trim any whitespace in the outputted move

    # get the data printed to err stream of c++ agent, and print to the err stream of this python process.
    # remember that a thread running the function "enqueue_output" is constantly reading err stream of the c++ agent
    # and putting the data in a queue named "q". Here, we just get from that queue.
    while True:
        try:
            line = q.get_nowait()
        except Empty:
            # no standard error received, break
            break
        else:
            # standard error output received, print it out
            print(line.decode(), file=sys.stderr, end='')

    # return the move
    return agent_res