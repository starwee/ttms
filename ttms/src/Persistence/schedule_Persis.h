#ifndef SCHEDULE_PERSIST_H_
#define SCHEDULE_PERSIST_H_

#include "../Service/schedule_Ser.h"

int Schedule_Perst_Insert(schedule_t *data);

int Schedule_Perst_Update(const schedule_t * data);

int Schedule_Perst_DeleteByID(int ID);

int Schedule_Perst_SelectByID(int ID, schedule_t *buf);

int Schedule_Perst_SelectAll(schedule_list_t list);

int Schedule_Perst_SelectAllByPlayID(schedule_list_t list, int ID);

int Schedule_Perst_SelectAllByPlayName(schedule_list_t list, char name[]);

int dim(char key[],char str[]);

 int Schedule_Perst_DeleteAllByPlayID(int playID);
 
 int Schedule_Perst_DeleteAllByStudioID(int studioID);

#endif /* SCHEDULE_PERSIST_H_ */
