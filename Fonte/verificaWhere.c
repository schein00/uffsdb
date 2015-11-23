#include "buffend.h"

int verificaWhere(rc_select *GLOBAL_SELECT, tp_buffer *buffer, int page,int i){
	char c = GLOBAL_SELECT->op;
	switch(c){
	 case '=':
	    if(GLOBAL_SELECT->values == buffer[page].data[i])           return 1;			 
	 case '>':	    
	    if(GLOBAL_SELECT->values > buffer[page].data[i])            return 1;
	 case '<':   
	    if(GLOBAL_SELECT->values < buffer[page].data[i])            return 1;
	 case '!':   
	    if(GLOBAL_SELECT->values != buffer[page].data[i])       	return 1;
	 default:	
		return 0;
	}
	
}