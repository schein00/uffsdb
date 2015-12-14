/* FUNÇÕES AUXILIARES DO INTERPRETADOR DE COMANDOS (yacc + lex)
 * O yacc vai chamando essas funções conforme vai identificando os tokens.
 * Essas funcções irão preencher uma estrutura, ou parte dela, e chamarão
 * as funções do banco de dados para executar as operações.
 */
#define OP_INSERT 			1
#define OP_CREATE_TABLE 	2
#define OP_DROP_TABLE 		3
#define OP_CREATE_DATABASE 	4
#define OP_DROP_DATABASE 	5
#define OP_SELECT_ALL 		6


#define LEFT 			1
#define RIGHT 			2

#define NATURAL_JOIN	3
#define JOIN_ON	 	4

#define AND_LOGIC		5
#define OR_LOGIC		6

#define OP_IGUAL		11
#define OP_DIFERENTE	12
#define OP_MENOR		13
#define OP_MAIOR		14

// flag usada para saber de que lado ser inserido o valor do teste
int position;

/* Estrutura global que guarda as informações obtidas pelo yacc
 * na identificação dos tokens
 */
extern rc_insert GLOBAL_DATA;


/*
* Estrutura global que guarda as informacoes obtidas pelo yacc
* na identificacao dos tokens para select
*/
extern rc_select GLOBAL_SELECT;

/* Estrutura auxiliar do reconhecedor.
 */
extern rc_parser GLOBAL_PARSER;

/* Funcções do yacc
 */
int yyparse();
int yylex();
int yylex_destroy();
extern int  yylineno;

/* Função padrão do yacc chamada quando um erro sintático é
 * identificado.
 */
void yyerror(char *s, ...);

/* Imprime o erro caso o comando seja inválido
 */
void invalidCommand(char *command);

/* Mensagem exibida quando o usuário tenta executar um comando
 * sem estar conectado à nenhuma base de dados.
 */
void notConnected();

/* Estabelece a conexão com uma base de dados. Se conectar,
 * define os parâmetros globais para a exibição do console.
 */
void connect(char *nome);


/* Define o nome do banco de dados ou da tabela identificado
 * no token
 */
void setObjName(char **nome);

/* Adiciona na estrutura uma nova coluna identificada no
 * comando INSERT.
 */
void setColumnInsert(char **nome);

/* Define o valor que será inserido, identificado no token
 * do comando INSERT
 */
void setValueInsert(char *nome, char type);

/* Adiciona na estrutura uma nova coluna identificada no
 * comando CREATE TABLE, isso inicializará outras variáveis
 * também (type, attribute, ...)
 */
void setColumnCreate(char **nome);

/* Define o tipo da coluna identificada no comando CREATE
 * TABLE.
 */
void setColumnTypeCreate(char type);

/* Define o tamanho da coluna identificada no comando
 * CREATE TABLE
 */
void setColumnSizeCreate(char *size);

/* Define a coluna identificada no comando CREATE TABLE
 * como PRIMARY KEY
 */
void setColumnPKCreate();

/* Define a tabela que a coluna identificada no CREATE TABLE
 * irá referenciar
 */
void setColumnFKTableCreate(char **nome);

/* Define a coluna da tabela que a coluna identificada
 * no CREATE TABLE irá referenciar.
 */
void setColumnFKColumnCreate(char **nome);

/* Reinicializa todas as propriedades da estrutura que armazena
 * os dados identificados pelos tokens SQL.
 */
void clearGlobalStructs();

/* Define a operação que será realizada pelo identificador através
 * dos tokens iniciais.
 */
void setMode(char mode);


// Reseta a estrutura usada pelo select, ser feito sempre que encontrar um SELECT
void resetSelect();

//  Define o nome da tabela para qual foi usado o select
void setObjNameSelect(char **name);

// Define quais as colunas devem ser projetadas
void setColumnProjection(char **name);

// Cria um novo nodo da lista de testes do where
void addWhereCondition();

// Seta o operador logico entre o teste atual e o anterior
void setOpLogic(int logic);

// Seta a operacao de cada teste
void setCondition(int OP);

// Seta o lado onde sera inserido o valor ou a coluna de teste
void setPosition(int p);

// Seta o nome da coluna do teste
void setColumnTest(char **name);

// Seta o valor do teste
void addValueTest(char **value);

// Adiciona um novo join
// podendo ser um NATURAL JOIN ou um JOIN ... ON ...
void addNewJoin(int type);

//Adiciona a coluna que sera usada no teste do join
void setColumnJoin(char **name);

// Adiciona a tabela que sera usada no join
// join sera feita entre a tabela do FROM tabela, e a tabela indicada no JOIN ou NATURAL JOIN
void setTableJoin(char **name);

//Adiciona o operador da condicao do join
void setConditionJoin(int OP);
