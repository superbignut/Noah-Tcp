#ifndef __TEST_H_
#define __TEST_H_

#include <stdint.h>

/* rfc5735 is intended to collect these scattered
    references and provide a current list of special use IPv4 addresses.*/

/* 127.0.0.0/8 Loopback */
#define LOOPBACK_IP_ADDR "127.0.0.1" 
#define LOOPBACK_NETMAST "255.0.0.0"

#define ETHER_TAP_NAME "tap0"

/* rfc7042 */
/* 00-00-5E-00-53-00 through 00-00-5E-00-53-FF: assigned for use in documentation. */
#define ETHER_TAP_HW_ADDR "00:00:5e:00:53:01"

/* rfc5731 puzzled.*/
#define ETHER_TAP_IP_ADDR "192.0.2.2"
/* 192.0.2.0/24 - This block is assigned as "TEST-NET-1" for use in documentation and example code. */
#define DEFAULT_GATEWAY "192.0.2.1"

const uint8_t test_data[] = {
    0x45, 0x00, 0x00, 0x30,
    0x00, 0x80, 0x00, 0x00,
    0xff, 0x01, 0xbd, 0x4a, 
    0x7f, 0x00, 0x00, 0x01, 
    0x7f, 0x00, 0x00, 0x01, 
    0x08, 0x00, 0x35, 0x64, 
    0x00, 0x80, 0x00, 0x01, 
    0x31, 0x32, 0x33, 0x34, 
    0x34, 0x36, 0x37, 0x38, 
    0x39, 0x30, 0x21, 0x40, 
    0x23, 0x24, 0x25, 0x5e, 
    0x26, 0x2a, 0x28, 0x29
};

#endif