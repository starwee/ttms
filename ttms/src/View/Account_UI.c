#include "../View/Account_UI.h"

#include "../Common/List.h"

#include "../Service/Account_Ser.h"

#include<string.h>

static const int ACCOUNT_PAGE_SIZE = 5;

#include <stdio.h>

void Account_UI_MgtEntry(void) {
	int i, id;
	char choice;

	account_list_t head;
	account_node_t *pos;
	Pagination_t paging;

	List_Init(head, account_node_t);
	paging.offset = 0;
	paging.pageSize = ACCOUNT_PAGE_SIZE;
	//��������
	paging.totalRecords = Account_Srv_FetchAll(head);
	Paging_Locate_FirstPage(head, paging);

	do {
		system("cls");
		printf(
				"\n\t\t\t===========================================================\n");
		printf(
				"\t\t\t************************ �û��б� *************************\n");
		printf("\n\t\t\t%5s  %10s  %10s  %20s\n", "ID", "�û���", "���",
				"����");
		printf(
				"\n\t\t\t===========================================================\n");
		//��ʾ����
		Paging_ViewPage_ForEach(head, paging, account_node_t, pos, i){
			printf("\n\t\t\t%5d  %10s  %10s  %20s", pos->data.id,
					pos->data.name, pos->data.status, pos->data.cipher);
					printf(
				"\n\t\t\t-----------------------------------------------------------\n");
		}

		printf(
				"\n\t\t\t------------ ��:%2d ------------------ ҳ %2d/%2d ------------\n",
				paging.totalRecords, Pageing_CurPage(paging),
				Pageing_TotalPages(paging));
		printf(
				"\n\t\t\t===========================================================\n");
		printf(
				"\n\t\t\t[P]��ҳ|[N]��ҳ|[A]����û�|[U]�޸���Ϣ|[D]ɾ���û�|[R]����\n");
		printf(
				"\n\t\t\t===========================================================\n");
		printf("\n\t\t\t\t\tѡ��:");
		fflush(stdin);
		scanf("%c", &choice);
		fflush(stdin);

		switch (choice) {
		case 'a':
		case 'A':
			if(Account_UI_Add()){
				
				paging.totalRecords = Account_Srv_FetchAll(head);
				Paging_Locate_LastPage(head, paging, account_node_t);
			
			}
			break;
		case 'd':
		case 'D':
			printf("\n\t\t\t\t\t����ID:");
			scanf("%d", &id);
			if(Account_UI_Delete(id)){
				paging.totalRecords = Account_Srv_FetchAll(head);
				Paging_Locate_LastPage(head, paging, account_node_t);
			}
			break;
		case 'u':
		case 'U':
			printf("\n\t\t\t\t\t����ID:");
			scanf("%d", &id);
			if (Account_UI_Modify(id)) {	
				paging.totalRecords = Account_Srv_FetchAll(head);
				List_Paging(head, paging, account_node_t);
			}
			break;
		case 'p':
		case 'P':
			if (!Pageing_IsFirstPage(paging)) {
				Paging_Locate_OffsetPage(head, paging, -1, account_node_t);
			}
			break;
		case 'n':
		case 'N':
			if (!Pageing_IsLastPage(paging)) {
				Paging_Locate_OffsetPage(head, paging, 1, account_node_t);
			}
			break;
		}
	} while (choice != 'r' && choice != 'R');
	
	List_Destroy(head, account_node_t);
}

