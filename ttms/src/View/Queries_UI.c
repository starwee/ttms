#include "../Service/schedule_Ser.h"
#include "../Service/Play_Ser.h"
#include "../Common/Common.h"
#include "../Common/List.h"
#include "../Service/Studio.h"
#include <stdio.h>
#include "Queries_UI.h"

static const int PLAY_PAGE_SIZE=5;
static const int SCHEDULE_PAGE_SIZE=5;


void Queries_Menu()
{
	int i, id;
	char choice;

	play_list_t head;
	play_node_t *pos;
	Pagination_t paging;

	List_Init(head, play_node_t);
	paging.offset = 0;
	paging.pageSize = PLAY_PAGE_SIZE;

	//��������
	paging.totalRecords = Play_Srv_FetchAll(head);
	Paging_Locate_FirstPage(head, paging);

	do {
		system("cls"); 
		printf(
			"\n\t\t\t===========================================================\n");
		printf("\t\t\t************************ ��ѯ��Ӱ *************************\n");		
		printf("\n\t\t\t\t%5s  %10s  %5s  %10s", "ID", "Ӱ��", "�۸�","Ʊ��");
		printf(
			"\n\t\t\t===========================================================\n");
		//��ʾ����
		Paging_ViewPage_ForEach(head, paging, play_node_t, pos, i){
			printf("\n\t\t\t\t%5d  %10s  %5d  %10d", pos->data.id,
					pos->data.name, pos->data.price,pos->data.sale);
					printf(
			"\n\t\t\t-----------------------------------------------------------\n");
		}

		printf(
				"\n\t\t\t------------ ��:%2d ----------------- ҳ %2d/%2d ---------\n",
				paging.totalRecords, Pageing_CurPage(paging),
				Pageing_TotalPages(paging));
		printf(
			"\n\t\t\t===========================================================\n");
		printf(
				"\n\t\t\t\t[P]��һҳ|[N]��һҳ|[Q]����|[R]����");
		printf(
			"\n\t\t\t===========================================================\n");
		printf("\n\t\t\t\t\tѡ��:");
		fflush(stdin);
		scanf("%c",&choice); 
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
			Schedule_UI_q();
		
		 break;
		}
	} while (choice != 'r' && choice != 'R');
	//�ͷ�����ռ�
	List_Destroy(head, play_node_t);
}

void Schedule_UI_q()
{
	int i, id;
	char choice;

	schedule_list_t head;
	schedule_node_t *pos;
	Pagination_t paging;

	List_Init(head, schedule_node_t);
	paging.offset = 0;
	paging.pageSize = SCHEDULE_PAGE_SIZE;
    char name[20]; 
    printf("\n\t\t\t\t\t�����Ӱ����");
	fflush(stdin);
	gets(name);
	//��������
	
	paging.totalRecords = Schedule_Srv_FetchAllByPlayName(head,name);
	Paging_Locate_FirstPage(head, paging);
play_t play;
	        studio_t studio;
	do { 
	system("cls"); 
		printf(
			"\n\t\t\t===========================================================\n");
		printf(
				"\t\t\t************************ �����ݳ� *************************\n");
		printf("\n\t\t\t%5s  %8s    %10s  %8s    %10s \n", "ID", "��Ӱ", "�ݳ���",
			"����","ʱ��"	);
		printf(
			"\n\t\t\t===========================================================\n");
		//��ʾ����
		Paging_ViewPage_ForEach(head, paging, schedule_node_t, pos, i){
			
	        Play_Srv_FetchByID(pos->data.play_id,&play);
	        Studio_Srv_FetchByID(pos->data.studio_id,&studio);
			printf("\t\t\t%5d    %8s %8s     %4d-%2d-%2d     %2d:%2d:%2d  \n", pos->data.id,
					play.name, studio.name,pos->data.date.year,
					pos->data.date.month,pos->data.date.day,
					pos->data.time.hour,pos->data.time.minute,pos->data.time.second);
					printf(
			"\t\t\t----------------------------------------------------------\n");
		}

		printf(
				"\n\t\t\t------------ ��:%2d ------------------ ҳ %2d/%2d ---------\n",
				paging.totalRecords, Pageing_CurPage(paging),
				Pageing_TotalPages(paging));
		printf(
			"\n\t\t\t===========================================================\n");
		printf(
				"\n\t\t\t\t\t[P]��һҳ|[N]��һҳ|[R]����");
		printf(
			"\n\t\t\t===========================================================\n");
		printf("\n\t\t\t\t\tѡ��:");
		fflush(stdin);
		scanf("%c", &choice);
		fflush(stdin);

		switch (choice) {
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
