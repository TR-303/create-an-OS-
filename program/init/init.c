int main() {
	while(1) {
		int pid = wait();
		// if(pid != -1) {
		// 	char message[] = "proc    exited.\n";
		// 	message[5] = '0' + pid / 10;
		// 	message[6] = '0' + pid % 10;
		// 	std_write(message);
		// }
	}
}