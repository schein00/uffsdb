//BufferPool
#include "buffend.h"
#include <string.h>

/* ----------------------------------------------------------------------------------------------
    Objetivo:   Utilizada para impressão de tabelas (select * from table;).
    Parametros: Nome da tabela (char).
    Retorno:    void.
   ---------------------------------------------------------------------------------------------*/

void imprime(rc_insert *GLOBAL_DATA, rc_select GLOBAL_DATA_SELECT, rc_parser *GLOBAL_PARSER) {
  
   
    int j,erro, x, p, cont=0;
    struct fs_objects objeto;
	
    if(!verificaNomeTabela(GLOBAL_DATA->objName)){
        printf("\nERROR: relation \"%s\" was not found.\n\n\n", GLOBAL_DATA->objName);
        return;
    }

    objeto = leObjeto(GLOBAL_DATA->objName);

    tp_table *esquema = leSchema(objeto);

    if(esquema == ERRO_ABRIR_ESQUEMA){
        printf("ERROR: schema cannot be created.\n");
        free(esquema);
        return;
    }

    tp_buffer *bufferpoll = initbuffer();

    if(bufferpoll == ERRO_DE_ALOCACAO){
        free(bufferpoll);
        free(esquema);
        printf("ERROR: no memory available to allocate buffer.\n");
        return;
    }

    erro = SUCCESS;
    for(x = 0; erro == SUCCESS; x++)

        erro = colocaTuplaBuffer(bufferpoll, x, esquema, objeto);

	if(GLOBAL_PARSER->col_count == 0 && GLOBAL_PARSER->col_test_count == 0){

 	int ntuples = --x;
	p = 0;
	while(x){
	    column *pagina = getPage(bufferpoll, esquema, objeto, p);
	    if(pagina == ERRO_PARAMETRO){
            printf("ERROR: could not open the table.\n");
            free(bufferpoll);
            free(esquema);
            return;
	    }

	    if(!cont) {
	        for(j=0; j < objeto.qtdCampos; j++){
	            if(pagina[j].tipoCampo == 'S')
	                printf(" %-20s ", pagina[j].nomeCampo);
	        	else
	                printf(" %-10s ", pagina[j].nomeCampo);
	            if(j<objeto.qtdCampos-1)
	            	printf("|");
	        }
	        printf("\n");
	        for(j=0; j < objeto.qtdCampos; j++){
	            printf("%s",(pagina[j].tipoCampo == 'S')? "----------------------": "------------");
	            if(j<objeto.qtdCampos-1)
	            	printf("+");
	        }
	        printf("\n");
	    }
	    cont++;
		for(j=0; j < objeto.qtdCampos*bufferpoll[p].nrec; j++){
        	if(pagina[j].tipoCampo == 'S')
            	printf(" %-20s ", pagina[j].valorCampo);
        	else if(pagina[j].tipoCampo == 'I'){
            	int *n = (int *)&pagina[j].valorCampo[0];
            	printf(" %-10d ", *n);
        	} else if(pagina[j].tipoCampo == 'C'){
            	printf(" %-10c ", pagina[j].valorCampo[0]);
        	} else if(pagina[j].tipoCampo == 'D'){
            	double *n = (double *)&pagina[j].valorCampo[0];
    	        printf(" %-10f ", *n);
        	}
            if(j>=1 && ((j+1)%objeto.qtdCampos)==0)
            	printf("\n");
        	else
        		printf("|");
    	}
    	x-=bufferpoll[p++].nrec;
    }
    printf("\n(%d %s)\n\n",ntuples,(1>=ntuples)?"row": "rows");

	}else{

    int ntuples = --x;
	p = 0;
	while(x){
	//aqui deve ser carregado as tuplas com as clasulas do where passar o rc select pra carregar a pagina
	//creio que seja a melhor forma pois ai só tera as tuplas das projeções e não precisa mexer drasticamente a função imprime, pois para baixo é apenas printfs
	    column *pagina = getPage(bufferpoll, esquema, objeto, p);

	    if(pagina == ERRO_PARAMETRO){
            printf("ERROR: could not open the table.\n");
            free(bufferpoll);
            free(esquema);
            return;
	    }
	    int z;
	    if(!cont) {
	        for(j=0; j < GLOBAL_PARSER->col_count ; j++){
		  	for(z=0; z < objeto.qtdCampos; z++)  {
			
		    if(strcmp(GLOBAL_DATA_SELECT.columnName[j],pagina[z].nomeCampo)==0){ 
	            	if(pagina[j].tipoCampo == 'S')
	                printf(" %-20s ", pagina[z].nomeCampo);
	        	else
	                printf(" %-10s ", pagina[z].nomeCampo);
	            if(j< GLOBAL_PARSER->col_count-1)
	            	printf("|");
	        }}}
	        printf("\n");
	        for(j=0; j < GLOBAL_PARSER->col_count ; j++){
	            printf("%s",(pagina[j].tipoCampo == 'S')? "----------------------": "------------");
	            if(j<GLOBAL_PARSER->col_count-1)
	            	printf("+");
	        }
	        printf("\n");
	    }
	    cont++;
	
		for(j=0; j < GLOBAL_PARSER->col_count  ; j++){
		for(z=0; z < objeto.qtdCampos*bufferpoll[p].nrec; z++){
			
		if(strcmp(GLOBAL_DATA_SELECT.columnName[j],pagina[z].nomeCampo)==0){
		
        	if(pagina[z].tipoCampo == 'S')
            	printf(" %-20s ", pagina[z].valorCampo);
        	else if(pagina[z].tipoCampo == 'I'){
            	int *n = (int *)&pagina[z].valorCampo[0];
            	printf(" %-10d ", *n);
        	} else if(pagina[z].tipoCampo == 'C'){
            	printf(" %-10c ", pagina[z].valorCampo[0]);
        	} else if(pagina[z].tipoCampo == 'D'){
            	double *n = (double *)&pagina[z].valorCampo[0];
    	        printf(" %-10f ", *n);
		} 
		
            if(GLOBAL_PARSER->col_count>1 && (z%GLOBAL_PARSER->col_count)==0)
            	printf("|");
        	else
        		printf("\n");
	}
    	}
} 
    	x-=bufferpoll[p++].nrec;
    }
    printf("\n(%d %s)\n\n",ntuples,(1>=ntuples)?"row": "rows");
}
    free(bufferpoll);
    free(esquema);
}
