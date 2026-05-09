#include <iostream>
#include <fstream>
#include <new>
#include <iomanip>

#include "plane.h"

using namespace std;


int RunProgram(const char filename[]) {
    Plane* planes = nullptr;
    int count = 0;

    int readResult = ReadFileDynamic(filename, planes, count);

    if (readResult != READ_SUCCESS) {
        PrintReadError(readResult);
        delete[] planes;
        return 1;
    }

    if (count == 0) {
        cout << "There are no correct records in file." << endl;
        delete[] planes;
        return 0;
    }

    int* index = new(nothrow) int[count];

    if (index == nullptr) {
        cout << "Error: memory was not allocated for index array." << endl;
        delete[] planes;
        return 1;
    }

    if (!CreateIndexArr(index, count)) {
        cout << "Error: index array was not created." << endl;
        delete[] index;
        delete[] planes;
        return 1;
    }

    if (!BubbleSort(planes, count, index)) {
        cout << "Error: sorting failed." << endl;
        delete[] index;
        delete[] planes;
        return 1;
    }

    if (!PrintSorted(planes, count, index)) {
        cout << "Error: printing failed." << endl;
        delete[] index;
        delete[] planes;
        return 1;
    }

    delete[] index;
    delete[] planes;

    return 0;
}

void PrintReadError(int errorCode) {
    if (errorCode == ERR_FILE_OPEN) {
        cout << "Error: file was not opened." << endl;
    }
    else if (errorCode == ERR_MEMORY) {
        cout << "Error: memory was not allocated." << endl;
    }
    else if (errorCode == ERR_WRONG_ARGUMENTS) {
        cout << "Error: wrong function arguments." << endl;
    }
    else {
        cout << "Unknown error." << endl;
    }
}

bool IsDigit(char symbol) {
    return symbol >= '0' && symbol <= '9';
}

int StrLen(const char str[]) {
    if (str == nullptr) return -1;

    int len = 0;

    while (str[len] != '\0') {
        len++;
    }

    return len;
}

bool CopyIfFits(char destination[], int destinationSize, const char source[]) {
    if (destination == nullptr) return false;
    if (source == nullptr) return false;
    if (destinationSize <= 0) return false;

    int length = StrLen(source);

    if (length < 0) return false;

    // Нужно место ещё и под '\0'
    if (length >= destinationSize) {
        return false;
    }

    for (int i = 0; i <= length; i++) {
        destination[i] = source[i];
    }

    return true;
}

bool IsTimeCorrect(const char time[]) {
    if (time == nullptr) return false;

    // Формат строго HH:MM
    if (!IsDigit(time[0])) return false;
    if (!IsDigit(time[1])) return false;
    if (time[2] != ':') return false;
    if (!IsDigit(time[3])) return false;
    if (!IsDigit(time[4])) return false;
    if (time[5] != '\0') return false;

    int hours = (time[0] - '0') * 10 + (time[1] - '0');
    int minutes = (time[3] - '0') * 10 + (time[4] - '0');

    if (hours > 23) return false;
    if (minutes > 59) return false;

    return true;
}

int TimeToMinutes(const char time[]) {
    if (!IsTimeCorrect(time)) return -1;

    int hours = (time[0] - '0') * 10 + (time[1] - '0');
    int minutes = (time[3] - '0') * 10 + (time[4] - '0');

    return hours * 60 + minutes;
}

bool IsBoardNumberCorrect(const char boardnum[]) {
    if (boardnum == nullptr) return false;

    int length = 0;
    int dashPosition = -1;

    while (boardnum[length] != '\0') {
        if (boardnum[length] == '-') {
            if (dashPosition != -1) return false;
            dashPosition = length;
        }

        length++;

        if (length >= BOARD_NUMBER_SIZE) {
            return false;
        }
    }

    if (length == 0) return false;
    if (dashPosition == -1) return false;
    if (dashPosition == 0) return false;
    if (dashPosition == length - 1) return false;

    // До дефиса не должно быть цифр
    for (int i = 0; i < dashPosition; i++) {
        if (IsDigit(boardnum[i])) {
            return false;
        }
    }

    // После дефиса должны быть только цифры
    for (int i = dashPosition + 1; i < length; i++) {
        if (!IsDigit(boardnum[i])) {
            return false;
        }
    }

    return true;
}

bool EnsurePlaneCapacity(Plane*& planes, int& capacity, int count) {
    if (planes == nullptr) return false;
    if (capacity <= 0) return false;
    if (count < 0) return false;

    if (count < capacity) {
        return true;
    }

    int newCapacity = capacity * 2;

    if (newCapacity <= capacity) {
        return false;
    }

    Plane* newPlanes = new(nothrow) Plane[newCapacity];

    if (newPlanes == nullptr) {
        return false;
    }

    for (int i = 0; i < count; i++) {
        newPlanes[i] = planes[i];
    }

    delete[] planes;

    planes = newPlanes;
    capacity = newCapacity;

    return true;
}

