#include"../common/common.h"
#include"../Persistence/SalesAnalysis_Persis.h"
#include"../Service/Play_Ser.h"
#include"../Persistence/Play_Persis.h"
#include<stdio.h>

int SalesAnalysis_Ser_StaticSale(play_list_t head)
{
	int count=Play_Perst_SelectAll(head);
	SalesAnalysis_Ser_SortBySale(head);
	
	return count;
}
void SalesAnalysis_Ser_SortBySale(play_list_t head)
{
	play_node_t *p=NULL,*q=NULL;
	play_t tmp;
	for(p=head->prev;p!=head->next;p=p->prev){
		for(q=head->next;q!=p;q=q->next){
			if(q->data.sale<q->next->data.sale){
				tmp=q->data;
				q->data=q->next->data;
				q->next->data=tmp;
			}
		}
	}
}