int Account_UI_Add(void) {
	account_t rec,account;
	int newRecCount = 0;
	char choice;

	do {
		system("cls");
		printf(
				"\n\t\t\t===========================================================\n");
		printf("\t\t\t**********************  ������û�  ***********************\n");
		printf(
				"\t\t\t===========================================================\n");
		printf("\n\t\t\t\t\t�û�����");
		fflush(stdin);
		gets(rec.name);
		while(Account_Srv_FetchByName(rec.name, &account)!=0){
		printf("\n\n\t\t\t\t\t�û����Ѵ��ڣ�����\n");
		printf("\n\t\t\t\t\t[C]��������|[R]�˳�\n"); 
		fflush(stdin);
		int c;
		printf("\n\t\t\t\t\tѡ��"); 
		scanf("%c",&c);
		if(c=='R'||c=='r') return;
		system("cls");
		printf(
				"\n\t\t\t===========================================================\n");
		printf("\t\t\t**********************  ������û�  ***********************\n");
		printf(
				"\t\t\t===========================================================\n");
		printf("\n\t\t\t\t\t�û�����");
		fflush(stdin); 
		gets(rec.name);
	}
		printf("\n\t\t\t\t\t���(manage/seller/user)��");
		fflush(stdin);
		gets(rec.status);
		while(strcmp(rec.status,"manage")!=0&&strcmp(rec.status,"seller")!=0&&strcmp(rec.status,"user")!=0){
			printf("\n\n\t\t\t\t\t����");
			 printf("\n\t\t\t\t\t[C]��������|[R]�˳�\n"); 
		fflush(stdin);
		int c;
		printf("\n\t\t\t\t\tѡ��"); 
		scanf("%c",&c);
		if(c=='R') return;
		system("cls");
		printf(
				"\n\t\t\t===========================================================\n");
		printf("\t\t\t**********************  ������û�  ***********************\n");
		printf(
				"\t\t\t===========================================================\n");
				printf("\n\t\t\t\t\t�û�����%s\n",rec.name);
		printf("\n\t\t\t\t\t���(manage/seller/user)��");
		fflush(stdin); 
		gets(rec.status);
		}
		printf("\n\t\t\t\t\t���룺");
		strcpy(rec.cipher,get_passwd());
		char passwd[20];
    printf("\n\n\t\t\t\t\t�ٴ��������룺") ;
	 fflush(stdin);
	 strcpy(passwd,get_passwd());
	 while(strcmp(passwd,rec.cipher)!=0){
	 	printf("\n\n\n\t\t\t\t\t���벻һ�£�");
	 	printf("\n\n\t\t\t\t\t[C]��������|[R]����\n");
	 	printf("\n\t\t\t\t\tѡ��"); 
		 fflush(stdin);
		 char c;
		 scanf("%c",&c);
		 if(c=='r'||c=='R') return;
		 system("cls");
		printf(
				"\n\t\t\t===========================================================\n");
		printf("\t\t\t**********************  ������û�  ***********************\n");
		printf(
				"\t\t\t===========================================================\n");
				printf("\n\t\t\t\t\t�û�����%s\n",rec.name);
		printf("\n\t\t\t\t\t���(manage/seller/user)��%s",rec.status);
		 printf("\n\n\t\t\t\t\t���룺");
		 fflush(stdin);
	strcpy(rec.cipher,get_passwd());
    printf("\n\n\t\t\t\t\t�ٴ��������룺") ;
	 fflush(stdin);
	 strcpy(passwd,get_passwd());	
	 }
		printf(
				"\n\t\t\t===========================================================\n");

		if (Account_Srv_Add(&rec)) {
			newRecCount += 1;
			printf("\n\t\t\t\t\t���û���ӳɹ�!\n");
		} else
			printf("\n\t\t\t\t\t���û����ʧ��!\n");
		printf(
				"\n\t\t\t===========================================================\n");
		printf("\t\t\t\t\t[A]�������|[R]����");
		printf(
				"\n\t\t\t===========================================================\n");
		printf("\n\t\t\t\t\tѡ��"); 
		fflush(stdin);
		scanf("%c", &choice);
	} while ('a' == choice || 'A' == choice);
	return newRecCount;
}

int Account_UI_Delete(int id) {

	int rtn = 0;

	if (Account_Srv_DeleteByID(id)) {
		printf(
				"\n\t\t\t\t\t�û�ɾ���ɹ�!\n\n\t\t\t\t\t�� [Enter] ����!\n");
		rtn = 1;
	} else {
		printf("\n\t\t\t\t\t�û�������!\n\n\t\t\t\t\t�� [Enter] ����!\n");
	}
system("pause");
	getchar();
	return rtn;
}

