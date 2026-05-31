#include <stdio.h>

#define BUFSIZE 1024

int buff[BUFSIZE];

void
push(int num, int ** ptr)
{
	if ((*ptr + 1) >= (buff + BUFSIZE)) { //overpushing protection
		puts("buffer is full");
		return;
	}

	**ptr = num;
	(*ptr)++;
}

int
pop (int ** ptr)
{	
	if (*ptr <= buff) {  // underpopping protection
		puts("buffer is empty");
		return 0;
	}

	(*ptr)--;
	return **ptr;	
}

int
main(void)
{
	int * ptr;

	ptr = buff;

	while (1) {
		char c, flag;
		int num, tmp;

		num = c = flag = 0;

		while ((c = getchar()) != '\n' && c != ' ') {
			
			if (c == EOF)
				return 0;


			if ((c - 48) <= 9 && (c - 48) >= 0) { 
				num = (num * 10) + (c - 48);
				flag = 1;
			}
			
			else {
				switch (c) {
					case '+':
						push(pop(&ptr) + pop(&ptr), &ptr);
						break;
					case '-':
						tmp = pop(&ptr);
						push(pop(&ptr) - tmp, &ptr);
						break;
					case '*':
						push(pop(&ptr) * pop(&ptr), &ptr);
						break;
					case '/':
						tmp = pop(&ptr);
						push(pop(&ptr) / tmp, &ptr);
						break;
					default:
						puts("Invalid char");
				}

				printf("= %d\n", *(ptr - 1));
			}
		}

		if (flag) {
			push(num, &ptr);
			flag = 0;
		}

	}

	return 0;
}	
