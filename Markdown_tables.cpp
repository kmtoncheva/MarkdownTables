#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include "constants.h"
#include "Markdown_tables.h"
#pragma warning(disable : 4996)

void Error() {
    std::cout << "Error" << std::endl;
}
bool checkWord(const char* name) {
    return (name != nullptr && strlen(name) < WORD_MS);
}

Row MarkdownTable::getRow(std::ifstream& ifs) const {
    char buff[BUFF_MS];
    char word[WORD_MS + 1] = {}; 
    ifs.getline(buff, BUFF_MS);
    std::stringstream strBuffer(buff);
    strBuffer.get();
    Row row;
    while (strBuffer >> word) {
        if (strcmp(word, cellBorder) == 0) {
            row.columnCounter++;
        }
        if(strcmp(word, cellBorder) != 0){
            if (strlen(row.columns[row.columnCounter]) <= 0) {
                strcat(row.columns[row.columnCounter], word);
            }
            else {
                char currWord[WORD_MS + 1] = {'\0'}; 
                strcat(currWord, word);
                strcat(row.columns[row.columnCounter], currWord);
            }
        }
    }
    return row;
}

size_t MarkdownTable::numberOfRow(size_t columnNumber, const char* word) const {
    size_t number = -1;
    for (size_t j = 2; j < rowCounter; ++j)
    {
        if (strcmp(word, row[j].columns[columnNumber]) == 0) {
            number = j;
            break;
        }
    }
    return number;
}

size_t MarkdownTable::numberOfColumn(const char* name) const {
    size_t number = -1; 
    for (size_t j = 0; j < row[0].columnCounter; ++j)
    {
        if (strcmp(name, row[0].columns[j]) == 0)
        {
            number = j;
            break;
        }
    }
    return number;
}

void MarkdownTable::displayRows(size_t number) const {
    std::cout << cellBorder;
    if (number == 1) {
        for (size_t i = 0; i < row[number].columnCounter; i++)
        {
            std::cout << std::setfill(filling) << std::setw(WORD_MS + 3) << cellBorder;
        }
        std::cout << std::endl << std::setfill(space);
        return;
    }
    for (size_t i = 0; i < row[number].columnCounter; i++)
    {
        size_t paddRight = WORD_MS - strlen(row[number].columns[i]) + 3;
        size_t paddLeft = 0;
        
        if (index[i] == 1) {
            paddRight = 0;
            paddLeft = WORD_MS;
        }
        if (index[i] == 2) {
            paddRight = (WORD_MS - strlen(row[number].columns[i])) / 2 + 3;
            paddLeft = paddRight + strlen(row[number].columns[i]) - 3;
            if (strlen(row[number].columns[i]) % 2 != 0) paddRight += 1;
        }
        std::cout << std::setw(paddLeft) << row[number].columns[i] << std::setw(paddRight) << cellBorder;
    }
    std::cout << std::endl;
}

void MarkdownTable::setRow(size_t number, std::ofstream& ofs) const {
    ofs << cellBorder;
    if (number == 1) {
        for (size_t i = 0; i < row[number].columnCounter; i++)
        {
            char symbol = filling;
            if (index[i] == 0 || index[i] == 2) {
                symbol = alignmentColon;
            }
            ofs << std::cout.fill(filling) << symbol;
            if (index[i] == 1 || index[i] == 2) {
                symbol = alignmentColon;
            }
            else symbol = filling;
            ofs << std::setw(WORD_MS - 1) << symbol << cellBorder;
        }
        ofs << std::setfill(space);
        return;
    }
    for (size_t i = 0; i < row[number].columnCounter; i++)
    {
        size_t paddRight = WORD_MS - strlen(row[number].columns[i]) + 3;
        size_t paddLeft = 0;

        if (index[i] == 1) {
            paddLeft = WORD_MS;
            paddRight = 0;
        }
        if (index[i] == 2)
        {
            paddRight = (WORD_MS - strlen(row[number].columns[i])) / 2 + 3;
            paddLeft = paddRight + strlen(row[number].columns[i]) - 3;

            if (strlen(row[number].columns[i]) % 2 != 0) paddRight += 1;
        }
        ofs << std::setw(paddLeft) << row[number].columns[i] << std::setw(paddRight) << cellBorder;
    }
}

