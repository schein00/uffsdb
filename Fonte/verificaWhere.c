#include "buffend.h"

int verificaWhere(rc_select GLOBAL_SELECT, tp_buffer *buffer, int page,int i, int j){
	
	switch(*GLOBAL_SELECT->op){
	 case '=':
	    if(*GLOBAL_SELECT->valor == buffer[page].data[i])           return 1;			 
	case '>':	    
	    if(*GLOBAL_SELECT->valor > buffer[page].data[i])            return 1;
	 case '<':   
	    if(*GLOBAL_SELECT->valor < buffer[page].data[i])            return 1;
	 case '!':   
	    if(*GLOBAL_SELECT->valor != buffer[page].data[i])       	return 1;
	}else{
		return 0;
	}
	


}
