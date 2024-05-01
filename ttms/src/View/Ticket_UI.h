#ifndef TICKET_UI_H_
#define TICKET_UI_H_

void Ticket_UI_MgtEntry(int userID,int userstatus); 

void Play_UI(int userID,int userstatus); 

void Schedule_UI(int userID,int userstatus);

void Sale_UI(int userID,int userstatus);

void Sale_ticket(int userstatus,int user_id);

void Sale_UI_ReturnTicket();
 
void qticket(int userID,int c);

#endif /* TICKET_UI_H_ */
