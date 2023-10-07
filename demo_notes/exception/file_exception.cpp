//
// Created by dejavu on 2023/6/13.
//

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

class FileException : public exception
{
public:
    FileException(const string& message) : m_message(message) {}
    const char* what() const noexcept override { return m_message.c_str(); }
private:
    string m_message;
};

double readNumberFromFile(const string& filename)
{
    ifstream file(filename);
    if (!file.is_open())
    {
        throw FileException("Cannot open file: " + filename);
    }
    double number;
    if (!(file >> number))
    {
        throw FileException("Invalid number in file: " + filename);
    }
    return number;
}

double divide(double x, double y)
{
    if (y == 0)
    {
        throw runtime_error("Divide by zero exception!");
    }
    return x / y;
}

int main2()
{
    string filename;
    cout << "Enter filename: ";
    cin >> filename;

    try
    {
        double a = readNumberFromFile(filename);
        double b = readNumberFromFile(filename);
        double result = divide(a, b);
        cout << "Result = " << result << endl;
    }
    catch (const FileException& e)
    {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }
    catch (const runtime_error& e)
    {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }
    return 0;
}


int main()
{
    string filename;
    cout << "Enter filename: ";
    cin >> filename;

    try
    {
        ifstream file(filename);
        if (!file.is_open())
        {
            throw runtime_error("Cannot open file: " + filename);
        }
        // 文件操作代码...
    }
    catch (const exception& e)
    {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }

    return 0;
}