#ifndef ZMQ_HELPERS_H
#define ZMQ_HELPERS_H

#include <zmq.hpp>
#include "zmq-propagation-messages.pb.h"

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

inline static void GlobalSend(std::string message, GlobalContainer_MessageType message_type, zmq::socket_t & zmq_socket) {
    GlobalContainer global_container = GlobalContainer();
    global_container.set_content(message);
    global_container.set_type(message_type);
    global_container.SerializeToString(&message);
    send(zmq_socket, message);
}

inline static void MetaSend(std::string message, Meta_MessageType message_type, zmq::socket_t & zmq_socket) {
    Meta meta = Meta();
    meta.set_content(message);
    meta.set_type(message_type);
    meta.SerializeToString(&message);
    GlobalSend(message, GlobalContainer_MessageType_META, zmq_socket);
}

inline static void MesoSend(int simulation_id, std::string message, Meso_MessageType message_type, zmq::socket_t & zmq_socket) {
    Meso meso = Meso();
    meso.set_content(message);
    meso.set_simulation_id(simulation_id);
    meso.set_type(message_type);
    meso.SerializeToString(&message);
    GlobalSend(message, GlobalContainer_MessageType_MESO, zmq_socket);
}

inline static Meso MesoRecv(Meso_MessageType message_type, zmq::socket_t & zmq_socket) {
    std::string message;
    GlobalContainer global_container = GlobalContainer();
    Meso meso = Meso();
    message = s_recv(zmq_socket);
    global_container.ParseFromString(message);
    assert(global_container.type() == GlobalContainer_MessageType_MESO);
    meso.ParseFromString(global_container.content());
    assert(meso.type() == message_type);
    return meso;
}

#endif // ZMQ_HELPERS_H