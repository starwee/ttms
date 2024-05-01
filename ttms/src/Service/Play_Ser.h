#ifndef PLAY_H_
#define PLAY_H_

typedef struct play_t{
	int id;
	char name[32];
	int price; 
	int sale;
}play_t;

typedef struct play_node{
	play_t data;
	struct play_node *prev,*next;
}play_node_t,*play_list_t;

int Play_Srv_Add(play_t *data);
int Play_Srv_Modify(const play_t *data) ;
int Play_Srv_DeleteByID(int ID);
int Play_Srv_FetchAll(play_list_t list);
int Play_Srv_FetchByID(int ID, play_t *buf);

#endif //PLAY_H_
