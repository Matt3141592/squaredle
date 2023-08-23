# Squaredle
A program that recursively checks all routes to produce possible words that are checked against a dictionary.

rows and cols must be changed to match the dimensions of the puzzle.

The size of grid and used arrays just need to big enough (check comments). Can leave as 10 and 10 as puzzle doesn't get that big.

![Screenshot from 2023-08-21 14-38-55](https://github.com/Matt3141592/squaredle/assets/85036247/ffa1fd94-1c90-41db-ab71-652326af89d4)

For the above puzzle, grid should be -> char grid[10][10] = {"0yrn0", "teieg", "vh0yn", "eewoa", "0ryd0"} and rows and cols should both be 5.

Compile with: make program or make v2

v2 runs about 9 times quicker than program does as it uses a trie instead of a hash table to store the dictionary. This allows it to stop searching for a word once it knows that there are no words that starts with its prefix.

MAX does not affect v2 as much but still helps as less words are loaded into the trie.
