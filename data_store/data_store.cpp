//
// Created by wuxw on 2024/3/14/014.
//

#include <iostream>
#include <memory>


#include "data_store.h"

DataStore::DataStore() {
  std::cout << __func__ << std::endl;
}

DataStore::~DataStore() {
  std::cout << __func__ << std::endl;
}

bool DataStore::initStore(const char *db_path) {
  if (strlen(db_path)<1){
    std::cerr << "Invalid file path: "<< db_path << std::endl;
    return -1;
  }

  char db_file[256];
  strcpy_s(db_file, 256, db_path);

  try {
    // db_ = std::make_unique<sqlite3pp::database>(db_file);
    db_ = sqlite3pp::database(db_file,
                               SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE);
    std::cout << "Succeed to open database:" << db_file << std::endl;
  } catch (std::exception& ex) {
    std::cerr << "Failed to open database:" << db_file << " error:" << ex.what()
              << std::endl;
    return -2;
  }

  if (!site_store_.initStore(&db_))
    return -3;
}


int DataStore::PersistSync(const SvrSitePtr site){
  std::unique_lock<std::mutex> lock(db_mutex_);
  // Persist all data in a transaction
  return site_store_.createRecord(site);
}

bool DataStore::UpdateSync(const SvrSitePtr site){
  return site_store_.updateRecord(site);
}

bool DataStore::DeleteSync(const SvrSitePtr site){
  return site_store_.deleteRecord(site->id);
}

void DataStore::QuerySync(SvrSites *sites)
{
  site_store_.getAllRecords(sites);
}