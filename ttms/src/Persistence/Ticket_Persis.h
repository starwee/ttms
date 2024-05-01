#ifndef TICKET_PERSIST_H_
#define TICKET_PERSIST_H_

#include "../Service/Ticket_Ser.h"

int Ticket_Perst_Insert(ticket_t *data);

int Ticket_Perst_InsertBatch(ticket_list_t list);

int Ticket_Perst_Update(const ticket_t * data);

int Ticket_Perst_DeleteByID(int ID);

int Ticket_Perst_SelectByID(int ID, ticket_t *buf);

int Ticket_Perst_SelectAll(ticket_list_t list);

int Ticket_Perst_DeleteAllByScheduleID(int scheduleID);

int Ticket_Perst_SelectByScheduleID(ticket_list_t list, int scheduleID);

int Ticket_Perst_SelectByUserID(ticket_list_t list, int userID);

int Ticket_Perst_DeleteAllByPlayID(int playID);

int Ticket_Perst_DeleteAllByStudioID(int studioID);

#endif /* TICKET_PERSIST_H_ */
