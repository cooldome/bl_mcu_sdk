#include "usbd_core.h"
#include "usbd_audio.h"

struct usbd_audio_control_info audio_control_info = { 0xdb00, 0x0000, 0x0100, 0xf600, 0 };

int audio_class_request_handler(struct usb_setup_packet *setup, uint8_t **data, uint32_t *len)
{
    USBD_LOG_DBG("Class request:"
                 "bRequest 0x%02x, bmRequestType 0x%02x len %d",
                 setup->bRequest, setup->bmRequestType, *len);

    switch (setup->bRequest) {
        case AUDIO_REQUEST_SET_CUR:

            if (setup->wValueL == 0x01) {
                if (setup->wValueH == AUDIO_FU_CONTROL_MUTE) {
                    memcpy(&audio_control_info.mute, *data, *len);
                } else if (setup->wValueH == AUDIO_FU_CONTROL_VOLUME) {
                    memcpy(&audio_control_info.vol_current, *data, *len);
                    USBD_LOG_DBG("vol:0x%x\r\n", audio_control_info.vol_current);
                }
            }

            break;

        case AUDIO_REQUEST_GET_CUR:
            if (setup->wValueH == AUDIO_FU_CONTROL_MUTE) {
                *data = (uint8_t *)&audio_control_info.mute;
                *len = 1;
            } else if (setup->wValueH == AUDIO_FU_CONTROL_VOLUME) {
                *data = (uint8_t *)&audio_control_info.vol_current;
                *len = 2;
            }

            break;

        case AUDIO_REQUEST_SET_RES:
            break;

        case AUDIO_REQUEST_GET_MIN:
            *data = (uint8_t *)&audio_control_info.vol_min;
            *len = 2;
            break;

        case AUDIO_REQUEST_GET_MAX:
            *data = (uint8_t *)&audio_control_info.vol_max;
            *len = 2;
            break;

        case AUDIO_REQUEST_GET_RES:
            *data = (uint8_t *)&audio_control_info.vol_res;
            *len = 2;
            break;

        default:
            USBD_LOG_ERR("Unhandled request 0x%02x", setup->bRequest);
            break;
    }

    return 0;
}

void audio_notify_handler(uint8_t event, void *arg)
{
    switch (event) {
        case USB_EVENT_RESET:

            break;

        case USB_EVENT_SOF:
            break;

        case USB_EVENT_SET_INTERFACE:
            usbd_audio_set_interface_callback(((uint8_t *)arg)[3]);
            break;

        default:
            break;
    }
}

void usbd_audio_add_interface(usbd_class_t *class, usbd_interface_t *intf)
{
    static usbd_class_t *last_class = NULL;

    if (last_class != class) {
        last_class = class;
        usbd_class_register(class);
    }

    intf->class_handler = audio_class_request_handler;
    intf->custom_handler = NULL;
    intf->vendor_handler = NULL;
    intf->notify_handler = audio_notify_handler;
    usbd_class_add_interface(class, intf);
}