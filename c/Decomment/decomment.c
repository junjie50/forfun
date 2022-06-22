#include <stdio.h>

enum State { COMMENT, STRING, NORMAL, SLASHSTART, ASTERISKEND , CHARACTER};

int main(void) {
	char curr = '0';
	int escape = 0;
	enum State currstate = NORMAL;
	int line = 1;
	int stringStart = 0;
	while(curr != EOF && (curr = getchar()) != EOF) {
		if(curr == '\n') {
			line ++;
		}
		switch(currstate){
			case STRING : 
				if(!escape && curr == '"') {
					currstate = NORMAL;
				}
				else if (curr == '\\') {
					escape = 1;
				}
				else {
					escape = 0;
				}
				putchar(curr);
				break;

			case CHARACTER:
				if(!escape && curr == '\'') {
					currstate = NORMAL;
				}
				else if (curr == '\\') {
					escape = 1;
				}
				else {
					escape = 0;
				}
				putchar(curr);
				break;
			case SLASHSTART :
				// check if next one is  asterisk. If is start state comment.
				if(curr == '*') {
					putchar(' ');
					currstate = COMMENT;
				}
				else if (curr == '/') {
					putchar('/');
				}
				else {
					putchar('/');
					putchar(curr);
					currstate = NORMAL;
				}
				break;

			case NORMAL :
				// Check for the start of the comment slash.
				if(curr == '/') {
					currstate = SLASHSTART;
				}
				else if(curr == '"') {
					putchar(curr);
					currstate = STRING;
					stringStart = line; 
				}
				else if(curr == '\'') {
					putchar(curr);
					currstate = CHARACTER;
					stringStart = line; 
				}
				else {
					putchar(curr);
				}
				break;
			
			case ASTERISKEND :
				// check if next one is slash. If is end state comment.
				// replace one more space.
				if(curr == '/') {
					currstate = NORMAL;
				}
				else if(curr == '*') {
					// Dont do anyth as still in comment mode
				}
				else if(curr == '\n') {
					// Replace each new line with space and a new line in the comment.
					putchar('\n');
				}
				else {
					currstate = COMMENT;
				}
				break;

			case COMMENT :
				//Check for the end of the comment asterisk + slash.
				if(curr == '*') {
					currstate = ASTERISKEND;
				}
				else if(curr == '\n') {
					// Replace each new line with a new line in the comment.
					putchar('\n');
				}
				else {
					// Do nothing as is within the comment state.
				}
				break;
		}
	}

	if(currstate == STRING || currstate == CHARACTER) {
		fprintf(stderr, "Error: line %d: unterminated comment\n", stringStart);
		return 1;
	}
	else if(currstate != NORMAL) {
		return 1;
	}
	return 0;
}