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

	//��������
	paging.totalRecords = Schedule_Srv_FetchAll(head);
	Paging_Locate_FirstPage(head, paging);
play_t play;
	        studio_t studio;
	do { 
	system("cls");
		printf(
				"\n\t\t\t===========================================================\n");
		printf(
				"\t\t\t************************ �ݳ��б� *************************\n");
				
		printf("\n\t\t\t%3s    %6s      %7s     %6s      %5s\n", "ID", "��Ӱ", "�ݳ���",
			"����","ʱ��"	);
			
		printf(
				"\n\t\t\t===========================================================\n");
		//��ʾ����
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
				"\n\t\t\t---------- ��:%2d ------------------ ҳ %2d/%2d --------------\n",
				paging.totalRecords, Pageing_CurPage(paging),
				Pageing_TotalPages(paging));
		printf(
				"\n\t\t\t===========================================================\n");
		printf(
				"\n\t\t\t[P]��һҳ|[N]��һҳ|[A]���|[D]ɾ��|[U]�޸�|[R]����");
		printf(
				"\n\t\t\t===========================================================\n");
		printf("\n\t\t\t\t\tѡ��:");
		fflush(stdin);
		scanf("%c", &choice);
		fflush(stdin);

		switch (choice) {
		case 'a':
		case 'A':
                printf("\n\t\t\t\t\t�����ӰID:");
			scanf("%d", &id);
			play_t play;
			if(!Play_Srv_FetchByID(id,&play) ) {
				printf("\n\t\t\t\t\tδ�ҵ��õ�Ӱ��\n");
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
			printf("\n\t\t\t\t\t����ID:");
			scanf("%d", &id);
			if (Schedule_UI_Delete(id)) {	
				paging.totalRecords = Schedule_Srv_FetchAll(head);
				List_Paging(head, paging, schedule_node_t);
			}
			break;
		case 'u':
		case 'U':
			printf("\n\t\t\t\t\t����ID:");
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
		printf("\t\t\t***********************  �������ݳ�  **********************\n");
		printf(
				"\n\t\t\t===========================================================\n");
		printf("\n\t\t\t\t\t�ݳ���ID:");
		fflush(stdin);
		scanf("%d",&(rec.studio_id));
		while(Studio_Srv_FetchByID(rec.studio_id, &studio)==0){
		printf("\n\n\t\t\t\t\t�ݳ��������ڣ�����\n");
		printf("\n\t\t\t\t\t[C]��������|[R]�˳�\n"); 
		
		int c;
		printf("\n\t\t\t\t\tѡ��"); fflush(stdin);
		scanf("%c",&c);
		if(c=='R'||c=='r') return;
		system("cls");
		printf(
				"\n\t\t\t===========================================================\n");
		printf("\t\t\t***********************  �������ݳ�  **********************\n");
		printf(
				"\n\t\t\t===========================================================\n");
		printf("\n\t\t\t\t\t�ݳ�����");
		fflush(stdin); 
		scanf("%d",&rec.studio_id);
	}
		printf("\n\t\t\t\t\t���ڣ�(�������ÿո����)");
		fflush(stdin);
		scanf("%d%d%d", &(rec.date.year),&(rec.date.month),&(rec.date.day));
		printf("\n\t\t\t\t\tʱ��:(ʱ���ÿո����)");
		scanf("%d%d", &(rec.time.hour),&(rec.time.minute));
		rec.time.second=0;
		printf(
				"\n\t\t\t===========================================================\n");

		if (Schedule_Srv_Add(&rec)) {
			newRecCount += 1;
			printf("\n\t\t\t\t\t���ݳ����ųɹ�!\n");
		} else
			printf("\n\t\t\t\t\t���ݳ�����ʧ��!\n");
		printf(
				"\n\t\t\t===========================================================\n");
		printf("\n\t\t\t\t\t[A]�������|[R]����:");
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
			printf("\n\t\t\t\t\t���Ʊ��ɾ���ɹ�!\n");
		printf(
				"\n\t\t\t\t\t�ݳ�ɾ���ɹ�!\n\n\t\t\t\t\t�� [Enter] ����!\n");
		rtn = 1;
	} else {
		printf("\n\t\t\t\t\t���ݳ��ƻ���δ�˳�!\n\n\t\t\t\t\t�� [Enter] ����!\n");
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
	printf("\t\t\t**********************  �޸��ݳ�����  **********************");
	printf(
				"\n\t\t\t===========================================================\n");
	printf("\n\t\t\t\t\t��ӰID:");
	scanf("%d",&(rec.play_id));
	printf("\n\t\t\t\t\t�ݳ���ID:", &(rec.studio_id));
	fflush(stdin);
	scanf("%d",&(rec.studio_id));
	printf("\n\t\t\t\t\t���ڣ�(�������ÿո����)");
	scanf("%d%d%d", &(rec.date.year),&(rec.date.month),&(rec.date.day));
	printf("\n\t\t\t\t\tʱ��:(ʱ���ÿո����)");
	scanf("%d%d", &(rec.time.hour),&(rec.time.minute));
	rec.time.second=0;
	printf(
				"\n\t\t\t===========================================================\n");

	if (Schedule_Srv_Modify(&rec)) {
		rtn = 1;
		printf(
				"\n\t\t\t\t\t�ݳ����³ɹ�!\n\n\t\t\t\t\t�� [Enter] ����!\n");
	} else
		printf("\n\t\t\t\t\t�ݳ�����ʧ��!\n\n\t\t\t\t\t�� [Enter] ����!\n");
    system("pause");
	getchar();
	return rtn;
}
