#include <iostream>
#include <string>

#include "UDP.hpp"


int main(void)
{
	uint8_t buffer[1024];
	uint32_t ip_address;
	uint16_t port_number;
	int n;
	UDP socket(12345); // inizializzazione socket con porta UDP numero 12345

	while (true)
	{
		// attesa eventuale ricezione datagram
		if (socket.wait(1000) > 0)
		{
			// verifica ricezione datagram
			if ((n = socket.receive(&ip_address, &port_number, buffer, sizeof(buffer))) > 0)
			{
				buffer[n] = '\0';
				std::string s((char*)buffer);
				std::cout << s << std::endl;
				// ritrasmissione datagram ricevuto al programma che lo ha inviato 
				socket.send(ip_address, port_number, buffer, n);
			}
		}
	}
	return 0;
}
