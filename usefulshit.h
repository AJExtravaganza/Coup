#include <iostream>
#include "limits.h"

void getSelection(int &selection, int validSelections);


void getSelection(int &selection, int validSelections)
{
    int input = 0;
    std::cin >> input;
    if (std::cin.fail() || input < 0 || input > validSelections) // Zero for debugging.  Use 1 for realsies
    {
        std::cout << "Invalid Selection.  Please input [1-" << validSelections << "]: ";
        std::cin.clear();
        std::cin.ignore(INT_MAX, '\n');
        getSelection(input, validSelections);
    }
    selection = input;
}
