//w tym pliku znajdują się funkcje do zapisywania i odczytywania plików

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "header.h"
#include <math.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
//Michał Łukasik 330623
void saveDay(Day n)
{
	if(n==NULL) return;
	int len;
	if(n->dayId==0) len=1;
	else len=floor(log10(n->dayId))+1;
	char name[len+1];
	sprintf(name, "%ld", n->dayId);
	name[len]='p';
	for(unsigned int i=len+1;i<strlen(name);i++)
	{
		name[i]='\0';
	}
	FILE *f1=fopen(name, "w");
	if(f1==NULL) exit(1);
	Transaction temp=n->plus;
	char tDesc[40];
	while(temp!=NULL)
	{
		strcpy(tDesc,temp->description);
		for(int i=0;i<40;i++)
		{
			if(tDesc[i]==' ') tDesc[i]='#';
		}
		fprintf(f1,"& %f %d %d %s\n",temp->amount,temp->method,temp->repeat,tDesc);
		temp=temp->next;
	}
	fclose(f1);
	name[len]='m';
	f1=fopen(name, "w");
	if(f1==NULL) exit(1);
	temp=n->minus;
	while(temp!=NULL)
	{
		strcpy(tDesc,temp->description);
		for(int i=0;i<40;i++)
		{
			if(tDesc[i]==' ') tDesc[i]='#';
		}
		fprintf(f1,"& %f %d %d %s\n",temp->amount,temp->method,temp->repeat,tDesc);
		temp=temp->next;
	}
	fclose(f1);
}

Day loadDay(Day n)
{
	if(n==NULL) return n;
	int len;
	if(n->dayId==0) len=1;
	else len=floor(log10(n->dayId))+1;
	char name[len+1];
	sprintf(name, "%ld", n->dayId);
	name[len]='p';
	for(unsigned int i=len+1;i<strlen(name);i++)
	{
		name[i]='\0';
	}
	double amount;
	int method;
	int repeat;
	char description[40];
	FILE *f1=fopen(name, "r");
	if(f1==NULL) return n;
	int counter=0;
	int b;
	do
	{
		b=fgetc(f1);
		if(b=='&') counter++;
	}
	while(b!=EOF);
	rewind(f1);
	for(int i=0;i<counter;i++)
	{
		fscanf(f1,"& %lf %d %d %s\n",&amount,&method,&repeat,description);
		for(int i=0;i<40;i++)
		{
			if(description[i]=='#') description[i]=' ';
		}
		n->plus=newTransaction(n->plus, amount, method, description, repeat);
	}
	fclose(f1);
	name[len]='m';
	f1=fopen(name, "r");
	if(f1==NULL) return n;
	counter=0;
	do
	{
		b=fgetc(f1);
		if(b=='&') counter++;
	}
	while(b!=EOF);
	rewind(f1);
	for(int i=0;i<counter;i++)
	{
		fscanf(f1,"& %lf %d %d %s\n",&amount,&method,&repeat,description);
		for(int i=0;i<40;i++)
		{
			if(description[i]=='&') description[i]=' ';
		}
		n->minus=newTransaction(n->minus, amount, method, description, repeat);
	}
	fclose(f1);
	return n;
}

void updateData()
{
	FILE *f1=fopen("data","r");
	int i,d,m,y;
	if(f1!=NULL)
	{
		fscanf(f1,"%d %d %d %d",&i,&d,&m,&y);
		fclose(f1);
		f1=fopen("data","w");
		if(f1!=NULL)
		{
			fprintf(f1,"%d %d %d %d",i+1,d,m,y);
			fclose(f1);
		}
	}
}

void deleteEverything()
{
	FILE *f1=fopen("data","r");
	if(f1==NULL) return;
	int n;
	fscanf(f1,"%d",&n);
	fclose(f1);
	for(int i=0;i<=n;i++)
	{
		int len;
		if(i==0) len=1;
		else len=floor(log10(i))+1;
		char name[len+1];
		sprintf(name, "%d", i);
		name[len]='p';
		for(unsigned int i=len+1;i<strlen(name);i++)
		{
			name[i]='\0';
		}
		remove(name);
		name[len]='m';
		remove(name);
	}
	remove("data");
	remove("methods");
	chdir("..");
	remove("zapisy");
}

void createMethods()
{
	FILE *f1=fopen("methods","w");
	if(f1==NULL) return;
	fprintf(f1,"0 gotowka\n1 oszczednosci\n2 -\n3 -\n4 -\n5 -\n6 -\n7 -\n8 -\n9 -\n");
	fclose(f1);
}

void saveMethods(char n[10][20])
{
	FILE *f1=fopen("methods","w");
	if(f1==NULL) return;
	for(int i=0;i<10;i++)
	{
		fprintf(f1,"%d %s\n",i,n[i]);
	}
	fclose(f1);
}
