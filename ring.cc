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
 */

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/netanim-module.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("FirstScriptExample");

int main (int argc, char *argv[]){
  CommandLine cmd;
  cmd.Parse (argc, argv);

  Time::SetResolution (Time::NS);
  LogComponentEnable ("UdpEchoClientApplication", LOG_LEVEL_INFO);
  LogComponentEnable ("UdpEchoServerApplication", LOG_LEVEL_INFO);

  NodeContainer nodes;
  nodes.Create (4);

  PointToPointHelper ptp1;

  ptp1.SetDeviceAttribute ("DataRate", StringValue ("5Mbps"));
  ptp1.SetChannelAttribute ("Delay", StringValue ("2ms"));



  NetDeviceContainer d1,d2,d3,d4;
  d1 = ptp1.Install (nodes.Get(0),nodes.Get(1));
  d2 = ptp1.Install (nodes.Get(1),nodes.Get(2));
  d3 = ptp1.Install (nodes.Get(2),nodes.Get(3));
  d4 = ptp1.Install (nodes.Get(3),nodes.Get(0));


  InternetStackHelper stack;
  stack.Install (nodes);

  Ipv4AddressHelper address;

  address.SetBase ("10.1.1.0", "255.255.255.0");
  Ipv4InterfaceContainer i1 = address.Assign (d1);

  address.SetBase ("10.1.2.0", "255.255.255.0");
  Ipv4InterfaceContainer i2 = address.Assign (d2);

  Ipv4InterfaceContainer i3 = address.Assign (d3);
  address.SetBase ("10.1.3.0", "255.255.255.0");

  Ipv4InterfaceContainer i4 = address.Assign (d4);
  address.SetBase ("10.1.4.0", "255.255.255.0");

  Ipv4GlobalRoutingHelper::PopulateRoutingTables();

  uint16_t port1=9;
  uint16_t port2=12;


  UdpEchoServerHelper s1(port1);
  UdpEchoServerHelper s2(port2);

  ApplicationContainer serverApps;
  s1.Install (nodes.Get (0));
  s2.Install (nodes.Get (3));

  serverApps.Start (Seconds (1.0));
  serverApps.Stop (Seconds (10.0));

  UdpEchoClientHelper c1 (i2.GetAddress (1), port1);

  UdpEchoClientHelper c2 (i3.GetAddress(0), port2);


  c1.SetAttribute ("MaxPackets", UintegerValue (5));
  c1.SetAttribute ("Interval", TimeValue (Seconds (1.0)));
  c1.SetAttribute ("PacketSize", UintegerValue (1024));

  c2.SetAttribute ("MaxPackets", UintegerValue (5));
  c2.SetAttribute ("Interval", TimeValue (Seconds (1.0)));
  c2.SetAttribute ("PacketSize", UintegerValue (1024));


  ApplicationContainer clientApps1,clientApps2;
  clientApps1 = c1.Install (nodes.Get (0));
  clientApps1.Start (Seconds (2.0));
  clientApps1.Stop (Seconds (10.0));

  clientApps2 = c2.Install (nodes.Get (3));
  clientApps2.Start (Seconds (2.0));
  clientApps2.Stop (Seconds (10.0));


  AnimationInterface anim("trial.xml");
  anim.SetConstantPosition(nodes.Get(0),100,100);
  anim.SetConstantPosition(nodes.Get(1),200,100);
  anim.SetConstantPosition(nodes.Get(2),100,100);
  anim.SetConstantPosition(nodes.Get(0),200,200);

//  anim.EnablePacketMetaData();


  Simulator::Run ();
  Simulator::Destroy ();
  return 0;
}