int Account_UI_Modify(int id) {
	account_t rec;
	int rtn = 0;
    system("cls");
	/*Load record*/
	if (!Account_Srv_FetchByID(id, &rec)) {
		printf("\n\n\n\n\n\t\t\t\t\tδ�ҵ����û�!\n\n\n\n\n\t\t\t\t\t�� [Enter] ����!\n");
		fflush(stdin);
		getchar();
		return 0;
	}

	printf(
			"\n\t\t\t===========================================================\n");
	printf("\t\t\t*********************  �޸��û���Ϣ  **********************\n");
	printf(
			"\t\t\t===========================================================\n");
	printf("\n\t\t\t\t\t�û�����");
	fflush(stdin);
	gets(rec.name);
	account_t account; 
	while(Account_Srv_FetchByName(rec.name, &account)!=0){
		printf("\n\n\t\t\t\t\t�û����Ѵ��ڣ�����\n");
		printf("\n\t\t\t\t\t[C]��������|[R]�˳�\n"); 
		fflush(stdin);
		int choice;
		printf("\n\t\t\t\t\tѡ��"); 
		scanf("%c",&choice);
		if(choice=='R') return;
		system("cls");
			printf(
				"\n\t\t\t===========================================================\n");
		printf("\t\t\t*********************  �޸��û���Ϣ  **********************\n");
		printf(
				"\t\t\t===========================================================\n");
		printf("\n\t\t\t\t\t�û�����");
		fflush(stdin); 
		gets(rec.name);
	}
	printf("\n\t\t\t\t\t���(manage/seller/user):");
	fflush(stdin);
	gets(rec.status);
	while(strcmp(rec.status,"manage")!=0&&strcmp(rec.status,"seller")!=0&&strcmp(rec.status,"user")!=0){
			printf("\n\n\t\t\t\t\t����");
			 printf("\n\t\t\t\t\t[C]��������|[R]�˳�\n"); 
		fflush(stdin);
		int c;
		printf("\n\t\t\t\t\tѡ��"); 
		scanf("%c",&c);
		if(c=='R') return;
		system("cls");
		printf(
				"\n\t\t\t===========================================================\n");
		printf("\t\t\t*********************  �޸��û���Ϣ  **********************\n");
		printf(
				"\t\t\t===========================================================\n");
				printf("\n\t\t\t\t\t�û�����%s\n",rec.name);
		printf("\n\t\t\t\t\t���(manage/seller/user)��");
		fflush(stdin); 
		gets(rec.status);
		}
	printf("\n\t\t\t\t\t���룺");
	fflush(stdin);
	strcpy(rec.cipher,get_passwd());
	char passwd[20];
    printf("\n\n\t\t\t\t\t�ٴ��������룺") ;
	 fflush(stdin);
	 strcpy(passwd,get_passwd());
	 while(strcmp(passwd,rec.cipher)!=0){
	 	printf("\n\n\n\t\t\t\t\t���벻һ�£�");
	 	printf("\n\n\t\t\t\t\t[C]��������|[R]����\n");
	 	printf("\n\t\t\t\t\tѡ��"); 
		 fflush(stdin);
		 char choice;
		 scanf("%c",&choice);
		 if(choice=='r'||choice=='R') return;
		 system("cls");
		printf(
				"\n\t\t\t===========================================================\n");
		printf("\t\t\t*********************  �޸��û���Ϣ  **********************\n");
		printf(
				"\n\t\t\t===========================================================\n");
				printf("\n\t\t\t\t\t�û�����%s\n",rec.name);
		printf("\n\t\t\t\t\t���(manage/seller/user)��%s",rec.status);
		 printf("\n\n\t\t\t\t\t���룺");
		 fflush(stdin);
	strcpy(rec.cipher,get_passwd());
    printf("\n\n\t\t\t\t\t�ٴ��������룺") ;
	 fflush(stdin);
	 strcpy(passwd,get_passwd());	
	 }
	printf(
			"\n\t\t\t===========================================================\n");

	if (Account_Srv_Modify(&rec)) {
		rtn = 1;
		printf(
				"\n\t\t\t\t\t�û���Ϣ�޸ĳɹ�!\n\n\t\t\t\t\t�� [Enter] ����!\n");
	} else
		printf("\n\t\t\t\t\t�û���Ϣ�޸�ʧ��!\n\n\t\t\t\t\t�� [Enter] ����!\n");
	getchar();
	return rtn;
}