void MarkdownTable::openMarkdownTable(const char* name) 
{
    if (name == nullptr)
    {
        Error();
        return; 
    }
    rowCounter = 0;
    std::ifstream ifs(name, std::ios::in);
    if (!ifs.is_open())
    {
        Error();
        return;
    }
    while (!ifs.eof())
    {
        row[rowCounter++] = getRow(ifs);
    }
    for (size_t i = 0; i < row[0].columnCounter; i++)
    {
        if (row[1].columns[i][strlen(row[1].columns[i]) - 1] == alignmentColon) {
            index[i] = 1;
        }
        if (row[1].columns[i][0] == alignmentColon && row[1].columns[i][strlen(row[1].columns[i]) - 1] == alignmentColon) {
            index[i] = 2;
        }
    }
    ifs.close();
}

void MarkdownTable::displayTable() const {
    for (size_t i = 0; i < rowCounter; i++)
    {
        displayRows(i);
    }
    std::cout << std::endl;
}

void MarkdownTable::changeName(const char* origin,const char* result) {
    if (!checkWord(origin) ||  !checkWord(result))
    {
        Error();
    }
    else {
        
        for (size_t i = 0; i < row[0].columnCounter; i++)
        {
            if (strcmp(row[0].columns[i], origin) == 0) {
                strcpy(row[0].columns[i], result);
                return;
            }
        }
        Error();
    }
}

void MarkdownTable::addRow(char input[][WORD_MS + 1]) {
    if (input != nullptr)
    {
        Row add;
        for (size_t i = 0; i < row[0].columnCounter; i++)
        {
            strcpy(add.columns[i], input[i]);
        }
        add.columnCounter = row[0].columnCounter;
        row[rowCounter++] = add;
    }
    else {
        Error();
    }
}

void MarkdownTable::changeDueToNumOfRow(const char* nameOfCol, size_t numOfRow, const char* newWord) {
    if (checkWord(nameOfCol) && checkWord(newWord) && numOfRow > 0 && numOfRow <= rowCounter) {
        size_t columnCurr = numberOfColumn(nameOfCol);
        strcpy(row[numOfRow + 1].columns[columnCurr], newWord);
    }
    else { Error(); }
}

void MarkdownTable::changeDueToWordOfRow(const char* nameOfCol, const char* oldWord, const char* newWord) {
    if (!checkWord(nameOfCol) || !checkWord(oldWord) || !checkWord(newWord))
    {
        Error();

    }
    else {
        size_t columnCurr = numberOfColumn(nameOfCol);
        size_t rowCurr = numberOfRow(columnCurr, oldWord);
        strcpy(row[rowCurr].columns[columnCurr], newWord);
    }
}

void MarkdownTable::selectDueToWord(const char* nameOfCol, const char* word) const {
    if (!checkWord(nameOfCol) || !checkWord(word))
    {
        Error();
    }
    else {
        size_t columnCurr = numberOfColumn(nameOfCol); 
        for (size_t i = 0; i < rowCounter; i++)
        {
            if (strcmp(word, row[i].columns[columnCurr]) != 0 && i != 0 && i != 1)
            {
                continue;
            }
            displayRows(i);
        }
        std::cout << std::endl;
    }
}

void MarkdownTable::setTable(const char* name) const {
    if (name!= nullptr)
    {
        std::ofstream ofs(name, std::ios::out | std::ios::out);
        if (!ofs.is_open())
        {
            Error();
            return;
        }
        ofs << std::endl;
        for (size_t i = 0; i < rowCounter; i++)
        {
            setRow(i, ofs);
            if (i != rowCounter - 1) ofs << std::endl;
        }
        ofs.close();
        std::cout << "File successfully saved!" << std::endl;
    }
    else {
        Error();
    }
}




