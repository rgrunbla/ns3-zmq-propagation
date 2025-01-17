/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Author: Rémy Grünblatt <remy@grunblatt.org>
 */
#ifndef ZMQ_MOBILITY_MODEL_H
#define ZMQ_MOBILITY_MODEL_H

#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"
#include "ns3/mobility-model.h"
#include "ns3/nstime.h"
#include "zmq-helpers.h"
#include "zmq.hpp"

namespace ns3 {

/**
 * \ingroup mobility
 *
 * \brief Mobility model for which the current acceleration does not change once
 * it has been set and until it is set again explicitly to a new value.
 */
class ZmqMobilityModel : public MobilityModel {
public:
  /**
   * Register this type with the TypeId system.
   * \return the object TypeId
   */
  static TypeId GetTypeId(void);
  TypeId GetInstanceTypeId() const;
  /**
   * Create position located at coordinates (0,0,0) with
   * speed (0,0,0).
   */
  ZmqMobilityModel();
  virtual ~ZmqMobilityModel();

  void setupAndConnect(zmq::context_t &);

  virtual glm::dquat GetOrientation(void) const;
  virtual void SetOrientation(const glm::dquat &orientation);

private:
  virtual void DoDispose (void);
  virtual Vector DoGetPosition(void) const;
  virtual void DoSetPosition(const Vector &position);
  virtual Vector DoGetVelocity(void) const;

  Vector m_position;        //!< the position
  glm::dquat m_orientation; //!< the orientation

  /* Zero MQ */
  bool connected;
  std::unique_ptr<zmq::socket_t> zmq_sock;
  int m_simulationId;
  std::string m_zmqEndpoint;
};

} // namespace ns3

#endif /* ZMQ_MOBILITY_MODEL_H */
