#include "public.h"
#include <SPI.h>
#include <Ethernet.h>
#include <EthernetUdp.h>
#include "config_comm.h"

char packetBuffer[UDP_TX_PACKET_MAX_SIZE]; //buffer to hold incoming packet,

EthernetUDP Udp;
void comm_init()
{
  byte mac[] = {UDP_MAC};
  IPAddress ip(UDP_IP);
  Serial.begin(BD_RATE);
  Ethernet.begin(mac, ip);
  Udp.begin(UDP_PORT);
  logs.println("System running...");
}
void run_cmd(char buf[], u8 sz)
{
  for (u8 i = 0; i < sz; i++)
  {
    receive_data(buf[i]);
  }
}
void get_udp()
{
  int packetSize = Udp.parsePacket();
  if (packetSize)
  {
    logs.print("Received packet of size ");
    logs.println(packetSize);
    logs.print("From ");
    IPAddress remote = Udp.remoteIP();
    for (int i = 0; i < 4; i++)
    {
      logs.print(remote[i], DEC);
      if (i < 3)
      {
        logs.print(".");
      }
    }
    logs.print(", port ");
    logs.println(Udp.remotePort());

    // read the packet into packetBufffer
    Udp.read(packetBuffer, UDP_TX_PACKET_MAX_SIZE);
    logs.println("Contents:");
    for (int i = 0; i < packetSize; i++)
    {
      logs.print(packetBuffer[i], DEC);
      logs.print(",");
    }
    logs.println();
    packetBuffer[packetSize] = 0;
    shell_exec(packetBuffer,packetSize);
  }
}
void get_uart()
{

}

void get_data()
{
  get_udp();
  get_uart();
}

