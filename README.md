## Chess bot!

### To add:::
git add .

git commit -m "Your descriptive commit message"

git push origin main


### My initial add:::
git init

git add .

git commit -m "Initial commit"

git remote add origin https://github.com/YOUR_USERNAME/REPOSITORY_NAME.git

git branch -M main

git push -u origin main

### Running the c++ bot:::
Compile it:

g++ my_chess_bot.cpp -o my_chess_bot.out

### Running the c++ chess-library test:::
From https://disservin.github.io/chess-library/pages/examples.html

g++ -std=c++17 -O3 -march=native -o test_c test_c.cpp

./test_c

### Test bot vs random:::

python test.py --model_1 bot_simple.py --n_games 1

