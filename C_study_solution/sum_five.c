#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

int main(void) {
	int score = 0;
	int i = 0;
	int j = 0;
	for (i; i < 5; i++)
	{
		scanf("%d", &j);
		score += j;
	}
	printf("%d", score);
	return 0;
}