#include "buffend.h"
#include "string.h"
column * getPageImprime(tp_buffer *buffer, tp_table *campos, struct fs_objects objeto, int page, rc_select *GLOBAL_DATA_SELECT, rc_parser*GLOBAL_PARSER){

  

    if(page >= PAGES)
        return ERRO_PAGINA_INVALIDA;

    if(buffer[page].nrec == 0) //Essa página não possui registros
        return ERRO_PARAMETRO;
    
	//Para melhorar essa logica, adicionei um campo qtdcampos na struct, pode ser usado para refatorar o codigo mais tarde precisa saber como iremos preencher essa variavel quando não tiver '*'
	

	//Aloca a quantidade de campos necessária
    	column *colunas = (column *)malloc(sizeof(column)*GLOBAL_PARSER->col_count *(buffer[page].nrec)); 
/*

    	if(!colunas){
       	 return ERRO_DE_ALOCACAO; 
	}	


    	//adiciona espaço para a variavel colunas, pela quantidade de campos da tabela vezes o numero de registros do pagina do buffer
	memset(colunas, 0, sizeof(column)*GLOBAL_PARSER->col_count*(buffer[page].nrec));
   


    

    int i=0, j=0, t=0, h=0;
    char c = "NOME";
    char *f = NULL;
	f= &c;
    rc_select *aux= NULL;
   
   if(objcmp(GLOBAL_DATA_SELECT->columnName,f) == 1){

 printf("k");

	}

*/

	
   
    return colunas; //Retorna a 'page' do buffer



}
