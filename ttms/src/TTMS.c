
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
		printf("\n\n\n\n\n\t\t\t\t首次使用该系统，自动生成管理员账号");
		printf("\n\n\t\t\t\t用户名：manage");
		printf("\n\n\t\t\t\t密码：123456");
		printf("\n\n\t\t\t\t请使用该账号登录管理员端");
		printf("\n\n\t\t\t\t按 [Enter] 继续");
		fflush(stdin);
		getchar(); 
	}
	char choice;
	
	do{
		
		printf("\n\n\n\n\n\n\t\t\t\t\t\t[1]登陆");
	printf("\n\n\n\t\t\t\t\t\t[2]注册");
	printf("\n\n\n\t\t\t\t\t\t[0]退出");
	
	printf("\n\n\n\t\t\t\t\t\t选择：");
	do{
		fflush(stdin);
	scanf("%c",&choice);
	}while(choice<'0'||choice>'2');
	 
	if(choice=='1')  login();
	else if(choice=='2') logon();
	}while(choice!='0');
	return EXIT_SUCCESS;
}
