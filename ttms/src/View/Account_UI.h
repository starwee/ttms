#ifndef ACCOUNT_UI_H_
#define ACCOUNT_UI_H_

void Account_UI_MgtEntry(void);

int Account_UI_Add(void);

int Account_UI_Delete(int id);

int Account_UI_Modify(int id) ;

void fixcipher(int id);

 char* get_passwd();


#endif /* ACCOUNT_UI_H_ */
