
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <io.h>
#include "./Common/List.h"
#include "./Service/Account_Ser.h"
#include "./View/Main_Menu.h"

int main(void) {
	setvbuf(stdout, NULL, _IONBF, 0);
	account_list_t list;
	List_Init(list, account_node_t);
	Account_Srv_FetchAll(list);
	if(list->next==list){
		account_t manage;
		strcpy(manage.name,"manage");
		strcpy(manage.status,"manage");
		strcpy(manage.cipher,"123456");
		Account_Srv_Add(&manage); 
		printf("\n\n\n\n\n\t\t\t\t�״�ʹ�ø�ϵͳ���Զ����ɹ���Ա�˺�");
		printf("\n\n\t\t\t\t�û�����manage");
		printf("\n\n\t\t\t\t���룺123456");
		printf("\n\n\t\t\t\t��ʹ�ø��˺ŵ�¼����Ա��");
		printf("\n\n\t\t\t\t�� [Enter] ����");
		fflush(stdin);
		getchar(); 
	}
	char choice;
	
	do{
		
		printf("\n\n\n\n\n\n\t\t\t\t\t\t[1]��½");
	printf("\n\n\n\t\t\t\t\t\t[2]ע��");
	printf("\n\n\n\t\t\t\t\t\t[0]�˳�");
	
	printf("\n\n\n\t\t\t\t\t\tѡ��");
	do{
		fflush(stdin);
	scanf("%c",&choice);
	}while(choice<'0'||choice>'2');
	 
	if(choice=='1')  login();
	else if(choice=='2') logon();
	}while(choice!='0');
	return EXIT_SUCCESS;
}
