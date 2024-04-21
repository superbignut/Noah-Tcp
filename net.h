#ifndef __NET_H_
#define __NET_H_

#include <stddef.h> // size_t
#include <stdint.h> // INT_MAX

#ifndef IFNAMSIZ // interface name size
#define IFNAMSIZ 16
#endif

#define NET_DEVICE_TYPE_FAKE        0x0000
#define NET_DEVICE_TYPE_LOOPBACK    0x0001
#define NET_DEVICE_TYPE_ETHERNET    0x0002

#define NET_DEVICE_FLAG_UP          0x0001
#define NET_DEVICE_FLAG_LOOPBACK    0x0010
#define NET_DEVICE_FLAG_BROADCAST   0x0020
#define NET_DEVICE_FLAG_P2P         0x0040
#define NET_DEVICE_FLAG_NEED_ARP    0x0100

#define NET_DEVICE_ADDR_LEN 16

/// x is a struct net_device * xxx.
#define NET_DEVICE_IS_UP(x) ((x)->flags & NET_DEVICE_FLAG_UP)
#define NET_DEVICE_STATE(x) (NET_DEVICE_IS_UP(x) ? "up" : "down")

// !MUT
/* In computer networking, the maximum transmission unit (MTU) is the size of the largest protocol data unit (PDU) 
that can be communicated in a single network layer transaction.The MTU relates to, but is not identical to the maximum 
frame size that can be transported on the data link layer, e.g., Ethernet frame.*/

struct net_device{
    struct net_device *next;
    unsigned int index;
    char name[IFNAMSIZ];
    uint16_t type;
    uint16_t mtu;
    uint16_t flags;
    uint16_t hlen;
    uint16_t alen;
    uint8_t addr[NET_DEVICE_ADDR_LEN];
    union
    {
        uint8_t peer[NET_DEVICE_ADDR_LEN];
        uint8_t broadcast[NET_DEVICE_ADDR_LEN];
    };
    struct net_device_ops *ops;
    void *priv;
};

struct net_device_ops{
    int(*open)(struct net_device *dev);
    int(*close)(struct net_device *dev);
    int(*transmit)(struct net_device *dev, uint16_t type, const uint8_t *data, size_t len, const void *dst);
};

struct net_device* net_device_alloc();

int net_device_register(struct net_device *dev);

int net_device_output(struct net_device *dev, uint16_t type, const uint8_t *data, size_t len, const void *dst);

int net_input_handler(uint16_t type, const uint8_t *data, size_t len, struct net_device *dev);

int net_run();

void net_shutdown();

int net_init();

#endif