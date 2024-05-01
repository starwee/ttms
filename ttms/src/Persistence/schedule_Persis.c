#include "Schedule_Persis.h"
#include "../Service/schedule_Ser.h"
#include "EntityKey_Persist.h"	 
#include "../common/List.h"
#include <stdlib.h>
#include <stdio.h>
#include<unistd.h>
#include <assert.h>
#include "Ticket_Persis.h"
#include "../Service/Ticket_Ser.h"
#include <string.h>
#include "../Service/Play_Ser.h"

static const char SCHEDULE_DATA_FILE[] = "Schedule.dat"; //演出厅文件名常量 
static const char SCHEDULE_DATA_TEMP_FILE[] = "ScheduleTmp.dat"; //演出厅临时文件名常量 
static const char SCHEDULE_KEY_NAME[] = "Schedule"; //演出厅名常量 

int Schedule_Perst_Insert(schedule_t *data) {	 
	assert(NULL!=data);

	
	long key = EntKey_Perst_GetNewKeys(SCHEDULE_KEY_NAME, 1); //为新演出厅分配获取
	if(key<=0)			//主键分配失败，直接返回
		return 0;
	data->id = key;		//赋给新对象带回到UI层
	



	FILE *fp = fopen(SCHEDULE_DATA_FILE, "ab");
	int rtn = 0;
	if (NULL == fp) {
		printf("无法打开文件 %s!\n", SCHEDULE_DATA_FILE);
		return 0;
	}

	rtn = fwrite(data, sizeof(schedule_t), 1, fp);

	fclose(fp);
	ticket_list_t list;
	List_Init(list, ticket_node_t);
	Ticket_Srv_ScheduleInit(list,key);
	Ticket_Perst_InsertBatch(list);
	return rtn;
}

int Schedule_Perst_Update(const schedule_t * data) {
	assert(NULL!=data);

	FILE *fp = fopen(SCHEDULE_DATA_FILE, "rb+");
	if (NULL == fp) {
		printf("无法打开文件 %s!\n", SCHEDULE_DATA_FILE);
		return 0;
	}

	schedule_t buf;
	int found = 0;

	while (!feof(fp)) {
		if (fread(&buf, sizeof(schedule_t), 1, fp)) {
			if (buf.id == data->id) {
				fseek(fp, -((int)sizeof(schedule_t)), SEEK_CUR);
				fwrite(data, sizeof(schedule_t), 1, fp);
				found = 1;
				break;
			}

		}
	}
	fclose(fp);

	return found;
}

