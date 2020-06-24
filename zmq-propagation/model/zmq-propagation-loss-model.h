#ifndef CUSTOM_PROPAGATION_LOSS_MODEL_H
#define CUSTOM_PROPAGATION_LOSS_MODEL_H

#include "ns3/object.h"
#include "ns3/integer.h"
#include "ns3/random-variable-stream.h"
#include "ns3/propagation-loss-model.h"
#include <map>

/* Zero MQ */
#include <zmq.hpp>

namespace ns3 {

/**
 * \defgroup propagation Propagation Models
 *
 */

class MobilityModel;

/**
 * \ingroup propagation
 *
 * \brief The propagation loss depends only on the distance (range) between transmitter and receiver.
 *
 * The single MaxRange attribute (units of meters) determines path loss.
 * Receivers at or within MaxRange meters receive the transmission at the
 * transmit power level. Receivers beyond MaxRange receive at power
 * -1000 dBm (effectively zero).
*/
class ExternalPropagationLossModel : public PropagationLossModel
{
public:
  /**
   * \brief Get the type ID.
   * \return the object TypeId
   */
  static TypeId GetTypeId (void);
  TypeId GetInstanceTypeId () const;
  ExternalPropagationLossModel ();
  ~ExternalPropagationLossModel ();

private:
  /**
   * \brief Copy constructor
   *
   * Defined and unimplemented to avoid misuse
   */
  ExternalPropagationLossModel (const ExternalPropagationLossModel&);
  /**
   * \brief Copy constructor
   *
   * Defined and unimplemented to avoid misuse
   * \returns
   */
  ExternalPropagationLossModel& operator= (const ExternalPropagationLossModel&);
  virtual double DoCalcRxPower (double txPowerDbm,
                                Ptr<MobilityModel> a,
                                Ptr<MobilityModel> b) const;
  virtual int64_t DoAssignStreams (int64_t stream);
  
  /* Zero MQ */
  zmq::context_t zmq_ctx;
  mutable zmq::socket_t zmq_sock;
  int m_simulationId;
  std::string m_zmqEndpoint;
};

} // namespace ns3

#endif /* PROPAGATION_LOSS_MODEL_H */
