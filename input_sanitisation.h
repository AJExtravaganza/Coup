#include <iostream>
#include "limits.h"

bool getSelection();
int getSelection(int lowerBound, int upperBound);
double getSelection(double lowerBound, double upperBound);


bool getSelection()
{
    std::cout << "Continue? [y/n]: ";
    char input = ' ';
    bool choice = false;
    std::cin >> input;
    input = tolower(input);
    if (std::cin.fail() || !(input == 'y' || input == 'n')) // Zero for debugging.  Use 1 for realsies
    {
        std::cout << "Invalid Selection." << std::endl;
        std::cin.clear();
        std::cin.ignore(INT_MAX, '\n');
        choice = getSelection();
    }
    else if (input == 'y')
    {
        choice = true;
    }
    else if (input == 'n')
    {
        choice = false;
    }

    return choice;
}

int getSelection(int lowerBound, int upperBound)
{
    int input = 0;
    std::cin >> input;
    if (std::cin.fail() || input < lowerBound || input > upperBound)
    {
        std::cout << "Invalid Selection.  Please input [" << lowerBound << "-" << upperBound << "]: ";
        std::cin.clear();
        std::cin.ignore(INT_MAX, '\n');
        input = getSelection(lowerBound, upperBound);
    }

    return input;
}

double getSelection(double lowerBound, double upperBound)
{
    double input = 0;
    std::cin >> input;
    if (std::cin.fail() || input < lowerBound || input > upperBound)
    {
        std::cout << "Invalid Selection.  Please input [" << lowerBound << "-" << upperBound << "]: ";
        std::cin.clear();
        std::cin.ignore(INT_MAX, '\n');
        input = getSelection(lowerBound, upperBound);
    }

    return input;
}
