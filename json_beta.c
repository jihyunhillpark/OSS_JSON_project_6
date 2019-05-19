#include <stdio.h>

typedef enum{
    undefined = 0,
    object = 1,
    array = 2,
    string = 3,
    primitive = 4
}jsontype_t;

typedef struct{
    jsontype_t type;
    int start;
    int end;
    int size
}jsontoken_t;

int main()
{
 

}