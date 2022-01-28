#include<stdio.h>
#include <stdlib.h>
#include <string.h>

char main() {
	char str[20];
	int str_length;

	printf("Enter a password: ");
	scanf("%s", str);

	str_length = strlen(str);

	for (int i = 0; i < str_length; i ++) {
		if (str[i] == 'e') {
			str[i] = '3';
		}
		if (str[i] == 'l') {
                        str[i] = '1';
                }
		if (str[i] == 'a') {
                        str[i] = '@';
                }
	}

	printf("%s\n", str);
	return 0;
}
