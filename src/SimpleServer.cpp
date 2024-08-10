#include "olc_net.hpp"

// using namespace boost;

class CustomServer : public olc::net::server_interface<CustomMsgTypes>
{
public:
	CustomServer(uint16_t nPort) : olc::net::server_interface<CustomMsgTypes>(nPort)
	{

	}
protected:
	virtual bool OnClientConnect(std::shared_ptr<olc::net::connection<CustomMsgTypes>> client) override
	{
		olc::net::message<CustomMsgTypes> msg;
		msg.header.id = CustomMsgTypes::ServerAccept;
		client->Send(msg);
		return true;
	}

	// Called when a client appears to have disconnected
	virtual void OnClientDisconnect(std::shared_ptr<olc::net::connection<CustomMsgTypes>> client) override
	{
		std::cout << "Removing client [" << client->GetID() << "]\n";
	}

	// Called when a message arrives
	virtual void OnMessage(std::shared_ptr<olc::net::connection<CustomMsgTypes>> client, olc::net::message<CustomMsgTypes>& msg) override
	{
		switch (msg.header.id)
		{
		case CustomMsgTypes::ServerPing:
		{
			std::cout << "[" << client->GetID() << "] Server Ping\n";

			// Simply bounce message back to client
			client->Send(msg);
		}
		break;
		case CustomMsgTypes::MessageAll:
		{
			std::cout << "[" << client->GetID() << "] Message to All Clients\n";

			// Construct a new message and send it to all clients
			olc::net::message<CustomMsgTypes> new_msg;
			new_msg.header.id = CustomMsgTypes::ServerMessage;
			new_msg << client->GetID();
			MessageAllClients(new_msg, client);
		}
		break;
		}
	}
};

int main(int argc, char **argv)
{
	CustomServer server(60000);
	server.Start();

	while (1)
	{
		server.Update(-1, true);
	}

	return 0;
}
