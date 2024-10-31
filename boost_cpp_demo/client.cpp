#include <boost/asio.hpp>
#include <iostream>

int main() {
  boost::asio::io_context io_context;
  boost::asio::ip::tcp::resolver resolver(io_context);
  boost::asio::ip::tcp::socket socket(io_context);

  // 解析服务器地址
  auto endpoint_iterator =
      resolver.resolve(boost::asio::ip::tcp::v4(), "localhost", "1234");

  // 连接服务器
  boost::asio::connect(socket, endpoint_iterator);

  for (;;) {
    std::string message;
    std::getline(std::cin, message);

    // 发送消息到服务器
    boost::asio::write(socket, boost::asio::buffer(message + '\n'));

    // 接收服务器回显的消息
    boost::system::error_code ec;
    std::string reply;
    while (std::getline(socket, reply, '\n'), !ec) {
      std::cout << "Server reply: " << reply << std::endl;
    }
  }

  return 0;
}
