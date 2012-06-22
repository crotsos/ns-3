/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2011 Blake Hurd
 *
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
 * Author: Blake Hurd  <naimorai@gmail.com>
 */
#ifndef MIRAGE_HELPER_H
#define MIRAGE_HELPER_H

#include "ns3/net-device-container.h"
#include "ns3/object-factory.h"
#include <string>

namespace ns3 {

class Node;
class AttributeValue;

/**
 * \brief Add capability to switch multiple LAN segments (IEEE 802.1D bridging)
 */
class MirageHelper
{
public:
  MirageHelper ();

  void
  SetDeviceAttribute (std::string n1, const AttributeValue &v1);

  NetDeviceContainer
  Install (Ptr<Node> node, NetDeviceContainer c);

  NetDeviceContainer
  Install (std::string nodeName, NetDeviceContainer c);

private:
  ObjectFactory m_deviceFactory;
};

} // namespace ns3

#endif /* OPENFLOW_SWITCH_HELPER_H */
