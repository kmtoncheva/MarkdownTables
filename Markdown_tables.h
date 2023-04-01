#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include "constants.h"

struct Row {
    char columns[COLUMN_MS][WORD_MS + 1] = {}; 
    int columnCounter = 0; 
};

class MarkdownTable {
private:
    Row row[ROWS_MS]; 
    size_t rowCounter = 0;
    size_t index[COLUMN_MS] = {};

public:
    Row getRow(std::ifstream& ifs) const; 
    size_t numberOfRow(size_t columnNumber, const char* word) const;
    size_t numberOfColumn(const char* col) const; 
    void displayRows(size_t number) const;
    void openMarkdownTable(const char* name); 
    void displayTable() const;
    void changeName(const char* origin, const char* result);
    void addRow(char input[][WORD_MS + 1]);
    void changeDueToNumOfRow(const char* nameOfCol, size_t numOfRow, const char* newWord);
    void changeDueToWordOfRow(const char* nameOfCol, const char* oldWord, const char* newWord);
    void selectDueToWord(const char* nameOfCol, const char* word) const;
    void setRow(size_t number, std::ofstream& ofs) const;
    void setTable(const char* name) const;
};

