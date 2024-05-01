#include "../View/Main_Menu.h"

#include <stdio.h>

#include "../View/Studio_UI.h"

#include "../View/SalesAnalysis_UI.h" 

#include "Ticket_UI.h"

#include "../Service/Ticket_Ser.h"

#include "../Common/List.h"


void Main_Menu(int status,int userID) {
	char choice;
	
	do { 
	    system("cls");
	    printf("\n\t\t\t\t�û�ID:%d",userID);
		printf("\n\t\t\t\t===============================\n");
		printf("\n\t\t\t\t\t  ****Ӱ��\n\n");
		if(status==1){
			printf("\t\t\t\t\t[S]Ӱ������.\n\n");
		printf("\t\t\t\t\t[P]��Ŀ����.\n\n");
		printf("\t\t\t\t\t[A]�û�����.\n\n");
		}
		if(status==2){
			printf("\t\t\t\t\t[T]��Ʊ.\n\n");
		printf("\t\t\t\t\t[R]��Ʊ.\n\n");
		}
		if(status==3){
			printf("\t\t\t\t\t[B]��Ʊ\n\n"); 
			printf("\t\t\t\t\t[R]��Ʊ.\n\n");
			printf("\t\t\t\t\t[M]�ҵĶ���\n\n");
		}
			printf("\t\t\t\t\t[Q]��ѯ�ݳ�\n\n");
		printf("\t\t\t\t\t[N]Ʊ������.\n\n");
		printf("\t\t\t\t\t[F]�޸�����\n\n");
		printf("\t\t\t\t\t[E]�˳�.\n");
		printf("\n\t\t\t\t===============================\n");
		printf("\t\t\t\t\t��ѡ��:");
		do{
			fflush(stdin);
		scanf("%c",&choice);
		}while(choice!='s'&&choice!='S'&&choice!='P'&&choice!='p'&&choice!='Q'
		&&choice!='q'&&choice!='T'&&choice!='t'&&choice!='B'&&choice!='b'&&
		choice!='R'&&choice!='r'&&choice!='M'&&choice!='m'&&choice!='N'&&
		choice!='n'&&choice!='A'&&choice!='a'&&choice!='F'&&choice!='f'&&
		choice!='E'&&choice!='e');
		switch (choice) {
		case 'S':
		case 's':
			if(status==1) Studio_UI_MgtEntry();
			break;
		case 'P':
		case 'p':
			 if(status==1)Play_UI_MgtEntry(0);
			break;
		case 'Q':
		case 'q':
			Queries_Menu();
			break;
		case 'T':
		case 't':
			if(status==2)Ticket_UI_MgtEntry(userID,2);
			break;
		case 'B':
		case 'b':
			if(status==3)Ticket_UI_MgtEntry(userID,3);
			
		    break; 
		case 'R':
		case 'r':
			if(status==2)Sale_UI_ReturnTicket();
			if(status==3)qticket(userID,1);
			break;
		case 'M':
		case 'm':
			if(status==3)qticket(userID,0);
			break;
		case 'N':
		case 'n':
			SalesAanalysis_UI_MgtEntry();
			break;
		case 'A':
		case 'a':
			if(status==1)Account_UI_MgtEntry();
			break;
		case 'F':
		case 'f':
			fixcipher(userID);
			break;
		}
	} while ('E' != choice && 'e' != choice);
}