int Schedule_Perst_DeleteByID(int ID) {

	//将原始文件重命名，然后读取数据重新写入到数据文件中，并将要删除的实体过滤掉。

	//对原始数据文件重命名
	if(rename(SCHEDULE_DATA_FILE, SCHEDULE_DATA_TEMP_FILE)<0){
		printf("无法打开文件 %s!\n", SCHEDULE_DATA_FILE);
		return 0;
	}

	FILE *fpSour, *fpTarg;
	fpSour = fopen(SCHEDULE_DATA_TEMP_FILE, "rb");
	if (NULL == fpSour ){
		printf("无法打开文件 %s!\n", SCHEDULE_DATA_FILE);
		return 0;
	}

	fpTarg = fopen(SCHEDULE_DATA_FILE, "wb");
	if ( NULL == fpTarg ) {
		printf("无法打开文件 %s!\n", SCHEDULE_DATA_TEMP_FILE);
		return 0;
	}


	schedule_t buf;

	int found = 0;
	while (!feof(fpSour)) {
		if (fread(&buf, sizeof(schedule_t), 1, fpSour)) {
			if (ID == buf.id) {
				found = 1;
				continue;
			}
			fwrite(&buf, sizeof(schedule_t), 1, fpTarg);
		}
	}

	fclose(fpTarg);
	fclose(fpSour);

	//删除临时文件
	remove(SCHEDULE_DATA_TEMP_FILE);
	return found;
}
int Schedule_Perst_SelectByID(int ID, schedule_t *buf) {
	assert(NULL!=buf);

	FILE *fp = fopen(SCHEDULE_DATA_FILE, "rb");
	if (NULL == fp) {
		return 0;
	}

	schedule_t data;
	int found = 0;

	while (!feof(fp)) {
		if (fread(&data, sizeof(schedule_t), 1, fp)) {
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

int Schedule_Perst_SelectAll(schedule_list_t list) {
	schedule_node_t *newNode;
	schedule_t data;
	int recCount = 0;

	assert(NULL!=list);

	List_Free(list, schedule_node_t);

	FILE *fp = fopen(SCHEDULE_DATA_FILE, "rb");
	if (NULL == fp) { //文件不存在
		return 0;
	}

	while (!feof(fp)) {
		if (fread(&data, sizeof(schedule_t), 1, fp)) {
			newNode = (schedule_node_t*) malloc(sizeof(schedule_node_t));
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

int Schedule_Perst_SelectAllByPlayID(schedule_list_t list, int ID) {
    
	schedule_t data;
	int recCount = 0;
    schedule_node_t* newNode;
	assert(NULL!=list);

	List_Free(list, schedule_node_t);

	FILE *fp = fopen(SCHEDULE_DATA_FILE, "rb");
	if (NULL == fp) { //文件不存在
		return 0;
	}

	while (!feof(fp)) {
		if (fread(&data, sizeof(schedule_t), 1, fp)) {
		newNode = (schedule_node_t*) malloc(sizeof(schedule_node_t));
			if (!newNode) {
				printf(
						"存储已满!!!\n\n");
				break;
			}
			if(data.play_id==ID){
			newNode->data = data;
			List_AddTail(list, newNode); 
			recCount++;
			}
		}
	}
	fclose(fp);
	return recCount;
}

int Schedule_Perst_SelectAllByPlayName(schedule_list_t list, char name[]) {
    
	schedule_t data;
	int recCount = 0;
    schedule_node_t* newNode;
	
    assert(NULL!=list);
	List_Free(list, schedule_node_t);

	FILE *fp = fopen(SCHEDULE_DATA_FILE, "rb");
	if (NULL == fp) { //文件不存在
		return 0;
	}

	while (!feof(fp)) {
		if (fread(&data, sizeof(schedule_t), 1, fp)) {
		newNode = (schedule_node_t*) malloc(sizeof(schedule_node_t));
			if (!newNode) {
				printf(
						"存储已满!!!\n\n");
				break;
			}
			play_t play;
			Play_Srv_FetchByID(data.play_id, &play);
			if(dim(name,play.name)){
			newNode->data = data;
			List_AddTail(list, newNode); 
			recCount++;
			}
		}
	}
	fclose(fp);
	return recCount;
}

int dim(char key[],char str[])
{
	int l=strlen(key),r=strlen(str);
	int i,flag=0;
	char temp[20];
	for(i=0;i<=r-l;i++){
		int j=i,k=0;
		for(j;j<i+l;j++){
			temp[k++]=str[j];
		}
		temp[k]='\0';
		if(strcmp(key,temp)==0)
		{
			flag=1;
			break; 
		}
	}
	return flag;
 } 
 
 int Schedule_Perst_DeleteAllByPlayID(int playID) {
	
	
	if(rename(SCHEDULE_DATA_FILE, SCHEDULE_DATA_TEMP_FILE)<0){
		printf("无法打开文件 %s!\n", SCHEDULE_DATA_FILE);
		return 0;
	}

	FILE *fpSour, *fpTarg;
	fpSour = fopen(SCHEDULE_DATA_TEMP_FILE, "rb");
	if (NULL == fpSour ){
		printf("无法打开文件 %s!\n", SCHEDULE_DATA_FILE);
		return 0;
	}

	fpTarg = fopen(SCHEDULE_DATA_FILE, "wb");
	if ( NULL == fpTarg ) {
		printf("无法打开文件 %s!\n", SCHEDULE_DATA_TEMP_FILE);
		return 0;
	}


	schedule_t buf;

	int found = 0;
	while (!feof(fpSour)) {
		if (fread(&buf, sizeof(schedule_t), 1, fpSour)) {
			if (playID == buf.play_id) {
				found = 1;
				continue;
			}
			fwrite(&buf, sizeof(schedule_t), 1, fpTarg);
		}
	}

	fclose(fpTarg);
	fclose(fpSour);

	//删除临时文件
	remove(SCHEDULE_DATA_TEMP_FILE);
	return found;
}
int Schedule_Perst_DeleteAllByStudioID(int studioID) {
	
	
	if(rename(SCHEDULE_DATA_FILE, SCHEDULE_DATA_TEMP_FILE)<0){
		printf("无法打开文件 %s!\n", SCHEDULE_DATA_FILE);
		return 0;
	}

	FILE *fpSour, *fpTarg;
	fpSour = fopen(SCHEDULE_DATA_TEMP_FILE, "rb");
	if (NULL == fpSour ){
		printf("无法打开文件 %s!\n", SCHEDULE_DATA_FILE);
		return 0;
	}

	fpTarg = fopen(SCHEDULE_DATA_FILE, "wb");
	if ( NULL == fpTarg ) {
		printf("无法打开文件 %s!\n", SCHEDULE_DATA_TEMP_FILE);
		return 0;
	}


	schedule_t buf;

	int found = 0;
	while (!feof(fpSour)) {
		if (fread(&buf, sizeof(schedule_t), 1, fpSour)) {
			if (studioID == buf.studio_id) {
				found = 1;
				continue;
			}
			fwrite(&buf, sizeof(schedule_t), 1, fpTarg);
		}
	}

	fclose(fpTarg);
	fclose(fpSour);

	//删除临时文件
	remove(SCHEDULE_DATA_TEMP_FILE);
	return found;
}
