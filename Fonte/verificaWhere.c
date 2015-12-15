#include "buffend.h"



int verificaWhere(rc_select *select, column *c, int j){
	int int_c, int_s;
	double double_c, double_s;

	if(select->where->pWhere == NULL){
		if(select->join == NULL){
			if(select->where->typeLeft == 0){
				if(strcmp(select->where->left, c[j].nomeCampo) == 0){

					if(select->where->typeRight == ALPHANUM_TYPE && (c[j].tipoCampo == 'C' || c[j].tipoCampo == 'S')){
						if(select->where->OP == OP_IGUAL){
							if(strcmp( select->where->right, c[j].valorCampo)==0){
								return 0;
							}else{
								return 1;
							}
						}else if(select->where->OP == OP_DIFERENTE){
							if(!strcmp(c->valorCampo, select->where->right))
								return 1;
							else
								return 0;
						}
				
					}else if((select->where->typeRight == NUMBER_TYPE && c[j].tipoCampo == 'D') ){
						double_c = (double)c[j].valorCampo[0];
						double_s = atof(select->where->right);
						
						if(select->where->OP == OP_IGUAL){
	
							if(double_c == double_s)
								return 0;

						}else if(select->where->OP == OP_DIFERENTE){

							if(double_c != double_s)
								return 0;

						}else if(select->where->OP == OP_MAIOR){

							if(double_c > double_s)
								return 0;

						} else if(select->where->OP == OP_MENOR){

							if(double_c < double_s)
								return 0;
						}
						return 1;
					}else if (select->where->typeRight == INT_TYPE && c[j].tipoCampo == 'I'){
						int_c = (int)c[j].valorCampo[0];
						int_s = atoi(select->where->right);
						
						if(select->where->OP == OP_IGUAL){

							if(int_c == int_s)
								return 0;

						}else if(select->where->OP == OP_DIFERENTE){

							if(int_c != int_s)
								return 0;

						}else if(select->where->OP == OP_MAIOR){

							if(int_c > int_s)
								return 0;

						} else if(select->where->OP == OP_MENOR){

							if(int_c < int_s)
								return 0;
						}
						return 1;

					}
					
				}
			}
		}
	}						
	return 0;
}
