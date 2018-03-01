#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>

int main(void) {
	char c;
	int n;
	fcntl(0, F_SETFL, O_NONBLOCK);
	struct termios terminal;
	tcgetattr(0, &terminal);
	terminal.c_lflag &= ~(ICANON|ECHO);
	terminal.c_cc[VTIME] = 1;
	terminal.c_cc[VMIN] = 1;
	tcsetattr(0, TCSANOW, &terminal);
	while (1) {
		printf("Tapez une touche...\n");
		n = read(0, &c, 1);
		if ((n>=1) && (c=='z')) {
			printf("DEPLACEMENT EN HAUT\n");
		}
		sleep(1);
	}
	return 0;
}
