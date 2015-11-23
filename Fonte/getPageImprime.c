#include "buffend.h"
#include "string.h"
column * getPageImprime(tp_buffer *buffer, tp_table *campos, struct fs_objects objeto, int page, rc_select *GLOBAL_DATA_SELECT, rc_parser*GLOBAL_PARSER){

  

    if(page >= PAGES)
        return ERRO_PAGINA_INVALIDA;

    if(buffer[page].nrec == 0) //Essa página não possui registros
        return ERRO_PARAMETRO;
    
	//Para melhorar essa logica, adicionei um campo qtdcampos na struct, pode ser usado para refatorar o codigo mais tarde precisa saber como iremos preencher essa variavel quando não tiver '*'
	

 char c = '*';
 
    if( objcmp(GLOBAL_DATA_SELECT->columnName,c) == 0){
	 GLOBAL_PARSER->col_count = objeto.qtdCampos; 
		return ERRO_DE_ALOCACAO;
	}

	//Aloca a quantidade de campos necessária
    	column *colunas = (column *)malloc(sizeof(column)*GLOBAL_PARSER->col_count *(buffer[page].nrec)); 


    	if(!colunas){
       	 return ERRO_DE_ALOCACAO; 
	}	


    	//adiciona espaço para a variavel colunas, pela quantidade de campos da tabela vezes o numero de registros do pagina do buffer
	memset(colunas, 0, sizeof(column)*GLOBAL_PARSER->col_count*(buffer[page].nrec));
   


    

    int i=0, j=0, t=0, h=0;
    
    if (!buffer[page].position)
        return colunas;

    while(i < buffer[page].position){
        t=0;
        
	if(j >= GLOBAL_PARSER->col_count) j=0;
	
	if(GLOBAL_DATA_SELECT->columnName == campos[j].nome){
        colunas[h].valorCampo = (char *)malloc(sizeof(char)*campos[j].tam+1);
        memset(colunas[h].valorCampo, '\0', campos[j].tam+1);
        colunas[h].tipoCampo = campos[j].tipo;  //Guarda tipo do campo
	
        strcpy(colunas[h].nomeCampo, campos[j].nome); //Guarda nome do campo
	
	//em desenvolvimento
        while(t < campos[j].tam){
		//if(GLOBAL_DATA_SELECT->andOr == 'AND') 
		
		
		if(GLOBAL_DATA_SELECT->nameTeste == campos[j].nome){
			if(verificaWhere(GLOBAL_DATA_SELECT,buffer,page,i) == 1)   
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
