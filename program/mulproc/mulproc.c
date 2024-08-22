#include "interface.h"

int main() {
	std_write("father start\n");
	int pid = fork();
	if(pid == 0) {
		std_write("child start\n");
		for(int i = 0;i < 1000000;++i)if(i % 100000 == 0)std_write(".");
		std_write("\nchild end\n");
		return 0;
	} else if(pid > 0) {
		wait();
		std_write("father end\n");
		return 0;
	}
}