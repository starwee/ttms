#include"../Service/schedule_Ser.h"
#include"../Service/Ticket_Ser.h"
#include"../Service/Seat.h"
#include"../Service/Play_Ser.h"
#include"../Service/Studio.h"
#include"../Service/Account_Ser.h"
#include"Ticket_UI.h"
#include "../Common/common.h"
#include "../Common/List.h"
#include<stdio.h>



static const int PLAY_PAGE_SIZE=5;
static const int SCHEDULE_PAGE_SIZE=5;
static const int TICKET_PAGE_SIZE = 5;

void Ticket_UI_MgtEntry(int userID,int userstatus){
	Play_UI(userID,userstatus) ;
}

void Play_UI(int userID,int userstatus)
{
	int i, id;
	char choice;

	play_list_t head;
	play_node_t *pos;
	Pagination_t paging;

	List_Init(head, play_node_t);
	paging.offset = 0;
	paging.pageSize = PLAY_PAGE_SIZE;

	//载入数据
	paging.totalRecords = Play_Srv_FetchAll(head);
	Paging_Locate_FirstPage(head, paging);

	do {
		system("cls");
			printf(
				"\n\t\t\t===========================================================\n");
		printf(
				"\t\t\t************************ 剧目清单 *************************\n");
		printf("\t\t\t\t%5s  %10s  %5s  %10s\n", "ID", "影名", "价格","票房");
			printf(
				"\t\t\t===========================================================\n");
		//显示数据
		Paging_ViewPage_ForEach(head, paging, play_node_t, pos, i){
			printf("\n\t\t\t\t%5d  %10s  %5d  %10d\n", pos->data.id,
					pos->data.name, pos->data.price,pos->data.sale);
		printf(
				"\t\t\t-----------------------------------------------------------\n");
		}

		printf(
				"\n\t\t\t------------ 共:%2d ------------------ 页 %2d/%2d ----------\n",
				paging.totalRecords, Pageing_CurPage(paging),
				Pageing_TotalPages(paging));
		printf(
				"\n\t\t\t===========================================================\n");
		printf(
				"\n\t\t\t\t[P]上一页|[N]下一页|[Q]查询演出|[R]返回");
		printf(
				"\n\t\t\t===========================================================\n");
		printf("\n\t\t\t\t\t选择:");
		fflush(stdin);
		scanf("%c", &choice);
		fflush(stdin);

		switch (choice) {
		case 'p':
		case 'P':
			if (!Pageing_IsFirstPage(paging)) {
				Paging_Locate_OffsetPage(head, paging, -1, play_node_t);
			}
			break;
		case 'n':
		case 'N':
			if (!Pageing_IsLastPage(paging)) {
				Paging_Locate_OffsetPage(head, paging, 1, play_node_t);
			}
			break;
	    case 'Q':
		case 'q':
			Schedule_UI(userID,userstatus);
		
		 break;
		}
	} while (choice != 'r' && choice != 'R');
	
	List_Destroy(head, play_node_t);
}

