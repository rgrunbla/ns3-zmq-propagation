#ifndef PROPAGATION_DELAY_MODEL_CUSTOM_H
#define PROPAGATION_DELAY_MODEL_CUSTOM_H

#include "ns3/ptr.h"
#include "ns3/object.h"
#include "ns3/nstime.h"
#include "ns3/random-variable-stream.h"
#include "ns3/propagation-delay-model.h"

namespace ns3 {

/**
 * \ingroup propagation
 *
 * \brief the propagation speed is constant
 */
class ExternalPropagationDelayModel : public PropagationDelayModel
{
public:
  /**
   * \brief Get the type ID.
   * \return the object TypeId
   */
  static TypeId GetTypeId (void);

  /**
   * Use the default parameters from PropagationDelayExternal.
   */
  ExternalPropagationDelayModel ();
  virtual Time GetDelay (Ptr<MobilityModel> a, Ptr<MobilityModel> b) const;
  /**
   * \param speed the new speed (m/s)
   */
  void SetSpeed (double speed);
  /**
   * \returns the current propagation speed (m/s).
   */
  double GetSpeed (void) const;
private:
  virtual int64_t DoAssignStreams (int64_t stream);
  double m_speed; //!< speed
};

} // namespace ns3

#endif /* PROPAGATION_DELAY_MODEL_H */
