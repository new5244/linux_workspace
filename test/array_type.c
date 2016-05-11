#include <stdio.h>
#include <string.h>

typedef char STR[32];

int main(void)
{
	STR a[2] = { {0, }, { 1, 2, 3, 0,  } };
	int i;

	for ( i = 0 ; i < sizeof(STR); i++) {
		printf("[%d] %d\n", i, a[0][i]);
	}
	for ( i = 0 ; i < sizeof(STR); i++) {
		printf("[%d] %d\n", i, a[1][i]);
	}

	return 0;
}
