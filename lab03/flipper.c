#include <stdio.h>
#include <string.h>
#include <dirent.h>

void reverse(FILE* inputFile, FILE* outputFile){
    char line[1024];

    while(fgets(line, sizeof(line), inputFile)) {
        size_t len = strlen(line);
        if (line[len - 1] == '\n') {
            line[--len] = '\0';  // remove newline at end of line
        }
        for(size_t i = 0; i < len / 2; i++) {
            char temp = line[i];
            line[i] = line[len - i - 1];
            line[len - i - 1] = temp;
        }
        fputs(line, outputFile);
        fputc('\n', outputFile);  // add newline at end of line
    }
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <source directory> <destination directory>\n", argv[0]);
        return -1;
    }

    DIR *dir;
    struct dirent *entry;
    char inputPath[1024];
    char outputPath[1024];

    if ((dir = opendir(argv[1])) == NULL) {
        printf("Error: Cannot open directory '%s'\n", argv[1]);
        return -1;
    }

    while ((entry = readdir(dir)) != NULL) {
        if (strstr(entry->d_name, ".txt") != NULL) {
            snprintf(inputPath, sizeof(inputPath), "%s/%s", argv[1], entry->d_name);
            snprintf(outputPath, sizeof(outputPath), "%s/%s", argv[2], entry->d_name);

            FILE* inputFile = fopen(inputPath, "r");
            if (inputFile == NULL) {
                printf("Error: Cannot open file '%s'\n", inputPath);
                continue;
            }

            FILE* outputFile = fopen(outputPath, "w");
            if (outputFile == NULL) {
                printf("Error: Cannot open file '%s'\n", outputPath);
                fclose(inputFile);
                continue;
            }

            reverse(inputFile, outputFile);

            fclose(inputFile);
            fclose(outputFile);
        }
    }

    closedir(dir);
}