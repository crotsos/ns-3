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
 * Author: Charalampos Rotsos <cr409@cl.cam.ac.uk>
 */
#include "mirage-net-device.h"

NS_LOG_COMPONENT_DEFINE ("MirageNetDevice");

namespace ns3 {

  NS_OBJECT_ENSURE_REGISTERED (MirageNetDevice);

  TypeId
  MirageNetDevice::GetTypeId (void) {
    static TypeId tid = TypeId ("ns3::MirageNetDevice")
      .SetParent<NetDevice> ()
      .AddConstructor<MirageNetDevice> ()
      /*     .AddAttribute ("ID",
       *                    "The identification of the MirageNetDevice/Datapath, needed for OpenFlow compatibility.",
       *                    UintegerValue (GenerateId ()),
       *                    MakeUintegerAccessor (&MirageNetDevice::m_id),
       *                    MakeUintegerChecker<uint64_t> ())
       *     .AddAttribute ("FlowTableLookupDelay",
       *                    "A real switch will have an overhead for looking up in the flow table. For the default, we simulate a standard TCAM on an FPGA.",
       *                    TimeValue (NanoSeconds (30)),
       *                    MakeTimeAccessor (&MirageNetDevice::m_lookupDelay),
       *                    MakeTimeChecker ())
       *     .AddAttribute ("Flags", // Note: The Controller can configure this value, overriding the user's setting.
       *                    "Flags to turn different functionality on/off, such as whether to inform the controller when a flow expires, or how to handle fragments.",
       *                    UintegerValue (0), // Look at the ofp_config_flags enum in openflow/include/openflow.h for options.
       *                    MakeUintegerAccessor (&MirageNetDevice::m_flags),
       *                    MakeUintegerChecker<uint16_t> ())
       *     .AddAttribute ("FlowTableMissSendLength", // Note: The Controller can configure this value, overriding the user's setting.
       *                    "When forwarding a packet the switch didn't match up to the controller, it can be more efficient to forward only the first x bytes.",
       *                    UintegerValue (OFP_DEFAULT_MISS_SEND_LEN), // 128 bytes
       *                    MakeUintegerAccessor (&MirageNetDevice::m_missSendLen),
       *                    MakeUintegerChecker<uint16_t> ())
       */
      ;
    return tid;
  }

  MirageNetDevice::MirageNetDevice () : m_node (0), m_ifIndex (0), m_mtu (0xffff) {
    NS_LOG_FUNCTION_NOARGS ();
  }

  MirageNetDevice::~MirageNetDevice () {
    NS_LOG_FUNCTION_NOARGS ();
  }

  void
  MirageNetDevice::DoDispose () {
      NS_LOG_FUNCTION_NOARGS ();
      NetDevice::DoDispose ();
    }

  void
  MirageNetDevice::ReceiveFromDevice (Ptr<NetDevice> netdev, Ptr<const Packet> packet, 
        uint16_t protocol, const Address& src, const Address& dst, PacketType packetType) {
    NS_LOG_FUNCTION_NOARGS ();
    NS_LOG_INFO ("--------------------------------------------");
    NS_LOG_DEBUG ("UID is " << packet->GetUid ());

    Mac48Address dst48 = Mac48Address::ConvertFrom (dst);
    NS_LOG_INFO ("Received packet from " << Mac48Address::ConvertFrom (src) << 
        " looking for " << dst48);

    for (std::vector<ns3::Ptr<NetDevice> >::iterator it = m_ports.begin(); 
        it != m_ports.end(); it++) {
      (*it)->SendFrom(packet->Copy(), src, dst, protocol);
    }
  }

  int
  MirageNetDevice::AddSwitchPort (Ptr<NetDevice> switchPort) {
    NS_LOG_FUNCTION_NOARGS ();
    NS_ASSERT (switchPort != this);
    if (!Mac48Address::IsMatchingType (switchPort->GetAddress ())) {
      NS_FATAL_ERROR ("Device does not support eui 48 addresses: cannot be added to switch.");
    }
    if (!switchPort->SupportsSendFrom ()) {
      NS_FATAL_ERROR ("Device does not support SendFrom: cannot be added to switch.");
    }
    if (m_address == Mac48Address ()) {
      m_address = Mac48Address::ConvertFrom (switchPort->GetAddress ());
    }

    NS_LOG_DEBUG ("RegisterProtocolHandler for " << switchPort->GetInstanceTypeId ().GetName ());
    m_node->RegisterProtocolHandler (MakeCallback (&MirageNetDevice::ReceiveFromDevice, this),
        0, switchPort, true);

    m_ports.push_back(switchPort);
    return 0;
  }

