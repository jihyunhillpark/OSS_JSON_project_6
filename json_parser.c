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
}js_error;

/*Function Declaration*/
static void js_parser_init(js_parser *parser);
static tok_t *js_alloc_token(js_parser *parser,tok_t *tokens,const int num_tokens);
static void js_fill_token(tok_t *token, const type_t type,const int start, const int end);
static int js_parse_string(js_parser *parser, const char *js,const int len, tok_t *tokens,const int num_tokens);
static int js_parse_primitive(js_parser *parser, const char *js,const int len, tok_t *tokens,const int num_tokens);
char print_applicaion_menu();
void print_tokens(js_parser *, tok_t *, char*, int);
void print_week_plan_and_cost(tok_t *,char*,int count);
void print_frequency_of_sepcific_meals(tok_t *, char *, int count);
void print_menu_of_day(tok_t *tokens,const char *js,int count);
int jsoneq(const char *json, tok_t *tok,const char *s);

int main (int argc,char **argv){
    /*File Read & Copy to Buffer*/
    FILE *fp;
    const int maxBufLen = 1000;
    const int maxDataLen = 5000;
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
    int pair_count=0;
    int r;
    int i;
    int flag=1;
  
    p.pos = 1;// skip first '{'
    for(;p.pos<strlen(data) && data[p.pos]!='\0';p.pos++){
        /*for token size of tokens[0]*/
        char d;
        d=data[p.pos];
        if(d=='{')
            flag = 0;
        if(d=='}')
            flag = 1;

        if(d==':'&& flag) pair_count ++;
    }
    


    js_parser_init(&p); //Initialize
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
                break;

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
    tokens[0].size=pair_count;

    char option;
    option = print_applicaion_menu();

    switch(option)
    {
        case '1' : print_menu_of_day(tokens,data,count);break;
        case '2' : print_frequency_of_sepcific_meals(tokens, data, count); break;
        case '3' : print_week_plan_and_cost(tokens,data,count); break;
        case '4' : print_tokens(&p, tokens, data, count); break;
    }

}
char print_applicaion_menu()
{
    char a;
    printf("==============================Options==============================\n");
    printf("1. View menu by day\n");
    printf("2. Frequency of specific menu by user.\n");
    printf("3. Total expenditure on food expenses\n");
    printf("4. Print out tokens\n");
    printf("===================================================================\n");  
    scanf("%c", &a);
    getchar();
    return a;
}
void print_menu_of_day(tok_t *tokens,const char *js,int count){


    char str[50];
    int a;

    printf("\nEnter any day of the week to show its menu : ");
    scanf("%s",str);
    getchar();
    for(a=1;a<count;a++){
        if(jsoneq(js,&tokens[a],str)==0){
            printf("Menu of %s is %.*s\n",str,
            tokens[a+1].end-tokens[a+1].start,js+tokens[a+1].start);
            break;
        }
    }
}

int jsoneq(const char *json, tok_t *tok,const char *s){
    if(tok->type == STRING && (int)strlen(s) == tok->end - tok->start &&  
    strncmp(json + tok->start,s,tok->end - tok->start)==0){
        return 0;
    }
    return -1;
} 
void print_frequency_of_sepcific_meals(tok_t* tokens, char * data, int count)
{
    char user[20] = { '\0',};
    char sp_menu[50] = { '\0',};
    int i = 0;
    int c = 0;
    char sub[20] = {'\0',};
    char sub_m[50] = {'\0',};
    int token_l;
    tok_t * menu;
    printf("write user and menu :");
    scanf("%s",user);
    getchar();
    scanf("%s",sp_menu);
    getchar();
    
    while(i < count)
    {   
        token_l = tokens[i].end - tokens[i].start;
        c = 0;
        while (c < 20 && c < token_l) {
            sub[c] = data[(tokens[i].start) + c];
            ++c;
        }
        //printf("%d",token_l);
        //printf("%s\n",  sub);
        if(strncmp(sub, user, strlen(user)) == 0){
            
            int meals = tokens[i + 4].size;
            int meal_count = 0;
            for ( c = 0; c < meals ; c++)
            {
                menu = &tokens[i+6];
                int j = 0;
                while (j < 20 && c < (menu -> end) - (menu->start)) {
                    sub_m[j] = data[menu->start + j];
                    ++j;
                }
                if(strncmp(sp_menu, sub_m , strlen(sp_menu)) == 0) meal_count++;
                i+=2;

            }
            printf("%s ate %s %d times this week!\n", user, sp_menu, meal_count);
            break;
        } 
        c = 0;
        while ( c <20 ) {
            sub[c] = '\0';
            ++c;
        }
        i++;
    }
}
void print_week_plan_and_cost(tok_t* tokens, char* data, int count)
{
    char user[20] = { '\0',};
    int i = 0;
    int c = 0;
    char sub[20] = {'\0',};
    int token_l;
    tok_t * menu;
    printf("write user's name :");
    scanf("%s",user);
    getchar();
    
    while(i < count)
    {   
        token_l = tokens[i].end - tokens[i].start;
        c = 0;
        while (c < 20 && c < token_l) {
            sub[c] = data[(tokens[i].start) + c];
            ++c;
        }
        //printf("%d",token_l);
        //printf("%s\n",  sub);
        if(strncmp(sub, user, strlen(user)) == 0){
            int meals = tokens[i + 4].size;
            printf("%s \'s ", user);
            printf("total cost : %d\n\n", tokens[i + 4].size * 4000);
            for ( c = 0; c < meals ; c++)
            {
                menu = &tokens[i+5];
                printf("%.*s : ", (menu->end) - (menu->start), data + (menu->start));
                menu = &tokens[i+6];
                printf("%.*s\n", (menu->end) - (menu->start), data + (menu->start));
                i+=2;
            }
            
            break;
        } 
        c = 0;
        while ( c <20 ) {
            sub[c] = '\0';
            ++c;
        }
        i++;
    }
}
void print_tokens(js_parser* p, tok_t * tokens, char * data, int count)
{
   /*print out information of tokens*/
    js_parser_init(p); 
    int j = 0;
    tok_t* toktok = &tokens[j];
    /*FILE *fp2 = fopen("tokens_info.txt","w+");*/
    char* token_type;

    for(int a;a<count;a++){

        switch(toktok->type){
        case 0: token_type = "UNDEFINED"; break;
        case 1: token_type = "OBJECT"; break;
        case 2: token_type = "ARRAY"; break;
        case 3: token_type = "STRING"; break;
        case 4: token_type = "PRIMITIVE"; break;
        }
        /*
        fprintf(fp2,"[%d] %.*s ",j, (toktok->end) - (toktok->start), data + (toktok->start));
        fprintf(fp2,"(Size=%d, %d~%d, %s)\n", toktok->size, toktok->start,toktok->end, token_type);
        */

        printf("[%d] %.*s ",j, (toktok->end) - (toktok->start), data + (toktok->start));
        printf("(Size=%d, %d~%d, %s)\n", toktok->size, toktok->start,toktok->end, token_type);

        j++;
        toktok = &tokens[j];
    }
    /*fclose(fp2);*/
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