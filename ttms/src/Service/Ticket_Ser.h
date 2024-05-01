#ifndef TICKET_H_
#define TICKET_H_

typedef struct{
	int id;
	int schedule_id;
	int user_id;
	int seat_id;
}ticket_t;

typedef struct ticket_node_t{
	ticket_t data;
	struct ticket_node_t* next,*prev;
}ticket_node_t,*ticket_list_t;

int Ticket_Srv_Add(ticket_t *data);

int Ticket_Srv_Modify(const ticket_t *data);

int Ticket_Srv_DeleteByID(int ID);

int Ticket_Srv_FetchByID(int ID, ticket_t *buf);

int Ticket_Srv_ScheduleInit(ticket_list_t list,int id);

int Ticket_Srv_DeleteAllByScheduleID(int scheduleID);

int Ticket_Srv_FetchByScheduleID(ticket_list_t list, int scheduleID);

int Ticket_Srv_FetchByUserID(ticket_list_t list, int userID);

int Ticket_Srv_DeleteAllByPlayID(int playID);

int Ticket_Srv_DeleteAllByStudioID(int studioID);

#endif //TICKET_H_
