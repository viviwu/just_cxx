//
// Created by dejavu on 2023/6/13.
//

#include <iostream>
#include <cmath>
#include <complex>
using namespace std;

// ͨ��ģ��ʵ��
template <typename T>
T square_root(T x)
{
    return sqrt(x);
}

// �������͵�ģ���ػ�ʵ��
template <>
complex<double> square_root(complex<double> x)
{
    double real = x.real();
    double imag = x.imag();
    double r = sqrt(real * real + imag * imag);
    double theta = atan2(imag, real);
    return complex<double>(sqrt(r) * cos(theta / 2), sqrt(r) * sin(theta / 2));
}

int main()
{
    // ����ͨ��ģ��ʵ��
    cout << square_root(4) << endl; // ���2
    cout << square_root(2.25) << endl; // ���1.5

    // ������Ը������͵�ģ���ػ�ʵ��
    complex<double> c(3, 4);
    cout << square_root(c) << endl; // ���(2, 1)
    return 0;
}
/*
������Ĵ����У����Ƕ�����һ����ƽ�����ĺ���ģ��square_root��
 ��ͨ��ģ��ʵ���У�����ʹ��C++��׼���е�sqrt����������ƽ������
 ����Ը������͵�ģ���ػ�ʵ���У�����ʹ�ü����귨�����㸴����ƽ������
���������У����Ƕ�ͨ��ģ��ʵ�ֺ���Ը������͵�ģ���ػ�ʵ�ֽ����˲��ԣ�������˼�������
*/