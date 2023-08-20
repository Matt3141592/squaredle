#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>
#include <time.h>

typedef struct node
{
	char str[50];
	struct node *next;
}node;

const int N = 262144;
const int rows = 5;
const int cols = 5;
node *table[262144];

int hash(char *str)
{
	unsigned long hash = 5381;
	
	while (*str)
		hash = ((hash << 5) + hash) + tolower(*(str++));
	
	return hash & (N - 1);
}

int check(char *word)
{
    node *cursor = table[hash(word)];
    
    while (cursor != NULL)
    {
        if (!strcasecmp(cursor -> str, word))
            return 1;
        cursor = cursor -> next;
    }
    return 0;
}

void freelist(node *list)
{
    node *prev = list;
    while (list)
    {
        list = list -> next;
        free(prev);
        prev = list;
    }
}

void load(void)
{
	FILE *in = fopen("dictionary.txt", "r");
	char str[50];
	
	while (fscanf(in, "%s", str) != EOF)
		if (strlen(str) > 3)
		{
			int x = hash(str);
			node *n = malloc(sizeof(node));
			strcpy(n -> str, str);
			n -> next = table[x];
			table[x] = n;
		}
	fclose(in);
}

void unload(void)
{
	for (int i = 0; i < N; i++)
		freelist(table[i]);
}

void print(void)
{
	node *temp;
	for (int i = 0; i < N; i++)
	{
		temp = table[i];
		while (temp)
		{
			printf("%s\n", temp -> str);
			temp = temp -> next;
		}
	}
}

void squaredle(char grid[rows][cols], int used[rows][cols], int x, int y, char word[], int len)
{
	if (used[x][y])
		return;
	used[x][y] = 1;
	
	word[len++] = grid[x][y];
	word[len] = '\0';
	if (check(word))
		printf("%s\n", word);
	
	if (x + 1 < rows)
		squaredle(grid, used, x + 1, y, word, len);
	if (x - 1 >= 0)
		squaredle(grid, used, x - 1, y, word, len);
	if (y + 1 < cols)
		squaredle(grid, used, x, y + 1, word, len);
	if (y - 1 >= 0)
		squaredle(grid, used, x, y - 1, word, len);
	if ((x - 1 >= 0) && (y - 1 >= 0))
		squaredle(grid, used, x - 1, y - 1, word, len);
	if ((x + 1 < rows) && (y - 1 >= 0))
		squaredle(grid, used, x + 1, y - 1, word, len);
	if ((x - 1 >= 0) && (y + 1 < cols))
		squaredle(grid, used, x - 1, y + 1, word, len);
	if ((x + 1 < rows) && (y + 1 < cols))
		squaredle(grid, used, x + 1, y + 1, word, len);
		
	used[x][y] = 0;
}

int main(void)
{
	clock_t begin = clock();
	load();
	char grid[5][5] = {{'0','y','r','n','0'},
					   {'t','e','i','e','g'},
					   {'v','h','0','y','n'},
					   {'e','e','w','o','a'},
					   {'0','r','y','d','0'}}; 
	int used[5][5] = {0};
	
	for (int i = 0; i < rows; i++)
		for (int j = 0; j < cols; j++)
			used[i][j] = '0' == grid[i][j];

	char word[30];
	
	for (int i = 0; i < rows; i++)
		for (int j = 0; j < cols; j++)
			squaredle(grid, used, i, j, word, 0);
	unload();
	printf("%f\n", (double)(clock() - begin) / CLOCKS_PER_SEC);
}
	
