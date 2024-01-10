extern "C"
{
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/select.h>
}

#include "UDP.hpp"

uint32_t dotted2binary(char p_acIP[])
{
	uint32_t l_ulAdd = inet_network(p_acIP);
	return l_ulAdd;
}

UDP::UDP(uint16_t p_usPort)
{
	struct sockaddr_in l_localAddress; 
	
	m_bValidSocket = false;
	if ((m_sd = socket(AF_INET, SOCK_DGRAM | SOCK_NONBLOCK, IPPROTO_UDP)) < 0)
	  return;
	memset(&l_localAddress, 0, sizeof(l_localAddress));
	l_localAddress.sin_family = AF_INET;
	l_localAddress.sin_addr.s_addr = 0;
	l_localAddress.sin_port = htons(p_usPort);
	if (bind(m_sd, (struct sockaddr *)&l_localAddress, sizeof(l_localAddress)) < 0)
	{
	  close(m_sd);
	  return;
	}
	m_bValidSocket = true;
	return;
}

UDP::~UDP(void)
{
  if (m_bValidSocket)
    close(m_sd);
  return;
}

int UDP::send(uint32_t p_ulRemoteHost, uint16_t p_usRemotePort, uint8_t p_aucBuffer[], int p_iNbytes)
{
  struct sockaddr_in l_remoteAddress;                       

  if (!m_bValidSocket)
  	return -1;
	
  memset((char*)&l_remoteAddress, 0 , sizeof(l_remoteAddress));
  l_remoteAddress.sin_family = AF_INET;
  l_remoteAddress.sin_port = htons(p_usRemotePort);
  l_remoteAddress.sin_addr.s_addr = htonl(p_ulRemoteHost);
 
  if (sendto(m_sd, (void*)p_aucBuffer, p_iNbytes, 0, (struct sockaddr*)&l_remoteAddress, sizeof(l_remoteAddress)) < 0)
  	return -1;
  return 0;
}

         
int UDP::receive(uint32_t *p_pulRemoteHost, uint16_t *p_pusRemotePort, uint8_t *p_aucBuffer, int p_iDim)
{
  struct sockaddr_in l_remoteAddress;
  unsigned int l_iRemoteAddressSize = sizeof(l_remoteAddress);
  int l_iNbytes;

  if (!m_bValidSocket)
    return -1;
  
  if ((l_iNbytes = recvfrom(m_sd, (void*)p_aucBuffer, p_iDim, 0, (struct sockaddr*)&l_remoteAddress, &l_iRemoteAddressSize)) <= 0)
    return -1;
  *p_pulRemoteHost=ntohl(l_remoteAddress.sin_addr.s_addr);
  *p_pusRemotePort=ntohs(l_remoteAddress.sin_port);
	
  return (l_iNbytes);
}

int UDP::wait(unsigned long p_ulTimeout)
{
   int l_iN;
   struct timeval l_timeout;
   fd_set l_socketSet;
  
   if (!m_bValidSocket)
  	 return -1;

   FD_ZERO(&l_socketSet);
   FD_SET(m_sd, &l_socketSet);
   l_timeout.tv_sec = p_ulTimeout/1000;
   l_timeout.tv_usec = (p_ulTimeout%1000)*1000;
   
   l_iN = select(m_sd+1, &l_socketSet, NULL, NULL, &l_timeout);
   if (l_iN < 0)
	 return -1;
   if (l_iN == 0)
	 return 0;
   if (FD_ISSET(m_sd, &l_socketSet))
	 return 1;
   else
	   return 0;
}
