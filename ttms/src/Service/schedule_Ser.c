
#include "../Persistence/schedule_Persis.h"
#include <stdlib.h>
#include <assert.h>
#include "../Service/schedule_Ser.h"




int Schedule_Srv_Add(schedule_t *data) {
	return Schedule_Perst_Insert(data);
}

int Schedule_Srv_Modify(const schedule_t *data) {
	return Schedule_Perst_Update(data);
}

int Schedule_Srv_DeleteByID(int ID) {
	return Schedule_Perst_DeleteByID(ID);
}

int Schedule_Srv_FetchAll(schedule_list_t list) {
	return Schedule_Perst_SelectAll(list);
}

int Schedule_Srv_FetchByID(int ID, schedule_t *buf) {
	return Schedule_Perst_SelectByID(ID, buf);

}

int Schedule_Srv_FetchAllByPlayID(schedule_list_t list, int id) {
	return Schedule_Perst_SelectAllByPlayID(list, id);

}

int Schedule_Srv_FetchAllByPlayName(schedule_list_t list, char name[]) {
	return Schedule_Perst_SelectAllByPlayName(list, name);

}

int Schedule_Srv_DeleteAllByPlayID(int playID){
	return Schedule_Perst_DeleteAllByPlayID(playID);
}

int Schedule_Srv_DeleteAllByStudioID(int studioID){
	return Schedule_Perst_DeleteAllByStudioID(studioID);
}
