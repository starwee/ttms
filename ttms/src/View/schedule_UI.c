#include "schedule_UI.h"
#include "Play_UI.h"

#include "../Common/list.h"
#include "../Service/schedule_Ser.h"
#include "../Service/Play_Ser.h"
#include "../Service/Studio.h"
#include "../Persistence/EntityKey_Persist.h"
#include "../Persistence/schedule_Persis.h"

#include "../View/Studio_UI.h"
#include "../Common/common.h"

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

static const char SCHEDULE_KEY_NAME[] = "Schedule";

static const int SCHEDULE_PAGE_SIZE = 5;

void Schedule_UI_MgtEnt(){
		int i, id;
	char choice;

	schedule_list_t head;
	schedule_node_t *pos;
	Pagination_t paging;

	List_Init(head, schedule_node_t);
	paging.offset = 0;
	paging.pageSize = SCHEDULE_PAGE_SIZE;

	//载入数据
	paging.totalRecords = Schedule_Srv_FetchAll(head);
	Paging_Locate_FirstPage(head, paging);
play_t play;
	        studio_t studio;
	do { 
	system("cls");
		printf(
				"\n\t\t\t===========================================================\n");
		printf(
				"\t\t\t************************ 演出列表 *************************\n");
				
		printf("\n\t\t\t%3s    %6s      %7s     %6s      %5s\n", "ID", "电影", "演出厅",
			"日期","时间"	);
			
		printf(
				"\n\t\t\t===========================================================\n");
		//显示数据
		Paging_ViewPage_ForEach(head, paging, schedule_node_t, pos, i){
			
	        Play_Srv_FetchByID(pos->data.play_id,&play);
	        Studio_Srv_FetchByID(pos->data.studio_id,&studio);
			printf("\n\t\t\t%3d    %10s  %6s      %4d-%2d-%2d  %2d:%2d:%2d\n", pos->data.id,
					play.name, studio.name,pos->data.date.year,
					pos->data.date.month,pos->data.date.day,
					pos->data.time.hour,pos->data.time.minute,pos->data.time.second);
	        printf(
				"\t\t\t-----------------------------------------------------------\n");
		}
            
		printf(
				"\n\t\t\t---------- 总:%2d ------------------ 页 %2d/%2d --------------\n",
				paging.totalRecords, Pageing_CurPage(paging),
				Pageing_TotalPages(paging));
		printf(
				"\n\t\t\t===========================================================\n");
		printf(
				"\n\t\t\t[P]上一页|[N]下一页|[A]添加|[D]删除|[U]修改|[R]返回");
		printf(
				"\n\t\t\t===========================================================\n");
		printf("\n\t\t\t\t\t选择:");
		fflush(stdin);
		scanf("%c", &choice);
		fflush(stdin);

		switch (choice) {
		case 'a':
		case 'A':
                printf("\n\t\t\t\t\t输入电影ID:");
			scanf("%d", &id);
			play_t play;
			if(!Play_Srv_FetchByID(id,&play) ) {
				printf("\n\t\t\t\t\t未找到该电影！\n");
				system("pause");
				continue; 
			}
			if (Schedule_UI_Add(id)) 
			{
				paging.totalRecords = Schedule_Srv_FetchAll(head);
				Paging_Locate_LastPage(head, paging, schedule_node_t);
			}
			break;
		case 'd':
		case 'D':
			printf("\n\t\t\t\t\t输入ID:");
			scanf("%d", &id);
			if (Schedule_UI_Delete(id)) {	
				paging.totalRecords = Schedule_Srv_FetchAll(head);
				List_Paging(head, paging, schedule_node_t);
			}
			break;
		case 'u':
		case 'U':
			printf("\n\t\t\t\t\t输入ID:");
			scanf("%d", &id);
			if (Schedule_UI_Modify(id)) {	
				paging.totalRecords = Schedule_Srv_FetchAll(head);
				List_Paging(head, paging, schedule_node_t);
			}
			break;
		case 'p':
		case 'P':
			if (!Pageing_IsFirstPage(paging)) {
				Paging_Locate_OffsetPage(head, paging, -1, schedule_node_t);
			}
			break;
		case 'n':
		case 'N':
			if (!Pageing_IsLastPage(paging)) {
				Paging_Locate_OffsetPage(head, paging, 1, schedule_node_t);
			}
			break;
		}
	} while (choice != 'r' && choice != 'R');
	
	List_Destroy(head, schedule_node_t);
}


