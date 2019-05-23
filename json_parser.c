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

/*parser structure*/
typedef struct{
   unsigned int toknext ;
   unsigned int pos ;
}js_parser;

/*Function Declaration*/
static void js_parser_init(js_parser *parser);
static tok_t *js_alloc_token(js_parser *parser,tok_t *tokens,int num_tokens);

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
    //printf("The string is : \n%s\n",data);
    fclose(fp);

    /* Make Tokens */
    tok_t tokens[1024];



    /*Declare variables*/
    tok_t *token;
    js_parser p;
    js_parser_init(&p); //Initialize
    int count = p.toknext;


    

    /*Sweeping all Data*/
    for(;p.pos<strlen(data) && data[p.pos]!='\0';p.pos++){
        char c;
        type_t type;

        c=data[p.pos];
        switch(c){
        /*OBJECT*/
            case '{':
                token = js_alloc_token(&p,tokens,1024);
                token->start = p.pos;
                break;
            case '}':
                token->end = p.pos;
                break;   

        }

    }
    




}

/*Function Definition*/
static void js_parser_init(js_parser *parser)
{
    parser->pos = 0;
    parser->toknext = 0;
}

static tok_t *js_alloc_token(js_parser *parser,tok_t *tokens,int num_tokens)
{
    tok_t *tok;
    if(parser->toknext >= num_tokens ){
        return NULL;
    }
    tok = &tokens[parser->toknext];
    parser->toknext++;
    return tok;
}
