//
// Created by wuxw on 2024/6/25/025.
//

#ifndef CMAKE2VS_STS_USER_DEF_H
#define CMAKE2VS_STS_USER_DEF_H

typedef struct {
  int rid;
  char name[32];
  char uscc[18];  //统一社会信用代码 Unified Social Credit Code
  char tin[18];   //纳税人识别号 tax id no
  bool indi;
  int type;
} Holder;

#endif  //CMAKE2VS_STS_USER_DEF_H
