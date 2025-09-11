%{
#include <stdio.h>
%}


%option yylineno

%%

[ \t\r\n]		{ }
"print"			{return TOK_PRINT; }
"+"			{return '+'; }
"-"			{return '-'; }
"*"			{return '*'; }
"/"			{return '/'; }
"("			{return '('; }
")"			{return ')'; }
";"			{return ';'; }
"="			{return '='; }

[0-9]+			{return TOK_INT; }
[0-9]+\.[0-9]+		{return TOK_FLT; }
[a-zA-Z_][a-zA-Z_0-9]*	{return TOK_IDENT; }

.			{printf("Caractere nao presente no alfabeto %c\n", yytext[0]); }

%%			


