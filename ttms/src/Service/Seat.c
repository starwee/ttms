
#include <stdlib.h>
#include "../Common/List.h"
#include "Seat.h"

int Seat_Srv_Add(const seat_t *data){
	return Seat_Perst_Insert(data);
}

int Seat_Srv_AddBatch(seat_list_t list){
	return Seat_Perst_InsertBatch(list);
}

int Seat_Srv_Modify(const seat_t *data){
	return Seat_Perst_Update(data);
}

int Seat_Srv_DeleteByID(int ID){
	return Seat_Perst_DeleteByID(ID);
}

int Seat_Srv_FetchByID(int ID, seat_t *buf){
	return Seat_Perst_SelectByID(ID, buf);
}

int Seat_Srv_DeleteAllByRoomID(int roomID){
	return Seat_Perst_DeleteAllByRoomID(roomID);
}

int Seat_Srv_FetchByRoomID(seat_list_t list, int roomID){
      return Seat_Perst_SelectByRoomID(list,roomID);
}

int Seat_Srv_FetchValidByRoomID(seat_list_t list, int roomID)
{
       int seatcount=Seat_Perst_SelectByRoomID(list,roomID);
       seat_node_t *node=list->next;
       while(node!=list){
       	if(node->data.status!=1){
       		List_FreeNode(node);
       		seatcount--;
		   }
       	node=node->next;
	   }
       return seatcount;
}

int Seat_Srv_RoomInit(seat_list_t list, int roomID, int rowsCount,
		int colsCount) {
	int flag=1,i,j;
	for(i=1;i<=rowsCount;i++){
		for(j=1;j<=colsCount;j++){
			seat_node_t* newNode=(seat_node_t*)malloc(sizeof(seat_node_t));
			newNode->data.status=1;
			newNode->data.roomID=roomID;
			newNode->data.row=i;
			newNode->data.column=j; 
			List_AddTail(list, newNode);
		}
		
	} 
       return flag;
}

void Seat_Srv_SortSeatList(seat_list_t list) {
       seat_node_t* p,*q;
       seat_t data;
	   
	   for(p=list->prev;p!=list->next;p=p->prev){
	   	for(q=list->next;q!=p;q=q->next){
	   		if(q->data.row>q->next->data.row){
	   			data=q->data;
	   			q->data=q->next->data;
	   			q->next->data=data;
			   }
		   }
	   }
	   seat_node_t* l=list->next,*r=list->prev;
	   while(l!=list){
	   	while(r!=list&&r->next->data.row==l->data.row) r=r->next;
	   	p=l;
	   	q=r;
	   	for(p=r;p!=l->next;p=p->prev){
	   	for(q=l;q!=p;q=q->next){
	   		if(q->data.column>q->next->data.column){
	   			data=q->data;
	   			q->data=q->next->data;
	   			q->next->data=data;
			   }
		   }
	   }
	   l=r->next;
	   }
}

void Seat_Srv_AddToSoftedList(seat_list_t list, seat_node_t *node) {
       seat_node_t* tmp;
       tmp=list->next;
       while(tmp->data.row<node->data.row) tmp=tmp->next;
       while(tmp->data.row==node->data.row&&node->data.column>tmp->data.column) tmp=tmp->next;
	   List_InsertBefore(tmp, node);
}

seat_node_t * Seat_Srv_FindByRowCol(seat_list_t list, int row, int column) {
       seat_node_t* node=list->next;
       while(node!=list){
       	if(node->data.column==column&&node->data.row==row) break;
       	node=node->next;
	   }   
	   return node;
}

seat_node_t * Seat_Srv_FindByID(seat_list_t list, int ID) {
       seat_node_t* node=list->next;
       while(node!=list){
       	if(node->data.id==ID) break;
       	node=node->next;
	   }   
	   return node;
}
