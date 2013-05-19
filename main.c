
#include "parser.h"
#include <stdio.h>
#include <string.h>

void readLine(char *line) {
	char c;
	while ( (c = getchar()) != '\n') {
		*line = c;
		++line;
	}
	*line = '\0';
}

int main(int argc, char *argv[]) {
	char line[1024];

	puts("Mathematical Expression Evaluator\n");
	puts("Supports parenthesis and operators +, -, *, /, and ^");
	puts("Type exit to leave program.\n");

	for(;;) {
		printf("> ");
		readLine(line);
		if (strcmp(line, "exit") == 0)
			break;

		int result = eval(line);
		if (getError()) {
			printf("Failed to evaluate.\n\n");
		} else {
			printf("%d\n\n", result);
		}
	}

	return 0;
}
