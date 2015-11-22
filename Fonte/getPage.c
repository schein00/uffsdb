//BufferPool
#include "buffend.h"

// RETORNA PAGINA DO BUFFER
column * getPage(tp_buffer *buffer, tp_table *campos, struct fs_objects objeto, int page, rc_select *GLOBAL_DATA_SELECT){

    if(page >= PAGES)
        return ERRO_PAGINA_INVALIDA;

    if(buffer[page].nrec == 0) //Essa página não possui registros
        return ERRO_PARAMETRO;
    
	//Para melhorar essa logica, adicionei um campo qtdcampos na struct, pode ser usado para refatorar o codigo mais tarde precisa saber como iremos preencher essa variavel quando não tiver '*'
    if(*GLOBAL_DATA_SELECT->colunmName = '*'){
	 *GLOBAL_DATA_SELECT -> qtdCampos = *objeto -> qtdCampos; 
	}

	//Aloca a quantidade de campos necessária
    	column *colunas = (column *)malloc(sizeof(column)**GLOBAL_DATA_SELECT->qtdCampos *(buffer[page].nrec)); 


    	if(!colunas){
       	 return ERRO_DE_ALOCACAO; 
	}	


    	//adiciona espaço para a variavel colunas, pela quantidade de campos da tabela vezes o numero de registros do pagina do buffer
	memset(colunas, 0, sizeof(column)**GLOBAL_DATA_SELECT->qtdCampos*(buffer[page].nrec));
   


    

    int i=0, j=0, t=0, h=0;
     
    if (!buffer[page].position)
        return colunas;

    while(i < buffer[page].position){
        t=0;
        
	if(j >= *GLOBAL_DATA_SELECT->qtdCampos) j=0;
	
	if(*GLOBAL_DATA_SELECT->colunmname = campos[j].nome){
        colunas[h].valorCampo = (char *)malloc(sizeof(char)*campos[j].tam+1);
        memset(colunas[h].valorCampo, '\0', campos[j].tam+1);
        colunas[h].tipoCampo = campos[j].tipo;  //Guarda tipo do campo
	
        strcpy(colunas[h].nomeCampo, campos[j].nome); //Guarda nome do campo
	
	//em desenvolvimento
        while(t < campos[j].tam){
		switch(GLOBAL_DATA_SELECT->andoor)
		case 'AND':
		case 'OR': 
			
		defaut:
		if(GLOBAL_DATA_SELECT->nameTeste == campos[j].name){
			if(verificaWhere(&GLOBAL_DATA_SELECT,&buffer,page,i) == 1)   
				 colunas[h].valorCampo[t] = buffer[page].data[i]; //Copia os dados
		}
            t++;
            i++;
        }
        colunas[h].valorCampo[t] = '\0';
	}
        h++;
        j++;
    
}
    return colunas; //Retorna a 'page' do buffer
}
