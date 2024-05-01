#include "Account_Persis.h"
#include "../Service/Account_Ser.h"
#include "EntityKey_Persist.h"	 
#include "../common/list.h"
#include <stdlib.h>
#include <stdio.h>
#include<unistd.h>
#include <assert.h>
#include<string.h>

static const char ACCOUNT_DATA_FILE[] = "Account.dat"; //用户文件名常量 
static const char ACCOUNT_DATA_TEMP_FILE[] = "AccountTmp.dat"; //用户临时文件名常量 
static const char ACCOUNT_KEY_NAME[] = "Account"; //用户名常量 

int Account_Perst_Insert(account_t *data) {	 
	assert(NULL!=data);

	
	long key = EntKey_Perst_GetNewKeys(ACCOUNT_KEY_NAME, 1); //为新演出厅分配获取
	if(key<=0)			//主键分配失败，直接返回
		return 0;
	data->id = key;		//赋给新对象带回到UI层
	



	FILE *fp = fopen(ACCOUNT_DATA_FILE, "ab");
	int rtn = 0;
	if (NULL == fp) {
		printf("无法打开文件 %s!\n", ACCOUNT_DATA_FILE);
		return 0;
	}

	rtn = fwrite(data, sizeof(account_t), 1, fp);

	fclose(fp);
	return rtn;
return 0; 
}

int Account_Perst_Update(const account_t *data) {
	assert(NULL!=data);

	FILE *fp = fopen(ACCOUNT_DATA_FILE, "rb+");
	if (NULL == fp) {
		printf("无法打开文件 %s!\n", ACCOUNT_DATA_FILE);
		return 0;
	}

	account_t buf;
	int found = 0;

	while (!feof(fp)) {
		if (fread(&buf, sizeof(account_t), 1, fp)) {
			if (buf.id == data->id) {
				fseek(fp, -((int)sizeof(account_t)), SEEK_CUR);
				fwrite(data, sizeof(account_t), 1, fp);
				found = 1;
				break;
			}

		}
	}
	fclose(fp);

	return found;
}

int Account_Perst_DeleteByID(int ID) {

	//将原始文件重命名，然后读取数据重新写入到数据文件中，并将要删除的实体过滤掉。

	//对原始数据文件重命名
	if(rename(ACCOUNT_DATA_FILE, ACCOUNT_DATA_TEMP_FILE)<0){
		printf("无法打开文件 %s!\n", ACCOUNT_DATA_FILE);
		return 0;
	}

	FILE *fpSour, *fpTarg;
	fpSour = fopen(ACCOUNT_DATA_TEMP_FILE, "rb");
	if (NULL == fpSour ){
		printf("无法打开文件 %s!\n", ACCOUNT_DATA_FILE);
		return 0;
	}

	fpTarg = fopen(ACCOUNT_DATA_FILE, "wb");
	if ( NULL == fpTarg ) {
		printf("无法打开文件 %s!\n", ACCOUNT_DATA_TEMP_FILE);
		return 0;
	}


	account_t buf;

	int found = 0;
	while (!feof(fpSour)) {
		if (fread(&buf, sizeof(account_t), 1, fpSour)) {
			if (ID == buf.id) {
				found = 1;
				continue;
			}
			fwrite(&buf, sizeof(account_t), 1, fpTarg);
		}
	}

	fclose(fpTarg);
	fclose(fpSour);

	//删除临时文件
	remove(ACCOUNT_DATA_TEMP_FILE);
	return found;
}

int Account_Perst_SelectByID(int ID, account_t *buf) {
	assert(NULL!=buf);

	FILE *fp = fopen(ACCOUNT_DATA_FILE, "rb");
	if (NULL == fp) {
		return 0;
	}

	account_t data;
	int found = 0;

	while (!feof(fp)) {
		if (fread(&data, sizeof(account_t), 1, fp)) {
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

int Account_Perst_SelectAll(account_list_t list) {
	account_node_t *newNode;
	account_t data;
	int recCount = 0;

	assert(NULL!=list);

	List_Free(list, account_node_t);

	FILE *fp = fopen(ACCOUNT_DATA_FILE, "rb");
	if (NULL == fp) { //文件不存在
		return 0;
	}

	while (!feof(fp)) {
		if (fread(&data, sizeof(account_t), 1, fp)) {
			newNode = (account_node_t*) malloc(sizeof(account_node_t));
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

int Account_Perst_SelectByName(char name[], account_t *buf) {
	assert(NULL!=buf);

	FILE *fp = fopen(ACCOUNT_DATA_FILE, "rb");
	if (NULL == fp) {
		return 0;
	}

	account_t data;
	int found = 0;

	while (!feof(fp)) {
		if (fread(&data, sizeof(account_t), 1, fp)) {
			if (strcmp(name,data.name)==0) {
				*buf = data;
				found = 1;
				break;
			};

		}
	}
	fclose(fp);

	return found;
}
