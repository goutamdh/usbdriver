/* @Module: usbdriver.c  
 * @Author: Pawan Kumar
 * @Email: jmppawanhit@gmail.com
 * @License: GPL
 * @Domain: Kernel Module Programming
 * @Description: 
 * @Copyright: Copyright (C) 2018 pkmys@github.com
 */

/******************************************
 *               includes                 *
 ******************************************/
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/device.h>
#include <linux/init.h>
#include <linux/usb.h>

#include "usbdriver.h"

/*******************************************
 *                modinfo                  *
 *******************************************/
MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR(MOD_AUTHOR);
MODULE_DESCRIPTION(MOD_DESC);
MODULE_VERSION("0.1");
MODULE_SUPPORTED_DEVICE(MOD_SUPPORT); 

static int go_usb_probe(struct usb_interface *interface, const struct usb_device_id *id)
{
#ifdef DEBUG
    dev_info(&interface->dev, "A usb device attached");
#endif
    printk(KERN_INFO "%d %s: Usb drive (%04X:%04X) plugged\n",__LINE__, __func__, id->idVendor, id->idProduct);
    return 0;
}

static void go_usb_disconnect (struct usb_interface *interface)
{
#ifdef DEBUG
    dev_info(&interface->dev, "A usb device detached");
#endif
    printk(KERN_INFO "%d %s: Usb drive removed\n",__LINE__, __func__);
}

/* table of devices that work with this driver */
static struct usb_device_id go_usb_table[] = {
    {USB_DEVICE(USB_VENDOR_ID, USB_PRODUCT_ID)},
    {} /* Terminating entry */
};
MODULE_DEVICE_TABLE(usb, go_usb_table);

static struct usb_driver go_usb_driver = {
    .name       = "go_usb driver",
    .probe      = go_usb_probe,
    .disconnect = go_usb_disconnect,
    .id_table   = go_usb_table,
};

static int __init usb_go_usb_init(void)
{
    int result;

    /* register this driver with the USB subsystem */
    result = usb_register(&go_usb_driver);
    if (result < 0)
    {
        printk(KERN_ALERT "%d %s: usb_register failed for %s the driver. Error number %d\n",__LINE__, __func__, __FILE__, result);
        return -1;
    }
    printk(KERN_INFO "%d %s: usb_register success\n",__LINE__, __func__);

    return 0;
}
module_init(usb_go_usb_init);

static void __exit usb_go_usb_exit(void)
{
    /* deregister this driver with the USB subsystem */
    usb_deregister(&go_usb_driver);
    printk(KERN_INFO "%d %s: usb_deregister success\n",__LINE__, __func__);
}
module_exit(usb_go_usb_exit);


