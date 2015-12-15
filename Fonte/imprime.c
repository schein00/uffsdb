//BufferPool
#include "buffend.h"
#include <string.h>

/* ----------------------------------------------------------------------------------------------
    Objetivo:   Utilizada para impressão de tabelas (select * from table;).
    Parametros: Nome da tabela (char).
    Retorno:    void.
   ---------------------------------------------------------------------------------------------*/

void imprime(rc_select *GLOBAL_DATA_SELECT, rc_parser *GLOBAL_PARSER) {
   
    	int j, f, k, l,erro, x, y, yy, p, q, cont=0, aux, podeImprimi;
    	struct fs_objects objeto, objetoJ;


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
				podeImprimi = 0;
				aux =0;

				
		
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
			//creio que seja a melhor forma pois ai só tera as tuplas das projeções e não precisa mexer drasticamente a função imprime, 
			//pois para baixo é apenas printfs
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
		
				podeImprimi = 0, aux = 0;
	
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
		tp_table *esquemaJ = leSchema(objetoJ);

		if(esquemaJ == ERRO_ABRIR_ESQUEMA){
	    	    	printf("ERROR: schema cannot be created.\n");
	    	    	free(esquemaJ);
    	    		return;
    		}

		tp_buffer *bufferpollJ = initbuffer();	
	
    		if(bufferpollJ == ERRO_DE_ALOCACAO ){
     		free(bufferpollJ);
     	   	free(esquemaJ);
	        	printf("ERROR: no memory available to allocate buffer.\n");
     	   	return;
    		}

		erro = SUCCESS;
		for(y = 0; erro == SUCCESS; y++)
        		erro = colocaTuplaBuffer(bufferpollJ, y, esquemaJ, objetoJ);
		

		int ntuples = --x;		
		y--;
		p = 0;
		while(x){
			printf("while join x\n");
			column *pagina = getPage(bufferpoll, esquema, objeto, p);
   			if(pagina == ERRO_PARAMETRO){
         			printf("ERROR: could not open the table.\n");
         			free(bufferpoll);
         			free(esquema);
         			return;
    			}
			column *paginaJ = getPage(bufferpollJ, esquemaJ, objetoJ, p);
			if(paginaJ == ERRO_PARAMETRO){
         			printf("ERROR: could not open the table.\n");
         			free(bufferpollJ);
         			free(esquemaJ);
         			return;
    			}


			int podeImprimi = 0;
		
			if(!cont) {
				for(j=0; j < objeto.qtdCampos; j++){									
					if(pagina[j].tipoCampo == 'S')
						printf(" %-20s ", pagina[j].nomeCampo);
					else
						printf(" %-10s ", pagina[j].nomeCampo);
	
					if(j < (objeto.qtdCampos + objetoJ.qtdCampos)-1)
						printf("|");
				}
				for(l = 0; l < objetoJ.qtdCampos ; l++){
					
					if(pagina[ l + j].tipoCampo == 'S')
						printf(" %-20s ", paginaJ[ l+ j].nomeCampo);
					else
						printf(" %-10s ", paginaJ[l + j].nomeCampo);
	
					if(j < (objeto.qtdCampos + objetoJ.qtdCampos) - 1)
						printf("|");
				}
				
				printf("\n");
				for(j=0; j < objeto.qtdCampos + objetoJ.qtdCampos; j++){
					printf("%s",(pagina[j].tipoCampo == 'S')? "----------------------": "------------");
				}
				printf("\n");
			}
			cont++;
		

			for(k = 0; k < bufferpoll[p].nrec; k++){
				yy = y;
				q = 0;
				aux = 0;
				podeImprimi = 0;
				while(yy){
					printf("while join y%d\n", yy);
	
					column *paginaJ = getPage(bufferpollJ, esquemaJ, objetoJ, q);
		   			if(paginaJ == ERRO_PARAMETRO){
		         			printf("ERROR: could not open the table.\n");
		         			free(bufferpollJ);
		         			free(esquemaJ);
		         			return;
					}
					for(l = 0; l < bufferpollJ[q].nrec; l++){
						for(j = 0; j < objeto.qtdCampos && podeImprimi != 1; j++){						
							for(f = 0; f < objetoJ.qtdCampos  && podeImprimi != 1; f++){
							printf("P %s = %s\n",pagina[j].valorCampo , paginaJ[f].valorCampo);
								if(strcmp( pagina[j].nomeCampo, paginaJ[f].nomeCampo) == 0){
									printf("nome campo igual\n");
									if((pagina[j].tipoCampo == 'S' || pagina[j].tipoCampo == 'C' ) && 
										(paginaJ[f].tipoCampo == 'S' || paginaJ[f].tipoCampo == 'C' )){
										printf(" campo char\n");
										if(strcmp(pagina[j].valorCampo, paginaJ[f].valorCampo) != 0){
											podeImprimi = 1; printf("nome igual\n");}
									}else if(pagina[j].tipoCampo == 'I' && paginaJ[f].tipoCampo == 'I' ){
										int c = (int)pagina[j].valorCampo[0]; 
										int s = (int)paginaJ[f].valorCampo[0]; printf("P3 %d = %d\n", c, s);  printf("P %d = %d\n", j, f); 
										if(c != s){
											podeImprimi = 1; /*printf("P %d = %d\n", *c, *s);*/ }								
									}else if(pagina[j].tipoCampo == 'D' && paginaJ[f].tipoCampo == 'D' ){
										double c = atof(pagina[j].valorCampo);
										double s = atof(paginaJ[f].valorCampo);
										if(c != s)
											podeImprimi = 1;
									}
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
									if((objeto.qtdCampos+objetoJ.qtdCampos) != j + 1)            				
        									printf("|");
								}
								aux += objeto.qtdCampos;
								 for(j = 0; j  < objeto.qtdCampos ; j++){
   	     							if(paginaJ[j + aux].tipoCampo == 'S')
				   		     		    	printf(" %-20s ", paginaJ[j + aux].valorCampo);
				   	     			else if(paginaJ[j + aux].tipoCampo == 'I'){
   		         							int *n = (int *)&paginaJ[j + aux].valorCampo[0];
   		         							printf(" %-10d ", *n);
   	     							} else if(paginaJ[j + aux].tipoCampo == 'C'){
   		         							printf(" %-10c ", paginaJ[j + aux].valorCampo[0]);
   	     							} else if(paginaJ[j + aux].tipoCampo == 'D'){
            								double *n = (double *)&paginaJ[j + aux].valorCampo[0];
    	        								printf(" %-10f ", *n);
        								}
				
     			       				if((objeto.qtdCampos+objetoJ.qtdCampos)==j+1)
            								printf("\n");
        								else
        									printf("|");
								}
								aux += objetoJ.qtdCampos;								
							}else {
								ntuples--;
							}
							podeImprimi = 0; 
							aux += objeto.qtdCampos;			

							
						}
							
					}
					yy-=bufferpollJ[q++].nrec ;
				}
				
				yy = y;
			}
			
			x-=bufferpoll[p++].nrec ;			
		}	
		printf("\n(%d %s)\n\n",ntuples,(1>=ntuples)?"row": "rows");

	}
	
    	free(bufferpoll);
    	free(esquema);
}

