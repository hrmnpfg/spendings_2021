typedef struct time
{
	int day;
	int month;
	int year;
} structTime;

typedef struct transaction  //struktura poszczególnych płatności lub dochodów
{
	double amount; //kwota
	int method; //numer metody płatności
	char *description; //krótki opis
	int repeat; //czy jest to płatność okresowa
	struct transaction *next; //wskaźnik na następną transakcję
} structTransaction;
typedef struct transaction *Transaction;

typedef struct day //struktura poszczególnych dni
{
	long dayId; //sposób na indeksowanie i znajdywanie poszczególnych dni
	struct time dayDate; //zapis daty
	struct day *next; //wskaźnik na następny dzień
	struct day *previous; //wskaźnik na poprzedni dzień
	Transaction plus; //wskaźnik na listę przychodów
	Transaction minus; //wskaźnik na listę wydatków
} structDay;
typedef struct day *Day;

//zarządzanie strukturą
Day createDay();
Day newDay(Day n);
void freeDay(Day n);
Transaction createTransaction();
Transaction newTransaction(Transaction n, double amount, int method, char description[], int repeat);
void freeTransaction(Transaction n);
void addRepeats(Day n);

struct time incrementTime(struct time);

//obliczenia
double totalAmount(Day n);
double totalDay(Day n);
double periodAmount(Day n, int c);
double sumPlusMethod(Day n, int method);
double sumMinusMethod(Day n, int method);
double sumPlusMethodPeriod(Day n, int method, int c);
double sumMinusMethodPeriod(Day n, int method, int c);

//zarządzanie plikami
void saveDay(Day n);
Day loadDay(Day n);
void updateData();
void deleteEverything();
void createMethods();
void saveMethods(char n[10][20]);
