#include <net_common.hpp>

// using namespace boost;

// std::vector<char> vBuffer(20 * 1024);

// void GrabSomeData(asio::ip::tcp::socket &socket)
// {
// 	socket.async_read_some(asio::buffer(vBuffer.data(), vBuffer.size()),
// 		[&](std::error_code ec, std::size_t length)
// 		{
// 			if (!ec)
// 			{
// 				std::cout << "\n\nRead " << length << " bytes \n\n";
// 				for (std::size_t i = 0; i < length; ++i) std::cout << vBuffer[i];

// 				GrabSomeData(socket);
// 			}
// 		}
// 	);
// }

template<typename T>
class A : public std::enable_shared_from_this<A<T>>
{
public:
    T word;
    A(T t):word(t) {}
    std::shared_ptr<A> getptr() {
        // std::shared_ptr<A> smart_ptr = shared_from_this();
        return this->shared_from_this();
    }
};

int main(int argc, char **argv)
{
    auto a = std::make_shared<A<std::string>>("nugget");
    auto a_ptr = a->getptr();

    std::cout << a_ptr->word << std::endl;

    // boost::system::error_code ec;
    
    // // Create a "context" - eesintially platform specific interface
    // asio::io_context context;

    // // Give some fake tasks to asio so the context doesn't finish
    // asio::io_context::work idleWork(context);

    // // Start the context
	// std::thread thrContext = std::thread([&](){ context.run(); });

    // // Get the address of somewhere we wish to connect to
    // // asio::ip::tcp::endpoint endpoint(asio::ip::make_address("93.184.216.34", ec), 80);
    // asio::ip::tcp::endpoint endpoint(asio::ip::make_address("51.38.81.49", ec), 80);

    // // Create a socket, the context will deliver the implementation
    // asio::ip::tcp::socket socket(context);

    // // Tell socket to try and connect
    // socket.connect(endpoint, ec);

    // if (!ec) {
    //     std::cout << "Connect!" << std::endl;
    // }
    // else {
    //     std::cout << "Failed to connect to address:\n" << ec.message() << std::endl;
    // }

	// if (socket.is_open()) {

	// 	GrabSomeData(socket);

	// 	std::string sRequest =
	// 						"GET /index.html HTTP/1.1\r\n"
	// 						"Host: example.com\r\n"
	// 						"Connection: close \r\n\r\n";

    //     socket.write_some(asio::buffer(sRequest.data(), sRequest.size()), ec);

    //     // Program does something else, while handles data transfer in background
    //     using namespace std::chrono_literals;
	// 	std::this_thread::sleep_for(20000ms);

    //     context.stop();
    //     if (thrContext.joinable()) thrContext.join();

    //     // socket.wait(socket.wait_read);

    //     // size_t bytes = socket.available();
	// 	// std::cout << "Bytes Available: " << bytes << std::endl;

	// 	// if (bytes > 0) {

	// 	// 	std::vector<char> vBuffer(bytes);
	// 	// 	socket.read_some(asio::buffer(vBuffer.data(), vBuffer.size()), ec);

	// 	// 	for (auto const c : vBuffer) {std::cout << c;}
	// 	// }

    // }
    // else { std::cout << "Socket can not be opened!" << std::endl;}

	return 0;
}