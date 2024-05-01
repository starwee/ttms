#ifndef ACCOUNT_PERSIST_H_
#define ACCOUNT_PERSIST_H_

#include "../Service/Account_Ser.h"

int Account_Perst_Insert(account_t *data);

int Account_Perst_Update(const account_t *data);

int Account_Perst_DeleteByID(int ID);

int Account_Perst_SelectByID(int ID, account_t *buf);

int Account_Perst_SelectAll(account_list_t list);

int Account_Perst_SelectByName(char name[], account_t *buf);


#endif /* ACCOUNT_PERSIST_H_ */
