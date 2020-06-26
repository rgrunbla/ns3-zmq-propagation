#ifndef ZMQ_HELPERS_H
#define ZMQ_HELPERS_H

#include <zmq.hpp>
#include <google/protobuf/message_lite.h>
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


inline static void GlobalSend(google::protobuf::MessageLite &message, phi::GlobalContainer_MessageType message_type, zmq::socket_t & zmq_socket) {
    std::string content;
    phi::GlobalContainer global_container = phi::GlobalContainer();
    message.SerializeToString(&content);
    global_container.set_content(content);
    global_container.set_type(message_type);
    global_container.SerializeToString(&content);
    send(zmq_socket, content);
}


inline static void MetaSend(google::protobuf::MessageLite &message, phi::Meta_MessageType message_type, zmq::socket_t & zmq_socket) {
    phi::Meta meta = phi::Meta();
    std::string content;
    message.SerializeToString(&content);
    meta.set_content(content);
    meta.set_type(message_type);
    GlobalSend(meta, phi::GlobalContainer_MessageType_META, zmq_socket);
}

inline static phi::Meta MetaRecv(phi::Meta_MessageType message_type, zmq::socket_t & zmq_socket) {
    std::string message;
    phi::GlobalContainer global_container = phi::GlobalContainer();
    phi::Meta meta = phi::Meta();
    message = s_recv(zmq_socket);
    global_container.ParseFromString(message);
    assert(global_container.type() == phi::GlobalContainer_MessageType_META);
    meta.ParseFromString(global_container.content());
    assert(meta.type() == message_type);
    return meta;
}


inline static void MesoSend(int simulation_id, google::protobuf::MessageLite &message, phi::Meso_MessageType message_type, zmq::socket_t & zmq_socket) {
    std::string content;
    phi::Meso meso = phi::Meso();
    message.SerializeToString(&content);
    meso.set_content(content);
    meso.set_simulation_id(simulation_id);
    meso.set_type(message_type);
    GlobalSend(meso, phi::GlobalContainer_MessageType_MESO, zmq_socket);
}

inline static phi::Meso MesoRecv(phi::Meso_MessageType message_type, zmq::socket_t & zmq_socket) {
    std::string message;
    phi::GlobalContainer global_container = phi::GlobalContainer();
    phi::Meso meso = phi::Meso();
    message = s_recv(zmq_socket);
    global_container.ParseFromString(message);
    assert(global_container.type() == phi::GlobalContainer_MessageType_MESO);
    meso.ParseFromString(global_container.content());
    assert(meso.type() == message_type);
    return meso;
}



inline static void AckRecv(zmq::socket_t & zmq_socket) {
    std::string message;
    message = s_recv(zmq_socket);
}

#endif // ZMQ_HELPERS_H