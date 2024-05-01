#include "../View/Play_UI.h"

#include "../Common/List.h"
#include "../Service/Play_Ser.h"


static const int PLAY_PAGE_SIZE = 5;

#include <stdio.h>

void Play_UI_MgtEntry(void) {
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
				"\t\t\t************************ 剧目列表 *************************\n");
		printf("\n\t\t\t%5s  %10s  %5s  %10s\n", "ID", "影名", "价格","票房");
		printf(
				"\n\t\t\t===========================================================\n");
		//显示数据
		Paging_ViewPage_ForEach(head, paging, play_node_t, pos, i){
			printf("\n\t\t\t%5d  %10s  %5d  %10d\n", pos->data.id,
					pos->data.name, pos->data.price,pos->data.sale);
		printf(
				"\n\t\t\t-----------------------------------------------------------\n");		
		}

		printf(
				"\n\t\t\t------- 共:%2d ----------------------- 页 %2d/%2d ----\n",
				paging.totalRecords, Pageing_CurPage(paging),
				Pageing_TotalPages(paging));
		printf(
				"\n\t\t\t===========================================================\n");
		printf(
				"\n\t\t\t[P]上页|[N]下页|[A]添加|[D]删除|[U]修改|[S]安排演出|[R]返回");
		printf(
				"\n\t\t\t===========================================================\n");
		printf("\n\t\t\t\t\t选择:");
		fflush(stdin);
		scanf("%c", &choice);
		fflush(stdin);

		switch (choice) {
		case 'a':
		case 'A':
			if (Play_UI_Add()) 
			{
				paging.totalRecords = Play_Srv_FetchAll(head);
				Paging_Locate_LastPage(head, paging, play_node_t);
			}
			break;
		case 'd':
		case 'D':
			printf("\n\t\t\t\t\t输入ID:");
			scanf("%d", &id);
			if (Play_UI_Delete(id)) {	
				paging.totalRecords = Play_Srv_FetchAll(head);
				List_Paging(head, paging, play_node_t);
			}
			break;
		case 'u':
		case 'U':
			printf("\n\t\t\t\t\t输入ID:");
			scanf("%d", &id);
			if (Play_UI_Modify(id)) {	
				paging.totalRecords = Play_Srv_FetchAll(head);
				List_Paging(head, paging, play_node_t);
			}
			break;
			case 's':
			case 'S':
				Schedule_UI_MgtEnt();
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
		}
	} while (choice != 'r' && choice != 'R');
	
	List_Destroy(head, play_node_t);
}

int Play_UI_Add(void) {
	play_t rec;
	int newRecCount = 0;
	char choice;

	do {
		system("cls");
		printf(
				"\n\t\t\t===========================================================\n");
		printf("\t\t\t**********************  添加新电影  **********************");
		printf(
				"\n\t\t\t===========================================================\n");
		printf("\n\t\t\t\t\t影名：");
		fflush(stdin);
		printf("");
		gets(rec.name);
		printf("\n\t\t\t\t\t价格：");
		fflush(stdin);
		printf("");
		scanf("%d",&rec.price);
		rec.sale = 0;
		printf(
				"\n\t\t\t===========================================================\n");

		if (Play_Srv_Add(&rec)) {
			newRecCount += 1;
			printf("\n\t\t\t\t\t新电影添加成功!\n");
		} else
			printf("\n\t\t\t\t\t新电影添加失败!\n");
		printf(
				"\n\t\t\t===========================================================\n");
		printf("\n\t\t\t\t\t[A]继续添加|[R]返回\n");
		fflush(stdin);
		scanf("%c", &choice);
	} while ('a' == choice || 'A' == choice);
	return newRecCount;
}

int Play_UI_Modify(int id) {
	play_t rec;
	int rtn = 0;
	char newname[32];

	/*Load record*/
	if (!Play_Srv_FetchByID(id, &rec)) {
		printf("\n\t\t\t\t\t该电影尚未退出!\n\n\t\t\t\t\t按 [Enter] 返回!\n");
		getchar();
		return 0;
	}
    system("cls");
	printf(
			"\n\t\t\t===========================================================\n");
	printf("\t\t\t***********************  修改电影  ***********************\n");
	printf(
			"\n\t\t\t===========================================================\n");
	printf("\n\t\t\t\t\t电影ID:%d\n", rec.id);
	printf("\n\t\t\t\t\t电影名[%s]:", rec.name);
	fflush(stdin);
	gets(rec.name);

	printf(
				"\n\t\t\t===========================================================\n");

	if (Play_Srv_Modify(&rec)) {
		rtn = 1;
		printf(
				"\n\t\t\t\t\t电影数据更新成功!\n\n\t\t\t\t\t按 [Enter] 返回!\n");
	} else
		printf("\n\t\t\t\t\t电影数据更新失败!\n\n\t\t\t\t\t按 [Enter] 返回!\n");
	getchar();
	return rtn;
}

int Play_UI_Delete(int id) {

	int rtn = 0;

	if (Play_Srv_DeleteByID(id)) {
	    Ticket_Srv_DeleteAllByPlayID(id);
		Schedule_Srv_DeleteAllByPlayID(id);
		
	    system("cls");
		printf(
				"\n\t\t\t\t\t电影删除成功!\n\n\t\t\t\t\t按 [Enter]返回!\n");
		rtn = 1;
	} else {
		printf("\n\t\t\t\t\t电影删除失败!\n\n\t\t\t\t\t按 [Enter]返回!\n");
	}
	system("pause");
	getchar();
	return rtn;
}

