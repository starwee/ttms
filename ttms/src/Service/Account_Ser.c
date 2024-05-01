#include "Account_Ser.h"
#include "../Persistence/Account_Persis.h"
#include <stdlib.h>
#include <assert.h>

int Account_Srv_Add(account_t *data) {
	return Account_Perst_Insert(data);
}

int Account_Srv_Modify(const account_t *data) {
	return Account_Perst_Update(data);
}

int Account_Srv_DeleteByID(int ID) {
	return Account_Perst_DeleteByID(ID);
}

int Account_Srv_FetchByID(int ID, account_t *buf) {
	return Account_Perst_SelectByID(ID, buf);

}

int Account_Srv_FetchAll(account_list_t list) {
	return Account_Perst_SelectAll(list);
}

int Account_Srv_FetchByName(char name[], account_t *buf) {
	return Account_Perst_SelectByName(name, buf);

}
