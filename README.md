# Lightweight Chess Bot Programming

Code build for the Kaggle Competition [here](https://www.kaggle.com/competitions/fide-google-efficiency-chess-ai-challenge)

This code Implements iterative deepening approach using c++ in ```my_chess_bot.cpp```. 

Uses main.py to communicate in python between competition ai and c++ agent. 

Good sources for learning about the methods:::
- [Chess Programming Wiki](https://www.chessprogramming.org/Main_Page)
- [Great YouTube Series](https://www.youtube.com/watch?v=_vqlIPDR2TU&list=PLFt_AvWsXl0cvHyu32ajwh2qU1i6hl77c)
- [Bitboard Chess Implementation Youtube Video](https://www.youtube.com/watch?v=QUNP-UjujBM&list=PLmN0neTso3Jxh8ZIylk74JpwfiWNI76Cs&index=1)
- [C++ Bitboard Implementation Used](https://github.com/Disservin/chess-library?tab=readme-ov-file)

### Running the c++ bot:::
Compile it:

```g++ -std=c++17 my_chess_bot.cpp -o my_chess_bot.out```

### Test bot vs a bot who plays all random random:::

```python test.py --model_1 c++_com.py```

### Play vs the bot yourself
```python test_vs_human.py --model c++_com.py```


