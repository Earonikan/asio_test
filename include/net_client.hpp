#pragma once

#include "net_common.hpp"
#include "net_tsqueue.hpp"
#include "net_message.hpp"
#include "net_connection.hpp"

namespace olc
{
    namespace net
    {
        template<typename T>
        class client_interface
        {
        public:
            client_interface():m_socket(m_context)
            {
                //initialize socket with io_context, so it can do stuff
            }
            virtual ~client_interface()
            {
                //if the client is destroyed, always try and disconnect from server
                Disconnect();
            }

            //connect to server with hostname/ip-address and port
            bool Connect(const std::string &host, const uint16_t port)
            {
                try
                {
					// Resolve hostname/ip-address into tangiable physical address
					boost::asio::ip::tcp::resolver resolver(m_context);
					boost::asio::ip::tcp::resolver::results_type endpoints = resolver.resolve(host, std::to_string(port));

					// Create connection
					m_connection = std::make_unique<connection<T>>(
                            connection<T>::owner::client,
                            m_context,
                            boost::asio::ip::tcp::socket(m_context),
                            m_qMessagesIn);
					
					// Tell the connection object to connect to server
					m_connection->ConnectToServer(endpoints);

					// Start Context Thread
					thrContext = std::thread([this]() { m_context.run(); });
                }
                catch (std::exception &e)
                {
                    std::cerr << "Client Exception:" << e.what() << std::endl;
                }

                return false;
            }

            // to disconnect from server
            void Disconnect()
            {
                if (IsConnected())
                    m_connection->Disconnect();

                // down with asio context
                m_context.stop();
                // nad its thread
                if (thrContext.joinable())
                    thrContext.join();
                
                //destroy the connection object
                m_connection.release();

                std::cout << "Client has just disconnected" << std::endl;
            }

            //check if client actually connected to a server
            bool IsConnected()
            {
                if (m_connection)
                    return m_connection->IsConnected();
                else
                    return false;
            }

            // Send message to server
			void Send(const message<T>& msg)
			{
				if (IsConnected())
					m_connection->Send(msg);
			}

            //Retrieve queue of messages from server
            tsqueue<owned_message<T>> &Incoming()
            {
                return m_qMessagesIn;
            }
            
            // void Wait() {
            //     m_qMessagesIn.wait();
            // }

        protected:
            // asio context handles the data transfer...
            boost::asio::io_context m_context;
            // but it needs a thread of its own to execute its work commandss 
            std::thread thrContext;
            //this is th hardware socket tha is connected to the server
            boost::asio::ip::tcp::socket m_socket;
            // clien has a single instance of a "connection" object, which handles data transfer
            std::unique_ptr<connection<T>> m_connection;
        
        private:
            // thread safe queue of incoming messages from server
            tsqueue<owned_message<T>> m_qMessagesIn;
        };

    }
}