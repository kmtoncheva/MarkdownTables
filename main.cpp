#include "Markdown_tables.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>

int main()
{
    MarkdownTable example;
    example.openMarkdownTable("table.md");
    example.displayTable();
    example.changeName("Description", "Desc");
    example.displayTable();
    char add[COLUMN_MS][WORD_MS + 1] = { "Hello", "World" };
    example.addRow(add);
    example.displayTable();
    example.changeDueToNumOfRow("Syntax", 3, "new");
    example.displayTable();
    example.changeDueToWordOfRow("Syntax", "Header", "new");
    example.displayTable();
    example.selectDueToWord("Syntax", "new");
    example.setTable("newtable.md");

}