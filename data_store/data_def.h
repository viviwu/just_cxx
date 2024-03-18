//
// Created by wuxw on 2024/3/15/015.
//

#ifndef JUST_CXX_DATA_DEF_H
#define JUST_CXX_DATA_DEF_H

#include <memory>
#include <string>
#include <vector>

struct SvrSite {
  int id;
  std::string name;
  std::string address;
  int port;
  std::string protocol;
  std::string addr_type;

  SvrSite(){}

  SvrSite(std::string _name, std::string _address, int _port, std::string _protocol,
          std::string _addr_type)
      : id(0),
        name(_name),
        address(_address),
        port(_port),
        protocol(_protocol),
        addr_type(_addr_type) {}
};

typedef std::shared_ptr<SvrSite> SvrSitePtr;
typedef std::vector<SvrSitePtr> SvrSites;

#endif  //JUST_CXX_DATA_DEF_H
