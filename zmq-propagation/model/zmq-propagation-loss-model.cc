#include "zmq-propagation-loss-model.h"
#include "zmq-helpers.h"
#include "ns3/log.h"
#include "ns3/mobility-model.h"
#include "ns3/boolean.h"
#include "ns3/double.h"
#include "ns3/string.h"
#include "ns3/pointer.h"
#include "ns3/node.h"
#include "ns3/simulator.h"
#include <cmath>
#define PI 3.14159265
#include <math.h>       /* atan2 */

/* Zero MQ */
#include <zmq.hpp>

/* Protobuf */
#include "zmq-propagation-messages.pb.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("ExternalPropagationLossModel");

// ------------------------------------------------------------------------- //

NS_OBJECT_ENSURE_REGISTERED (ExternalPropagationLossModel);

TypeId
ExternalPropagationLossModel::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::ExternalPropagationLossModel")
    .SetParent<PropagationLossModel> ()
    .SetGroupName ("Propagation")
    .AddConstructor<ExternalPropagationLossModel> ()
    .AddAttribute ("SimulationId",
                   "The simulation ID to send to Phi",
                   IntegerValue (0),
                   MakeIntegerAccessor (&ExternalPropagationLossModel::m_simulationId),
                   MakeIntegerChecker<int> ())
  ;
  return tid;
}

ExternalPropagationLossModel::ExternalPropagationLossModel () : zmq_ctx(1), zmq_sock(zmq_ctx, ZMQ_REQ)
{
    this->zmq_sock.connect("ipc:///tmp/2");
}

ExternalPropagationLossModel::~ExternalPropagationLossModel ()
{
}


double
ExternalPropagationLossModel::DoCalcRxPower (double txPowerDbm,
                                          Ptr<MobilityModel> a,
                                          Ptr<MobilityModel> b) const
{
      LossQuery loss_query = LossQuery();
      LossAnswer loss_answer = LossAnswer();
      Meso meso;
      GlobalContainer global_container = GlobalContainer();
      std::string message;

      loss_query.set_source_agent_id(a->GetObject<ns3::Node> ()->GetId ());
      loss_query.set_dest_agent_id(b->GetObject<ns3::Node> ()->GetId ());
      loss_query.set_power(txPowerDbm);
      loss_query.set_clock(Simulator::Now().GetSeconds());
      loss_query.SerializeToString(&message);

      meso.set_type(Meso_MessageType_LOSS_QUERY);
      meso.set_simulation_id(this->m_simulationId);
      meso.set_content(message);
      meso.SerializeToString(&message);

      global_container.set_content(message);
      global_container.set_type(GlobalContainer_MessageType_MESO);
      global_container.SerializeToString(&message);

      send(this->zmq_sock, message);
      message = s_recv(this->zmq_sock);

      global_container.Clear();
      meso.Clear();

      global_container.ParseFromString(message);
      assert(global_container.type() == GlobalContainer_MessageType_MESO);
      meso.ParseFromString(global_container.content());
      assert(meso.type() == Meso_MessageType_LOSS_ANSWER);
      loss_answer.ParseFromString(meso.content());
      return txPowerDbm + loss_answer.power();
    }

int64_t
ExternalPropagationLossModel::DoAssignStreams (int64_t stream)
{
  return 0;
}

// ------------------------------------------------------------------------- //
} // namespace ns3
