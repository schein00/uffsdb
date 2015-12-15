%{
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include "../buffend.h"
#include "parser.h"

extern char* yytext[];
extern FILE * yyin;
extern FILE* outFile_p;

int yywrap() {
    return 1;
}

%}

%union {
    int intval;
    double floatval;
    int subtok;
    char *strval;
}

%%

%token  INSERT      INTO        VALUES      SELECT      FROM
        CREATE      TABLE       INTEGER     VARCHAR     DOUBLE
        CHAR        PRIMARY     KEY         REFERENCES  DATABASE
        DROP        OBJECT      NUMBER      VALUE       QUIT
        LIST_TABLES LIST_TABLE  ALPHANUM    CONNECT     HELP
        LIST_DBASES CLEAR	
		WHERE	AND	OR	JOIN		NATURAL	ON;

start: insert | select | create_table | create_database | drop_table | drop_database
     | table_attr | list_tables | connection | exit_program | semicolon {GLOBAL_PARSER.consoleFlag = 1; return 0;}
     | parentesis_open | parentesis_close| help_pls | list_databases | clear
     | qualquer_coisa | /*nothing*/;

/*--------------------------------------------------*/
/**************** GENERAL FUNCTIONS *****************/
/*--------------------------------------------------*/

/* CONNECTION */
connection: CONNECT OBJECT {connect(*yytext); GLOBAL_PARSER.consoleFlag = 1; return 0;};

qualquer_coisa: OBJECT {GLOBAL_PARSER.consoleFlag = 1; GLOBAL_PARSER.noerror = 0; return 0;};

/* EXIT */
exit_program: QUIT {exit(0);};

clear: CLEAR {clear(); GLOBAL_PARSER.consoleFlag = 1; return 0;};

parentesis_open: '(' {GLOBAL_PARSER.parentesis++;};

parentesis_close: ')' {GLOBAL_PARSER.parentesis--;};

/* TABLE ATTRIBUTES */
table_attr: LIST_TABLE OBJECT {
    if(connected.conn_active) {
        printTable(yylval.strval);
        GLOBAL_PARSER.consoleFlag = 1;
    } else
        notConnected();
    return 0;
};

/* LIST TABLES */
list_tables: LIST_TABLES {
    if(connected.conn_active) {
        printTable(NULL);
        GLOBAL_PARSER.consoleFlag = 1;
    } else
        notConnected();
    return 0;
};

/* LIST DATABASES */
list_databases: LIST_DBASES {
    showDB();
    GLOBAL_PARSER.consoleFlag = 1;
    return 0;
}

/* HELP */
help_pls: HELP {help(); GLOBAL_PARSER.consoleFlag = 1; return 0;}

/*--------------------------------------------------*/
/****************** SQL STATEMENTS ******************/
/*--------------------------------------------------*/

/* INSERT */
insert: INSERT INTO {setMode(OP_INSERT);} table opt_column_list VALUES parentesis_open value_list parentesis_close semicolon {
    if (GLOBAL_PARSER.col_count == GLOBAL_PARSER.val_count || GLOBAL_DATA.columnName == NULL)
        GLOBAL_DATA.N = GLOBAL_PARSER.val_count;
    else {
        printf("ERROR: The column counter doesn't match the value counter.\n");
        GLOBAL_PARSER.noerror=0;
    }
    return 0;
};

semicolon: ';';

table: OBJECT {setObjName(yytext);};

opt_column_list: /*optional*/ | parentesis_open column_list parentesis_close;

column_list: column | column ',' column_list;

column: OBJECT {setColumnInsert(yytext);};

value_list: value | value ',' value_list;

value: NUMBER {setValueInsert(yylval.strval, 'D');}
     | VALUE {setValueInsert(yylval.strval, 'I');}
     | ALPHANUM {setValueInsert(yylval.strval, 'S');};

/* CREATE TABLE */
create_table: CREATE TABLE {setMode(OP_CREATE_TABLE);} table parentesis_open table_column_attr parentesis_close semicolon {
    GLOBAL_DATA.N = GLOBAL_PARSER.col_count;

    return 0;
};

