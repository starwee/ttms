#include "Ticket_Ser.h"
#include "../Persistence/Ticket_Persis.h"
#include <stdlib.h>
#include "../Service/Seat.h"
#include <assert.h>
#include "Schedule_Ser.h"
#include "../Common/List.h"
#include <stdio.h>

int Ticket_Srv_Add(ticket_t *data) {
	return Ticket_Perst_Insert(data);
}

int Ticket_Srv_Modify(const ticket_t *data) {
	return Ticket_Perst_Update(data);
}

int Ticket_Srv_DeleteByID(int ID) {
	return Ticket_Perst_DeleteByID(ID);
}

int Ticket_Srv_FetchByID(int ID, ticket_t *buf) {
	return Ticket_Perst_SelectByID(ID, buf);

}

int Ticket_Srv_ScheduleInit(ticket_list_t list,int id) {
	seat_list_t head;
	List_Init(head, seat_node_t);
	schedule_t schedule;
	Schedule_Srv_FetchByID(id,&schedule);
	
	Seat_Srv_FetchValidByRoomID(head,schedule.studio_id);
    seat_node_t *pos;
	int flag=1,i,j;
	for(pos=head->next;pos!=head;pos=pos->next){
		ticket_node_t* newNode=(ticket_node_t*)malloc(sizeof(ticket_node_t));
		ticket_t data;
		fflush(stdin); 
		data.schedule_id=id;
		data.seat_id=pos->data.id;
		data.user_id=0;
		newNode->data=data;
		List_AddTail(list, newNode);
	}
       return flag;
}

int Ticket_Srv_DeleteAllByScheduleID(int scheduleID)
{
	return Ticket_Perst_DeleteAllByScheduleID(scheduleID);
}

int Ticket_Srv_FetchByScheduleID(ticket_list_t list, int scheduleID){
      return Ticket_Perst_SelectByScheduleID(list,scheduleID);
}

int Ticket_Srv_FetchByUserID(ticket_list_t list, int userID){
      return Ticket_Perst_SelectByUserID(list,userID);
}

int Ticket_Srv_DeleteAllByPlayID(int playID)
{
	return Ticket_Perst_DeleteAllByPlayID(playID);
}

int Ticket_Srv_DeleteAllByStudioID(int studioID)
{
	return Ticket_Perst_DeleteAllByStudioID(studioID);
}

