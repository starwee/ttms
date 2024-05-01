/*
* Copyright(C), 2007-2008, XUPT Univ.	 
* ������ţ�TTMS_UC_02
* File name: Seat_UI.c			  
* Description : ������λ���������	
* Author:   XUPT  		 
* Version:  v.1 	 
* Date: 	2015��4��22��	
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
	case SEAT_GOOD:		//����λ
		statusChar = '#';
		break;
	case SEAT_BROKEN:	//�𻵵���λ
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
		//��ʾ����
		for(pos=head->next;pos!=head;pos=pos->next){
			if(pos!=head->next&&pos->prev->data.row!=pos->data.row) printf("\n\t\t\t ");
			printf("%c      ",Seat_UI_Status2Char(pos->data.status));
		}
		printf(
				"\n\t\t\t===========================================================\n");
		printf(
				"\n\t\t\t\t\t[U]�޸���λ״̬|[R]����");
		printf(
				"\n\t\t\t===========================================================\n");
		printf("\n\t\t\t\t\tѡ��:");
		fflush(stdin);
		scanf("%c", &choice);
		fflush(stdin);
        studio_t studio;
		Studio_Srv_FetchByID(roomID, &studio);
		switch (choice) {
		case 'u':
		case 'U':
			
			if (Seat_UI_Modify(head,studio.rowsCount,studio.colsCount)) {	//������������
				Seat_Srv_FetchByRoomID(head,roomID);
				
			}
			break;
		}
	} while (choice != 'r' && choice != 'R');
	//�ͷ�����ռ�
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
	printf("\t\t\t***************************  ������λ  ***************************\n");
	printf("\t\t\t------------------------------------------------------------------\n");
	
						
			printf("\n\n\n\t\t\t\t\t�в��ɳ� %d and �в��ɳ� %d!\n",
					row, column);
			printf("\n\t\t\t\t\t��λ�кţ� ");
			scanf("%d", &newrow);
			printf("\n\t\t\t\t\t��λ�кţ� ");
			scanf("%d", &newcolumn);
			while (getchar() != '\n')
				continue;
		} while (newrow > row || newcolumn > column);

		p = Seat_Srv_FindByRowCol(list, newrow, newcolumn);
		if (p) {
			printf("\n\t\t\t�޸���λ��Ϣ[%d,%d]:[%c]Ϊ('#': ��λ����; '@':��λ��; ����:��λ������):", newrow, newcolumn,
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
				printf("\n\t\t\t��λ���³ɹ�\n");
			} else
				printf("\n\t\t\t��λ����ʧ��\n");
		} else
			printf("\n\t\t\t��λ������\n");
		printf("\t\t\t------------------------------------------------------------------\n");
		printf("\n\t\t\t[U]�����޸�|[R]����");
		printf("\n\n\t\t\tѡ��"); 
		choice = toupper(getchar());
		while (getchar() != '\n')
			continue;
	} while ('u' == choice || 'U' == choice);
	return rtn;

}

