#include <stdio.h>
#include <math.h>


#define BUFSIZE 1024

double buff[BUFSIZE];

char
see (void) {
	char c;
	
	c = getchar();
	ungetc((unsigned char)c, stdin);

	return c;
}

void
push(double num, double ** ptr)
{
	if (*ptr >= (buff + BUFSIZE)) { //overpushing protection
		puts("buffer is full");
		return;
	}

	**ptr = num;
	(*ptr)++;
}

double
pop (double ** ptr)
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
	double * ptr;

	ptr = buff;

	while (1) {
		int c, is_num, sig, next;
		double num, tmp, dec;
		
		num = is_num = dec = 0;
		sig = 1;
	
		while ((c = getchar()) != '\n' && c != ' ') {	
			if (c == EOF)
				return 0;


			else if (c <= '9' && c >= '0') {
				if (dec) {
					num = num + (double)(c - '0') / (pow(10, dec));
					dec++;
				} else {	
					num = (double)(num * 10) + (c - 48);
					is_num = 1;
				}
			}

			else if (c == ',' || c == '.') 
				dec = 1;	
			
			else if (c == '-' && (next = see()) <= '9' && next >= '0') {	
				sig = -1;	
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
						if (tmp == 0) {
							puts("Cant divide by zero");
							break;
						}
						push(pop(&ptr) / tmp, &ptr);
						break;
					default:
						puts("Invalid char");
				}

				printf("= %f\n", *(ptr - 1));
			}
		}

		if (is_num) {
			push(num * sig, &ptr);
			is_num = 0;
			sig = 1;
		}
	}

	return 0;
}	
