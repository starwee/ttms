

#include "../View/Studio_UI.h"

#include "../Common/List.h"
#include "../Service/Studio.h"
#include "../Service/Seat.h"


static const int STUDIO_PAGE_SIZE = 5;

#include <stdio.h>


void Studio_UI_MgtEntry(void) {
	int i, id;
	char choice;

	studio_list_t head;
	studio_node_t *pos;
	Pagination_t paging;

	List_Init(head, studio_node_t);
	paging.offset = 0;
	paging.pageSize = STUDIO_PAGE_SIZE;

	//载入数据
	paging.totalRecords = Studio_Srv_FetchAll(head);
	Paging_Locate_FirstPage(head, paging);

	do {
		system("cls");
		printf(
				"\n\t\t\t===========================================================\n");
		printf(
				"\n\t\t\t\t\t\t 演出厅列表\n");
		printf("\n\t\t\t\t %5s  %5s  %5s  %5s  %5s\n", "ID", "名称", "行数",
				"列数", "座位数");
		printf(
				"\n\t\t\t-----------------------------------------------------------\n");
		Paging_ViewPage_ForEach(head, paging, studio_node_t, pos, i){
			
			printf("\t\t\t\t%5d  %5s  %5d  %5d  %5d\n", pos->data.id,
					pos->data.name, pos->data.rowsCount, pos->data.colsCount,
					pos->data.seatsCount);
					printf(
				"\t\t\t-----------------------------------------------------------\n");
		}

		printf(
				"\n\n\t\t\t\t 共:%2d ------------- 页 %2d/%2d ------------\n",
				paging.totalRecords, Pageing_CurPage(paging),
				Pageing_TotalPages(paging));
		printf(
				"\n\t\t\t===========================================================\n");
		printf(
				"\n\t\t\t[P]上一页|[N]下一页|[A]添加|[D]删除|[U]修改|[S]座位|[R]返回");
		printf(
				"\n\t\t\t===========================================================\n");
		printf("\n\t\t\t\t\t选择:");
		fflush(stdin);
		scanf("%c", &choice);
		fflush(stdin);

		switch (choice) {
		case 'a':
		case 'A':
			if (Studio_UI_Add()) 
			{
				paging.totalRecords = Studio_Srv_FetchAll(head);
				Paging_Locate_LastPage(head, paging, studio_node_t);
			}
			break;
		case 'd':
		case 'D':
			printf("\n\t\t\t\t\t输入ID:");
			scanf("%d", &id);
			if (Studio_UI_Delete(id)) {	
				paging.totalRecords = Studio_Srv_FetchAll(head);
				List_Paging(head, paging, studio_node_t);
			}
			break;
		case 'u':
		case 'U':
			printf("\n\t\t\t\t\t输入ID:");
			scanf("%d", &id);
			if (Studio_UI_Modify(id)) {	
				paging.totalRecords = Studio_Srv_FetchAll(head);
				List_Paging(head, paging, studio_node_t);
			}
			break;
		case 's':
		case 'S':
			printf("\n\t\t\t\t\t输入ID:");
			scanf("%d", &id);
			Seat_UI_MgtEntry(id);
			paging.totalRecords = Studio_Srv_FetchAll(head);
			List_Paging(head, paging, studio_node_t)
			;
			break;
		case 'p':
		case 'P':
			if (!Pageing_IsFirstPage(paging)) {
				Paging_Locate_OffsetPage(head, paging, -1, studio_node_t);
			}
			break;
		case 'n':
		case 'N':
			if (!Pageing_IsLastPage(paging)) {
				Paging_Locate_OffsetPage(head, paging, 1, studio_node_t);
			}
			break;
		}
	} while (choice != 'r' && choice != 'R');
	
	List_Destroy(head, studio_node_t);
}


int Studio_UI_Add(void) {
	studio_t rec;
	int newRecCount = 0;
	char choice;

	do {
		system("cls"); 
		printf("\n\t\t\t===========================================================\n");
		printf("\t\t\t*********************  添加新演出厅  **********************");
		printf("\n\t\t\t===========================================================\n");
		printf("\n\t\t\t\t\t演出厅名称:");
		fflush(stdin);
		gets(rec.name);
		printf("\n\t\t\t\t\t座位行数:");
		scanf("%d", &(rec.rowsCount));
		printf("\n\t\t\t\t\t座位列数:");
		scanf("%d", &(rec.colsCount));
		rec.seatsCount =rec.rowsCount*rec.colsCount ;
		printf("\n\t\t\t===========================================================\n");

		if (Studio_Srv_Add(&rec)) {
			
			newRecCount += 1;
			printf("\n\t\t\t\t\t新演出厅添加成功!\n");
		} else
			printf("\n\t\t\t\t\t新演出厅添加失败!\n");
		printf("\n\t\t\t===========================================================\n");
		printf("\n\t\t\t\t\t[A]继续添加|[R]返回:");
		fflush(stdin);
		scanf("%c", &choice);
	} while ('a' == choice || 'A' == choice);
	return newRecCount;
}


int Studio_UI_Modify(int id) {
	studio_t rec;
	int rtn = 0;
	int newrow, newcolumn;
	seat_list_t list;
	int seatcount;
    
	
	if (!Studio_Srv_FetchByID(id, &rec)) {
		printf("\t\t\t\t\t该演出厅未退出!\n\t\t\t\t\t按 [Enter] 返回!\n");
		getchar();
		return 0;
	}
    system("cls");
	printf("\n\t\t\t===========================================================\n");
	printf("\t\t\t***********************  修改演出厅  **********************");
	printf("\n\t\t\t===========================================================\n");
	printf("\n\t\t\t\t\t演出厅ID:%d\n", rec.id);
	printf("\n\t\t\t\t\t演出厅名[%s]:", rec.name);
	fflush(stdin);
	gets(rec.name);

	List_Init(list, seat_node_t);
	seatcount = Seat_Srv_FetchByRoomID(list, rec.id);
	if (seatcount) {
	Seat_Srv_DeleteAllByRoomID(id);
	}
		printf("\n\t\t\t\t\t座位行数:");
		scanf("%d", &rec.rowsCount);
		printf("\n\t\t\t\t\t座位列数:");
		scanf("%d", &rec.colsCount);
		rec.seatsCount =rec.rowsCount*rec.colsCount ;
	

	printf("\n\t\t\t===========================================================\n");

	if (Studio_Srv_Modify(&rec)) {
		rtn = 1;
		printf(
				"\n\t\t\t\t\t演出厅修改成功!\n\n\t\t\t\t\t按 [Enter] 返回!\n");
	} else
		printf("\t\t\t\t\t演出厅修改失败!\n\n\t\t\t\t\t按 [Enter] 返回!\n");
		printf("\n\t\t\t===========================================================\n");
system("pause");
	getchar();
	return rtn;
}


int Studio_UI_Delete(int id) {

	int rtn = 0;
    system("cls");
	if (Studio_Srv_DeleteByID(id)) {
	Ticket_Srv_DeleteAllByStudioID(id);
		Schedule_Srv_DeleteAllByStudioID(id);
		
		 
		if (Seat_Srv_DeleteAllByRoomID(id))
			printf("\n\t\t\t\t\t演出厅座位删除成功!\n");
		printf(
				"\n\t\t\t\t\t演出厅删除成功!\n\n\t\t\t\t\t按 [Enter] 返回!\n");
		rtn = 1;
	} else {
		printf("\n\t\t\t\t\t该演出厅未退出!\n\n\t\t\t\t\t按 [Enter] 返回!\n");
	}
    system("pause");
	getchar();
	return rtn;
}

