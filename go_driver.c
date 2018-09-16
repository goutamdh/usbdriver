/* @Module: usbdriver.c  
 * @Author: Pawan Kumar
 * @Email: jmppawanhit@gmail.com
 * @License: GPL
 * @Domain: Kernel Module Programming
 * @Description: 
 * @Copyright: Copyright (C) 2018 pkmys@github.com
 */

#include "go_driver.h"

#ifdef DEBUG
static int debug_level = DEBUG_LEVEL_INFO;
#else
static int debug_level = 0;
#endif

module_param(debug_level, int, S_IRUGO | S_IWUSR);
MODULE_PARM_DESC(debug_level,"debug level bitmask CRITICAL=1 ERROR=3 WARN=7 INFO=15 DEBUG=31");

/* table of devices that work with this driver */
static struct usb_device_id go_usb_table[] = {
    {USB_DEVICE(USB_VENDOR_ID, USB_PRODUCT_ID)},
    {} /* Terminating entry */
};
MODULE_DEVICE_TABLE(usb, go_usb_table);

static int go_usb_probe(struct usb_interface *interface, const struct usb_device_id *id)
{
    struct usb_device *usb_dev = interface_to_usbdev(interface);
    go_usb_t *go_usb_dev;

    go_usb_dev = kzalloc(sizeof(*go_usb_dev), GFP_KERNEL);
    if (go_usb_dev == NULL)
    {
        DBG_CRIT("unable to allocate memory for %s %s", usb_dev->manufacturer, usb_dev->product);
        return -ENOMEM;
    }

    go_usb_dev->usb_dev = usb_get_dev(usb_dev);
    usb_set_intfdata(interface, go_usb_dev);

    dev_info(&interface->dev, "%s %s:SERIAL %s attached", usb_dev->manufacturer, usb_dev->product, usb_dev->serial);

    DBG_INFO("usb drive (idVendor %04X:idProduct %04X) plugged", id->idVendor, id->idProduct);

    return 0;
}

static void go_usb_disconnect(struct usb_interface *interface)
{
    go_usb_t *go_usb_dev;
    go_usb_dev = usb_get_intfdata(interface);

    usb_set_intfdata(interface, NULL);
    usb_put_dev(go_usb_dev->usb_dev);
    
    dev_info(&interface->dev, "%s %s: detached", go_usb_dev->usb_dev->manufacturer, go_usb_dev->usb_dev->product);
    
    kfree(go_usb_dev);

    DBG_INFO("usb device removed");
}

static struct usb_driver go_usb_driver = {
    .name = "go_usb",
    .probe = go_usb_probe,
    .disconnect = go_usb_disconnect,
    .id_table = go_usb_table,
};

static int __init go_usb_init(void)
{
    int result;

    /* register this driver with the USB subsystem */
    result = usb_register(&go_usb_driver);
    if (result < 0)
    {
        DBG_ERR("usb_register failed for %s the driver. Error number %d",__FILE__, result);
        return -1;
    }
    DBG_INFO("usb_register success");

    return 0;
}
module_init(go_usb_init);

static void __exit go_usb_exit(void)
{
    /* deregister this driver with the USB subsystem */
    usb_deregister(&go_usb_driver);
    DBG_INFO("usb_deregister success");
}
module_exit(go_usb_exit);

/*******************************************
 *                modinfo                  *
 *******************************************/
MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR(MOD_AUTHOR);
MODULE_DESCRIPTION(MOD_DESC);
MODULE_VERSION("0.1");
MODULE_SUPPORTED_DEVICE(MOD_SUPPORT);