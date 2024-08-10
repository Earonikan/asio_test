#include "olc_net.hpp"

// using namespace boost;

class CustomClient : public olc::net::client_interface<CustomMsgTypes>
{
public:
	void PingServer()	
	{
		olc::net::message<CustomMsgTypes> msg;
		msg.header.id = CustomMsgTypes::ServerPing;

		// Caution with this...
		auto timeNow = std::chrono::system_clock::now();		

		msg << timeNow;
		Send(msg);
		
	}

	void MessageAll()
	{
		olc::net::message<CustomMsgTypes> msg;
		msg.header.id = CustomMsgTypes::MessageAll;		
		Send(msg);
	}
};

int main(int argc, char **argv)
{
	CustomClient c;

	c.Connect("127.0.0.1", 60000);

	// bool key[3] = { false, false, false };
	// bool old_key[3] = { false, false, false };
	
	char key_buffer[1] = {};
	bool bQuit = false;

	boost::asio::io_service ioservice;        
    boost::asio::posix::stream_descriptor stream(ioservice, STDIN_FILENO);

	std::function<void(boost::system::error_code, size_t)> read_handler =
		[&](boost::system::error_code ec, size_t len) {   
            if (ec) {
                std::cerr << "exit with " << ec.message() << std::endl;
            } else {
				// if (key_buffer[0] == '0') {
				// 	c.Connect("127.0.0.1", 60000);
				// }
				if (key_buffer[0] == '1') {
					c.PingServer();
				}
				if (key_buffer[0] == '2') {
					c.MessageAll();
				}


				
				// if (key_buffer[0] == '3') {
				// 	c.Disconnect();
				// }
				if (key_buffer[0] == '3') {
					bQuit = true;
				}
                boost::asio::async_read(stream, boost::asio::buffer(key_buffer), read_handler);
            }
        };

    boost::asio::async_read(stream, boost::asio::buffer(key_buffer), read_handler);
	std::thread thrIOservice = std::thread([&](){ ioservice.run(); });

	while (!bQuit)
	{
		// c.Wait();

		if (c.IsConnected())
		{
			while (!c.Incoming().empty())
			{
				auto msg = c.Incoming().pop_front().msg;
 				switch (msg.header.id)
				{
				case CustomMsgTypes::ServerAccept:
				{				
					std::cout << "Server Accepted Connection\n";
				}
				break;

				case CustomMsgTypes::ServerPing:
				{
					// Server has responded to a ping request
					std::chrono::system_clock::time_point timeNow = std::chrono::system_clock::now();
					std::chrono::system_clock::time_point timeThen;
					msg >> timeThen;
					std::cout << "Ping: " << std::chrono::duration<double>(timeNow - timeThen).count() << "\n";
				}
				break;

				case CustomMsgTypes::ServerMessage:
				{
					// Server has responded to a message to all
					uint32_t clientID;
					msg >> clientID;
					std::cout << "Hello from [" << clientID << "]\n";
				}
				break;

				}
			}
		}
		else
		{
			std::cout << "Server Down\n";
			bQuit = true;
		}

	}

	ioservice.stop();
    if (thrIOservice.joinable()) thrIOservice.join();

	return 0;
}
