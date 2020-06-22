#include "zmq-propagation-delay-model.h"
#include "ns3/mobility-model.h"
#include "ns3/double.h"
#include "ns3/string.h"
#include "ns3/pointer.h"

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (ExternalPropagationDelayModel);

TypeId
ExternalPropagationDelayModel::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::ExternalPropagationDelayModel")
    .SetParent<PropagationDelayModel> ()
    .SetGroupName ("Propagation")
    .AddConstructor<ExternalPropagationDelayModel> ()
    .AddAttribute ("Speed", "The propagation speed (m/s) in the propagation medium being considered. The default value is the propagation speed of light in the vacuum.",
                   DoubleValue (299792458),
                   MakeDoubleAccessor (&ExternalPropagationDelayModel::m_speed),
                   MakeDoubleChecker<double> ())
  ;
  return tid;
}

ExternalPropagationDelayModel::ExternalPropagationDelayModel ()
{
}
Time
ExternalPropagationDelayModel::GetDelay (Ptr<MobilityModel> a, Ptr<MobilityModel> b) const
{
  double distance = a->GetDistanceFrom (b);
  double seconds = distance / m_speed;
  return Seconds (seconds);
}
void
ExternalPropagationDelayModel::SetSpeed (double speed)
{
  m_speed = speed;
}
double
ExternalPropagationDelayModel::GetSpeed (void) const
{
  return m_speed;
}

int64_t
ExternalPropagationDelayModel::DoAssignStreams (int64_t stream)
{
  return 0;
}


} // namespace ns3
