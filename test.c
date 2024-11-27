#include <stdio.h>

void print_maze_from_file(const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Error opening file: %s\n", filename);
        return;
    }

    char line[256];  
    while (fgets(line, sizeof(line), fp) != NULL) {
        printf("%s", line);  
    }

    fclose(fp);

}

int main() {
    print_maze_from_file("map.txt");
    return 0;
}
