#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    UNDEFINED = 0,
    OBJECT = 1,
    ARRAY = 2,
    STRING = 3,
    PRIMITIVE =4
}type_t;

typedef struct {
  type_t type;
  int start;
  int end;
  int size;
} tok_t;


int main (int argc,char **argv){
    /*File Read & Copy to Buffer*/
    FILE *fp;
    const int maxBufLen = 1000;
    const int maxDataLen = 1000;
    char buffer[maxBufLen];
    char data[maxDataLen];
    fp=fopen(argv[1],"r");
    int length = 0;
    if (argc != 2){
        printf("interface error!\n");
    }
    while(fscanf(fp,"%s",buffer)>0)
    {
        if((length+strlen(buffer)+1)>=maxDataLen)break;
        strncpy(&data[length],buffer,strlen(buffer));
        length += strlen(buffer)+1;
        data[length-1]=' ';
    }
    data[length] = 0x00;
    printf("The string is : \n%s\n",data);
    fclose(fp);


}
