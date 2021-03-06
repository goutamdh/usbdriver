/* @Module: usbdriver.h  
 * @Author: Pawan Kumar
 * @Email: jmppawanhit@gmail.com
 * @License: GPL
 * @Domain: Kernel Module Programming
 * @Description: 
 * @Copyright: Copyright (C) 2018 pkmys@github.com
 */

#ifndef _GO_DRIVER_H_
#define _GO_DRIVER_H_
#endif

/******************************************
 *               includes                 *
 ******************************************/
#include <linux/errno.h>
#include <linux/mutex.h>
#include <linux/usb.h>


#define DBG_DEBUG(fmt, args...)                                 \
    if ((debug_level & DEBUG_LEVEL_DEBUG) == DEBUG_LEVEL_DEBUG) \
    printk(KERN_DEBUG "%s l%d:\t[debg] " fmt "\n",               \
           __FUNCTION__, __LINE__, ##args)
#define DBG_INFO(fmt, args...)                                \
    if ((debug_level & DEBUG_LEVEL_INFO) == DEBUG_LEVEL_INFO) \
    printk(KERN_INFO "%s l%d:\t[info] " fmt "\n",             \
           __FUNCTION__, __LINE__, ##args)
#define DBG_WARN(fmt, args...)                                \
    if ((debug_level & DEBUG_LEVEL_WARN) == DEBUG_LEVEL_WARN) \
    printk(KERN_WARNING "%s l%d:\t[warn] " fmt "\n",          \
           __FUNCTION__, __LINE__, ##args)
#define DBG_ERR(fmt, args...)                                   \
    if ((debug_level & DEBUG_LEVEL_ERROR) == DEBUG_LEVEL_ERROR) \
    printk(KERN_ERR "%s l%d:\t[eror] " fmt "\n",               \
           __FUNCTION__, __LINE__, ##args)
#define DBG_CRIT(fmt, args...)                                        \
    if ((debug_level & DEBUG_LEVEL_CRITICAL) == DEBUG_LEVEL_CRITICAL) \
    printk(KERN_CRIT "%s l%d:\t[crit] " fmt "\n",                     \
           __FUNCTION__, __LINE__, ##args)

#define DEBUG
#define DEBUG_LEVEL_DEBUG       0x1F
#define DEBUG_LEVEL_INFO        0x0F
#define DEBUG_LEVEL_WARN        0x07
#define DEBUG_LEVEL_ERROR       0x03
#define DEBUG_LEVEL_CRITICAL    0x01

#define USB_VENDOR_ID           0x0781
#define USB_PRODUCT_ID          0x558A

#define GO_USB_MINOR_BASE       0xC0

#define DEVICE_NAME             "go_usb"
#define MOD_AUTHOR              "Pawan Kumar <jmppawanhit@gmail.com>"
#define MOD_DESC                "go_usb driver"
#define MOD_SUPPORT             "usb device"

struct go_usb
{
    struct usb_device            *usb_dev;
    struct usb_interface         *interface;
    struct mutex		          io_mutex;
    __u8                         *bulk_in_buffer;
    __u8                          bulk_in_endpointAddr;
    __u8                          bulk_out_endpointAddr;
    __u16                         bulk_in_size;
    __u16                          open_count;
};

static int go_usb_probe(struct usb_interface *interface, const struct usb_device_id *id);
static void go_usb_disconnect(struct usb_interface *interface);
static int __init go_usb_init(void);
static void __exit go_usb_exit(void);
static struct usb_driver go_usb_driver;