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

g++ -std=c++17 my_chess_bot.cpp -o my_chess_bot.out

g++ -std=c++17 -Os -ffunction-sections -fdata-sections -Wl,-dead_strip my_chess_bot.cpp -o my_chess_bot.out

x86_64-linux-musl-g++ -std=c++17 -Os -ffunction-sections -fdata-sections -Wl,--gc-sections my_chess_bot.cpp -o my_chess_bot.out

x86_64-linux-musl-g++ -static -std=c++17 -Os -ffunction-sections -fdata-sections -Wl,--gc-sections my_chess_bot.cpp -o my_chess_bot.out

x86_64-linux-musl-strip my_chess_bot.out

### Running the c++ chess-library test:::
From https://disservin.github.io/chess-library/pages/examples.html

g++ -std=c++17 -O3 -march=native -o test_c test_c.cpp

./test_c

### Test bot vs random:::
python test.py --model_1 bot_simple.py --n_games 1

python test.py --model_1 c++_com.py --model_2 c++_com.py

python test_vs_human.py --model c++_com.py

### Docker
docker run --rm -it --platform linux/amd64 ubuntu:20.04

docker run --rm -v $(pwd):/src -w /src gcc g++ my_chess_bot.cpp -o my_chess_bot.out

docker run --rm -v $(pwd):/src -w /src alpine sh -c "apk add --no-cache g++ && g++ my_chess_bot.cpp -o my_chess_bot.out"

