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
#include "zmq-mobility-model.h"
#include "ns3/simulator.h"
#include "ns3/string.h"

namespace ns3 {
  NS_OBJECT_ENSURE_REGISTERED (ZmqMobilityModel);

  TypeId ZmqMobilityModel::GetTypeId (void)
  {
    static TypeId tid = TypeId ("ns3::ZmqMobilityModel")
      .SetParent<MobilityModel> ()
      .SetGroupName ("Mobility")
      .AddConstructor<ZmqMobilityModel> ()
      .AddAttribute ("ZmqEndpoint",
                    "The endpoint used to communicate with Phi",
                    StringValue ("ipc:///tmp/PhiEndpoint"),
                    MakeStringAccessor (&ZmqMobilityModel::m_zmqEndpoint),
                    MakeStringChecker ())
    ;
    return tid;
  }

  TypeId
  ZmqMobilityModel::GetInstanceTypeId () const
  {
    return GetTypeId ();
  }

  ZmqMobilityModel::ZmqMobilityModel () : zmq_ctx(1), zmq_sock(zmq_ctx, ZMQ_REQ)
  {
      this->zmq_sock.connect(m_zmqEndpoint);
  }

  ZmqMobilityModel::~ZmqMobilityModel ()
  {
  }

  inline Vector
  ZmqMobilityModel::DoGetVelocity (void) const
  {
    // TODO
    return Vector (0.0, 0.0, 0.0);
  }

  inline Vector
  ZmqMobilityModel::DoGetPosition (void) const
  {
    // TODO
    return Vector (0.0, 0.0, 0.0);
  }

  void 
  ZmqMobilityModel::DoSetPosition (const Vector &position)
  {
    m_position = position;
    NotifyCourseChange ();
  }

  inline glm::dquat
  ZmqMobilityModel::DoGetOrientation (void) const
  {
    // TODO
    return glm::dquat(glm::dvec3(0.0, 0.0, 0.0));
  }

  void 
  ZmqMobilityModel::DoSetOrientation (const glm::dquat &orientation)
  {
    m_orientation = orientation;
    NotifyCourseChange ();
  }
} // namespace ns3
