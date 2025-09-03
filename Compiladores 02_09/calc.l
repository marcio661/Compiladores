%{
#include <stdio.h>
%}


%option yylineno

%%

[ \t\r\n]		{printf("espaco\n"); }
"print"			{printf("print\n"); }
"+"			{printf("+\n"); }
"-"			{printf("-\n"); }
"*"			{printf("*\n");  }
"/"			{printf("/\n");  }
"("			{printf("(\n");  }
")"			{printf(")\n");  }
";"			{printf(";\n");  }
"="			{printf("=\n");  }

[0-9]+			{printf("<TOK_INT, %d>\n", atoi(yytext));  }
[0-9]+\.[0-9]+		{printf("<TOK_FLT, %f>\n", atof(yytext));   }
[a-zA-Z_][a-zA-Z_0-9]*	{printf("<TOK_IDENT, %s>\n", yytext);   }

.			{ printf("Caractere nao presente no alfabeto %c\n", yytext[0]); }

%%			


