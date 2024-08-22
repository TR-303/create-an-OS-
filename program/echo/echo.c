int main(int argc, char** argv) {
	for(int i = 1;i < argc;++i) {
		std_write(argv[i]);
		std_write(" ");
	}
	std_write("\n");
}