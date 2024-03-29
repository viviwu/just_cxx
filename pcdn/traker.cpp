#include <arpa/inet.h>
#include <sys/socket.h>
#include <iostream>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>

std::unordered_map<std::string, std::string>
    id_ip_map;  // 存储 client_id 和 IP 映射

void handle_client(int client_fd) {
  char buffer[1024];
  std::string client_id, target_id;

  // 读取 client ID
  int bytes_read     = recv(client_fd, buffer, 1024, 0);
  buffer[bytes_read] = '\0';
  client_id          = buffer;

  // 获取 client IP
  struct sockaddr_in client_addr;
  socklen_t addr_len = sizeof(client_addr);
  getpeername(client_fd, (struct sockaddr*)&client_addr, &addr_len);
  std::string client_ip = inet_ntoa(client_addr.sin_addr);

  // 存储 client_id 和 IP 映射
  id_ip_map[client_id] = client_ip;

  // 读取目标 client ID
  bytes_read         = recv(client_fd, buffer, 1024, 0);
  buffer[bytes_read] = '\0';
  target_id          = buffer;

  // 查找目标 client IP
  std::string target_ip = id_ip_map[target_id];

  // 发送目标 client IP 给请求方
  send(client_fd, target_ip.c_str(), target_ip.length(), 0);

  close(client_fd);
}

int main() {
  int server_fd, client_fd;
  struct sockaddr_in server_addr;

  // 创建 socket
  server_fd = socket(AF_INET, SOCK_STREAM, 0);

  // 设置服务器地址
  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sin_family      = AF_INET;
  server_addr.sin_addr.s_addr = INADDR_ANY;
  server_addr.sin_port        = htons(8888);

  // 绑定socket
  bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));

  // 监听连接
  listen(server_fd, 5);

  std::vector<std::thread> thread_pool;

  while (true) {
    // 接受连接
    struct sockaddr_in client_addr;
    socklen_t addr_len = sizeof(client_addr);
    client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &addr_len);

    // 创建新线程处理客户端请求
    std::thread client_thread(handle_client, client_fd);
    thread_pool.push_back(std::move(client_thread));
  }

  for (auto& thread : thread_pool) {
    thread.join();
  }

  close(server_fd);
  return 0;
}