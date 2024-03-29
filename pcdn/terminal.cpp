#include <arpa/inet.h>
#include <sys/socket.h>
#include <iostream>
#include <string>
#include <thread>
#include <unordered_map>

std::unordered_map<std::string, int>
    peer_connections;  // 存储与其他 Terminal 的连接

void handle_peer_connection(int peer_fd, std::string peer_id) {
  char buffer[1024];
  while (true) {
    int bytes_read = recv(peer_fd, buffer, 1024, 0);
    if (bytes_read <= 0) {
      break;
    }
    buffer[bytes_read] = '\0';
    std::cout << "Received from " << peer_id << ": " << buffer << std::endl;
  }
  close(peer_fd);
  peer_connections.erase(peer_id);
}

int main() {
  std::string self_id;
  std::cout << "Enter your ID: ";
  std::cin >> self_id;

  int tracker_fd = socket(AF_INET, SOCK_STREAM, 0);
  struct sockaddr_in tracker_addr;
  tracker_addr.sin_family      = AF_INET;
  tracker_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
  tracker_addr.sin_port        = htons(8888);

  connect(tracker_fd, (struct sockaddr*)&tracker_addr, sizeof(tracker_addr));

  // 发送自己的 client ID 给 Tracker
  send(tracker_fd, self_id.c_str(), self_id.length(), 0);

  int server_fd = socket(AF_INET, SOCK_STREAM, 0);
  struct sockaddr_in server_addr;
  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sin_family      = AF_INET;
  server_addr.sin_addr.s_addr = INADDR_ANY;
  server_addr.sin_port        = htons(9999);

  bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));
  listen(server_fd, 5);

  std::thread server_thread([&]() {
    while (true) {
      struct sockaddr_in client_addr;
      socklen_t addr_len = sizeof(client_addr);
      int client_fd =
          accept(server_fd, (struct sockaddr*)&client_addr, &addr_len);

      char buffer[1024];
      int bytes_read     = recv(client_fd, buffer, 1024, 0);
      buffer[bytes_read] = '\0';
      std::string peer_id(buffer);

      peer_connections[peer_id] = client_fd;
      std::thread peer_thread(handle_peer_connection, client_fd, peer_id);
      peer_thread.detach();
    }
  });

  std::string command;
  while (true) {
    std::cout << "Enter command (1-8): ";
    std::cin >> command;

    if (command == "1") {
      // 连接 Tracker 注册自身信息 (已完成)
    } else if (command == "2") {
      // 查询可访问的其他 Terminal 信息
      std::string target_id;
      std::cout << "Enter target ID: ";
      std::cin >> target_id;
      send(tracker_fd, target_id.c_str(), target_id.length(), 0);

      char buffer[1024];
      int bytes_read     = recv(tracker_fd, buffer, 1024, 0);
      buffer[bytes_read] = '\0';
      std::string target_ip(buffer);

      std::cout << "Target IP: " << target_ip << std::endl;

    } else if (command == "3") {
      // 连接其他已知的 Terminal
      std::string target_id, target_ip;
      std::cout << "Enter target ID: ";
      std::cin >> target_id;
      std::cout << "Enter target IP: ";
      std::cin >> target_ip;

      int peer_fd = socket(AF_INET, SOCK_STREAM, 0);
      struct sockaddr_in peer_addr;
      peer_addr.sin_family      = AF_INET;
      peer_addr.sin_addr.s_addr = inet_addr(target_ip.c_str());
      peer_addr.sin_port        = htons(9999);

      if (connect(peer_fd, (struct sockaddr*)&peer_addr, sizeof(peer_addr)) ==
          0) {
        send(peer_fd, self_id.c_str(), self_id.length(), 0);
        // 继续上面的 Terminal 客户端代码
      } else if (command == "4") {
        // 向其他 Terminal 通信发信息
        std::string target_id, message;
        std::cout << "Enter target ID: ";
        std::cin >> target_id;
        std::cout << "Enter message: ";
        std::cin.ignore();
        std::getline(std::cin, message);

        auto it = peer_connections.find(target_id);
        if (it != peer_connections.end()) {
          int peer_fd = it->second;
          send(peer_fd, message.c_str(), message.length(), 0);
        } else {
          std::cout << "Target not connected" << std::endl;
        }
      } else if (command == "5") {
        // 向其他 Terminal 发文件数据
        std::string target_id, file_path;
        std::cout << "Enter target ID: ";
        std::cin >> target_id;
        std::cout << "Enter file path: ";
        std::cin >> file_path;

        auto it = peer_connections.find(target_id);
        if (it != peer_connections.end()) {
          int peer_fd = it->second;
          // 发送文件数据
          // ...
        } else {
          std::cout << "Target not connected" << std::endl;
        }
      } else if (command == "6") {
        // 断开与其他 Terminal 的连接
        std::string target_id;
        std::cout << "Enter target ID: ";
        std::cin >> target_id;

        auto it = peer_connections.find(target_id);
        if (it != peer_connections.end()) {
          int peer_fd = it->second;
          close(peer_fd);
          peer_connections.erase(it);
        } else {
          std::cout << "Target not connected" << std::endl;
        }
      } else if (command == "7") {
        // 断开与 Tracker 的连接
        close(tracker_fd);
        std::cout << "Disconnected from Tracker" << std::endl;
      } else if (command == "8") {
        // 退出
        close(server_fd);
        for (auto& pair : peer_connections) {
          close(pair.second);
        }
        peer_connections.clear();
        std::cout << "Exiting..." << std::endl;
        break;
      } else {
        std::cout << "Invalid command" << std::endl;
      }