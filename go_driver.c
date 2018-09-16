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
static int debug_level = DEBUG_LEVEL_ERROR;
#else
static int debug_level = 0;
#endif

module_param(debug_level, int, S_IRUGO | S_IWUSR);
MODULE_PARM_DESC(debug_level, "debug level bitmask CRITICAL=1 ERROR=3 WARN=7 INFO=15 DEBUG=31");

/* table of devices that work with this driver */
static struct usb_device_id go_usb_table[] = {
    //    {.driver_info = 69},
    {USB_DEVICE(USB_VENDOR_ID, USB_PRODUCT_ID)},
    {} /* Terminating entry */
};
MODULE_DEVICE_TABLE(usb, go_usb_table);

ssize_t go_usb_read(struct file *file, char __user *buffer, size_t len, loff_t *offset)
{
    DBG_INFO("Read called.");
    return 0;
}
ssize_t go_usb_write(struct file *file, const char __user *buffer, size_t len, loff_t *offset)
{
    DBG_INFO("Write called.");
    return 0;
}
int go_usb_open(struct inode *inode, struct file *file)
{
    DBG_INFO("Ubs opened");
    return 0;
}
int go_usb_release(struct inode *inode, struct file *file)
{
    DBG_INFO("Usb released");
    return 0;
}

struct file_operations go_usb_fops = {
    .owner = THIS_MODULE,
    .read = go_usb_read,
    .write = go_usb_write,
    .open = go_usb_open,
    .release = go_usb_release,
};

struct usb_class_driver go_usb_class = {
    .name = "go_usb%d",
    .fops = &go_usb_fops,
    .minor_base = GO_USB_MINOR_BASE,
};

static int go_usb_probe(struct usb_interface *interface, const struct usb_device_id *id)
{
    struct usb_device *usb_dev = interface_to_usbdev(interface);
    struct usb_host_interface *iface_desc;
    struct usb_endpoint_descriptor *endpt;
    go_usb_t *go_usb_dev;
    __u16 buffer_size;
    int retval = -ENOMEM;
    int i;

    go_usb_dev = kzalloc(sizeof(*go_usb_dev), GFP_KERNEL);
    if (go_usb_dev == NULL)
    {
        DBG_CRIT("unable to allocate memory for %s %s", usb_dev->manufacturer, usb_dev->product);
        return retval;
    }

    go_usb_dev->usb_dev = usb_get_dev(usb_dev);
    go_usb_dev->interface = interface;

    /* set up the endpoint information */
    /* use only the first bulk-in and bulk-out endpoints */
    iface_desc = interface->cur_altsetting;
    for (i = 0; i < iface_desc->desc.bNumEndpoints; ++i)
    {
        endpt = &iface_desc->endpoint[i].desc;
        DBG_DEBUG("find endpoint: 0x%x, size: %d\n", endpt->bEndpointAddress, endpt->wMaxPacketSize);

        if (!go_usb_dev->bulk_in_endpointAddr &&
            (endpt->bEndpointAddress & USB_DIR_IN) &&
            ((endpt->bmAttributes & USB_ENDPOINT_XFERTYPE_MASK) == USB_ENDPOINT_XFER_BULK))
        {
            /* we found a bulk in endpoint */
            buffer_size = endpt->wMaxPacketSize;
            go_usb_dev->bulk_in_size = buffer_size;
            go_usb_dev->bulk_in_endpointAddr = endpt->bEndpointAddress;
            go_usb_dev->bulk_in_buffer = kmalloc(buffer_size, GFP_KERNEL);
            if (!go_usb_dev->bulk_in_buffer)
            {
                DBG_ERR("Could not allocate bulk_in_buffer");
                goto error;
            }
            DBG_INFO("found bulk in addr: 0x%x, size: %d\n", go_usb_dev->bulk_in_endpointAddr, go_usb_dev->bulk_in_size);
        }

        if (!go_usb_dev->bulk_out_endpointAddr &&
            !(endpt->bEndpointAddress & USB_DIR_IN) &&
            ((endpt->bmAttributes & USB_ENDPOINT_XFERTYPE_MASK) == USB_ENDPOINT_XFER_BULK))
        {
            /* we found a bulk out endpoint */
            go_usb_dev->bulk_out_endpointAddr = endpt->bEndpointAddress;
            DBG_INFO("found bulk out addr: 0x%x, size: %d\n", go_usb_dev->bulk_in_endpointAddr, go_usb_dev->bulk_in_size);
        }
    }
    if (!(go_usb_dev->bulk_in_endpointAddr && go_usb_dev->bulk_out_endpointAddr))
    {
        DBG_ERR("Could not find both bulk-in and bulk-out endpoints");
        retval = -ENODATA;
        goto error;
    }

    usb_set_intfdata(interface, go_usb_dev);
    /* we can register the device now, as it is ready */
    retval = usb_register_dev(interface, &go_usb_class);
    if (retval)
    {
        /* something prevented us from registering this driver */
        DBG_ERR("Not able to get a minor for this device.");
        usb_set_intfdata(interface, NULL);
        goto error;
    }

    dev_info(&interface->dev, "%s %s:SERIAL %s attached", usb_dev->manufacturer, usb_dev->product, usb_dev->serial);
    DBG_INFO("usb drive (idVendor %04X:idProduct %04X) plugged", id->idVendor, id->idProduct);

    return 0;

error:
    usb_put_dev(go_usb_dev->usb_dev);
    DBG_ERR("operation terminated with exit code 0x%x(%d)", retval, retval);
    kfree(go_usb_dev);
    return retval;
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
    .name = "go_usb_driver",
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
        DBG_ERR("usb_register failed for %s the driver. Error number %d", __FILE__, result);
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