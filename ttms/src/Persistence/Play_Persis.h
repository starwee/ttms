#ifndef SALEANALYSIS_PERSIST_H_
#define SALEANALYSIS_PERSIST_H_

#include "../Service/Play_Ser.h"


int Play_Perst_SelectAll(play_list_t list);
int Play_Perst_Insert(play_t *data);
int Play_Perst_Update(const play_t * data);
int Play_Perst_DeleteByID(int ID);
int Play_Perst_SelectByID(int ID, play_t *buf);



#endif /* SALEANALYSIS_PERSIST_H_ */
