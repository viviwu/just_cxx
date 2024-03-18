//
// Created by wuxw on 2024/3/15/015.
//

#include "site_store.h"

bool SiteStore::initStore(sqlite3pp::database* db) {
  db_ = db;
  try{
    db_->execute("CREATE TABLE IF NOT EXISTS svr_info (id INTEGER PRIMARY KEY AUTOINCREMENT, name TEXT, address TEXT, port INTEGER, protocol TEXT, addr_type TEXT)");
  }catch (std::exception& e) {
    std::cerr<<"Failed to create order table, ex: "<<std::endl;
    return false;
  }
  std::cout<<"Succeed to initialize order store!"<<std::endl;
  return true;
}

int SiteStore::createRecord(const SvrSitePtr info) {
  sqlite3pp::transaction xact(*db_);
  try {
    sqlite3pp::command cmd(*db_, "INSERT INTO svr_info (name, address, port, protocol, addr_type) VALUES (?, ?, ?, ?, ?)");
    cmd.bind(1, info->name, sqlite3pp::nocopy);
    cmd.bind(2, info->address, sqlite3pp::nocopy);
    cmd.bind(3, info->port);
    cmd.bind(4, info->protocol, sqlite3pp::nocopy);
    cmd.bind(5, info->addr_type, sqlite3pp::nocopy);
    cmd.execute();
    int64_t id = db_->last_insert_rowid();
    int ret = xact.commit();
    if (ret != 0)
    {
      std::cerr<<"Persist order transaction commint error:"<<ret<<std::endl;
      xact.rollback();
    }
    return id;
  } catch (std::exception& ex) {
    std::cerr<<"Failed to persist order:"<<info->name<<" exception:"<< ex.what();
    xact.rollback();
    return -1;
  }
}

void SiteStore::getAllRecords(SvrSites *sites) {
  sqlite3pp::query query(*db_, "SELECT id, name, address, port, protocol, addr_type FROM svr_info");
  for (auto&& row : query) {
    auto info = std::make_shared<SvrSite>();
    row.getter() >> info->id >> info->name >> info->address >> info->port >> info->protocol >> info->addr_type;
    sites->emplace_back(info);
//    std::cout<<info->name<<info->address<<info->port<<info->protocol<<std::endl;
  }
}


bool SiteStore::updateRecord(const SvrSitePtr info) {
  sqlite3pp::transaction xact(*db_);
  try {
    sqlite3pp::command cmd(*db_, "UPDATE svr_info SET name=?, address=?, port=?, protocol=?, addr_type=? WHERE id=?");
    cmd.bind(1, info->name, sqlite3pp::nocopy);
    cmd.bind(2, info->address,sqlite3pp::nocopy);
    cmd.bind(3, info->port);
    cmd.bind(4, info->protocol,sqlite3pp::nocopy);
    cmd.bind(5, info->addr_type, sqlite3pp::nocopy);
    cmd.bind(6, info->id);
    cmd.execute();
    xact.commit();
    return true;
  } catch (std::exception& e) {
    xact.rollback();
    return false;
  }
}

bool SiteStore::deleteRecord(int id) {
  sqlite3pp::transaction xact(*db_);
  try {
    sqlite3pp::command cmd(*db_, "DELETE FROM svr_info WHERE id=?");
    cmd.bind(1, id);
    cmd.execute();
    xact.commit();
    return true;
  } catch (std::exception& e) {
    xact.rollback();
    return false;
  }
}
