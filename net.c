

#include "net.h"
#include "util.h"
#include "platform.h"

/// @brief Devices list
static struct net_device *devices = NULL;


/// @brief Allocate the memory for a net_device.
struct net_device *net_device_alloc()
{
    struct net_device *dev;
    dev = memory_alloc(sizeof(*dev)); /* sizeof(*dev) not sizeof(dev) */
    if(dev == NULL)
        return NULL;
    return dev; 
}

/// @brief Give a name: net%d, and rigister the dev into static-devices.
/// @param dev 
/// @return 
int net_device_register(struct net_device *dev)
{
    static unsigned int index = 0;
    dev->index = index ++;
    snprintf(dev->name, sizeof(dev->name), "net%d", dev->index);
    /* Writes the results to a character string buffer. At most bufsz - 1 characters are written. 
    The resulting character string will be terminated with a null character, unless bufsz is zero. 
    If bufsz is zero, nothing is written and buffer may be a null pointer, however the return value
    (number of bytes that would be written not including the null terminator) is still calculated and returned.*/
    dev->next = devices; /* Add the dev to the head of devices. */
    devices = dev; /* Update the head of devices. */
    _info("registered dev=%s, type = 0x%04x", dev->name, dev->type);
    return 0;
}

/// @brief Open the device that has been closed, by calling relative open-function. Set the up_flag.
/// @param dev 
/// @return -1 when failure, 0 when success
static int net_device_open(struct net_device *dev)
{
    if(NET_DEVICE_IS_UP(dev)){ /* Open already. */
        _error("Already opened, dev=%s", dev->name);
        return -1;
    }
    if(dev->ops->open){ /* call the device-driver open-function. */
        if(dev->ops->open(dev) == -1){
            _error("failure, dev=%s", dev->name);
            return -1;
        }
    }
    dev->flags |= NET_DEVICE_FLAG_UP; // set the up_flag true: 0x0001 
    _info("dev=%s, state=%s", dev->name, NET_DEVICE_STATE(dev));
    return 0;
}

/// @brief Close the device that has been Opened, by calling relative close-function. Clear the up_flag.
/// @param dev 
/// @return -1 when failure, 0 when success
static int net_device_close(struct net_device *dev)
{
    if(!NET_DEVICE_IS_UP(dev)){ /* Close already. */
        _error("Already closed, dev=%s", dev->name);
        return -1;
    }
    if(dev->ops->close){ /* call the device-driver open-function. */
        if(dev->ops->close(dev) == -1){
            _error("failure, dev=%s", dev->name);
            return -1;
        }
    }
    dev->flags &= ~NET_DEVICE_FLAG_UP; // set the up_flag false : 0x0001 
    _info("dev=%s, state=%s", dev->name, NET_DEVICE_STATE(dev));
    return 0;
}

/// @brief Output the data to device-dst by calling relative transmit-function, len must less than dev->mtu.
/// @param dev 
/// @param type 
/// @param data 
/// @param len 
/// @param dst 
/// @return -1 when failure, 0 when success
int net_device_output(struct net_device *dev, uint16_t type, const uint8_t *data, size_t len, const void *dst)
{
    if(!NET_DEVICE_IS_UP(dev)){ /* Close already. */
        _error("not opened, dev=%s", dev->name);
        return -1;
    }
    if(len > dev->mtu){
        _error("too long data, dev=%s, mtu=%u, len=%zu", dev->name, dev->mtu, len);
        return -1;
    }
    _debug("dev=%s, type=0x%04x, len=%zu", dev->name, type, len);
    debug_dump(data, len);
    if(dev->ops->transmit){
        if(dev->ops->transmit(dev, type, data, len, dst) == -1){
            _error("device transmit failure, dev=%s, len=%zu",dev->name, len);
            return -1;
        }
    }
    return 0;
}

int net_input_handler(uint16_t type, const uint8_t *data, size_t len, struct net_device *dev)
{
    _debug("dev=%s, type=0x%04x, len=%zu", dev->name, type, len);
    debug_dump(data, len);
    return 0;
}

/// @brief Open all static-devices and 
/// @return -1 when failure, 0 when success
int net_run()
{
    struct net_device *dev;
    _debug("open all device...");
    for(dev = devices;dev;dev = dev->next){
        net_device_open(dev);
    }
    _debug("running...");
    return 0;
}

/// @brief Close all static-devices.
void net_shutdown()
{
    struct net_device *dev;
    _debug("close all device...");
    for(dev = devices;dev;dev = dev->next){
        net_device_close(dev);
    }
    _debug("shutting down");
}

/// @brief 
/// @return 
int net_init()
{
    _info("initialized");
    return 0;
}
