#ifndef ACCOUNT_H_
#define ACCOUNT_H_



typedef struct{
	int id;
	char name[20];
	char status[10];
	char cipher[20];
}account_t;

typedef struct account_node{
	account_t data;
	struct account_node *next,*prev;
}account_node_t,*account_list_t;

int Account_Srv_Add(account_t *data);

int Account_Srv_Modify(const account_t *data);

int Account_Srv_DeleteByID(int ID);

int Account_Srv_FetchByID(int ID, account_t *buf);

int Account_Srv_FetchAll(account_list_t list);

int Account_Srv_FetchByName(char name[], account_t *buf);
#endif //ACCOUNT_H_
