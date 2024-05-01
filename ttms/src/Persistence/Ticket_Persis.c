#include "Ticket_Persis.h"
#include "../Service/Ticket_Ser.h"
#include "EntityKey_Persist.h"	 
#include "../common/List.h"
#include <stdlib.h>
#include <stdio.h>
#include<unistd.h>
#include <assert.h>
#include "../Service/Seat.h"
#include "../Service/schedule_Ser.h"

static const char TICKET_DATA_FILE[] = "Ticket.dat"; //票务文件名常量 
static const char TICKET_DATA_TEMP_FILE[] = "TicketTmp.dat"; //票务临时文件名常量 
static const char TICKET_KEY_NAME[] = "Ticket"; //票务名常量 

int Ticket_Perst_Insert(ticket_t *data) {	 
	assert(NULL!=data);


	long key = EntKey_Perst_GetNewKeys(TICKET_KEY_NAME, 1); //为新演出厅分配获取
	if(key<=0)			//主键分配失败，直接返回
		return 0;
	data->id = key;		//赋给新对象带回到UI层




	FILE *fp = fopen(TICKET_DATA_FILE, "ab");
	int rtn = 0;
	if (NULL == fp) {
		printf("无法打开文件 %s!\n", TICKET_DATA_FILE);
		return 0;
	}

	rtn = fwrite(data, sizeof(ticket_t), 1, fp);

	fclose(fp);
	return rtn;
}

int Ticket_Perst_InsertBatch(ticket_list_t list) {
	ticket_node_t *p=list->next;
	ticket_t data;
	assert(NULL!=list);
	int rtn=0;
	 while(p!=list){
	 	data=p->data;
	 
	long key = EntKey_Perst_GetNewKeys(TICKET_KEY_NAME, 1); //为新演出厅分配获取
	if(key<=0)			//主键分配失败，直接返回
		return 0;
	data.id = key;		//赋给新对象带回到UI层
	



	FILE *fp = fopen(TICKET_DATA_FILE, "ab");
	if (NULL == fp) {
		printf("无法打开文件 %s!\n", TICKET_DATA_FILE);
		return 0;
	}

	rtn = fwrite(&data, sizeof(ticket_t), 1, fp);

	fclose(fp);
	p=p->next;
	 }
return rtn;
}

int Ticket_Perst_Update(const ticket_t * data) {
	assert(NULL!=data);

	FILE *fp = fopen(TICKET_DATA_FILE, "rb+");
	if (NULL == fp) {
		printf("无法打开文件 %s!\n", TICKET_DATA_FILE);
		return 0;
	}

	ticket_t buf;
	int found = 0;

	while (!feof(fp)) {
		if (fread(&buf, sizeof(ticket_t), 1, fp)) {
			if (buf.id == data->id) {
				fseek(fp, -((int)sizeof(ticket_t)), SEEK_CUR);
				fwrite(data, sizeof(ticket_t), 1, fp);
				found = 1;
				break;
			}

		}
	}
	fclose(fp);

	return found;
}

int Ticket_Perst_DeleteByID(int ID) {

	//将原始文件重命名，然后读取数据重新写入到数据文件中，并将要删除的实体过滤掉。

	//对原始数据文件重命名
	if(rename(TICKET_DATA_FILE, TICKET_DATA_TEMP_FILE)<0){
		printf("无法打开文件 %s!\n", TICKET_DATA_FILE);
		return 0;
	}

	FILE *fpSour, *fpTarg;
	fpSour = fopen(TICKET_DATA_TEMP_FILE, "rb");
	if (NULL == fpSour ){
		printf("无法打开文件 %s!\n", TICKET_DATA_FILE);
		return 0;
	}

	fpTarg = fopen(TICKET_DATA_FILE, "wb");
	if ( NULL == fpTarg ) {
		printf("无法打开文件 %s!\n", TICKET_DATA_TEMP_FILE);
		return 0;
	}


	ticket_t buf;

	int found = 0;
	while (!feof(fpSour)) {
		if (fread(&buf, sizeof(ticket_t), 1, fpSour)) {
			if (ID == buf.id) {
				found = 1;
				continue;
			}
			fwrite(&buf, sizeof(ticket_t), 1, fpTarg);
		}
	}

	fclose(fpTarg);
	fclose(fpSour);

	//删除临时文件
	remove(TICKET_DATA_TEMP_FILE);
	return found;
}

int Ticket_Perst_SelectByID(int ID, ticket_t *buf) {
	assert(NULL!=buf);

	FILE *fp = fopen(TICKET_DATA_FILE, "rb");
	if (NULL == fp) {
		return 0;
	}

	ticket_t data;
	int found = 0;

	while (!feof(fp)) {
		if (fread(&data, sizeof(ticket_t), 1, fp)) {
			if (ID == data.id) {
				*buf = data;
				found = 1;
				break;
			};

		}
	}
	fclose(fp);

	return found;
}

int Ticket_Perst_SelectAll(ticket_list_t list) {
	ticket_node_t *newNode;
	ticket_t data;
	int recCount = 0;

	assert(NULL!=list);

	List_Free(list, ticket_node_t);

	FILE *fp = fopen(TICKET_DATA_FILE, "rb");
	if (NULL == fp) { //文件不存在
		return 0;
	}
	while (!feof(fp)) {
		
		if (fread(&data, sizeof(ticket_t), 1, fp)) {
			newNode = (ticket_node_t*) malloc(sizeof(ticket_node_t));
			if (!newNode) {
				printf(
						"存储已满!!!\n\n");
				break;
			}
			newNode->data = data;
			List_AddTail(list, newNode);
			recCount++;
		}
	}
	fclose(fp);
	return recCount;
}

