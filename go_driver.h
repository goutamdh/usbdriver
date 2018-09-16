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
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/usb.h>
#include <linux/slab.h>

#define DBG_DEBUG(fmt, args...)                                 \
    if ((debug_level & DEBUG_LEVEL_DEBUG) == DEBUG_LEVEL_DEBUG) \
    printk(KERN_DEBUG "[debug] %s(%d): " fmt "\n",              \
           __FUNCTION__, __LINE__, ##args)
#define DBG_INFO(fmt, args...)                                \
    if ((debug_level & DEBUG_LEVEL_INFO) == DEBUG_LEVEL_INFO) \
    printk(KERN_INFO "[info]  %s(%d): " fmt "\n",             \
           __FUNCTION__, __LINE__, ##args)
#define DBG_WARN(fmt, args...)                                \
    if ((debug_level & DEBUG_LEVEL_WARN) == DEBUG_LEVEL_WARN) \
    printk(KERN_WARNING "[warn]  %s(%d): " fmt "\n",          \
           __FUNCTION__, __LINE__, ##args)
#define DBG_ERR(fmt, args...)                                   \
    if ((debug_level & DEBUG_LEVEL_ERROR) == DEBUG_LEVEL_ERROR) \
    printk(KERN_ERR "[eror]   %s(%d): " fmt "\n",               \
           __FUNCTION__, __LINE__, ##args)
#define DBG_CRIT(fmt, args...)                                        \
    if ((debug_level & DEBUG_LEVEL_CRITICAL) == DEBUG_LEVEL_CRITICAL) \
    printk(KERN_CRIT "[crit]  %s(%d): " fmt "\n",                     \
           __FUNCTION__, __LINE__, ##args)

#define DEBUG
#define DEBUG_LEVEL_DEBUG     0x1F
#define DEBUG_LEVEL_INFO      0x0F
#define DEBUG_LEVEL_WARN      0x07
#define DEBUG_LEVEL_ERROR     0x03
#define DEBUG_LEVEL_CRITICAL  0x01

#define USB_VENDOR_ID         0x0781
#define USB_PRODUCT_ID        0x558a

#define DEVICE_NAME "go_usb"
#define MOD_AUTHOR "Pawan Kumar <jmppawanhit@gmail.com>"
#define MOD_DESC "go_usb driver"
#define MOD_SUPPORT "usb device"

typedef struct go_usb
{
    struct usb_device *usb_dev;
    int go_usb_var;
} go_usb_t;

static int go_usb_probe(struct usb_interface *interface, const struct usb_device_id *id);
static void go_usb_disconnect(struct usb_interface *interface);
static int __init go_usb_init( void );
static void __exit go_usb_exit( void );