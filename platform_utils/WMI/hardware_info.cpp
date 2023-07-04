//
// Created by dejavu on 2023/7/1.
//
#include <iostream>
#include "hdev_info_query.h"

#define MAX_NO 8
using namespace std;

int main() {

  INT query_type = 2;
  T_DEVICE_PROPERTY *properties = (T_DEVICE_PROPERTY *) malloc(MAX_NO * sizeof(T_DEVICE_PROPERTY));
  memset(properties, ' ', MAX_NO * sizeof(T_DEVICE_PROPERTY));
  INT ret = DeviceInfoQuery(query_type, properties, MAX_NO);
  cout << "DeviceTotal = " << ret << endl;
  if (ret < 0) return ret;

  for (int i = 0; i < MAX_NO; i++) {
    cout << "+++++++++++++++++++" << i << "+++++++++++++++++++" << endl;
    cout <<"DeviceProperty: "<< properties[i].szProperty << endl;
  }

//  getchar();
  return 0;
}