int Ticket_Perst_DeleteAllByScheduleID(int scheduleID) {
	
	
	if(rename(TICKET_DATA_FILE, TICKET_DATA_TEMP_FILE)<0){
		printf("无法打开文件 %s!\n", TICKET_DATA_FILE);
		return 0;
	}

	FILE *fpSour, *fpTarg;
	fpSour = fopen(TICKET_DATA_TEMP_FILE, "rb");
	if (NULL == fpSour ){
		printf("无法打开文件 %s!\n", TICKET_DATA_FILE);
		return 0;
	}

	fpTarg = fopen(TICKET_DATA_FILE, "wb");
	if ( NULL == fpTarg ) {
		printf("无法打开文件 %s!\n", TICKET_DATA_TEMP_FILE);
		return 0;
	}


	ticket_t buf;

	int found = 0;
	while (!feof(fpSour)) {
		if (fread(&buf, sizeof(ticket_t), 1, fpSour)) {
			if (scheduleID == buf.schedule_id) {
				found = 1;
				continue;
			}
			fwrite(&buf, sizeof(ticket_t), 1, fpTarg);
		}
	}

	fclose(fpTarg);
	fclose(fpSour);

	//删除临时文件
	remove(TICKET_DATA_TEMP_FILE);
	return found;
}

int Ticket_Perst_SelectByScheduleID(ticket_list_t list, int scheduleID) {
    
	ticket_t data;
	int recCount = 0;
    ticket_node_t* newNode;
	assert(NULL!=list);

	List_Free(list, ticket_node_t);

	FILE *fp = fopen(TICKET_DATA_FILE, "rb");
	if (NULL == fp) { //文件不存在
		return 0;
	}

	while (!feof(fp)) {
		if (fread(&data, sizeof(ticket_t), 1, fp)) {
		newNode = (ticket_node_t*) malloc(sizeof(ticket_node_t));
			if (!newNode) {
				printf(
						"存储已满!!!\n\n");
				break;
			}
			if(data.schedule_id==scheduleID){
			newNode->data = data;
			List_AddTail(list, newNode); 
			recCount++;
			}
		}
	}
	fclose(fp);
	return recCount;
}

int Ticket_Perst_SelectByUserID(ticket_list_t list, int userID) {
    
	ticket_t data;
	int recCount = 0;
    ticket_node_t* newNode;
	assert(NULL!=list);

	List_Free(list, ticket_node_t);

	FILE *fp = fopen(TICKET_DATA_FILE, "rb");
	if (NULL == fp) { //文件不存在
		return 0;
	}

	while (!feof(fp)) {
		if (fread(&data, sizeof(ticket_t), 1, fp)) {
		newNode = (ticket_node_t*) malloc(sizeof(ticket_node_t));
			if (!newNode) {
				printf(
						"存储已满!!!\n\n");
				break;
			}
			if(data.user_id==userID){
			newNode->data = data;
			List_AddTail(list, newNode); 
			recCount++;
			}
		}
	}
	fclose(fp);
	return recCount;
}

int Ticket_Perst_DeleteAllByPlayID(int playID) {
	
	
	if(rename(TICKET_DATA_FILE, TICKET_DATA_TEMP_FILE)<0){
		printf("无法打开文件 %s!\n", TICKET_DATA_FILE);
		return 0;
	}

	FILE *fpSour, *fpTarg;
	fpSour = fopen(TICKET_DATA_TEMP_FILE, "rb");
	if (NULL == fpSour ){
		printf("无法打开文件 %s!\n", TICKET_DATA_FILE);
		return 0;
	}

	fpTarg = fopen(TICKET_DATA_FILE, "wb");
	if ( NULL == fpTarg ) {
		printf("无法打开文件 %s!\n", TICKET_DATA_TEMP_FILE);
		return 0;
	}


	ticket_t buf;

	int found = 0;
	while (!feof(fpSour)) {
		if (fread(&buf, sizeof(ticket_t), 1, fpSour)) {
			schedule_t schedule;
			Schedule_Srv_FetchByID(buf.schedule_id, &schedule);
			if (playID == schedule.play_id) {
				found = 1;
				continue;
			}
			fwrite(&buf, sizeof(ticket_t), 1, fpTarg);
		}
	}

	fclose(fpTarg);
	fclose(fpSour);

	//删除临时文件
	remove(TICKET_DATA_TEMP_FILE);
	return found;
}

int Ticket_Perst_DeleteAllByStudioID(int studioID) {
	
	
	if(rename(TICKET_DATA_FILE, TICKET_DATA_TEMP_FILE)<0){
		printf("无法打开文件 %s!\n", TICKET_DATA_FILE);
		return 0;
	}

	FILE *fpSour, *fpTarg;
	fpSour = fopen(TICKET_DATA_TEMP_FILE, "rb");
	if (NULL == fpSour ){
		printf("无法打开文件 %s!\n", TICKET_DATA_FILE);
		return 0;
	}

	fpTarg = fopen(TICKET_DATA_FILE, "wb");
	if ( NULL == fpTarg ) {
		printf("无法打开文件 %s!\n", TICKET_DATA_TEMP_FILE);
		return 0;
	}


	ticket_t buf;

	int found = 0;
	while (!feof(fpSour)) {
		if (fread(&buf, sizeof(ticket_t), 1, fpSour)) {
			schedule_t schedule;
			Schedule_Srv_FetchByID(buf.schedule_id, &schedule);
			if (studioID == schedule.studio_id) {
				found = 1;
				continue;
			}
			fwrite(&buf, sizeof(ticket_t), 1, fpTarg);
		}
	}

	fclose(fpTarg);
	fclose(fpSour);

	//删除临时文件
	remove(TICKET_DATA_TEMP_FILE);
	return found;
}
