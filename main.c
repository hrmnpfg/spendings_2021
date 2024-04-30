//w tym pliku znajduje się główna funkcja programu odpowiedzialna za interfejs

#include "ncursesw/ncurses.h"
#include <stdlib.h>
#include <string.h>
#include "header.h"
#include <unistd.h> 
#include <sys/stat.h>
#include <sys/types.h>
#include <locale.h>
//Michał Łukasik 330623
int main()
{
	setlocale(LC_ALL, "");
	mkdir("zapisy", 0777);
	chdir("zapisy");
	initscr();
	cbreak();
	keypad(stdscr,TRUE);
	int mx=0, my=0;
	getmaxyx(stdscr, my, mx);
	if(my<20||mx<80)
	{
		move(0,0);
		printw("zbyt male okno, rozmiar powinien wynosic przynajmniej 20x80");
		move(1,0);
		printw("aktualny rozmiar: %dx%d",my,mx);
		move(20,80);
		printw(".");
		refresh();
		getch();
		endwin();
		exit(0);
	}
	refresh();
	Day n=NULL;
	FILE *f1=fopen("data", "r");
	if(f1!=NULL)
	{
		int scan,day,month,year;
		fscanf(f1,"%d %d %d %d",&scan,&day,&month,&year);
		for(int i=0;i<=scan;i++)
		{
			n=newDay(n);
			n=loadDay(n);
			if(i==0)
			{
				n->dayDate.day=day;
				n->dayDate.month=month;
				n->dayDate.year=year;
			}
		}
		fclose(f1);
	}
	else
	{
		n=newDay(n);
		printw("Startowa data: (format dd/mm/yyyy)\n");
		scanw("%d/%d/%d",&n->dayDate.day,&n->dayDate.month,&n->dayDate.year);
		f1=fopen("data", "w");
		if(f1==NULL)
		{
			printw("\nNieudany zapis do pliku");
			exit(0);
		}
		fprintf(f1,"%d %d %d %d",0,n->dayDate.day,n->dayDate.month,n->dayDate.year);
		fclose(f1);
		double start;
		printw("Startowa kwota:\n");
		scanw("%lf",&start);
		n->plus=newTransaction(n->plus,start,0,"Poczatkowa kwota",0);
		saveDay(n);
	}
	char methods[10][20];
	f1=fopen("methods","r");
	if(f1==NULL)
	{
		createMethods();
		f1=fopen("methods","r");
	}
	int f=0;
	
	for(int i=0;i<10;i++)
	fscanf(f1,"%d %s",&f,methods[i]);
	fclose(f1);
	
	clear();
	refresh();
	
	
	
	int width;
	while(1)
	{	
		width=mx/6;
		move(1,5*width);
		printw("nowy dzien");
		move(2,5*width);
		printw("nacisnij n");
		Day temp=n;
		for(int i=4;i>=0;i--)
		{
			move(0,i*width);
			if(temp!=NULL) printw("%d",temp->dayDate.year);
			move(1,i*width);
			if(temp!=NULL) printw("%d/%d",temp->dayDate.day,temp->dayDate.month);
			move(2,i*width);
			if(temp!=NULL) printw("%.2f",totalDay(temp));
			move(3,i*width);
			if(temp!=NULL&&temp->previous!=NULL) printw("zmiana o:");
			move(4,i*width);
			if(temp!=NULL&&temp->previous!=NULL) printw("%.2f",totalDay(temp)-totalDay(temp->previous));
			if(temp!=NULL) temp=temp->previous;
		}
		for(int i=0;i<mx;i++)
		{
			move(5,i);
			printw("-");
		}
		for(int i=6;i<my;i++)
		{
			move(i,4.5*width);
			printw("|");
		}
		move(7,5*width-3);
		printw("aktualny bilans:");
		move(8,5*width-3);
		printw("%.2f",totalAmount(n));
		
		if(strcmp(methods[1],"oszczednosci")==0)
		{
			move(10,5*width-3);
			printw("oszczednosci:");
			move(11,5*width-3);
			printw("%.2f",sumPlusMethod(n,1)-sumMinusMethod(n,1));
		}
		move(7,3*width);
		printw("dodaj traksakcje");
		move(8,3*width);
		printw("nacisnij p");
		
		move(7,2*width);
		printw("usun wpisy");
		move(8,2*width);
		printw("nacisnij u");
		
		move(7,1*width);
		printw("edytuj dni");
		move(8,1*width);
		printw("nacisnij e");
		
		
		move(7,0);
		printw("pokaz dni");
		move(8,0);
		printw("nacisnij s");
		
		move(11,0);
		printw("usun wszystkie dane");
		move(12,0);
		printw("nacisnij t");
		
		move(11,2*width);
		printw("edytuj i pokaz metody platnosci");
		move(12,2*width);
		printw("nacisnij m");
		
		if(n->dayId>14)
		{
			move(14,5*width-3);
			printw("tygodniowa zmiana");
			move(15,5*width-3);
			printw("%.2f",periodAmount(n,7)-(periodAmount(n,14)-periodAmount(n,7)));
		}
		
		move(19,0);
		printw("q lub esc zamyka program");
		
		
		refresh();
		f=getch();
		clear();
		if(f==27||f=='q') break;
		if(f=='n')
		{
			n=newDay(n);
			addRepeats(n);
			saveDay(n);
			updateData(n);
		}
		if(f=='m')
		{
			int flocal;
			clear();
			move(0,0);
			for(int i=0;i<10;i++) printw("%d %s\n",i,methods[i]);
			move(11,0);
			printw("wybierz metode do edytowania");
			double tempMethod;
			for(int i=0;i<10;i++)
			{
				move(i,30);
				tempMethod=sumPlusMethod(n,i);
				if(tempMethod>0) printw("suma przychodow:%.2f",tempMethod);
				move(i,60);
				tempMethod=sumMinusMethod(n,i);
				if(tempMethod>0) printw("suma wydatkow:%.2f",tempMethod);
				if(mx>=120&&n->dayId>=7)
				{
					move(i,90);
					tempMethod=sumPlusMethodPeriod(n,i,7);
					if(tempMethod>0) printw("przychody z tygodnia:%.2f",tempMethod);
				}
				if(mx>=160&&n->dayId>=7)
				{
					move(i,125);
					tempMethod=sumMinusMethodPeriod(n,i,7);
					if(tempMethod>0) printw("wydatki z tygodnia:%.2f",tempMethod);
				}
			}
			move(19,0);
			printw("dowolny klawisz poza wyborem wraca");
			refresh();
			flocal=getch();
			if(flocal<'0'||flocal>'9')
			{
				clear();
				refresh();
				continue;
			}
			else
			{
				clear();
				flocal=flocal-'0';
				move(0,0);
				printw("wpisz nowa nazwe dla metody");
				move(1,0);
				printw("%d %s",flocal,methods[flocal]);
				move(3,0);
				char new[19];
				for(int i=0;i<19;i++)
				{
					new[i]=getch();
					if(new[i]=='\n')
					{
						new[i]='\0';
						break;
					}
				}
				new[19]='\0';
				strcpy(methods[flocal],new);
				saveMethods(methods);
				clear();
				refresh();
			}
		}
		if(f=='p')
		{
			int flocal;
			clear();
			move(0,0);
			for(int i=0;i<10;i++) printw("%d %s\n",i,methods[i]);
			move(0,25);
			printw("wybierz metode dla ktorej chcesz wykonac operacje");
			move(19,0);
			printw("dowolny klawisz poza wyborem wraca");
			refresh();
			flocal=getch();
			if(flocal<'0'||flocal>'9')
			{
				clear();
				refresh();
				continue;
			}
			else
			{
				clear();
				move(0,0);
				flocal=flocal-'0';
				int tempMethod=flocal;
				printw("%s\n0 - transakcja\n1 - wplata",methods[flocal]);
				move(19,0);
				printw("dowolny klawisz poza wyborem wraca");
				refresh();
				flocal=getch();
				if(flocal<'0'||flocal>'1')
				{
					clear();
					refresh();
					continue;
				}
				else
				{
					clear();
					move(0,0);
					printw("podaj kwote\n");
					refresh();
					double tempAmount;
					scanw("%lf",&tempAmount);
					move(2,0);
					printw("czy operacja sie powtarza?\n0 - nie\n1-31 - tak, co tyle dni\n");
					move(5,0);
					refresh();
					int tempRepeat;
					scanw("%d",&tempRepeat);
					move(6,0);
					printw("dodaj krotki opis operacji\n");
					char new[39];
					for(int i=0;i<39;i++)
					{
						new[i]=getch();
						if(new[i]=='\n')
						{
							if(i==0) new[i]='-';
							else new[i]='\0';
							break;
						}
					}
					if(flocal=='0') n->minus=newTransaction(n->minus,tempAmount,tempMethod,new,tempRepeat);
					else n->plus=newTransaction(n->plus,tempAmount,tempMethod,new,tempRepeat);
					clear();
					refresh();
					saveDay(n);
					continue;
				}
			}
		}
		if(f=='s')
		{
			Day temp=n;
			int flocal;
			int width2=mx/3;
			while(1)
			{
				clear();
				move(0,width2);
				printw("%d/%d/%d",temp->dayDate.day,temp->dayDate.month,temp->dayDate.year);
				move(1,width2);
				printw("%.2f",totalDay(temp));
				move(3,width2);
				for(int i=0;i<width2;i++) printw("-");
				move(4,width2);
				printw("aktualnie zaznaczony dzien");
				if(temp->next!=NULL)
				{
					move(0,2*width2);
					printw("%d/%d/%d",temp->next->dayDate.day,temp->next->dayDate.month,temp->next->dayDate.year);
					move(1,2*width2);
					printw("%.2f",totalDay(temp->next));
				}
				else
				{
					move(1,2*width2);
					printw("brak nowszych dni");
				}
				if(temp->previous!=NULL)
				{
					move(0,0);
					printw("%d/%d/%d",temp->previous->dayDate.day,temp->previous->dayDate.month,temp->previous->dayDate.year);
					move(1,0);
					printw("%.2f",totalDay(temp->previous));
				}
				else
				{
					move(1,0);
					printw("brak starszych dni");
				}
				move(18,0);
				printw("n - poprzedni dzien, m - nastepny dzien");
				move(19,0);
				printw("e - szczegoly, dowolny inny klawisz wraca");
				refresh();
				flocal=getch();
				if(flocal=='n')
				{
					if(temp->previous!=NULL) temp=temp->previous;
				 	continue;
				}
				if(flocal=='m')
				{
					if(temp->next!=NULL) temp=temp->next;
				 	continue;
				}
				if(flocal=='e')
				{
					Transaction temp2=temp->plus;
					Transaction temp3=temp->minus;
					while(1)
					{
						clear();
						int flocal2;
						if(temp2!=NULL)
						{
							move(0,mx/2);
							printw("%.2f", temp2->amount);
							move(1,mx/2);
							printw("%s", methods[temp2->method]);
							move(2,mx/2);
							printw("%s", temp2->description);
						}
						else
						{
							move(0,mx/2);
							printw("brak nastepnych wplat");
						}
						if(temp3!=NULL)
						{
							move(0,0);
							printw("%.2f", temp3->amount);
							move(1,0);
							printw("%s", methods[temp3->method]);
							move(2,0);
							printw("%s", temp3->description);
						}
						else
						{
							move(0,0);
							printw("brak nastepnych transakcji");
						}
						move(17,0);
						printw("n - pokaz kolejna transakcje, m - pokaz kolejna wplate");
						move(18,0);
						printw("dowolny inny klawisz wraca");
						refresh();
						flocal2=getch();
						if(flocal2=='n')
						{
							if(temp3!=NULL) temp3=temp3->next;
							continue;
						}
						if(flocal2=='m')
						{
							if(temp2!=NULL) temp2=temp2->next;
							continue;
						}
						else break;
				 	}
				 	continue;
				}
				else
				{
					clear();
					refresh();
					break;
				}
			}
		}
		if(f=='t')
		{
			deleteEverything();
			endwin();
			freeDay(n);
			exit(0);
		}
		if(f=='u')
		{
			clear();
			move(0,0);
			printw("podaj dzien na ktorym chcesz wykonywac operacje (format dd/mm/yyyy)\n");
			refresh();
			int lD=-1,lM=-1,lY=-1;
			scanw("%d/%d/%d",&lD,&lM,&lY);
			Day temp=n;
			int found=-1;
			while(temp!=NULL)
			{
				if(lD==temp->dayDate.day&&lM==temp->dayDate.month&&lY==temp->dayDate.year)
				{
					found=temp->dayId;
					break;
				}
				temp=temp->previous;
			}
			if(found==-1)
			{
				move(2,0);
				printw("niepoprawna data");
				refresh();
				getch();
				clear();
				continue;
			}
			else
			{
				clear();
				Transaction temp2=temp->plus;
				Transaction temp3=temp->minus;
				while(1)
				{
					clear();
					int flocal2;
					if(temp2!=NULL)
					{
						move(0,mx/2);
						printw("%.2f", temp2->amount);
						move(1,mx/2);
						printw("%s", methods[temp2->method]);
						move(2,mx/2);
						printw("%s", temp2->description);
					}
					else
					{
						move(0,mx/2);
						printw("brak nastepnych wplat");
					}
					if(temp3!=NULL)
					{
						move(0,0);
						printw("%.2f", temp3->amount);
						move(1,0);
						printw("%s", methods[temp3->method]);
						move(2,0);
						printw("%s", temp3->description);
					}
					else
					{
						move(0,0);
						printw("brak nastepnych transakcji");
					}
					move(18,0);
					printw("n - pokaz kolejna transakcje, m - pokaz kolejna wplate, dowolny inny klawisz wraca");
					move(19,0);
					printw("j - usuwa transakcje, k - usuwa wplate");
					refresh();
					flocal2=getch();
					if(flocal2=='n')
					{
						if(temp3!=NULL) temp3=temp3->next;
						continue;
					}
					if(flocal2=='m')
					{
						if(temp2!=NULL) temp2=temp2->next;
						continue;
					}
					if(flocal2=='j')
					{
						Transaction temp4=temp->minus;
						if(temp4->next==NULL)
						{
							temp->minus=NULL;
							free(temp4->description);
							free(temp4);
							clear();
							saveDay(temp);
							break;
						}
						while(temp4->next!=temp3) temp4=temp4->next;
						temp4->next=temp3->next;
						free(temp3->description);
						free(temp3);
						clear();
						saveDay(temp);
						break;
					}
					if(flocal2=='k')
					{
						Transaction temp4=temp->plus;
						if(temp4->next==NULL)
						{
							temp->plus=NULL;
							free(temp4->description);
							free(temp4);
							clear();
							saveDay(temp);
							break;
						}
						while(temp4->next!=temp2) temp4=temp4->next;
						temp4->next=temp2->next;
						free(temp2->description);
						free(temp2);
						clear();
						saveDay(temp);
						break;
					}
					else
					{
						clear();
						break;
					}
				 }
			}
		}
		if(f=='e')
		{
			Day temp=n;
			int flocal;
			int width2=mx/3;
			while(1)
			{
				clear();
				move(0,width2);
				printw("%d/%d/%d",temp->dayDate.day,temp->dayDate.month,temp->dayDate.year);
				move(1,width2);
				printw("%.2f",totalDay(temp));
				move(3,width2);
				for(int i=0;i<width2;i++) printw("-");
				move(4,width2);
				printw("aktualnie zaznaczony dzien");
				if(temp->next!=NULL)
				{
					move(0,2*width2);
					printw("%d/%d/%d",temp->next->dayDate.day,temp->next->dayDate.month,temp->next->dayDate.year);
					move(1,2*width2);
					printw("%.2f",totalDay(temp->next));
				}
				else
				{
					move(1,2*width2);
					printw("brak nowszych dni");
				}
				if(temp->previous!=NULL)
				{
					move(0,0);
					printw("%d/%d/%d",temp->previous->dayDate.day,temp->previous->dayDate.month,temp->previous->dayDate.year);
					move(1,0);
					printw("%.2f",totalDay(temp->previous));
				}
				else
				{
					move(1,0);
					printw("brak starszych dni");
				}
				move(18,0);
				printw("n - poprzedni dzien, m - nastepny dzien");
				move(19,0);
				printw("e - wybierz dzien, dowolny inny klawisz wraca");
				refresh();
				flocal=getch();
				if(flocal=='n')
				{
					if(temp->previous!=NULL) temp=temp->previous;
				 	continue;
				}
				if(flocal=='m')
				{
					if(temp->next!=NULL) temp=temp->next;
				 	continue;
				}
				if(flocal=='e')
				{
					Transaction temp2=temp->plus;
					Transaction temp3=temp->minus;
					while(1)
					{
						clear();
						int flocal2;
						if(temp2!=NULL)
						{
							move(0,mx/2);
							printw("%.2f", temp2->amount);
							move(1,mx/2);
							printw("%s", methods[temp2->method]);
							move(2,mx/2);
							printw("%s", temp2->description);
						}
						else
						{
							move(0,mx/2);
							printw("brak nastepnych wplat");
						}
						if(temp3!=NULL)
						{
							move(0,0);
							printw("%.2f", temp3->amount);
							move(1,0);
							printw("%s", methods[temp3->method]);
							move(2,0);
							printw("%s", temp3->description);
						}
						else
						{
							move(0,0);
							printw("brak nastepnych transakcji");
						}
						move(18,0);
						printw("n - pokaz kolejna transakcje, m - pokaz kolejna wplate");
						move(19,0);
						printw("j - edytuje transakcje, k - edytuje wplate, dowolny inny klawisz wraca");
						refresh();
						flocal2=getch();
						if(flocal2=='n')
						{
							if(temp3!=NULL) temp3=temp3->next;
							continue;
						}
						if(flocal2=='m')
						{
							if(temp2!=NULL) temp2=temp2->next;
							continue;
						}
						if(flocal2=='j')
						{
							clear();
							move(0,0);
							printw("zmien kwote, obecna kwota %.2f\n",temp3->amount);
							refresh();
							scanw("%lf",&temp3->amount);
							move(2,0);
							printw("zmien opis operacji\n");
							char new[39];
							for(int i=0;i<39;i++)
							{
								new[i]=getch();
								if(new[i]=='\n')
								{
									if(i==0) new[i]='-';
									else new[i]='\0';
									break;
								}
							}
							strcpy(temp3->description,new);
							saveDay(temp);
							break;
						}
						if(flocal2=='k')
						{
							clear();
							move(0,0);
							printw("zmien kwote, obecna kwota %.2f\n",temp2->amount);
							refresh();
							scanw("%lf",&temp2->amount);
							move(2,0);
							printw("zmien opis operacji\n");
							char new[39];
							for(int i=0;i<39;i++)
							{
								new[i]=getch();
								if(new[i]=='\n')
								{
									if(i==0) new[i]='-';
									else new[i]='\0';
									break;
								}
							}
							strcpy(temp2->description,new);
							saveDay(temp);
							break;
						}
						else break;
				 	}
				 	continue;
				}
				else
				{
					clear();
					refresh();
					break;
				}
			}
		}
	}
	endwin();
	freeDay(n);
}
