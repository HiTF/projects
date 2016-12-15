#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h> 
#include <string.h>

#define NAMESIZE 100
int h = 0;

void DFS(const char *s, int deep, const char *file) { 
	DIR * hw;
	struct dirent *entry;
	h++;
	printf("Find %s in directory %s\n", file, s);
	if (h <= deep) { 
		if ((hw = opendir(s)) == NULL) {
			fprintf(stderr, "Error\n");
			perror("Your error");
			exit(EXIT_FAILURE);
		}
		while ( (entry = readdir(hw)) != NULL) {
			printf("%ld - %s [%d] %d\n",
			entry->d_ino, entry->d_name, entry->d_type, entry->d_reclen);
			if (!strcmp(entry->d_name, file)) {
				printf("finded %s in dirctory %s\n", file, s);
				exit(EXIT_SUCCESS);    
			}
			if (entry->d_type == 4) {
				char new_dir[NAMESIZE];
				strcpy(new_dir, s);
				strcat(new_dir, "/");
				strcat(new_dir, entry->d_name); 
				DFS(new_dir, deep, file);
			}
		}
		closedir(hw);
	}
    h--;
}


int main(int argc, char** argv) {
	if(argc != 4) {
		fprintf(stderr, "Usage: %s directory search_of_depth name\n", argv[0]);
		exit(EXIT_FAILURE); 
	}
	int deep1 = atoi(argv[2]);
	char directory[NAMESIZE];
	char file[NAMESIZE];
	strcpy(directory, argv[1]);
	strcpy(file, argv[3]);
	DFS(directory, deep1, file);
	return 0;
}
