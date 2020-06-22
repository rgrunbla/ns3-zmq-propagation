#ifndef ZMQ_HELPERS_H
#define ZMQ_HELPERS_H

#include <zmq.hpp>

inline static std::string s_recv (zmq::socket_t & socket, zmq::recv_flags flags = zmq::recv_flags::none) {
    zmq::message_t message;
    socket.recv(message, flags);
    return std::string(static_cast<char*>(message.data()), message.size());
}

inline static zmq::detail::send_result_t send(zmq::socket_t& socket, const std::string& string) {
    zmq::message_t message(string.size());
    std::memcpy (message.data(), string.data(), string.size());
    zmq::detail::send_result_t rc = socket.send (message, zmq::send_flags::dontwait);
    return (rc);
}

#endif // ZMQ_HELPERS_H