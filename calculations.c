//w tym pliku znajdują się funkcje odpowiedzialne za wykonywanie obliczeń na strukturze

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "header.h"
//Michał Łukasik 330623
double totalAmount(Day n)
{
	if(n==NULL) return 0;
	while(n->previous!=NULL) n=n->previous;
	double sum=0;
	while(n!=NULL)
	{
		Transaction temp=n->plus;
		while(temp!=NULL)
		{
			sum=sum+temp->amount;
			temp=temp->next;
		}
		temp=n->minus;
		while(temp!=NULL)
		{
			sum=sum-temp->amount;
			temp=temp->next;
		}
		n=n->next;
	}
	return sum;
}

double periodAmount(Day n, int c)
{
	if(n==NULL||c<=0) return 0;
	if(n->dayId<=c) return totalAmount(n);
	for(int i=1;i<c;i++)
	{
		n=n->previous;
	}
	double sum=0;
	while(n!=NULL)
	{
		Transaction temp=n->plus;
		while(temp!=NULL)
		{
			sum=sum+temp->amount;
			temp=temp->next;
		}
		temp=n->minus;
		while(temp!=NULL)
		{
			sum=sum-temp->amount;
			temp=temp->next;
		}
		n=n->next;
	}
	return sum;
}

double sumPlusMethod(Day n, int method)
{
	if(n==NULL) return 0;
	while(n->previous!=NULL) n=n->previous;
	double sum=0;
	while(n!=NULL)
	{
		Transaction temp=n->plus;
		while(temp!=NULL)
		{
			if(temp->method==method) sum=sum+temp->amount;
			temp=temp->next;
		}
		n=n->next;
	}
	return sum;
}

double sumMinusMethod(Day n, int method)
{
	if(n==NULL) return 0;
	while(n->previous!=NULL) n=n->previous;
	double sum=0;
	while(n!=NULL)
	{
		Transaction temp=n->minus;
		while(temp!=NULL)
		{
			if(temp->method==method) sum=sum+temp->amount;
			temp=temp->next;
		}
		n=n->next;
	}
	return sum;
}

double sumPlusMethodPeriod(Day n, int method, int c)
{
	if(n==NULL||c<=0) return 0;
	if(n->dayId<=c) return sumPlusMethod(n,method);
	for(int i=1;i<c;i++)
	{
		n=n->previous;
	}
	double sum=0;
	while(n!=NULL)
	{
		Transaction temp=n->plus;
		while(temp!=NULL)
		{
			if(temp->method==method) sum=sum+temp->amount;
			temp=temp->next;
		}
		n=n->next;
	}
	return sum;
}

double sumMinusMethodPeriod(Day n, int method, int c)
{
	if(n==NULL||c<=0) return 0;
	if(n->dayId<=c) return sumMinusMethod(n,method);
	for(int i=1;i<c;i++)
	{
		n=n->previous;
	}
	double sum=0;
	while(n!=NULL)
	{
		Transaction temp=n->minus;
		while(temp!=NULL)
		{
			if(temp->method==method) sum=sum+temp->amount;
			temp=temp->next;
		}
		n=n->next;
	}
	return sum;
}

double totalDay(Day n)
{
	if(n==NULL) return 0;
	double sum=0;
	Transaction temp;
	temp=n->minus;
	while(temp!=NULL)
	{
		sum=sum-temp->amount;
		temp=temp->next;
	}
	temp=n->plus;
	while(temp!=NULL)
	{
		sum=sum+temp->amount;
		temp=temp->next;
	}
	return sum;
}
