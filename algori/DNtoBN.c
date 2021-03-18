#include <stdio.h>

int DtoB(void) {
	int n, i;
    int start = 0;
    if (scanf("%d", &n)) {
        for (i = 31; i >= 0; i--) {
            if (start || ((n >> i) & 1) == 1) {
                start = 1;
                printf("%d", (n >> i) & 1);
            }
        }
    }
    return 0;
}