table_column_attr: column_create type attribute | column_create type attribute ',' table_column_attr;

type: INTEGER {setColumnTypeCreate('I');}
    | VARCHAR {setColumnTypeCreate('S');} parentesis_open NUMBER parentesis_close {setColumnSizeCreate(yylval.strval);}
    | DOUBLE {setColumnTypeCreate('D');};
    | CHAR {setColumnTypeCreate('C');};

column_create: OBJECT {setColumnCreate(yytext);};

attribute: /*optional*/
         | PRIMARY KEY {setColumnPKCreate();}
         | REFERENCES table_fk '(' column_fk ')';

table_fk: OBJECT {setColumnFKTableCreate(yytext);};

column_fk: OBJECT {setColumnFKColumnCreate(yytext);};

/* DROP TABLE */
drop_table: DROP TABLE {setMode(OP_DROP_TABLE);} OBJECT {setObjName(yytext);} semicolon  {return 0;};

/* CREATE DATABASE */
create_database: CREATE DATABASE {setMode(OP_CREATE_DATABASE);} OBJECT {setObjName(yytext);} semicolon {return 0;};

/* DROP DATABASE */
drop_database: DROP DATABASE {setMode(OP_DROP_DATABASE);} OBJECT {setObjName(yytext);} semicolon {return 0;};

/*-------------- SELECT ------------------ */
select: SELECT {setMode(OP_SELECT_ALL); resetSelect();} column_list_select FROM table_select clause_join clause_where semicolon {return 0;};

//tabela onde sera 'executado' o select
table_select: OBJECT {setObjNameSelect(yytext);};

//define lista de colunas que serao projatadas
column_list_select: '*' | column_projection | column_projection ',' column_list_select;

//coluna que sera projetada e adiciona na estrutura usada pelo select
column_projection: OBJECT {setColumnProjection(yytext);};

//definicao da sintaxe do where
clause_where: /*optional*/ | WHERE {addWhereCondition(); } test test_list;

//definicao da sintaxe dos AND e OR
test_list: /*optional*/ | {addWhereCondition();} op_logic  test test_list;

//operador logico
op_logic: AND {setOpLogic(AND_LOGIC);} | OR {setOpLogic(OR_LOGIC);};


//definicao da sintaxe de uma operacao (valor operador valor)
test: {setPosition(LEFT);} camp_test  condition {setPosition(RIGHT);} camp_test;

//condicao que serao feitas entre os valores e adicao a estrutura usada pelo select
condition: '=' {setCondition(OP_IGUAL);} 
         | '>' {setCondition(OP_MAIOR);}
         | '<' {setCondition(OP_MENOR);} 
	    | '!' {setCondition(OP_DIFERENTE);};

//definicao da sintaxe de um valor, podendo ser uma coluna ou um valor incerido
camp_test: column_test | value_test;

//definicao da coluna do teste e 
column_test: OBJECT {setColumnTest(yytext);};

//Definicao de um valor pededno ser float, int e caracteres
value_test: ALPHANUM {addTypeValue(ALPHANUM_TYPE); addValueTest(*yytext); }
            | VALUE {addTypeValue(NUMBER_TYPE); addValueTest(*yytext); }
            | NUMBER { addTypeValue(INT_TYPE); addValueTest(*yytext);};


//definicao da sintaxe do joins
clause_join: /*optional*/ | NATURAL JOIN {addNewJoin(NATURAL_JOIN); setTableJoin(yytext);} table_join  | JOIN {addNewJoin(JOIN_ON);} table_join ON join_test;

join_test: {setPosition(LEFT);} column_join condition_join {setPosition(RIGHT);} column_join;

table_join: OBJECT {setTableJoin(yytext);};

column_join: OBJECT {setColumnJoin(yytext);};

condition_join: '=' {setConditionJoin(OP_IGUAL);} 
         	    | '>' {setConditionJoin(OP_MAIOR);}
              | '<' {setConditionJoin(OP_MENOR);} 
      	    | '!' {setConditionJoin(OP_DIFERENTE);};


/*-------------- SELECT ------------------ */

/* END */
%%