void logon()
{
	system("cls");
	account_t data;
	strcpy(data.status,"user");
	printf("\n\n\n\t\t\t\t\t�û�����");
	fflush(stdin);
	gets(data.name);
    account_t account;
	while(Account_Srv_FetchByName(data.name, &account)!=0){
		printf("\n\n\t\t\t\t\t�û����Ѵ��ڣ�����\n");
		printf("\n\t\t\t\t\t[C]��������|[R]�˳�\n"); 
		fflush(stdin);
		int choice;
		printf("\n\t\t\t\t\tѡ��"); 
		scanf("%c",&choice);
		if(choice=='R') return;
		system("cls");
		printf("\n\n\n\t\t\t\t\t�û�����");
		fflush(stdin); 
		gets(data.name);
	}
	printf("\n\n\t\t\t\t\t���룺");
	fflush(stdin);
	strcpy(data.cipher,get_passwd());
	char passwd[20];
    printf("\n\n\n\t\t\t\t\t�ٴ��������룺") ;
	 fflush(stdin);
	 strcpy(passwd,get_passwd());
	 while(strcmp(passwd,data.cipher)!=0){
	 	printf("\n\n\n\t\t\t\t\t���벻һ�£�");
	 	printf("\n\t\t\t\t\t[C]��������|[R]����\n");
	 	printf("\n\t\t\t\t\tѡ��"); 
		 fflush(stdin);
		 char choice;
		 scanf("%c",&choice);
		 if(choice=='r'||choice=='R') return;
		 system("cls");
		printf("\n\n\n\t\t\t\t\t�û�����%s",data.name);
		 printf("\n\n\n\t\t\t\t\t���룺");
		 fflush(stdin);
	strcpy(data.cipher,get_passwd());
    printf("\n\n\n\t\t\t\t\t�ٴ��������룺") ;
	 fflush(stdin);
	 strcpy(passwd,get_passwd());	
	 }
	if(Account_Srv_Add(&data)){
		printf("\n\n\t\t\t\t\tע��ɹ���");
	}
	else printf("\n\t\t\t\t\tע��ʧ�ܣ�"); 
	system("pause");
 } 
 
 void login()
 {
 	system("cls");
	account_t data;
	printf("\n\n\n\t\t\t\t\t�����û�����");
	fflush(stdin);
	gets(data.name);
    account_t account;
	while(Account_Srv_FetchByName(data.name, &account)==0){
		printf("\n\n\t\t\t\t\t�û��������ڣ�����\n");
		printf("\n\t\t\t\t\t[C]��������|[R]�˳�\n"); 
		fflush(stdin);
		int choice;
		printf("\n\t\t\t\t\tѡ��"); 
		scanf("%c",&choice);
		if(choice=='R'||choice=='r') return;
		fflush(stdin); 
		system("cls");
		printf("\n\n\n\t\t\t\t\t�û�����");
		gets(data.name);
	}
	printf("\n\n\n\t\t\t\t\t���룺");
	fflush(stdin);
	strcpy(data.cipher,get_passwd());
	while(strcmp(account.cipher,data.cipher)!=0){
		printf("\n\n\t\t\t\t\t�������") ;
			printf("\n\t\t\t\t\t[C]��������|[R]�˳�\n"); 
		fflush(stdin);
		int choice;
		printf("\n\t\t\t\t\tѡ��"); 
		scanf("%c",&choice);
		if(choice=='R'||choice=='r') return;
		system("cls");
		printf("\n\n\n\t\t\t\t\t�������룺");
		strcpy(data.cipher,get_passwd());
	}
	
	if(strcmp(account.status,"manage")==0){
		printf("\n\n\t\t\t\t\t�Թ���Ա��ݵ�½�ɹ���\n\n\t\t\t\t\t�� [Enter] ����ϵͳ��\n\n");
		system("pause");
		Main_Menu(1,account.id); 
	}
	else if (strcmp(account.status,"seller")==0){
		printf("\n\n\t\t\t\t\t����ƱԱ��ݵ�½�ɹ���\n\n\t\t\t\t\t�� [Enter] ����ϵͳ��\n\n");
		system("pause");
		Main_Menu(2,account.id);
	}
	else if (strcmp(account.status,"user")==0){
		printf("\n\n\t\t\t\t\t����ͨ�û���ݵ�½�ɹ���\n\n\t\t\t\t\t�� [Enter] ����ϵͳ��\n\n");
		system("pause");
		Main_Menu(3,account.id);
	}
 }
 
 void fixcipher(int id)
 {
 	account_t rec;
	char cipher[20]; 
    system("cls");
	/*Load record*/
	if (!Account_Srv_FetchByID(id, &rec)) {
		printf("δ�ҵ����û�!\n�� [Enter] ����!\n");
		getchar();
		return;
	}

	system("cls");
	printf("\n\n\n\t\t\t\t\t�����룺");
	fflush(stdin);
	strcpy(cipher,get_passwd());
	char choice;
	while(strcmp(cipher,rec.cipher)!=0){
		printf("\n\t\t\t\t\t�������\n");
		 printf("\n\t\t\t\t\t[C]��������|[R]����\n");
		 fflush(stdin);
		 scanf("%c",&choice);
		 if(choice=='r'||choice=='R') return;
		 system("cls");
		 printf("\n\n\n\t\t\t\t\t�����룺");
		 fflush(stdin);
	strcpy(cipher,get_passwd());
	}
	printf("\n\n\n\t\t\t\t\t�����룺");
	fflush(stdin);
	strcpy(rec.cipher,get_passwd());
	char passwd[20];
    printf("\n\n\n\t\t\t\t\t�ٴ����������룺") ;
	 fflush(stdin);
	 strcpy(passwd,get_passwd());
	 while(strcmp(passwd,rec.cipher)!=0){
	 	printf("\n\n\n\t\t\t\t\t���벻һ�£�");
	 	printf("\n\n\n\t\t\t\t\t[C]��������|[R]����\n");
	 	printf("\n\t\t\t\t\tѡ��"); 
		 fflush(stdin);
		 char c;
		 scanf("%c",&c);
		 if(c=='r'||c=='R') return;
		 system("cls");
		printf("\n\n\n\t\t\t\t\t�����룺");
		int x=strlen(cipher);
		for(x;x!=0;x--) printf("*");
		 printf("\n\n\n\t\t\t\t\t�����룺");
		 fflush(stdin);
	strcpy(rec.cipher,get_passwd());
    printf("\n\n\n\t\t\t\t\t�ٴ����������룺") ;
	 fflush(stdin);
	 strcpy(passwd,get_passwd());	
	 }
	printf(
			"\n\t\t\t===========================================================\n");

	if (Account_Srv_Modify(&rec)) {
		printf(
				"\n\t\t\t\t\t�����޸ĳɹ�!\n\n\t\t\t\t\t�� [Enter] ����!\n");
	} else
		printf("\n\t\t\t\t\t�����޸�ʧ��!\n\n\t\t\t\t\t�� [Enter] ����!\n");
	getchar();
	return ;
 }
 
 char *get_passwd()
 {
 	static char p[20];
 	int i=0; 
 	fflush(stdin);
 	p[i++]=getch();
 	while(p[i-1]!='\r'){
 		if(p[i-1]=='\b'){
 			if(i-1==0){
 				i=0;
			 }
			 else{
			 	i--;
			 	i--;
			 	putchar('\b');
			 	putchar(' ');
			 	putchar('\b');
			 }
		 }
		 else {
		 	putchar('*');
		 }
		 p[i++]=getch();
	 }
	 i--; 
	 p[i]='\0';
 	return p;
  } 
