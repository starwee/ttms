
#include "../View/SalesAnalysis_UI.h"
#include "../Service/SalesAnalysis_Ser.h"
#include "../Common/List.h"

static const int SALEANALYSIS_PAGE_SIZE = 5;

#include<stdio.h>

void SalesAanalysis_UI_MgtEntry(void)
{
	int i;
	play_list_t head;
	play_node_t *pos;
	char choice;
	Pagination_t paging;
	
	List_Init(head,play_node_t);
	paging.offset = 0;
	paging.pageSize = SALEANALYSIS_PAGE_SIZE;
	
	//��������
	paging.totalRecords=SalesAnalysis_Ser_StaticSale(head);
	Paging_Locate_FirstPage(head, paging);
	do {
		system("cls"); 
		printf("\n\t\t\t===========================================================\n");
		printf(
				"\t\t\t************************** Ʊ�� ***************************\n");
		printf("\n\t\t\t\t%5s  %10s  %10s\n", "ID", "Ӱ��", "Ʊ��");
		printf("\n\t\t\t===========================================================\n");
		//��ʾ����
		Paging_ViewPage_ForEach(head, paging, play_node_t, pos, i){
			printf("\n\t\t\t\t%5d  %10s  %10d\n", pos->data.id,
					pos->data.name, pos->data.sale);
			printf("\n\t\t\t-----------------------------------------------------------\n");
		}

		printf(
				"\n\t\t\t------------- ��:%2d ------------------ ҳ %2d/%2d -----------\n",
				paging.totalRecords, Pageing_CurPage(paging),
				Pageing_TotalPages(paging));
		printf("\n\t\t\t===========================================================\n");
		printf(
				"\t\t\t\t[P]��һҳ|[N]��һҳ|[R]����");
		printf("\n\t\t\t===========================================================\n");
		printf("\n\t\t\t\t\tѡ��:");
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
		}
	} while (choice != 'r' && choice != 'R');
	
	List_Destroy(head, play_node_t);
	
	
	
	
}
