//
// Created by wuxw on 2024/3/15/015.
//

#ifndef JUST_CXX_SITE_STORE_H
#define JUST_CXX_SITE_STORE_H

#include <string>
#include <vector>
#include <iostream>

#include <sqlite3pp/sqlite3pp.h>
#include "data_def.h"

class SiteStore {

 public:
  // Initialize
  bool initStore(sqlite3pp::database* db);


  long long createRecord(const SvrSitePtr site);

  void getAllRecords(SvrSites *sites);

  bool updateRecord(const SvrSitePtr site);

  bool deleteRecord(int id);

 private:
  sqlite3pp::database* db_;
};

#endif  //JUST_CXX_SITE_STORE_H
