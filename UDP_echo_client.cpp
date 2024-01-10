#include <iostream>
#include <cstring>

#include "UDP.hpp"

#define TIMEOUT	1000 // 1 secondo

using namespace std;

int main(int argc, char* argv[])
{
	uint8_t send[1024]; // buffer di trasmissione
	uint8_t receive[1024]; // buffer di ricezione
	uint32_t ip_address;
	uint16_t port_number;
	int n;

	if (argc < 3)
	{
		cout << "Uso: " << argv[0] << " indirizzo-IP messaggio" << endl;
		return -1;
	}

	UDP socket(0); // inizializzazione socket con numero di porta UDP arbitrario
	ip_address = dotted2binary(argv[1]);
	port_number = 12345;
	n = strlen(argv[2]);
	memcpy(send, argv[2], n);
	// trasmissione stringa al server
	socket.send(ip_address, port_number, send, n);
	if (socket.wait(TIMEOUT) > 0)
	{
		if ((n = socket.receive(&ip_address, &port_number, receive, sizeof(receive))) > 0)
		{
			receive[n] = '\0';
			cout << "Ricevuto: " << receive << endl;
			return 0;
		}
	}
	// trascorso un tempo pari a TIMEOUT senza ricevere risposta
	cout << "Nessuna risposta ricevuta!" << endl;
	return -1;
}
