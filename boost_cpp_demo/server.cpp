#include <boost/asio.hpp>
#include <iostream>
#include <istream>
#include <sstream>
#include <string>

using namespace std;

void session(boost::asio::ip::tcp::socket socket) {
  try {
    std::string message;
    boost::system::error_code ec;

    // 读取客户端消息 >
    while (std::getline(socket, message, '\n'), !ec) {
      // 将消息回显给客户端 >
      socket.write_some(boost::asio::buffer(message + "\n"), ec);
    }
  } catch (std::exception& e) {
    std::cerr << "Exception in thread: " << e.what() << "\n";
  }
}

int main() {
  boost::asio::io_context io_context;
  boost::asio::ip::tcp::acceptor acceptor(
      io_context,
      boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 1234));

  for (;;) {
    boost::asio::ip::tcp::socket socket(io_context);
    acceptor.accept(socket);

    // 在新线程中启动会话 >
    boost::thread t(session, std::move(socket));
    t.detach();
  }

  return 0;
}
