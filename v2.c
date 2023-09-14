#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct node
{
	struct node *next[26];
	int end;
}node;

typedef struct linked
{
	char str[50];
	struct linked *next;
}linked;

node *tree;
linked *ans[20];
int letters[26];

//change these 5 variables
const int rows = 6;
const int cols = 6;
const int MAX = 36;
char grid[10][10] = {"cexhpc", "aniaay", "cttmrm", "cirgde", "enooir", "eltcal"}; //[>=rows][>=cols] use 0 for blanks. lower case
int used[10][10]; //[>=rows+1][>=cols+1] 

void freelist(linked *list)
{
	linked *prev = list;
	while (list)
	{
		list = list -> next;
		free(prev);
		prev = list;
	}
}

void unloadlinked()
{
	for (int i = 0; i < 20; i++)
		freelist(ans[i]);
}

node *newNode(void) // creates a new empty node.
{
	node *n = malloc(sizeof(node));
	
	n -> end = 0;
	for (int i = 0; i < 26; i++)
		n -> next[i] = NULL;
	
	return n;
}

void insert(node *root, char *str) // inserts word
{
	while (*str && *str != '\'')
	{
		if (!root -> next[*str - 'a'])
			root -> next[*str - 'a'] = newNode();
		
		root = root -> next[*str++ - 'a'];
	}
	
	root -> end = 1;
}

int valid(char *str)
{
	int a[26] = {0};
	while (*str)
		a[*str++ - 'a']++;
		
	for (int i = 0; i < 26; i++)
		if (a[i] > letters[i])
			return 0;
	return 1;
}

void load(int x)
{
	char dics[3][15] = {"dictionary.txt", "large.txt", "nwl.txt"};
	FILE *in = fopen(dics[x], "r");
	char buffer[50];
	printf("%s\n", dics[x]);
	
	while (fscanf(in, "%s", buffer) != EOF)
		if (strlen(buffer) > 3 && valid(buffer) && strlen(buffer) <= MAX)
			insert(tree, buffer);
	fclose(in);
}

void unload(node *root)
{
	for (int i = 0; i < 26; i++)
		if (root -> next[i])
			unload(root -> next[i]);
	free(root);
}

int check(node *root, char *str) //returns 0 if word is in the dictionary
{
	while (*str)
	{
		if (!root -> next[*str - 'a'])
			return 2;
		root = root -> next[*str++ - 'a'];
	}
	if (!root -> end)
		return 1;
	root -> end = 0;
	return 0;
}

void add(char *word, int len)
{
	linked *n = malloc(sizeof(linked));
	n -> next = ans[len];
	strcpy(n -> str, word);
	ans[len] = n;
}

linked *quick(linked *list)
{
	if (!list)
		return NULL;
	linked *small = NULL;
	linked *big = NULL;
	linked *piv = list;
	
	list = list -> next;
	while (list)
	{
		linked *temp = list -> next;
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
	
	linked *temp = small;
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
		
	used[x+1][y+1] = 0;
	
	word[len++] = grid[x][y];
	word[len] = '\0';
	int a = check(tree, word);
	if (!a)
		add(word, len);
	if (a == 2)
	{
		used[x+1][y+1] = 1;
		return;
	}
	
	for (int i = -1; i <= 1; i++)
		for (int j = -1; j <= 1; j++)
			squaredle(x + i, y + j, word, len);
		
	used[x+1][y+1] = 1;
}		

int main(int argc, char *argv[])
{
	clock_t begin = clock();
	tree = newNode();
	
	for (int i = 1; i <= rows; i++)
		for (int j = 1; j <= cols; j++)
		{
			used[i][j] = '0' != grid[i-1][j-1];
			letters[grid[i-1][j-1] - 'a']++;
		}
		
	if (argc > 2)
		load(1);
	else if (argc == 2)
		load(2);
	else 
		load(0);
			
	char word[30];
	
	for (int i = 0; i < rows; i++)
		for (int j = 0; j < cols; j++)
			squaredle(i, j, word, 0);
			
	for (int i = 4; i < 20; i++)
	{
		if (!ans[i])
			continue;
		printf("\n%i letters: ", i);
		ans[i] = quick(ans[i]);
		linked *temp = ans[i];
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
	unload(tree);
	unloadlinked();
	printf("\n%f\n", (double)(clock() - begin) / CLOCKS_PER_SEC);
}
		
			