  void
  MirageNetDevice::SetIfIndex (const uint32_t index) {
    NS_LOG_FUNCTION_NOARGS ();
    this->m_ifIndex = index;
  }

  uint32_t
  MirageNetDevice::GetIfIndex (void) const {
    NS_LOG_FUNCTION_NOARGS ();
    return m_ifIndex;
  }

  Ptr<Channel>
  MirageNetDevice::GetChannel (void) const {
    NS_LOG_FUNCTION_NOARGS ();
    return 0; // m_channel;
  }

  void
  MirageNetDevice::SetAddress (Address address) {
    NS_LOG_FUNCTION_NOARGS ();
    m_address = Mac48Address::ConvertFrom (address);
  }

  Address
  MirageNetDevice::GetAddress (void) const {
    NS_LOG_FUNCTION_NOARGS ();
    return m_address;
  }

  bool
  MirageNetDevice::Send (Ptr<Packet> packet, const Address& dest, uint16_t protocolNumber) {
    NS_LOG_FUNCTION_NOARGS ();
    return SendFrom (packet, m_address, dest, protocolNumber);
  }

  bool
  MirageNetDevice::SendFrom (Ptr<Packet> packet, const Address& src, 
      const Address& dest, uint16_t protocolNumber) {
    NS_LOG_FUNCTION_NOARGS ();
    return true;
  }


  /* Required methods from NetDevice inheritance
   * */

  Ptr<Node>
  MirageNetDevice::GetNode (void) const {
    NS_LOG_FUNCTION_NOARGS ();
    return m_node;
  }

  void
  MirageNetDevice::SetNode (Ptr<Node> node) {
    NS_LOG_FUNCTION_NOARGS ();
    m_node = node;
  }

  bool
  MirageNetDevice::NeedsArp (void) const {
    NS_LOG_FUNCTION_NOARGS ();
    return true;
  }

  void
  MirageNetDevice::SetReceiveCallback (NetDevice::ReceiveCallback cb) {
    NS_LOG_FUNCTION_NOARGS ();
    m_rxCallback = cb;
  }

  void
  MirageNetDevice::SetPromiscReceiveCallback (NetDevice::PromiscReceiveCallback cb) {
    NS_LOG_FUNCTION_NOARGS ();
    m_promiscRxCallback = cb;
  }

  bool
  MirageNetDevice::SupportsSendFrom () const {
    NS_LOG_FUNCTION_NOARGS ();
    return true;
  }

  Address
  MirageNetDevice::GetMulticast (Ipv6Address addr) const {
    NS_LOG_FUNCTION (this << addr);
    return Mac48Address::GetMulticast (addr);
  }

  bool
  MirageNetDevice::SetMtu (const uint16_t mtu) {
    NS_LOG_FUNCTION_NOARGS ();
    m_mtu = mtu;
    return true;
  }

  uint16_t
  MirageNetDevice::GetMtu (void) const {
    NS_LOG_FUNCTION_NOARGS ();
    return m_mtu;
  }


  bool
  MirageNetDevice::IsLinkUp (void) const  {
    NS_LOG_FUNCTION_NOARGS ();
    return true;
  }


  void
  MirageNetDevice::AddLinkChangeCallback (Callback<void> callback) {
  }

  bool
  MirageNetDevice::IsBroadcast (void) const {
    NS_LOG_FUNCTION_NOARGS ();
    return true;
  }

  Address
  MirageNetDevice::GetBroadcast (void) const  {
    NS_LOG_FUNCTION_NOARGS ();
    return Mac48Address ("ff:ff:ff:ff:ff:ff");
  }

  bool
  MirageNetDevice::IsMulticast (void) const {
    NS_LOG_FUNCTION_NOARGS ();
    return true;
  }

  Address
  MirageNetDevice::GetMulticast (Ipv4Address multicastGroup) const {
    NS_LOG_FUNCTION (this << multicastGroup);
    Mac48Address multicast = Mac48Address::GetMulticast (multicastGroup);
    return multicast;
  }


  bool
  MirageNetDevice::IsPointToPoint (void) const {
    NS_LOG_FUNCTION_NOARGS ();
    return false;
  }

  bool
  MirageNetDevice::IsBridge (void) const {
    NS_LOG_FUNCTION_NOARGS ();
    return true;
  }

} //namespace ns3
