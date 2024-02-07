//
// Created by dejavu on 2023/6/13.
//

#include <iostream>
using namespace std;

double divide(double x, double y)
{
    if (y == 0)
    {
        throw "exception: Divide by zero !!";
    }
    return x / y;
}

int main()
{
    double a, b, result;
    cout << "Enter two numbers: ";
    cin >> a >> b;
    try
    {
        result = divide(a, b);
        cout << "Result = " << result << endl;
    }
    catch (const char* msg)
    {
        cerr << "Error: " << msg << endl;
    }
    return 0;
}