int Schedule_UI_Add(int play_id)
{
	schedule_t rec;
	int newRecCount = 0;
	char choice;
	rec.play_id=play_id;
    studio_t studio; 
	do {
		system("cls");
		printf(
				"\n\t\t\t===========================================================\n");
		printf("\t\t\t***********************  安排新演出  **********************\n");
		printf(
				"\n\t\t\t===========================================================\n");
		printf("\n\t\t\t\t\t演出厅ID:");
		fflush(stdin);
		scanf("%d",&(rec.studio_id));
		while(Studio_Srv_FetchByID(rec.studio_id, &studio)==0){
		printf("\n\n\t\t\t\t\t演出厅不存在！！！\n");
		printf("\n\t\t\t\t\t[C]重新输入|[R]退出\n"); 
		
		int c;
		printf("\n\t\t\t\t\t选择："); fflush(stdin);
		scanf("%c",&c);
		if(c=='R'||c=='r') return;
		system("cls");
		printf(
				"\n\t\t\t===========================================================\n");
		printf("\t\t\t***********************  安排新演出  **********************\n");
		printf(
				"\n\t\t\t===========================================================\n");
		printf("\n\t\t\t\t\t演出厅：");
		fflush(stdin); 
		scanf("%d",&rec.studio_id);
	}
		printf("\n\t\t\t\t\t日期：(年月日用空格隔开)");
		fflush(stdin);
		scanf("%d%d%d", &(rec.date.year),&(rec.date.month),&(rec.date.day));
		printf("\n\t\t\t\t\t时间:(时分用空格隔开)");
		scanf("%d%d", &(rec.time.hour),&(rec.time.minute));
		rec.time.second=0;
		printf(
				"\n\t\t\t===========================================================\n");

		if (Schedule_Srv_Add(&rec)) {
			newRecCount += 1;
			printf("\n\t\t\t\t\t新演出安排成功!\n");
		} else
			printf("\n\t\t\t\t\t新演出安排失败!\n");
		printf(
				"\n\t\t\t===========================================================\n");
		printf("\n\t\t\t\t\t[A]继续添加|[R]返回:");
		fflush(stdin);
		scanf("%c", &choice);
	} while ('a' == choice || 'A' == choice);
	return newRecCount;
}

int Schedule_UI_Delete(int id) {

	int rtn = 0;

	if (Schedule_Srv_DeleteByID(id)) {
			printf(
				"\n\t\t\t===========================================================\n");
		 
		if (Ticket_Srv_DeleteAllByScheduleID(id))
			printf("\n\t\t\t\t\t相关票务删除成功!\n");
		printf(
				"\n\t\t\t\t\t演出删除成功!\n\n\t\t\t\t\t按 [Enter] 返回!\n");
		rtn = 1;
	} else {
		printf("\n\t\t\t\t\t该演出计划尚未退出!\n\n\t\t\t\t\t按 [Enter] 返回!\n");
	}
system("pause");
	getchar();
	return rtn;
}

int Schedule_UI_Modify(int id) {
	schedule_t rec;
	rec.id=id;
	int rtn = 0;

	
    system("cls");
	printf(
				"\n\t\t\t===========================================================\n");
	printf("\t\t\t**********************  修改演出安排  **********************");
	printf(
				"\n\t\t\t===========================================================\n");
	printf("\n\t\t\t\t\t电影ID:");
	scanf("%d",&(rec.play_id));
	printf("\n\t\t\t\t\t演出厅ID:", &(rec.studio_id));
	fflush(stdin);
	scanf("%d",&(rec.studio_id));
	printf("\n\t\t\t\t\t日期：(年月日用空格隔开)");
	scanf("%d%d%d", &(rec.date.year),&(rec.date.month),&(rec.date.day));
	printf("\n\t\t\t\t\t时间:(时分用空格隔开)");
	scanf("%d%d", &(rec.time.hour),&(rec.time.minute));
	rec.time.second=0;
	printf(
				"\n\t\t\t===========================================================\n");

	if (Schedule_Srv_Modify(&rec)) {
		rtn = 1;
		printf(
				"\n\t\t\t\t\t演出更新成功!\n\n\t\t\t\t\t按 [Enter] 返回!\n");
	} else
		printf("\n\t\t\t\t\t演出更新失败!\n\n\t\t\t\t\t按 [Enter] 返回!\n");
    system("pause");
	getchar();
	return rtn;
}
