

#ifndef SEAT_UI_H_
#define SEAT_UI_H_
#include "../Common/list.h"
#include "../Service/seat.h"


void Seat_UI_MgtEntry(int roomID);


char Seat_UI_Status2Char(seat_status_t status);


seat_status_t Seat_UI_Char2Status(char statusChar);


int Seat_UI_Modify(seat_list_t list, int rowsCount, int colsCount);


#endif /* SEAT_UI_H_ */
