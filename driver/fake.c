#include <stddef.h>
#include <stdint.h>

#include "fake.h"
#include "net.h"
#include "util.h"

#define FAKE_MTU UINT16_MAX

/// @brief fake_device's transmit ops-function
/// @param dev 
/// @param type 
/// @param data 
/// @param len 
/// @param dst 
/// @return 
static int fake_transmit(struct net_device *dev, uint16_t type, const uint8_t *data, size_t len, const void *dst)
{
    _debug("dev=%s, type=0x%04x, len=%zu", dev->name, type, len);
    debug_dump(data, len);
    return 0;
}

static struct net_device_ops fake_ops = {
    .transmit = fake_transmit,
};

/// @brief Initialize the fake_device, by net_device_alloc, net_device_register and ops.
struct net_device *fake_init()
{
    struct net_device *dev;
    dev = net_device_alloc();
    if(!dev){
        _error("net_device_alloc failure");
        return NULL;
    }
    dev->type = NET_DEVICE_TYPE_FAKE;
    dev->mtu = FAKE_MTU;
    dev->hlen = 0;
    dev->alen = 0;
    dev->ops = &fake_ops;
    if(net_device_register(dev) != 0){
        _error("net_device_register failure");
        return NULL;
    }
    _debug("initialized, dev=%s", dev->name);
    return dev;
}