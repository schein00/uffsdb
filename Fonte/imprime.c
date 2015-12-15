//BufferPool
#include "buffend.h"
#include <string.h>

/* ----------------------------------------------------------------------------------------------
    Objetivo:   Utilizada para impressão de tabelas (select * from table;).
    Parametros: Nome da tabela (char).
    Retorno:    void.
   ---------------------------------------------------------------------------------------------*/

void imprime(rc_select *GLOBAL_DATA_SELECT, rc_parser *GLOBAL_PARSER) {
  
   
    	int j, k,erro, x, p, cont=0;
    	struct fs_objects objeto, objetoJ;
	tp_table *esquemaJ = NULL;
	tp_buffer *bufferpollJ = NULL;
	

    	if(!verificaNomeTabela(GLOBAL_DATA_SELECT->objName)){
    	    printf("\nERROR: relation \"%s\" was not found.\n\n\n", GLOBAL_DATA_SELECT->objName);
    	    return;
    	}

    	objeto = leObjeto(GLOBAL_DATA_SELECT->objName);
    
		
    	tp_table *esquema = leSchema(objeto);
	
	
   	if(esquema == ERRO_ABRIR_ESQUEMA){
    	    	printf("ERROR: schema cannot be created.\n");
    	    	free(esquema);
    	    	return;
    	}

	
	tp_buffer *bufferpoll = initbuffer();	
	

    	if(bufferpoll == ERRO_DE_ALOCACAO ){
     	free(bufferpoll);
        	free(esquema);

        	printf("ERROR: no memory available to allocate buffer.\n");
        	return;
    	}

	erro = SUCCESS;

	for(x = 0; erro == SUCCESS; x++)
        	erro = colocaTuplaBuffer(bufferpoll, x, esquema, objeto);

	

	if(GLOBAL_DATA_SELECT->join == NULL){

		if(GLOBAL_DATA_SELECT->nColumn == 0){
			 printf("**\n");
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
				int podeImprimi = 0,aux =0;

				
		
				for(k = 0; k < bufferpoll[p].nrec; k++){
					if(GLOBAL_DATA_SELECT->where != NULL){
						for(j=0; j < objeto.qtdCampos && podeImprimi != 1; j++){
							podeImprimi = verificaWhere(GLOBAL_DATA_SELECT, pagina, j + aux);
						}
					}
	
					if(podeImprimi == 0){
						for(j=0; j < objeto.qtdCampos ; j++){
   	     					if(pagina[j + aux].tipoCampo == 'S')
				   		         	printf(" %-20s ", pagina[j + aux].valorCampo);
				   	     	else if(pagina[j + aux].tipoCampo == 'I'){
   		         					int *n = (int *)&pagina[j + aux].valorCampo[0];
   		         					printf(" %-10d ", *n);
   	     					} else if(pagina[j + aux].tipoCampo == 'C'){
   		         					printf(" %-10c ", pagina[j + aux].valorCampo[0]);
   	     					} else if(pagina[j + aux].tipoCampo == 'D'){
            						double *n = (double *)&pagina[j + aux].valorCampo[0];
    	        						printf(" %-10f ", *n);
        						}
		
     			       		if(objeto.qtdCampos==j+1)
            						printf("\n");
        						else
        							printf("|");
						}    	
					}else {
						ntuples--;
					}
					podeImprimi = 0; 
					aux += objeto.qtdCampos;
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
	     	   		for(j=0; j < GLOBAL_DATA_SELECT->nColumn ; j++){
			  			for(z=0; z < objeto.qtdCampos; z++)  {
				
			    				if(strcmp(GLOBAL_DATA_SELECT->columnName[j],pagina[z].nomeCampo)==0){ 
	     	       	
								if(pagina[j].tipoCampo == 'S')
	     	           				printf(" %-20s ", pagina[z].nomeCampo);
	     	   					else
	     	           				printf(" %-10s ", pagina[z].nomeCampo);
	     	       				
								if(j< GLOBAL_DATA_SELECT->nColumn-1)
	     	       					printf("|");
	     	   				}
						}
					}
	     	   		printf("\n");
	     	   		for(j=0; j < GLOBAL_DATA_SELECT->nColumn ; j++){
	     	       		printf("%s",(pagina[j].tipoCampo == 'S')? "----------------------": "------------");
	     	       		if(j<GLOBAL_DATA_SELECT->nColumn-1)
	     	       			printf("+");
	     	   		}
	     		   		printf("\n");
	    			}
	    			cont++;
		
				int podeImprimi = 0, aux = 0;
	
				for(k = 0; k < bufferpoll[p].nrec; k++){
					if(GLOBAL_DATA_SELECT->where != NULL){
						for(j=0; j < objeto.qtdCampos && podeImprimi != 1; j++){
							podeImprimi = verificaWhere(GLOBAL_DATA_SELECT, pagina, j + aux);
						}
					}
			
					if(podeImprimi == 0){
						for(z=0; z < GLOBAL_DATA_SELECT->nColumn ; z++){
							for(j=0; j < objeto.qtdCampos  ; j++){
								if(strcmp(GLOBAL_DATA_SELECT->columnName[z],pagina[j].nomeCampo)==0){
   	     							if(pagina[j + aux].tipoCampo == 'S')
   			         						printf(" %-20s ", pagina[j + aux].valorCampo);
   	     							else if(pagina[j + aux].tipoCampo == 'I'){
   			         						int *n = (int *)&pagina[j + aux].valorCampo[0];
   			         						printf(" %-10d ", *n);
   	     							} else if(pagina[j + aux].tipoCampo == 'C'){
   			         						printf(" %-10c ", pagina[j + aux].valorCampo[0]);
   	     							} else if(pagina[j + aux].tipoCampo == 'D'){
          	  							double *n = (double *)&pagina[j + aux].valorCampo[0];
    	        							printf(" %-10f ", *n);
        								}
	
						            	if(GLOBAL_DATA_SELECT->nColumn==(z+1))
						            		printf("\n");
						        		else
						        			printf("|");
								}   
 							}
						}	
					}else {
						ntuples--;
					}
					podeImprimi = 0; 
					aux += objeto.qtdCampos;
   				}
				x-=bufferpoll[p++].nrec;
   	
    			}
    			printf("\n(%d %s)\n\n",ntuples,(1>=ntuples)?"row": "rows");
		}
    	}else{

		if(!verificaNomeTabela(GLOBAL_DATA_SELECT->join->table)){
			printf("\nERROR: relation \"%s\" was not found.\n\n\n", GLOBAL_DATA_SELECT->join->table);
			return;
    		}
	
		objetoJ = leObjeto(GLOBAL_DATA_SELECT->join->table);
		esquemaJ = leSchema(objetoJ);

		if(esquemaJ == ERRO_ABRIR_ESQUEMA){
	    	    	printf("ERROR: schema cannot be created.\n");
	    	    	free(esquemaJ);
    	    		return;
    		}

		bufferpollJ = initbuffer();	
	
    		if(bufferpollJ == ERRO_DE_ALOCACAO ){
     		free(bufferpoll);
     	   	free(esquema);
	        	printf("ERROR: no memory available to allocate buffer.\n");
     	   	return;
    		}

	}
	
    	free(bufferpoll);
    	free(esquema);
	free(bufferpollJ);
    	free(esquemaJ);
}