void Schedule_UI(int userID,int userstatus)
{
	int i, id;
	char choice;

	schedule_list_t head;
	schedule_node_t *pos;
	Pagination_t paging;

	List_Init(head, schedule_node_t);
	paging.offset = 0;
	paging.pageSize = SCHEDULE_PAGE_SIZE;
    char playname[20];
    printf("\n\t\t\t\t\t输入电影名：");
	fflush(stdin);
	gets(playname);
	//载入数据
	paging.totalRecords = Schedule_Srv_FetchAllByPlayName(head,playname);
	Paging_Locate_FirstPage(head, paging);
play_t play;
	        studio_t studio;
	do { 
	system("cls");
		printf(
				"\n\t\t\t===========================================================\n");
		printf(
				"\t\t\t************************  搜索演出  ***********************\n");
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
				"\n\t\t\t-----------------------------------------------------------\n");
}
		printf(
				"\n\t\t\t------------- 共:%2d ------------------ 页 %2d/%2d ---------\n",
				paging.totalRecords, Pageing_CurPage(paging),
				Pageing_TotalPages(paging));
				printf(
				"\n\t\t\t===========================================================\n");
		printf(
				"\n\t\t\t\t[P]上一页|[N]下一页|[S]选择演出[R]返回\n");
		printf("\n\t\t\t\t\t选择:");
		fflush(stdin);
		scanf("%c", &choice);
		fflush(stdin);

		switch (choice) {
		case 'S':
		case 's':
			
			Sale_UI(userID,userstatus);
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

void Sale_UI(int userID,int userstatus)
{
	int i, id;
	char choice;

	ticket_list_t head;
	ticket_node_t *pos;
	Pagination_t paging;

	List_Init(head, ticket_node_t);
	paging.offset = 0;
	paging.pageSize = TICKET_PAGE_SIZE;
    int scheduleID;
    printf("\n\t\t\t\t\t演出ID：");
	scanf("%d",&scheduleID); 
	//载入数据
	paging.totalRecords = Ticket_Srv_FetchByScheduleID(head,scheduleID);
	Paging_Locate_FirstPage(head, paging);

	do {
		system("cls");
		printf(
				"\n\t\t\t===========================================================\n");
		printf(
				"\t\t\t*************************** 票目 **************************\n");
		printf(
				"\n\t\t\t===========================================================\n");
		//显示数据
		Paging_ViewPage_ForEach(head, paging, ticket_node_t, pos, i){
			studio_t studio;
			schedule_t schedule;
			Schedule_Srv_FetchByID(scheduleID,&schedule);
			play_t play;
			Play_Srv_FetchByID(schedule.play_id, &play);
			Studio_Srv_FetchByID(schedule.studio_id, &studio);
			seat_t seat;
			Seat_Srv_FetchByID(pos->data.seat_id,&seat);
			printf(
				"\t\t\t-----------------------------------------------------------\n");
			printf("\t\t\t\t%5d       %10s       %10s\n\n",pos->data.id,play.name,studio.name);
			printf("\t\t\t\t%5d行%5d座    %d元     ",seat.row,seat.column,play.price);
			if(pos->data.user_id==0) printf("待售\n\n");
			else printf("已售\n\n"); 
			printf("\t\t\t\t     %4d-%2d-%2d          %2d:%2d:%2d\n\n",schedule.date.year,schedule.date.month,schedule.date.day,
			schedule.time.hour,schedule.time.minute,schedule.time.second);
			printf(
				"\t\t\t-----------------------------------------------------------\n");
		}

		printf(
				"\n\t\t\t------------ 共:%2d ------------------ 页 %2d/%2d ------------\n",
				paging.totalRecords, Pageing_CurPage(paging),
				Pageing_TotalPages(paging));
		printf(
				"\n\t\t\t===========================================================\n");
		printf(
				"\n\t\t\t\t[P]上一页|[N]下一页|");
				if(userstatus==2) printf("[S]售票|[R]返回");
				if(userstatus==3) printf("[S]购票|[R]返回");
		printf(
				"\n\t\t\t===========================================================\n");
		printf("\n\t\t\t\t\t选择:");
		fflush(stdin);
		scanf("%c", &choice);
		fflush(stdin);

		switch (choice) {
		case 'p':
		case 'P':
			if (!Pageing_IsFirstPage(paging)) {
				Paging_Locate_OffsetPage(head, paging, -1, ticket_node_t);
			}
			break;
		case 'n':
		case 'N':
			if (!Pageing_IsLastPage(paging)) {
				Paging_Locate_OffsetPage(head, paging, 1, ticket_node_t);
			}
			break;
	    case 'S':
		case 's':
			Sale_ticket(userstatus,userID);
		paging.totalRecords = Ticket_Srv_FetchByScheduleID(head,scheduleID);
	Paging_Locate_FirstPage(head, paging);
		 break;
		}
	} while (choice != 'r' && choice != 'R');
	List_Destroy(head, ticket_node_t);
}

void Sale_ticket(int userstatus,int user_id)
{
	int id;
	char choice='0';
	do{
		system("cls");
		printf("\n\t\t\t\t\t输入票ID：");
	 scanf("%d",&id);
	 ticket_t data;
	 if(Ticket_Srv_FetchByID(id, &data)){
	 	if(data.user_id!=0){
	 	printf("\n\t\t\t\t\t该票已售出!\n"); 
	 }
	 else{
	 	int userID;
	 	if(userstatus==2){
	 		printf("\n\t\t\t\t\t输入购票者ID：");
		 scanf("%d",&userID);
		 }
	 	else{
	 		userID=user_id;
		 }
		 account_t account;
		 if(Account_Srv_FetchByID(userID,&account)){
		 	data.user_id=userID;
		 	schedule_t schedule;
		 	Schedule_Srv_FetchByID(data.schedule_id, &schedule);
		 	play_t play;
		 	Play_Srv_FetchByID(schedule.play_id,&play);
		 	play.sale+=play.price;
		 	Play_Srv_Modify(&play);
		 	Ticket_Srv_Modify(&data);
		 	if(userstatus==2)printf("\n\t\t\t\t\t售票成功!\n");
			 else printf("\n\t\t\t\t\t购票成功！"); 
		 }
		 else{
		 	printf("\n\t\t\t\t\t查无此人！\n");
		 }
	 }
	 }
	 else printf("\n\t\t\t\t\t该票未找到!\n");
	 if(userstatus==2)printf("\n\t\t\t\t\t[C]继续售票|[R]返回\n") ;
	 else printf("\n\t\t\t\t\t[C]继续购票|[R]返回\n") ;
	 fflush(stdin);
	 scanf("%c",&choice);
	 while(choice!='C'&&choice!='c'&&choice!='R'&&choice!='r'){
	 	printf("\n\t\t\t\t\t输入错误!\n"); 
	 	if(userstatus==2)printf("\n\t\t\t\t\t[C]继续售票|[R]返回\n") ;
	 else printf("\n\t\t\t\t\t[C]继续购票|[R]返回\n") ;
	 fflush(stdin);
	 scanf("%c",&choice);
	 }
	  
	}while(choice=='C'||choice=='c');
	 
}

 void Sale_UI_ReturnTicket()
{
	int id;
	char choice='0'; 
	do{
		system("cls");
		printf("\n\t\t\t\t\t输入票id：");
	scanf("%d",&id);
	ticket_t data;
	if(Ticket_Srv_FetchByID(id,&data)){
		if(data.user_id==0){
		printf("\n\t\t\t\t\t该票未售出！\n");
	}
	else{
		data.user_id=0;
		Ticket_Srv_Modify(&data);
		play_t play;
		schedule_t schedule;
		Schedule_Srv_FetchByID(data.schedule_id,&schedule);
		Play_Srv_FetchByID(schedule.play_id, &play);
		play.sale-=play.price;
		Play_Srv_Modify(&play);
		printf("\n\t\t\t\t\t退票成功！\n");
	}
	}
	else printf("\n\t\t\t\t\t未找到该票！\n");
	printf("\n\t\t\t\t\t[C]继续退票|[R]返回\n");
	fflush(stdin);
	scanf("%c",&choice);
	 while(choice!='C'&&choice!='c'&&choice!='R'&&choice!='r'){
	 	printf("\n\t\t\t\t\t输入错误!\n"); 
	 	printf("\n\t\t\t\t\t[C]继续退票|[R]返回\n") ;
	 fflush(stdin);
	 scanf("%c",&choice);
	 }
	}while(choice=='C'||choice=='c');	
}

void qticket(int userID,int c)
{
	
	int i, id;
	char choice;

	ticket_list_t head;
	ticket_node_t *pos;
	Pagination_t paging;

	List_Init(head, ticket_node_t);
	paging.offset = 0;
	paging.pageSize = TICKET_PAGE_SIZE;
	//载入数据
	paging.totalRecords = Ticket_Srv_FetchByUserID(head,userID);
	Paging_Locate_FirstPage(head, paging);

	do {
		system("cls");
		printf(
				"\n\t\t\t===========================================================\n");
		printf(
				"\t\t\t************************* 您的订单 ************************\n");
		printf(
				"\n\t\t\t===========================================================\n");
		//显示数据
		Paging_ViewPage_ForEach(head, paging, ticket_node_t, pos, i){
			studio_t studio;
			schedule_t schedule;
			Schedule_Srv_FetchByID(pos->data.schedule_id,&schedule);
			play_t play;
			Play_Srv_FetchByID(schedule.play_id, &play);
			Studio_Srv_FetchByID(schedule.studio_id, &studio);
			seat_t seat;
			Seat_Srv_FetchByID(pos->data.seat_id,&seat);
			printf(
				"\t\t\t-----------------------------------------------------------\n");
			printf("\t\t\t\t%5d       %10s       %10s\n\n",pos->data.id,play.name,studio.name);
			printf("\t\t\t\t%5d行%5d座    %d元     \n\n",seat.row,seat.column,play.price);
			printf("\t\t\t\t     %4d-%2d-%2d          %2d:%2d:%2d\n\n",schedule.date.year,schedule.date.month,schedule.date.day,
			schedule.time.hour,schedule.time.minute,schedule.time.second);
			printf(
				"\t\t\t-----------------------------------------------------------\n");
		}

		printf(
				"\n\t\t\t------------ 共:%2d ------------------ 页 %2d/%2d ------------\n",
				paging.totalRecords, Pageing_CurPage(paging),
				Pageing_TotalPages(paging));
		printf(
				"\n\t\t\t===========================================================\n");
		printf(
				"\n\t\t\t\t[P]上一页|[N]下一页");
				if(c==1) printf("|[s]退票");
				printf("|[R]返回\n");
				
		printf(
				"\n\t\t\t===========================================================\n");
		printf("\n\t\t\t\t\t选择:");
		fflush(stdin);
		scanf("%c", &choice);
		fflush(stdin);

		switch (choice) {
		case 'p':
		case 'P':
			if (!Pageing_IsFirstPage(paging)) {
				Paging_Locate_OffsetPage(head, paging, -1, ticket_node_t);
			}
			break;
		case 'n':
		case 'N':
			if (!Pageing_IsLastPage(paging)) {
				Paging_Locate_OffsetPage(head, paging, 1, ticket_node_t);
			}
			break;
	    case 'S':
		case 's':
			if(c==1){
				Sale_UI_ReturnTicket(); 
		paging.totalRecords = Ticket_Srv_FetchByUserID(head,userID);
	Paging_Locate_FirstPage(head, paging);
			}
			
		 break;
		}
	} while (choice != 'r' && choice != 'R');
	
	List_Destroy(head, ticket_node_t);
}
