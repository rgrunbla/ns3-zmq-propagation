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
                   MakeStringAccessor (&ExternalPropagationLossModel::m_zmqEndpoint),
                   MakeStringChecker ())             
  ;
  return tid;
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
  double t = (Simulator::Now () - m_baseTime).GetSeconds ();
  return Vector (m_baseVelocity.x + m_acceleration.x*t,
                 m_baseVelocity.y + m_acceleration.y*t,
                 m_baseVelocity.z + m_acceleration.z*t);
}

inline Vector
ZmqMobilityModel::DoGetPosition (void) const
{
  double t = (Simulator::Now () - m_baseTime).GetSeconds ();
  double half_t_square = t*t*0.5;
  return Vector (m_basePosition.x + m_baseVelocity.x*t + m_acceleration.x*half_t_square,
                 m_basePosition.y + m_baseVelocity.y*t + m_acceleration.y*half_t_square,
                 m_basePosition.z + m_baseVelocity.z*t + m_acceleration.z*half_t_square);
}

void 
ZmqMobilityModel::DoSetPosition (const Vector &position)
{
  m_baseVelocity = DoGetVelocity ();
  m_baseTime = Simulator::Now ();
  m_basePosition = position;
  NotifyCourseChange ();
}

void 
ZmqMobilityModel::SetVelocityAndAcceleration (const Vector &velocity,
                                                               const Vector &acceleration)
{
  m_basePosition = DoGetPosition ();
  m_baseTime = Simulator::Now ();
  m_baseVelocity = velocity;
  m_acceleration = acceleration;
  NotifyCourseChange ();
}


} // namespace ns3
