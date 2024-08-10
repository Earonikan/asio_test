#pragma once

#include "net_common.hpp"

namespace olc
{
    namespace net
    {
        template <typename T>
        struct message_header
        {
            T id{};
            uint32_t payload_size = 0;
        };

        template <typename T>
        struct message
        {
            message_header<T> header{};
            std::vector<uint8_t> body;

            // return size of entire message
            size_t size() { return sizeof(message_header<T>) + body.size(); }

            //override std::cout to reproduce friendly decription of message
            friend std::ostream &operator << (std::ostream &os, const message<T> &msg)
            {
                os << "ID:" << int(msg.header.id) << " Size:" << msg.header.payload_size;
                return os;
            }

            // Push any POD-like data into message
            template <typename DataType>
            friend message<T> &operator << (message<T> &msg, const DataType &data)
            {
                // check that the type of the data being pushed is trivially copyable
                static_assert(std::is_standard_layout<DataType>::value, "Data is too complex to be pushed into vector");

                //Cache current size of vector, as this will be the point we insert data
                size_t i = msg.body.size();

                //Resize the vector by the size of the data being pushed
                msg.body.resize(msg.body.size() + sizeof(DataType));

                //Physically copy the data into the newly allocated vector space
                // std::copy(&data, &data + sizeof(DataType), msg.body.data() + i); //back_inserter(msg.body));
                std::memcpy(msg.body.data() + i, &data, sizeof(DataType));

                //Recalculate the message size
                msg.header.payload_size = msg.body.size();
                
                //Return the target message so it can be "chained"
                return msg;
            }

            // Get any POD-like data from message
            template <typename DataType>
            friend message<T> &operator >> (message<T> &msg, DataType &data)
            {
                // check that the type of the data being pushed is trivially copyable
                static_assert(std::is_standard_layout<DataType>::value, "Data is too complex to be unpacked from vector");

                //Cache the location towards the end data being pushed is trivially copyable
                size_t i = msg.body.size() - sizeof(DataType);

                //Physically copy the data from the vector into the variable
                std::memcpy(&data, msg.body.data() + i, sizeof(DataType));
                //std::copy(msg.body.end() - sizeof(DataType), msg.body.end(), &data);

                //Shrink the vector to remove read bytes, and reset end position
                msg.body.resize(msg.body.size() - sizeof(DataType));

                //Recalculate the message size
                msg.header.payload_size = msg.body.size();

                //Return the target message so it can be "chained"
                return msg;
            }
        };

        // Forward declare
        template<typename T>
        class connection;

        template<typename T>
        struct owned_message
        {
        // public:
            std::shared_ptr<connection<T>> remote = nullptr;
            message<T> msg;
            owned_message(std::shared_ptr<connection<T>> remote_, message<T> msg_) : remote(move(remote_)), msg(msg_) {}

            //override std::cout to reproduce friendly decription of message
            friend std::ostream &operator << (std::ostream &os, const owned_message<T> &msg)
            {
                os << msg.msg;
                return os;
            }


        };

    }
}