int main() {
	while(1) {
		std_write(">");
		char buf[128];
		int r = std_read(buf, 127);
		buf[r] = '\0';

		int argc = 0;
		char* argv[16];

		for(int i = 0, f = 0;i < r;++i) {
			if(buf[i] != ' ' && f == 0) {
				argv[argc++] = &buf[i];
				f = 1;
			} else if(buf[i] == ' ' && f == 1) {
				f = 0;
				buf[i] = '\0';
			}
		}
		if(argc == 0)argv[0] = buf;

		int pid = fork();
		if(pid == 0) {
			if(exec(argv[0], argc, argv) == -1) std_write("Program not found!\n");
			return 0;
		} else if(pid > 0) wait();
	}
}