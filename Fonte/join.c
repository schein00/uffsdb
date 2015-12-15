#include "buffend.h"


tp_buffer *join(rc_select *select){
	int x, y;	

	if((!verificaNomeTabela(select->objName)) || (!verificaNomeTabela(select->join->table))){
		 printf("\nERROR: relation \"%s\" or \"%s\" was not found.\n\n\n", select->objName, select->join->table);
        	return;
	}
	struct fs_objects objetoS = leObjeto(select->objName);
	struct fs_objects objetoJ = leObjeto(select->join->table);
			
	tp_table *esquemaS = leSchema(objetoS);
	tp_table *esquemaJ = leSchema(objetoJ);

	if(esquemaS == ERRO_ABRIR_ESQUEMA || esquemaJ == ERRO_ABRIR_ESQUEMA ){
        	printf("ERROR: schema cannot be created.\n");
        	free(esquemaS);
		free(esquemaJ);
        	return;
    	}

	tp_buffer *bufferS = initBuffer();	
	tp_buffer *bufferJ = initBuffer();
	tp_buffer *bufferR = initBuffer();

	if(bufferS == ERRO_DE_ALOCACAO || bufferJ == ERRO_DE_ALOCACAO  ){
        	free(bufferS);
		free(bufferJ);	
		free(esquemaS);
		free(esquemaJ);

        	printf("ERROR: no memory available to allocate buffer.\n");
        	return;
    	}

	erro = SUCCESS;
	for(x = 0; erro == SUCCESS; x++)
        	erro = colocaTuplaBuffer(bufferS, x, esquemaS, objetoS);

	erro = SUCCESS;
	for(y = 0; erro == SUCCESS; y++)
        	erro = colocaTuplaBuffer(bufferJ, y, esquemaJ, objetoJ);

	int pS = 0, pJ = 0, yy, k, l, n, m;
	while(x){
	    	column *paginaS = getPage(bufferS, esquemaS, objetoS, pS);
	    	if(pagina == ERRO_PARAMETRO){
            	printf("ERROR: could not open the table.\n");
            	free(bufferS);
            	free(esquemaS);
            	return;
	    	}
		yy = y;
		pJ = 0;
		while(yy){
			column *paginaJ = getPage(bufferJ, esquemaJ, objetoJ, pJ);
		    	if(pagina == ERRO_PARAMETRO){
	            	printf("ERROR: could not open the table.\n");
	            	free(bufferJ);
	            	free(esquemaJ);
	            	return;
		    	}
		
			for(k = 0; k < bufferS[p].nrec; k++){
				for(l = 0; l < bufferJ[p].nrec; l++){
					for(n = 0; n < objectoS.qtdCampos; n++){
						for(m = 0 ; m < objectoJ.qtdCampos; m++ ){
							if()							
						


						}
					}
				}
			}

		}

	}
	


}


