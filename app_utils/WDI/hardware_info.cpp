//
// Created by dejavu on 2023/7/1.
//
#include <iostream>
#include "WMI_DeviceQuery.h"

#define MAX_NO 8
using namespace std;

int main() {

  INT iQueryType = 0;
  T_DEVICE_PROPERTY *properties = (T_DEVICE_PROPERTY *) malloc(MAX_NO * sizeof(T_DEVICE_PROPERTY));
  memset(properties, ' ', MAX_NO * sizeof(T_DEVICE_PROPERTY));
  INT ret = WMI_DeviceQuery(iQueryType, properties, MAX_NO);
  cout << "ret: " << ret << endl;
  if (ret < 0) return ret;

  for (int i = 0; i < MAX_NO; i++) {
    cout << properties[i].szProperty << endl;
    cout << "+++++++++++++++++++" << i << "+++++++++++++++++++" << endl;
    cout << "szProperty:" << endl;
    for (int j = 0; j < PROPERTY_MAX_LEN; j++) {
      printf("%02X", properties[i].szProperty[j]);
//      if (j == PROPERTY_MAX_LEN - 1) {
//        printf("%02X", properties[i].szProperty[j]);
//      } else
//        printf("%02X-", properties[i].szProperty[j]);
    }
    cout << endl;
  }
  getchar();
  return 0;
}