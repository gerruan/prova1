
#ifndef _UDP
#define _UDP

extern "C"
{
#include <stdint.h>
}


/*!
\brief convert an IP address from ASCII dotted format to binary format
\param[in] p_acIP address in ASCII dotted format
\return IP address in binary format
*/
uint32_t dotted2binary(char p_acIP[]);


/*!
\brief class used to instance objects that receive/transmit UDP datagram
*/
class UDP
{
 private:
 
 /*!
 \brief socket descriptor
 */
 int m_sd;

 /*!
 \brief socket descriptor validity
 */
 bool m_bValidSocket;

 

 public:
 
 /*!
 \brief class constructor
 \param[in] p_usPort local UDP port number (0 = random)
 */
 UDP(uint16_t p_usPort);

 
 /*!
 \brief ckass destructor
 */
 ~UDP (void);
 
 /*!
 \brief method to send data
 \param[in] p_ulRemoteHost receiver IP address
 \param[in] p_usRemotePort receiver UDP port number
 \param[in] p_aucBuffer data to send
 \param[in] p_iNbytes data size in bytes
 \return -1 in case of error, 0 otherwise
 */
 int send(uint32_t p_ulRemoteHost, uint16_t p_usRemotePort, uint8_t p_aucBuffer[], int p_iNbytes);
 
 /*!
 \brief method to receive data (return immediately if no data received)
 \param[out] p_pulRemoteHost sender IP address
 \param[out] p_pusRemotePort sender UDp port number
 \param[out] p_aucBuffer received data
 \param[in] p_iDim sizeo of buffer for received data
 \return -1 in case of error, 0 if no data received, >0 number of bytes received
 */
 int receive(uint32_t *p_pulRemoteHost, uint16_t *p_pusRemotePort, uint8_t *p_aucBuffer, int p_iDim);

 /*!
 \brief wait for input data with timeout
 \param[in] p_ulTimeout timeout (ms)
 \return -1 in case of error, 0 in case of timeout, 1 if some data received
 */
 int wait(unsigned long p_ulTimeout);
};

#endif
