#pragma once

#pragma once

#include "constants.h"

struct Plane {
    char time[TIME_SIZE];                  // время посадки
    char model[MODEL_SIZE];                // марка ЛА
    char boardNumber[BOARD_NUMBER_SIZE];   // бортовой номер
    char departure[DEPARTURE_SIZE];        // пункт отправления
    int minutes;                           // время в минутах
};

int RunProgram(const char filename[]);

int ReadFileDynamic(const char filename[], Plane*& planes, int& count);
bool EnsurePlaneCapacity(Plane*& planes, int& capacity, int count);
void PrintReadError(int errorCode);

int StrLen(const char str[]);
bool CopyIfFits(char destination[], int destinationSize, const char source[]);

bool IsDigit(char symbol);
bool IsTimeCorrect(const char time[]);
int TimeToMinutes(const char time[]);
bool IsBoardNumberCorrect(const char boardnum[]);

bool CreateIndexArr(int index[], int count);
bool BubbleSort(const Plane* planes, int count, int index[]);
bool PrintSorted(const Plane* planes, int count, const int index[]);