//
// Created by wuxw on 2024/3/14/014.
//

#include <sqlite3pp/sqlite3pp.h>
#include <iostream>
#include <memory>
#include <ostream>
#include <string>
#include <vector>

struct SvrSite {
  std::string name;
  std::string address;
  int port;
  std::string protocol;
  std::string addr_type;
};

class DataStore {
 public:
  static DataStore& getInstance() {
    static DataStore instance;
    return instance;
  }

  bool addConfig(const SvrSite& site) {
    try {
      sqlite3pp::command cmd(*m_db, "INSERT INTO app_sites (name, address, port, protocol, addr_type) VALUES (?, ?, ?, ?, ?)");
      cmd.bind(1, site.name, sqlite3pp::nocopy);
      cmd.bind(2, site.address, sqlite3pp::nocopy);
      cmd.bind(3, site.port);
      cmd.bind(4, site.protocol, sqlite3pp::nocopy);
      cmd.bind(5, site.addr_type, sqlite3pp::nocopy);
      cmd.execute();
      return true;
    } catch (std::exception& e) {
      return false;
    }
  }

  std::vector<SvrSite> getAllConfigs() {
    std::vector<SvrSite> sites;
    sqlite3pp::query query(*m_db, "SELECT name, address, port, protocol, addr_type FROM app_sites");
    for (auto&& row : query) {
      SvrSite site;
      row.getter() >> site.name >> site.address >> site.port >> site.protocol >> site.addr_type;
      sites.push_back(site);
    }
    return sites;
  }

  bool updateConfig(const SvrSite& site) {
    try {
      sqlite3pp::command cmd(*m_db, "UPDATE app_sites SET address=?, port=?, protocol=?, addr_type=? WHERE name=?");
      cmd.bind(1, site.address, sqlite3pp::nocopy);
      cmd.bind(2, site.port);
      cmd.bind(3, site.protocol, sqlite3pp::nocopy);
      cmd.bind(4, site.addr_type, sqlite3pp::nocopy);
      cmd.bind(5, site.name, sqlite3pp::nocopy);
      cmd.execute();
      return true;
    } catch (std::exception& e) {
      return false;
    }
  }

  bool deleteConfig(const std::string& name) {
    try {
      sqlite3pp::command cmd(*m_db, "DELETE FROM app_sites WHERE name=?");
      cmd.bind(1, name, sqlite3pp::nocopy);
      cmd.execute();
      return true;
    } catch (std::exception& e) {
      return false;
    }
  }

 private:
  DataStore() {
    m_db = std::make_unique<sqlite3pp::database>("app_sites.db");
    m_db->execute("CREATE TABLE IF NOT EXISTS app_sites (name TEXT PRIMARY KEY, address TEXT, port INTEGER, protocol TEXT, addr_type TEXT)");
  }

  std::unique_ptr<sqlite3pp::database> m_db;
};

int main() {
  DataStore& dataStore = DataStore::getInstance();

  SvrSite site1 = {"server1", "127.0.0.1", 8080, "http", "IPv4"};
  dataStore.addConfig(site1);

  SvrSite site2 = {"server2", "192.168.1.2", 9909, "tcp", "IPv4"};
  dataStore.addConfig(site2);

  std::vector<SvrSite> sites = dataStore.getAllConfigs();
  for (const auto& site : sites) {
    std::cout << "Name: " << site.name << ", Address: " << site.address << ", Port: " << site.port
              << ", Protocol: " << site.protocol << ", Address Type: " << site.addr_type << std::endl;
  }

  site1.port = 9090;
  dataStore.updateConfig(site1);

//  dataStore.deleteConfig("server1");

  return 0;
}
