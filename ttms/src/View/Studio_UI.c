

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

	//��������
	paging.totalRecords = Studio_Srv_FetchAll(head);
	Paging_Locate_FirstPage(head, paging);

	do {
		system("cls");
		printf(
				"\n\t\t\t===========================================================\n");
		printf(
				"\n\t\t\t\t\t\t �ݳ����б�\n");
		printf("\n\t\t\t\t %5s  %5s  %5s  %5s  %5s\n", "ID", "����", "����",
				"����", "��λ��");
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
				"\n\n\t\t\t\t ��:%2d ------------- ҳ %2d/%2d ------------\n",
				paging.totalRecords, Pageing_CurPage(paging),
				Pageing_TotalPages(paging));
		printf(
				"\n\t\t\t===========================================================\n");
		printf(
				"\n\t\t\t[P]��һҳ|[N]��һҳ|[A]���|[D]ɾ��|[U]�޸�|[S]��λ|[R]����");
		printf(
				"\n\t\t\t===========================================================\n");
		printf("\n\t\t\t\t\tѡ��:");
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
			printf("\n\t\t\t\t\t����ID:");
			scanf("%d", &id);
			if (Studio_UI_Delete(id)) {	
				paging.totalRecords = Studio_Srv_FetchAll(head);
				List_Paging(head, paging, studio_node_t);
			}
			break;
		case 'u':
		case 'U':
			printf("\n\t\t\t\t\t����ID:");
			scanf("%d", &id);
			if (Studio_UI_Modify(id)) {	
				paging.totalRecords = Studio_Srv_FetchAll(head);
				List_Paging(head, paging, studio_node_t);
			}
			break;
		case 's':
		case 'S':
			printf("\n\t\t\t\t\t����ID:");
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
		printf("\t\t\t*********************  ������ݳ���  **********************");
		printf("\n\t\t\t===========================================================\n");
		printf("\n\t\t\t\t\t�ݳ�������:");
		fflush(stdin);
		gets(rec.name);
		printf("\n\t\t\t\t\t��λ����:");
		scanf("%d", &(rec.rowsCount));
		printf("\n\t\t\t\t\t��λ����:");
		scanf("%d", &(rec.colsCount));
		rec.seatsCount =rec.rowsCount*rec.colsCount ;
		printf("\n\t\t\t===========================================================\n");

		if (Studio_Srv_Add(&rec)) {
			
			newRecCount += 1;
			printf("\n\t\t\t\t\t���ݳ�����ӳɹ�!\n");
		} else
			printf("\n\t\t\t\t\t���ݳ������ʧ��!\n");
		printf("\n\t\t\t===========================================================\n");
		printf("\n\t\t\t\t\t[A]�������|[R]����:");
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
		printf("\t\t\t\t\t���ݳ���δ�˳�!\n\t\t\t\t\t�� [Enter] ����!\n");
		getchar();
		return 0;
	}
    system("cls");
	printf("\n\t\t\t===========================================================\n");
	printf("\t\t\t***********************  �޸��ݳ���  **********************");
	printf("\n\t\t\t===========================================================\n");
	printf("\n\t\t\t\t\t�ݳ���ID:%d\n", rec.id);
	printf("\n\t\t\t\t\t�ݳ�����[%s]:", rec.name);
	fflush(stdin);
	gets(rec.name);

	List_Init(list, seat_node_t);
	seatcount = Seat_Srv_FetchByRoomID(list, rec.id);
	if (seatcount) {
	Seat_Srv_DeleteAllByRoomID(id);
	}
		printf("\n\t\t\t\t\t��λ����:");
		scanf("%d", &rec.rowsCount);
		printf("\n\t\t\t\t\t��λ����:");
		scanf("%d", &rec.colsCount);
		rec.seatsCount =rec.rowsCount*rec.colsCount ;
	

	printf("\n\t\t\t===========================================================\n");

	if (Studio_Srv_Modify(&rec)) {
		rtn = 1;
		printf(
				"\n\t\t\t\t\t�ݳ����޸ĳɹ�!\n\n\t\t\t\t\t�� [Enter] ����!\n");
	} else
		printf("\t\t\t\t\t�ݳ����޸�ʧ��!\n\n\t\t\t\t\t�� [Enter] ����!\n");
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
			printf("\n\t\t\t\t\t�ݳ�����λɾ���ɹ�!\n");
		printf(
				"\n\t\t\t\t\t�ݳ���ɾ���ɹ�!\n\n\t\t\t\t\t�� [Enter] ����!\n");
		rtn = 1;
	} else {
		printf("\n\t\t\t\t\t���ݳ���δ�˳�!\n\n\t\t\t\t\t�� [Enter] ����!\n");
	}
    system("pause");
	getchar();
	return rtn;
}

