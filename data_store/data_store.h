//
// Created by wuxw on 2024/3/15/015.
//

#ifndef JUST_CXX_DATA_STORE_H
#define JUST_CXX_DATA_STORE_H

#include <mutex>
#include "site_store.h"

class DataStore {
 public:
  static DataStore& getInstance() {
    static DataStore instance;
    return instance;
  }

  /** @brief Constructor */
  DataStore();

  /** @brief Destructor */
  ~DataStore();

  bool initStore(const char *db_path="");

  /**
     * @brief Persist SvrSite sync
     */
  int PersistSync(const SvrSitePtr site);

  bool UpdateSync(const SvrSitePtr site);

  bool DeleteSync(const SvrSitePtr site);

  void QuerySync(SvrSites *sites);

 private:
  sqlite3pp::database db_;
  SiteStore site_store_;

  mutable std::mutex          db_mutex_;           ///< Database threads mutex
  mutable std::mutex          mutex_;              ///< Threads mutex
};

#endif  //JUST_CXX_DATA_STORE_H
