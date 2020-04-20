#include <stdio.h>
#include <stdlib.h>

struct ssu_id {
    char name[64];
    int id;
};

int main(void)
{
    struct ssu_id test1, test2;
    char *fname = "ssu_exam.dat";
    FILE *fp;
    
    if((fp = fopen(fname, "w")) == NULL) {
        fprintf(stderr, "fopen error for %s\n", fname);
        exit(1);
    }
    
    printf("Input ID>> ");
    scanf("%d", &test1.id);
    printf("Input ID>> ");
    scanf("%d", &test2.id);
    
    if(fwrite(&test1, sizeof(struct ssu_id), 1, fp) != 1) {
        fprintf(stderr, "fwrite error\n");
        exit(1);
    }
    
    fclose(fp);
    
    if((fp = fopen(fname, "r")) == NULL) {
        fprintf(stderr, "fopen error for %s\n", fname);
        exit(1);
    }
    
    if(fread(&test2, sizeof(struct ssu_id), 1, fp) != 1) {
        fprintf(stderr, "fread error\n");
        exit(1);
    }
    
    printf("\nID    name\n");
    printf("============\n");
    printf("%d      %s\n", test2.id, test2.name);
    fclose(fp);
    exit(0);
}
