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

	//��������
	paging.totalRecords = Play_Srv_FetchAll(head);
	Paging_Locate_FirstPage(head, paging);

	do {
	    system("cls");
		printf(
				"\n\t\t\t===========================================================\n");
		printf(
				"\t\t\t************************ ��Ŀ�б� *************************\n");
		printf("\n\t\t\t%5s  %10s  %5s  %10s\n", "ID", "Ӱ��", "�۸�","Ʊ��");
		printf(
				"\n\t\t\t===========================================================\n");
		//��ʾ����
		Paging_ViewPage_ForEach(head, paging, play_node_t, pos, i){
			printf("\n\t\t\t%5d  %10s  %5d  %10d\n", pos->data.id,
					pos->data.name, pos->data.price,pos->data.sale);
		printf(
				"\n\t\t\t-----------------------------------------------------------\n");		
		}

		printf(
				"\n\t\t\t------- ��:%2d ----------------------- ҳ %2d/%2d ----\n",
				paging.totalRecords, Pageing_CurPage(paging),
				Pageing_TotalPages(paging));
		printf(
				"\n\t\t\t===========================================================\n");
		printf(
				"\n\t\t\t[P]��ҳ|[N]��ҳ|[A]���|[D]ɾ��|[U]�޸�|[S]�����ݳ�|[R]����");
		printf(
				"\n\t\t\t===========================================================\n");
		printf("\n\t\t\t\t\tѡ��:");
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
			printf("\n\t\t\t\t\t����ID:");
			scanf("%d", &id);
			if (Play_UI_Delete(id)) {	
				paging.totalRecords = Play_Srv_FetchAll(head);
				List_Paging(head, paging, play_node_t);
			}
			break;
		case 'u':
		case 'U':
			printf("\n\t\t\t\t\t����ID:");
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
		printf("\t\t\t**********************  ����µ�Ӱ  **********************");
		printf(
				"\n\t\t\t===========================================================\n");
		printf("\n\t\t\t\t\tӰ����");
		fflush(stdin);
		printf("");
		gets(rec.name);
		printf("\n\t\t\t\t\t�۸�");
		fflush(stdin);
		printf("");
		scanf("%d",&rec.price);
		rec.sale = 0;
		printf(
				"\n\t\t\t===========================================================\n");

		if (Play_Srv_Add(&rec)) {
			newRecCount += 1;
			printf("\n\t\t\t\t\t�µ�Ӱ��ӳɹ�!\n");
		} else
			printf("\n\t\t\t\t\t�µ�Ӱ���ʧ��!\n");
		printf(
				"\n\t\t\t===========================================================\n");
		printf("\n\t\t\t\t\t[A]�������|[R]����\n");
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
		printf("\n\t\t\t\t\t�õ�Ӱ��δ�˳�!\n\n\t\t\t\t\t�� [Enter] ����!\n");
		getchar();
		return 0;
	}
    system("cls");
	printf(
			"\n\t\t\t===========================================================\n");
	printf("\t\t\t***********************  �޸ĵ�Ӱ  ***********************\n");
	printf(
			"\n\t\t\t===========================================================\n");
	printf("\n\t\t\t\t\t��ӰID:%d\n", rec.id);
	printf("\n\t\t\t\t\t��Ӱ��[%s]:", rec.name);
	fflush(stdin);
	gets(rec.name);

	printf(
				"\n\t\t\t===========================================================\n");

	if (Play_Srv_Modify(&rec)) {
		rtn = 1;
		printf(
				"\n\t\t\t\t\t��Ӱ���ݸ��³ɹ�!\n\n\t\t\t\t\t�� [Enter] ����!\n");
	} else
		printf("\n\t\t\t\t\t��Ӱ���ݸ���ʧ��!\n\n\t\t\t\t\t�� [Enter] ����!\n");
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
				"\n\t\t\t\t\t��Ӱɾ���ɹ�!\n\n\t\t\t\t\t�� [Enter]����!\n");
		rtn = 1;
	} else {
		printf("\n\t\t\t\t\t��Ӱɾ��ʧ��!\n\n\t\t\t\t\t�� [Enter]����!\n");
	}
	system("pause");
	getchar();
	return rtn;
}

