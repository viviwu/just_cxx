//
// Created by dejavu on 2023/6/30.
//

#include <iostream>
#include <cstdlib> // ���� malloc ������ͷ�ļ�
#include "HackMacAddress.h"

#define MAX_NO 10
using namespace std;

 int main() {
   int card_no = MAX_NO;
  // ����ṹ��������ڴ�ռ�
   T_MAC_ADDRESS* devices = (T_MAC_ADDRESS*) malloc(MAX_NO * sizeof(T_MAC_ADDRESS));

   HackMacAddress(0, devices, card_no);
   cout<<"card_no: "<<card_no<<endl;
  // ��������Ľṹ�����鲢���ÿ��Ԫ�ص�ֵ��δ��ʼ����
  for (int i = 0; i < MAX_NO; i++) {
    // cout << "CurrentAddress: " << devices[i].CurrentAddress << ", PermanentAddress: " << devices[i].PermanentAddress << endl;
    cout <<"+++++++++++++++++++"<<i<<"+++++++++++++++++++"<< endl;
    cout <<"PermanentAddress:"<< endl;
    for (int j=0; j<MACADDRESS_BYTELEN;j++){
      if(j==MACADDRESS_BYTELEN-1){
        printf("%02X", devices[i].PermanentAddress[j]);
      }else
        printf("%02X-", devices[i].PermanentAddress[j]);
    }
    cout <<endl<<"CurrentAddress:"<< endl;
    for (int j=0; j<MACADDRESS_BYTELEN;j++){
      if(j==MACADDRESS_BYTELEN-1){
        printf("%02X", devices[i].CurrentAddress[j]);
      }else
        printf("%02X-", devices[i].CurrentAddress[j]);
    }
    cout<<endl;
  }

  // �ͷŷ�����ڴ�ռ�
  free(devices);

  getchar();

  return 0;
}