int ReadFileDynamic(const char filename[], Plane*& planes, int& count) {
    if (filename == nullptr) {
        return ERR_WRONG_ARGUMENTS;
    }

    ifstream file(filename);

    if (!file.is_open()) {
        return ERR_FILE_OPEN;
    }

    int capacity = START_CAPACITY;
    count = 0;

    planes = new(nothrow) Plane[capacity];

    if (planes == nullptr) {
        file.close();
        return ERR_MEMORY;
    }

    char timeBuffer[TEMP_SIZE];
    char modelBuffer[TEMP_SIZE];
    char boardBuffer[TEMP_SIZE];
    char departureBuffer[TEMP_SIZE];

    int recordNumber = 1;

    while (file >> setw(TEMP_SIZE) >> timeBuffer
        >> setw(TEMP_SIZE) >> modelBuffer
        >> setw(TEMP_SIZE) >> boardBuffer
        >> setw(TEMP_SIZE) >> departureBuffer) {

        Plane temp;
        bool isCorrect = true;

        bool timeCopied = CopyIfFits(temp.time, TIME_SIZE, timeBuffer);
        bool modelCopied = CopyIfFits(temp.model, MODEL_SIZE, modelBuffer);
        bool boardCopied = CopyIfFits(temp.boardNumber, BOARD_NUMBER_SIZE, boardBuffer);
        bool departureCopied = CopyIfFits(temp.departure, DEPARTURE_SIZE, departureBuffer);

        if (!timeCopied) {
            cout << "[ERROR] Record " << recordNumber << ": time is too long." << endl;
            isCorrect = false;
        }

        if (!modelCopied) {
            cout << "[ERROR] Record " << recordNumber << ": model is too long." << endl;
            isCorrect = false;
        }

        if (!boardCopied) {
            cout << "[ERROR] Record " << recordNumber << ": board number is too long." << endl;
            isCorrect = false;
        }

        if (!departureCopied) {
            cout << "[ERROR] Record " << recordNumber << ": departure is too long." << endl;
            isCorrect = false;
        }

        if (timeCopied && !IsTimeCorrect(temp.time)) {
            cout << "[ERROR] Record " << recordNumber << ": wrong time." << endl;
            isCorrect = false;
        }

        if (boardCopied && !IsBoardNumberCorrect(temp.boardNumber)) {
            cout << "[ERROR] Record " << recordNumber << ": wrong board number." << endl;
            isCorrect = false;
        }

        if (isCorrect) {
            if (!EnsurePlaneCapacity(planes, capacity, count)) {
                file.close();
                return ERR_MEMORY;
            }

            temp.minutes = TimeToMinutes(temp.time);

            if (temp.minutes == -1) {
                cout << "[ERROR] Record " << recordNumber << ": wrong time." << endl;
            }
            else {
                planes[count] = temp;
                count++;
            }
        }

        recordNumber++;
    }

    file.close();

    return READ_SUCCESS;
}

bool CreateIndexArr(int index[], int count) {
    if (index == nullptr) return false;
    if (count <= 0) return false;

    for (int i = 0; i < count; i++) {
        index[i] = i;
    }

    return true;
}

bool BubbleSort(const Plane* planes, int count, int index[]) {
    if (planes == nullptr) return false;
    if (index == nullptr) return false;
    if (count <= 0) return false;

    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - 1 - i; j++) {
            int leftIndex = index[j];
            int rightIndex = index[j + 1];

            if (leftIndex < 0 || leftIndex >= count) return false;
            if (rightIndex < 0 || rightIndex >= count) return false;

            if (planes[leftIndex].minutes > planes[rightIndex].minutes) {
                int temp = index[j];
                index[j] = index[j + 1];
                index[j + 1] = temp;
            }
        }
    }

    return true;
}

bool PrintSorted(const Plane* planes, int count, const int index[]) {
    if (planes == nullptr) return false;
    if (index == nullptr) return false;
    if (count <= 0) return false;

    cout << endl;

    for (int i = 0; i < 80; i++) cout << '-';
    cout << endl;

    cout << left
        << setw(12) << "Time"
        << setw(20) << "Model"
        << setw(20) << "Board number"
        << setw(25) << "Departure"
        << endl;

    for (int i = 0; i < 80; i++) cout << '-';
    cout << endl;

    for (int i = 0; i < count; i++) {
        int currentIndex = index[i];

        if (currentIndex < 0 || currentIndex >= count) {
            return false;
        }

        cout << left
            << setw(12) << planes[currentIndex].time
            << setw(20) << planes[currentIndex].model
            << setw(20) << planes[currentIndex].boardNumber
            << setw(25) << planes[currentIndex].departure
            << endl;
    }

    for (int i = 0; i < 80; i++) cout << '-';
    cout << endl;

    return true;
}