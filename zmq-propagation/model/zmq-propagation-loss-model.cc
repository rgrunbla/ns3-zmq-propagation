#include "zmq-propagation-loss-model.h"
#include "ns3/boolean.h"
#include "ns3/double.h"
#include "ns3/log.h"
#include "ns3/mobility-model.h"
#include "ns3/node.h"
#include "ns3/pointer.h"
#include "ns3/simulator.h"
#include "ns3/string.h"
#include "zmq-helpers.h"
#include <cmath>

/* Zero MQ */
#include <zmq.hpp>

/* Protobuf */
#include "zmq-propagation-messages.pb.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE("ExternalPropagationLossModel");

// ------------------------------------------------------------------------- //

NS_OBJECT_ENSURE_REGISTERED(ExternalPropagationLossModel);

TypeId ExternalPropagationLossModel::GetTypeId(void) {
  static TypeId tid =
      TypeId("ns3::ExternalPropagationLossModel")
          .SetParent<PropagationLossModel>()
          .SetGroupName("Propagation")
          .AddAttribute("SimulationId", "The simulation ID to send to Phi",
                        IntegerValue(0),
                        MakeIntegerAccessor(
                            &ExternalPropagationLossModel::m_simulationId),
                        MakeIntegerChecker<int>())
          .AddAttribute(
              "ZmqEndpoint", "The endpoint used to communicate with Phi",
              StringValue("ipc:///tmp/PhiEndpoint"),
              MakeStringAccessor(&ExternalPropagationLossModel::m_zmqEndpoint),
              MakeStringChecker());
  return tid;
}

TypeId ExternalPropagationLossModel::GetInstanceTypeId() const {
  return GetTypeId();
}

ExternalPropagationLossModel::ExternalPropagationLossModel(
    zmq::context_t &context)
    : zmq_sock(context, ZMQ_REQ) {
  ObjectBase::ConstructSelf(AttributeConstructionList());
  this->zmq_sock.connect(m_zmqEndpoint);
  this->zmq_sock.setsockopt(ZMQ_LINGER, 0);
}

ExternalPropagationLossModel::~ExternalPropagationLossModel() {
  this->zmq_sock.close();
}

double ExternalPropagationLossModel::DoCalcRxPower(double txPowerDbm,
                                                   Ptr<MobilityModel> a,
                                                   Ptr<MobilityModel> b) const {

  std::string message;
  phi::LossQuery loss_query = phi::LossQuery();
  phi::LossAnswer loss_answer = phi::LossAnswer();
  phi::Meso meso = phi::Meso();
  phi::GlobalContainer global_container = phi::GlobalContainer();

  loss_query.set_source_agent_id(a->GetObject<ns3::Node>()->GetId());
  loss_query.set_dest_agent_id(b->GetObject<ns3::Node>()->GetId());
  loss_query.set_power(txPowerDbm);
  loss_query.set_clock(Simulator::Now().GetSeconds());

  MesoSend(this->m_simulationId, loss_query, phi::Meso_MessageType_LOSS_QUERY,
           this->zmq_sock);
  meso = MesoRecv(phi::Meso_MessageType_LOSS_ANSWER, this->zmq_sock);

  loss_answer.ParseFromString(meso.content());

  return txPowerDbm + loss_answer.power();
}

int64_t ExternalPropagationLossModel::DoAssignStreams(int64_t stream) {
  return 0;
}

// ------------------------------------------------------------------------- //
} // namespace ns3
