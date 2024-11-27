```c
#include <stdio.h>
#include <stdlib.h>

//a basic file read and write implementation
int main()
{
    FILE * fp;
    char buffer[32];
    int number;

    fp = fopen("file.txt", "r+");
    if (fp == NULL)
    {
        printf("Could not open file");
        return 1;
    }

    fgets(buffer, 32, fp);
    number = atoi(buffer);

    // 移動檔案指標到檔案開頭
    fseek(fp, 0, SEEK_SET);

    fprintf(fp, "%d", number + 1);
    fclose(fp);

    return 0;
}
```
