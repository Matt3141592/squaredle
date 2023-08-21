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
node *table[262144];
node *ans[20];

//change these 5 variables
const int rows = 4;
const int cols = 4;
const int MAX = 12;
char grid[10][10] = {"entl", "mlea", "ielx", "lrpe"}; //[>=rows][>=cols] use 0 for blanks. lower case
int used[10][10]; //[>=rows+1][>=cols+1] 

int hash(char *str)
{
	unsigned long hash = 5381;
	
	while (*str)
		hash = ((hash << 5) + hash) + *(str++);
	
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
		if (strlen(str) > 3 && strlen(str) <= MAX)
		{
			int x = hash(str);
			node *n = malloc(sizeof(node));
			strcpy(n -> str, str);
			n -> next = table[x];
			table[x] = n;
		}
	fclose(in);
}

void unload(node *list[], int x)
{
	for (int i = 0; i < x; i++)
		freelist(list[i]);
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

void exists(char *word, int len)
{
	node *temp = ans[len];
	while (temp)
	{
		if (!strcasecmp(word, temp -> str))
			return;
		temp = temp -> next;
	}
	
	node *n = malloc(sizeof(node));
	n -> next = ans[len];
	strcpy(n -> str, word);
	ans[len] = n;
}

node *quick(node *list)
{
	if (!list)
		return NULL;
	node *small = NULL;
	node *big = NULL;
	node *piv = list;
	
	list = list -> next;
	while (list)
	{
		node *temp = list -> next;
		if (strcmp(list -> str, piv -> str) < 0)
		{
			list -> next = small;
			small = list;
		}
		else
		{
			list -> next = big;
			big = list;
		}
		list = temp;
	}
	small = quick(small);
	piv -> next = quick(big);
	
	node *temp = small;
	if (temp)
	{
		while (temp -> next)
			temp = temp -> next;
		temp -> next = piv;
	}
	else
		small = piv;
	return small;
}

void squaredle(int x, int y, char word[], int len)
{	
	if (!used[x+1][y+1])
		return;
		
	if (len >= MAX)
		return;
		
	used[x+1][y+1] = 0;
	
	word[len++] = grid[x][y];
	word[len] = '\0';
	if (check(word))
		exists(word, len);
	
	for (int i = -1; i <= 1; i++)
		for (int j = -1; j <= 1; j++)
			squaredle(x + i, y + j, word, len);
		
	used[x+1][y+1] = 1;
}

int main(void)
{
	clock_t begin = clock();
	load();
	
	for (int i = 1; i <= rows; i++)
		for (int j = 1; j <= cols; j++)
			used[i][j] = '0' != grid[i-1][j-1];
			
	char word[30];
	
	for (int i = 0; i < rows; i++)
		for (int j = 0; j < cols; j++)
			squaredle(i, j, word, 0);
			
	for (int i = 4; i <= MAX; i++)
	{
		if (!ans[i])
			continue;
		printf("\n%i letters: ", i);
		ans[i] = quick(ans[i]);
		node *temp = ans[i];
		int count = 0;
		while (temp)
		{
			if (!(count++ & 3))
				printf("\n");
			printf("%s   ", temp -> str);
			temp = temp -> next;
		}
		printf("\n");
	}
	unload(table, N);
	unload(ans, MAX + 1);
	printf("\n%f\n", (double)(clock() - begin) / CLOCKS_PER_SEC);
}
	
