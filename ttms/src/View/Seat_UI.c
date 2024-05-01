/*
* Copyright(C), 2007-2008, XUPT Univ.	 
* 用例编号：TTMS_UC_02
* File name: Seat_UI.c			  
* Description : 设置座位用例界面层	
* Author:   XUPT  		 
* Version:  v.1 	 
* Date: 	2015年4月22日	
*/

#include "Seat_UI.h"
#include "../Service/Seat.h"
#include "../Service/Studio.h"
#include "../Persistence/Studio_Persist.h"
#include "../Common/List.h"
#include <stdio.h>


inline char Seat_UI_Status2Char(seat_status_t status) {
char statusChar;
	switch (status) {
	case SEAT_GOOD:		//有座位
		statusChar = '#';
		break;
	case SEAT_BROKEN:	//损坏的座位
		statusChar = 'x';
		break;
	case SEAT_NONE:
	default:
		statusChar = ' ';
		break;
	}
	return statusChar;
}


inline seat_status_t Seat_UI_Char2Status(char statusChar) {
	seat_status_t status;
	switch(statusChar){
		case '#':
			status=SEAT_GOOD;
			break;
		case '@':
			status=SEAT_BROKEN;
			break;
		case '.':
		default:
			SEAT_NONE;
			break;	
	}
	return status;
}


void Seat_UI_MgtEntry(int roomID)
 {
    int i, id;
	char choice;

	seat_list_t head;
	seat_node_t *pos,*p;

	List_Init(head, seat_node_t);

	Seat_Srv_FetchByRoomID(head,roomID);
	
	do {
		system("cls"); 
		printf(
				"\n\t\t\t===========================================================\n");
		printf(
				"\t\t\t***********************************************************");
		printf(
				"\n\t\t\t-----------------------------------------------------------\n\n\n\t\t\t ");
		//显示数据
		for(pos=head->next;pos!=head;pos=pos->next){
			if(pos!=head->next&&pos->prev->data.row!=pos->data.row) printf("\n\t\t\t ");
			printf("%c      ",Seat_UI_Status2Char(pos->data.status));
		}
		printf(
				"\n\t\t\t===========================================================\n");
		printf(
				"\n\t\t\t\t\t[U]修改座位状态|[R]返回");
		printf(
				"\n\t\t\t===========================================================\n");
		printf("\n\t\t\t\t\t选择:");
		fflush(stdin);
		scanf("%c", &choice);
		fflush(stdin);
        studio_t studio;
		Studio_Srv_FetchByID(roomID, &studio);
		switch (choice) {
		case 'u':
		case 'U':
			
			if (Seat_UI_Modify(head,studio.rowsCount,studio.colsCount)) {	//从新载入数据
				Seat_Srv_FetchByRoomID(head,roomID);
				
			}
			break;
		}
	} while (choice != 'r' && choice != 'R');
	//释放链表空间
	List_Destroy(head, seat_node_t);
}


int Seat_UI_Modify(seat_list_t list, int row, int column) {
	int rtn = 0;
	int newrow, newcolumn;
	char choice;
	seat_node_t *p;
do {
do {system("cls");
	printf("\n\t\t\t==================================================================\n");
	printf("\t\t\t***************************  更新座位  ***************************\n");
	printf("\t\t\t------------------------------------------------------------------\n");
	
						
			printf("\n\n\n\t\t\t\t\t行不可超 %d and 列不可超 %d!\n",
					row, column);
			printf("\n\t\t\t\t\t座位行号： ");
			scanf("%d", &newrow);
			printf("\n\t\t\t\t\t座位列号： ");
			scanf("%d", &newcolumn);
			while (getchar() != '\n')
				continue;
		} while (newrow > row || newcolumn > column);

		p = Seat_Srv_FindByRowCol(list, newrow, newcolumn);
		if (p) {
			printf("\n\t\t\t修改座位信息[%d,%d]:[%c]为('#': 座位可用; '@':座位损坏; 其他:座位不存在):", newrow, newcolumn,
					Seat_UI_Status2Char(p->data.status));
			choice = toupper(getchar());
			while (getchar() != '\n')
				continue;
				seat_status_t oldstatus=p->data.status; 
			p->data.status = Seat_UI_Char2Status(choice);
		printf("\n\t\t\t------------------------------------------------------------------\n");
			if (Seat_Srv_Modify(&(p->data))) {
				studio_t studio;
				Studio_Srv_FetchByID(p->data.roomID, &studio);
				if(oldstatus==SEAT_GOOD&&(p->data.status==SEAT_BROKEN||p->data.status==SEAT_NONE)){
					studio.seatsCount--;
					Studio_Perst_Update(&studio,0);
				}
				else if((oldstatus==SEAT_BROKEN||oldstatus==SEAT_NONE)&&p->data.status==SEAT_GOOD){
					studio.seatsCount++;
					Studio_Perst_Update(&studio,0);
				}
				rtn = 1;
				printf("\n\t\t\t座位更新成功\n");
			} else
				printf("\n\t\t\t座位更新失败\n");
		} else
			printf("\n\t\t\t座位不存在\n");
		printf("\t\t\t------------------------------------------------------------------\n");
		printf("\n\t\t\t[U]继续修改|[R]返回");
		printf("\n\n\t\t\t选择："); 
		choice = toupper(getchar());
		while (getchar() != '\n')
			continue;
	} while ('u' == choice || 'U' == choice);
	return rtn;

}

