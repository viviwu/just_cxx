//
// Created by wuxw on 2024/3/15/015.
//

#include <memory>
#include "data_store.h"

int main() {
  DataStore& data_store = DataStore::getInstance();
  data_store.initStore("./app_sites.db");

  SvrSitePtr site1 =
      std::make_shared<SvrSite>("server1", "127.0.0.1", 8080, "http", "IPv4");
  site1->id = data_store.PersistSync(site1);
  std::cout << "id: " << site1->id << std::endl;

  SvrSitePtr site2 =
      std::make_shared<SvrSite>("server2", "192.168.1.2", 9909, "tcp", "IPv4");
  data_store.PersistSync(site2);

  SvrSites sites;
  data_store.QuerySync(&sites);
  std::cout << "sites.size:" << sites.size() << std::endl;
  for (const auto& site : sites) {
    std::cout << "Name: " << site->name << ", Address: " << site->address
              << ", Port: " << site->port << ", Protocol: " << site->protocol
              << ", Address Type: " << site->addr_type
              << "site->id: " << site->id << std::endl;
  }

  site1->port = 9090;
  data_store.UpdateSync(site1);

  data_store.DeleteSync(site1);

  return 0;
}
