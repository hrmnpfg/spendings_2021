//W tym pliku znajdują się funkcje zarządzające tworzeniem i modyfikowaniem dni

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "header.h"
//Michał Łukasik 330623
Day createDay()
{
	Day temp=malloc(sizeof(struct day));
	if(temp==NULL)
	{
		exit(1);
	}
	temp->plus=NULL;
	temp->minus=NULL;
	temp->next=NULL;
	return temp;
}

Day newDay(Day n)
{
	if(n==NULL)
	{
		n=createDay();
		n->dayId=0;
		n->previous=NULL;
		return n;
	}
	else n->next=createDay();
	n->next->dayDate=incrementTime(n->dayDate);
	n->next->previous=n;
	n->next->dayId=n->dayId+1;
	return n->next;
}

Transaction createTransaction(char description[])
{
	Transaction temp=malloc(sizeof(struct transaction));
	if(temp==NULL)
	{
		printf("F");
		exit(1);
	}
	temp->description=malloc(strlen(description)+1);
	strcpy(temp->description,description);
	if(temp->description==NULL) exit(1);
	temp->next=NULL;
	return temp;
}

Transaction newTransaction(Transaction n, double amount, int method, char description[], int repeat)
{
	if(n==NULL)
	{
		n=createTransaction(description);
		n->amount=amount;
		n->method=method;
		n->repeat=repeat;
		return n;
	}
	Transaction temp=n;
	while(n->next!=NULL)
	{
		n=n->next;
	}
	n->next=createTransaction(description);
	n->next->amount=amount;
	n->next->method=method;
	n->next->repeat=repeat;
	return temp;
}

void freeTransaction(Transaction n)
{
	if(n!=NULL)
	{
		freeTransaction(n->next);
		free(n->description);
		free(n);
	}
}

void freeDay(Day n)
{
	while(n!=NULL)
	{
		if(n->plus!=NULL) freeTransaction(n->plus);
		if(n->minus!=NULL) freeTransaction(n->minus);
		Day temp=n->previous;
		free(n);
		n=temp;
	}
}

struct time incrementTime(struct time n1)
{
	struct time n2;
	n2.day=n1.day+1;
	n2.month=n1.month;
	n2.year=n1.year;
	if(n2.month==1&&n2.day==32)
	{
		n2.month++;
		n2.day=1;
		return n2;
	}
	if(n2.month==2&&n2.day>=29)
	{
		if(n2.day==29&&n2.year%4==0&&n2.year%100!=0)
		{
			return n2;
		}
		n2.month++;
		n2.day=1;
		return n2;
	}
	if(n2.month==3&&n2.day==32)
	{
		n2.month=4;
		n2.day=1;
		return n2;
	}
	if(n2.month==4&&n2.day==31)
	{
		n2.month++;
		n2.day=1;
		return n2;
	}
	if(n2.month==5&&n2.day==32)
	{
		n2.month++;
		n2.day=1;
		return n2;
	}
	if(n2.month==6&&n2.day==31)
	{
		n2.month++;
		n2.day=1;
		return n2;
	}
	if(n2.month==7&&n2.day==32)
	{
		n2.month++;
		n2.day=1;
		return n2;
	}
	if(n2.month==8&&n2.day==32)
	{
		n2.month++;
		n2.day=1;
		return n2;
	}
	if(n2.month==9&&n2.day==31)
	{
		n2.month++;
		n2.day=1;
		return n2;
	}
	if(n2.month==10&&n2.day==32)
	{
		n2.month++;
		n2.day=1;
		return n2;
	}
	if(n2.month==11&&n2.day==31)
	{
		n2.month++;
		n2.day=1;
		return n2;
	}
	if(n2.month==12&&n2.day==32)
	{
		n2.year++;
		n2.month=1;
		n2.day=1;
		return n2;
	}
	return n2;
}

void addRepeats(Day n)
{
	if(n==NULL) return;
	Day temp=n;
	if(n->dayId>=31)
	{
		for(int i=0;i<31;i++) temp=temp->previous;
	}
	else
	{
		while(temp->previous!=NULL) temp=temp->previous;
	}
	while(temp->next!=NULL)
	{
		Transaction temp2=temp->plus;
		while(temp2!=NULL)
		{
			if(temp->dayId+temp2->repeat==n->dayId&&temp2->repeat>0) n->plus=newTransaction(n->plus,temp2->amount,temp2->method,temp2->description,temp2->repeat);
			temp2=temp2->next;
		}
		temp2=temp->minus;
		while(temp2!=NULL)
		{
			if(temp->dayId+temp2->repeat==n->dayId&&temp2->repeat>0) n->minus=newTransaction(n->minus,temp2->amount,temp2->method,temp2->description,temp2->repeat);
			temp2=temp2->next;
		}
		temp=temp->next;
	}
}
