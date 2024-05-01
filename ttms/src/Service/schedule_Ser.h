#ifndef SCHEDULE_H_
#define SCHEDULE_H_

#include "../Common/common.h" 
typedef struct{
	int id;
	int play_id;
	int studio_id;
	user_date_t date;
	user_time_t time;
} schedule_t;

typedef struct schedule_node{
	schedule_t data;
	struct schedule_node *next,*prev;
}schedule_node_t,*schedule_list_t; 


int Schedule_Srv_Add(schedule_t *data) ;

int Schedule_Srv_Modify(const schedule_t *data);

int Schedule_Srv_DeleteByID(int ID);

int Schedule_Srv_FetchByID(int ID, schedule_t *buf);

int Schedule_Srv_FetchAll(schedule_list_t list);

int Schedule_Srv_FetchAllByPlayID(schedule_list_t list, int id);

int Schedule_Srv_FetchAllByPlayName(schedule_list_t list, char name[]);

int Schedule_Srv_DeleteAllByPlayID(int playID);

int Schedule_Srv_DeleteAllByStudioID(int studioID);

#endif /* SCHEDULE_H_ */
