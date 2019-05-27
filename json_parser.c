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
   int toksuper;
}js_parser;

typedef enum {
  /* Not enough tokens were provided */
  NOTOKEN = -1,
  /* Invalid character inside JSON string */
  INVALID = -2,
  /* The string is not a full JSON packet, more bytes expected */
  JSON_ERROR = -3
};

/*Function Declaration*/
static void js_parser_init(js_parser *parser);
static tok_t *js_alloc_token(js_parser *parser,tok_t *tokens,const int num_tokens);
static void js_fill_token(tok_t *token, const type_t type,const int start, const int end);
static int js_parse_string(js_parser *parser, const char *js,const int len, tok_t *tokens,const int num_tokens);
static int js_parse_primitive(js_parser *parser, const char *js,const int len, tok_t *tokens,const int num_tokens);


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
    int r;
    int i;
  
  

    /*Sweeping all Data*/
    for(;p.pos<strlen(data) && data[p.pos]!='\0';p.pos++){

        char c;
        type_t type;

        c=data[p.pos];
        switch(c){
        
            case '{':
            case '[': 
                count++;    
                tok_t *token = js_alloc_token(&p,tokens,1024);
                if (token == NULL) {
                    return NOTOKEN;
                }
                if (p.toksuper != -1) {
                    tok_t *t = &tokens[p.toksuper];
                    t->size++;
                }

                token->type = (c == '{' ? OBJECT : ARRAY);
                token->start = p.pos;
                p.toksuper = p.toknext - 1;
                break;

            case '}':
            case ']':
      
                type = (c == '}' ? OBJECT : ARRAY);


                for (i = p.toknext - 1; i >= 0; i--) {
                    token = &tokens[i];
                    if (token->start != -1 && token->end == -1) {
                        if (token->type != type) {
                        return INVALID;
                        }
                        p.toksuper = -1;
                        token->end = p.pos + 1;
                    break;
                    }
                }
            case '\"':
                js_parse_string(&p,data,strlen(data),tokens,1024);
                count++;
                if(p.toksuper != -1){
                    tokens[p.toksuper].size++;
                }
                break;
            case '\t':
            case '\r':
            case '\n':
            case ' ':
                break;
            case ':' :
                p.toksuper = p.toknext -1;
                break;
            case ',':
                if (p.toksuper != -1 &&
                    tokens[p.toksuper].type != ARRAY &&
                    tokens[p.toksuper].type != OBJECT) {

                    for (i = p.toknext - 1; i >= 0; i--) {
                        if (tokens[i].type == ARRAY || tokens[i].type == OBJECT) {
                            if (tokens[i].start != -1 && tokens[i].end == -1) {
                                p.toksuper = i;
                                break;
                            }
                        }
                    }

                }
                break;
            
            default:
                js_parse_primitive(&p,data,strlen(data),tokens,1024);
                count++;
                if(p.toksuper != -1){
                    tokens[p.toksuper].size++;
                }
                break;



        }

    }
    

}

/*Function Definition*/
static void js_parser_init(js_parser *parser)
{
    parser->pos = 0;
    parser->toknext = 0;
    parser->toksuper = -1;
}

static tok_t *js_alloc_token(js_parser *parser,tok_t *tokens,int num_tokens)
{
    tok_t *tok;
    if(parser->toknext >= num_tokens ){
        return NULL;
    }
    tok = &tokens[parser->toknext];
    parser->toknext++;
    tok->start = tok->end = -1;
    tok->size = 0;
    return tok;
}

static void js_fill_token(tok_t *token, const type_t type,
                            const int start, const int end) {
  token->type = type;
  token->start = start;
  token->end = end;
  token->size = 0;
}

static int js_parse_string(js_parser *parser, const char *js, const int len, tok_t *tokens, const int num_tokens) {
  
  tok_t *token;

  int start = parser->pos;

  parser->pos++;

  /* Skip starting quote */
  for (; parser->pos < len && js[parser->pos] != '\0'; parser->pos++) {
    char c = js[parser->pos];

    /* Quote: end of string */
    if (c == '\"') {
      if (tokens == NULL) {
        return 0;
      }
      token = js_alloc_token(parser, tokens, num_tokens);
      if (token == NULL) {
        parser->pos = start;
        return NOTOKEN;
      }
      js_fill_token(token, STRING, start + 1, parser->pos);

      return 0;
    }
  }
 
}

static int js_parse_primitive(js_parser *parser, const char *js,
                                const int len, tok_t *tokens,
                                const int num_tokens) {
  tok_t *token;
  int start;

  start = parser->pos;

  for (; parser->pos < len && js[parser->pos] != '\0'; parser->pos++) {
    switch(js[parser->pos]){
        case ' ':
        case ',':
        case ']':
        case '}':
            token=js_alloc_token(parser,tokens,num_tokens);
            if(token==NULL){
                parser->pos = start;
                return NOTOKEN;
            }
            js_fill_token(token,PRIMITIVE,start,parser->pos);
            parser->pos--;
            return 0;
    }
   
  }



  
 
}