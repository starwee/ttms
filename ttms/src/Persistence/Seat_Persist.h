

#ifndef SEATPERSIST_H_
#define SEATPERSIST_H_

#include "../Service/Seat.h"

int Seat_Perst_Insert(seat_t *data);	

int Seat_Perst_InsertBatch(seat_list_t list);

int Seat_Perst_Update(const seat_t *data);

int Seat_Perst_DeleteByID(int ID);

int Seat_Perst_DeleteAllByRoomID(int roomID);

int Seat_Perst_SelectByRoomID(seat_list_t list, int roomID);

int Seat_Perst_SelectByID(int ID, seat_t *buf);

int Seat_Perst_SelectAll(seat_list_t list);

#endif /* SEATPERSIST_H_ */
