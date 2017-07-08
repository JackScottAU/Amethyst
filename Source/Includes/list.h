/* 
 * File:   list.h
 * Author: Jack Scott
 */

#ifndef LIST_H
#define	LIST_H

#ifdef	__cplusplus
extern "C" {
#endif
	
	typedef struct list_node_s {
		void* data;
		struct list_node_s* next;
	} list_node;

#ifdef	__cplusplus
}
#endif

#endif	/* LIST_H */

