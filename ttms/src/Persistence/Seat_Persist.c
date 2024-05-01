

#include "Seat_Persist.h"
#include "../Service/Seat.h"
#include "../Common/List.h"
#include <stdlib.h>
#include <stdio.h>
#include<unistd.h>
#include <assert.h>

static const char SEAT_DATA_FILE[] = "Seat.dat";
static const char SEAT_DATA_TEMP_FILE[] = "SeatTmp.dat";

//添加对象主键标识名称
static const char SEAT_KEY_NAME[] = "Seat";


int Seat_Perst_Insert(seat_t *data) {   
	assert(NULL!=data);
	
	long key = EntKey_Perst_GetNewKeys(SEAT_KEY_NAME, 1); //为新演出厅分配获取
	if(key<=0)			//主键分配失败，直接返回
		return 0;
	data->id = key;		//赋给新对象带回到UI层
	


	FILE *fp = fopen(SEAT_DATA_FILE, "ab");
	int rtn = 0;
	if (NULL == fp) {
		printf("无法打开文件 %s!\n", SEAT_DATA_FILE);
		return 0;
	}

	rtn = fwrite(data, sizeof(seat_t), 1, fp);

	fclose(fp);
	return rtn;
}


int Seat_Perst_InsertBatch(seat_list_t list) {
	seat_node_t *p=list->next;
	seat_t data;
	assert(NULL!=list);
	int rtn=0;
	 while(p!=list){
	 	data=p->data;
	 	
	long key = EntKey_Perst_GetNewKeys(SEAT_KEY_NAME, 1); //为新演出厅分配获取
	if(key<=0)			//主键分配失败，直接返回
		return 0;
	data.id = key;		//赋给新对象带回到UI层
	



	FILE *fp = fopen(SEAT_DATA_FILE, "ab");
	if (NULL == fp) {
		printf("无法打开文件 %s!\n", SEAT_DATA_FILE);
		return 0;
	}

	rtn = fwrite(&data, sizeof(seat_t), 1, fp);

	fclose(fp);
	p=p->next;
	 }
return rtn;
}


int Seat_Perst_Update(const seat_t *data) {
	assert(NULL!=data);

	FILE *fp = fopen(SEAT_DATA_FILE, "rb+");
	if (NULL == fp) {
		printf("无法打开文件 %s!\n", SEAT_DATA_FILE);
		return 0;
	}

	seat_t buf;
	int found = 0;

	while (!feof(fp)) {
		if (fread(&buf, sizeof(seat_t), 1, fp)) {
			if (buf.id == data->id) {
				fseek(fp, -((int)sizeof(seat_t)), SEEK_CUR);
				fwrite(data, sizeof(seat_t), 1, fp);
				found = 1;
				break;
			}

		}
	}
	fclose(fp);

	return found;
}


int Seat_Perst_DeleteByID(int ID) {
	
	
	if(rename(SEAT_DATA_FILE, SEAT_DATA_TEMP_FILE)<0){
		printf("无法打开文件 %s!\n", SEAT_DATA_FILE);
		return 0;
	}

	FILE *fpSour, *fpTarg;
	fpSour = fopen(SEAT_DATA_TEMP_FILE, "rb");
	if (NULL == fpSour ){
		printf("无法打开文件 %s!\n", SEAT_DATA_FILE);
		return 0;
	}

	fpTarg = fopen(SEAT_DATA_FILE, "wb");
	if ( NULL == fpTarg ) {
		printf("无法打开文件 %s!\n", SEAT_DATA_TEMP_FILE);
		return 0;
	}


	seat_t buf;

	int found = 0;
	while (!feof(fpSour)) {
		if (fread(&buf, sizeof(seat_t), 1, fpSour)) {
			if (ID == buf.id) {
				found = 1;
				continue;
			}
			fwrite(&buf, sizeof(seat_t), 1, fpTarg);
		}
	}

	fclose(fpTarg);
	fclose(fpSour);

	//删除临时文件
	remove(SEAT_DATA_TEMP_FILE);
	return found;
}


int Seat_Perst_DeleteAllByRoomID(int roomID) {
	
	
	if(rename(SEAT_DATA_FILE, SEAT_DATA_TEMP_FILE)<0){
		printf("无法打开文件 %s!\n", SEAT_DATA_FILE);
		return 0;
	}

	FILE *fpSour, *fpTarg;
	fpSour = fopen(SEAT_DATA_TEMP_FILE, "rb");
	if (NULL == fpSour ){
		printf("无法打开文件 %s!\n", SEAT_DATA_FILE);
		return 0;
	}

	fpTarg = fopen(SEAT_DATA_FILE, "wb");
	if ( NULL == fpTarg ) {
		printf("无法打开文件 %s!\n", SEAT_DATA_TEMP_FILE);
		return 0;
	}


	seat_t buf;

	int found = 0;
	while (!feof(fpSour)) {
		if (fread(&buf, sizeof(seat_t), 1, fpSour)) {
			if (roomID == buf.roomID) {
				found = 1;
				continue;
			}
			fwrite(&buf, sizeof(seat_t), 1, fpTarg);
		}
	}

	fclose(fpTarg);
	fclose(fpSour);

	//删除临时文件
	remove(SEAT_DATA_TEMP_FILE);
	return found;
}


int Seat_Perst_SelectByID(int ID, seat_t *buf) {
	assert(NULL!=buf);

	FILE *fp = fopen(SEAT_DATA_FILE, "rb");
	if (NULL == fp) {
		return 0;
	}

	seat_t data;
	int found = 0;

	while (!feof(fp)) {
		if (fread(&data, sizeof(seat_t), 1, fp)) {
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


int Seat_Perst_SelectAll(seat_list_t list) {
	seat_node_t *newNode;
	seat_t data;
	int recCount = 0;

	assert(NULL!=list);

	List_Free(list, seat_node_t);

	FILE *fp = fopen(SEAT_DATA_FILE, "rb");
	if (NULL == fp) { //文件不存在
		return 0;
	}

	while (!feof(fp)) {
		if (fread(&data, sizeof(seat_t), 1, fp)) {
			newNode = (seat_node_t*) malloc(sizeof(seat_node_t));
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


int Seat_Perst_SelectByRoomID(seat_list_t list, int roomID) {
    
	seat_t data;
	int recCount = 0;
    seat_node_t* newNode;
	assert(NULL!=list);

	List_Free(list, seat_node_t);

	FILE *fp = fopen(SEAT_DATA_FILE, "rb");
	if (NULL == fp) { //文件不存在
		return 0;
	}

	while (!feof(fp)) {
		if (fread(&data, sizeof(seat_t), 1, fp)) {
		newNode = (seat_node_t*) malloc(sizeof(seat_node_t));
			if (!newNode) {
				printf(
						"存储已满!!!\n\n");
				break;
			}
			if(data.roomID==roomID){
			newNode->data = data;
			List_AddTail(list, newNode); 
			recCount++;
			}
		}
	}
	fclose(fp);
	return recCount;
}
