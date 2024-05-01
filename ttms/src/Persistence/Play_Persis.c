
#include "../Persistence/Play_Persis.h" 
#include "../Persistence/SalesAnalysis_Persis.h" 
#include "../Service/SalesAnalysis_Ser.h" 
#include "../common/List.h"
#include <stdlib.h>
#include <stdio.h>
#include<unistd.h>
#include <assert.h>
#include "../Service/Play_Ser.h"

static const char PLAY_DATA_FILE[] = "Play.dat"; //演出文件名常量 
static const char PLAY_DATA_TEMP_FILE[] = "PlayTmp.dat"; //演出临时文件名常量 
static const char PLAY_KEY_NAME[] = "Play"; //演出名常量  

int Play_Perst_SelectAll(play_list_t list)
{
	play_node_t* newNode;
	play_t data;
	int recCount=0;
	
	assert(NULL!=list);
	
	List_Free(list, play_node_t);
	FILE *fp = fopen(PLAY_DATA_FILE, "rb");
	
	if (NULL == fp) { //文件不存在
		return 0;
	}
	while (!feof(fp)) {
		if (fread(&data, sizeof(play_t), 1, fp)) {
			newNode = (play_node_t*) malloc(sizeof(play_node_t));
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

int Play_Perst_Insert(play_t *data) {	 
	assert(NULL!=data);

	
	long key = EntKey_Perst_GetNewKeys(PLAY_KEY_NAME, 1); //为新演出厅分配获取
	if(key<=0)			//主键分配失败，直接返回
		return 0;
	data->id = key;		//赋给新对象带回到UI层
	



	FILE *fp = fopen(PLAY_DATA_FILE, "ab");
	int rtn = 0;
	if (NULL == fp) {
		printf("无法打开文件 %s!\n", PLAY_DATA_FILE);
		return 0;
	}

	rtn = fwrite(data, sizeof(play_t), 1, fp);

	fclose(fp);
	return rtn;
}

int Play_Perst_Update(const play_t * data) {
	assert(NULL!=data);

	FILE *fp = fopen(PLAY_DATA_FILE, "rb+");
	if (NULL == fp) {
		printf("无法打开文件 %s!\n", PLAY_DATA_FILE);
		return 0;
	}

	play_t buf;
	int found = 0;

	while (!feof(fp)) {
		if (fread(&buf, sizeof(play_t), 1, fp)) {
			if (buf.id == data->id) {
				fseek(fp, -((int)sizeof(play_t)), SEEK_CUR);
				fwrite(data, sizeof(play_t), 1, fp);
				found = 1;
				break;
			}

		}
	}
	fclose(fp);

	return found;
}

int Play_Perst_DeleteByID(int ID) {

	//将原始文件重命名，然后读取数据重新写入到数据文件中，并将要删除的实体过滤掉。

	//对原始数据文件重命名
	if(rename(PLAY_DATA_FILE, PLAY_DATA_TEMP_FILE)<0){
		printf("无法打开文件 %s!\n", PLAY_DATA_FILE);
		return 0;
	}

	FILE *fpSour, *fpTarg;
	fpSour = fopen(PLAY_DATA_TEMP_FILE, "rb");
	if (NULL == fpSour ){
		printf("无法打开文件 %s!\n", PLAY_DATA_FILE);
		return 0;
	}

	fpTarg = fopen(PLAY_DATA_FILE, "wb");
	if ( NULL == fpTarg ) {
		printf("无法打开文件 %s!\n", PLAY_DATA_TEMP_FILE);
		return 0;
	}


	play_t buf;

	int found = 0;
	while (!feof(fpSour)) {
		if (fread(&buf, sizeof(play_t), 1, fpSour)) {
			if (ID == buf.id) {
				found = 1;
				continue;
			}
			fwrite(&buf, sizeof(play_t), 1, fpTarg);
		}
	}

	fclose(fpTarg);
	fclose(fpSour);

	//删除临时文件
	remove(PLAY_DATA_TEMP_FILE);
	return found;
}
int Play_Perst_SelectByID(int ID, play_t *buf) {
	assert(NULL!=buf);

	FILE *fp = fopen(PLAY_DATA_FILE, "rb");
	if (NULL == fp) {
		return 0;
	}

	play_t data;
	int found = 0;

	while (!feof(fp)) {
		if (fread(&data, sizeof(play_t), 1, fp)) {
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

