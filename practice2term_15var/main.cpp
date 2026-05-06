#include <iostream>
#include <fstream>

using namespace std;


const char filename[] = "data.txt";

struct Plane {
    char time[6];          // время посадки
    char model[20];        // марка ЛА
    char boardNumber[20];  // бортовой номер
    char departure[30];    // пункт отправления
    int minutes;           // время в минутах
};


//функция проверки корректности данных
// 
bool IsTimeCorrect(const char time[]);

bool IsBoardNumberCorrect(const char boardnum[]);

int SumOfCorrStrings(const char filename[]);
int ReadFile(const char filename[], Plane* planes, int correct_count);
//считали данные, посчитали кол-во ликвидных строк, по кол-ву создаем дин. массив
//функция чтения файла

bool CreateIndexArr(int index[], int count);
bool BubbleSort(const Plane* planes, int count, int index[]);

bool PrintSorted(const Plane* planes, int count, int index[]);

int main() {
	
	return 0;
}

bool IsTimeCorrect(const char time[]) {
    if (time[0] < '0' || time[0] > '9') return false;
    if (time[1] < '0' || time[1] > '9') return false;
    if (time[2] != ':') return false;
    if (time[3] < '0' || time[3] > '9') return false;
    if (time[4] < '0' || time[4] > '9') return false;
    if (time[5] != '\0') return false;

    int hours = (time[0] - '0') * 10 + (time[1] - '0');
    int minutes = (time[3] - '0') * 10 + (time[4] - '0');

    if (hours < 0 || hours > 23) return false;
    if (minutes < 0 || minutes > 59) return false;

    return true;

}