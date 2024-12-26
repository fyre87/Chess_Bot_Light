## Chess bot!

### Base code for Kaggle competition here
```https://www.kaggle.com/competitions/fide-google-efficiency-chess-ai-challenge```

Implements iterative deepening approach using c++ in ```my_chess_bot.cpp```. 

Uses main.py to communicate in python between competition ai and c++ agent. 

### Running the c++ bot:::
Compile it:

```g++ -std=c++17 my_chess_bot.cpp -o my_chess_bot.out```

### Test bot vs random:::

```python test.py --model_1 c++_com.py --model_2 c++_com.py```

### Play vs the bot yourself
```python test_vs_human.py --model c++_com.py```


