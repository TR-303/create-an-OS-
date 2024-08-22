#include "interface.h"
#include "utils.h"

int main() {
	dirent_t dirs[64];

	int cnt = listdir(dirs);
	std_write("file\t\tsize\t\t\tspace\n");
	for(int i = 0;i < cnt;++i) {
		std_write(dirs[i].fullname);
		if(strlen(dirs[i].fullname) < 8)std_write("\t\t");
		else std_write("\t");

		char str1[25] = "                   byte ";
		int p = 17, size = dirs[i].size;
		while(size > 0) {
			str1[p--] = '0' + size % 10;
			size /= 10;
		}
		std_write(str1);

		char str2[25] = "                   byte ";
		p = 17;
		int space = dirs[i].space;
		while(space > 0) {
			str2[p--] = '0' + space % 10;
			space /= 10;
		}
		std_write(str2);

		std_write("\n");
	}
	return 0;
}