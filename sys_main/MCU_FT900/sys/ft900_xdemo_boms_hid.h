#include "ft900_usb.h"
#include "ft900_usb_hid.h"
#include "ft900_usb_boms.h"

#define MODULE_HID
#define MODULE_BOMS

#if !defined(MODULE_HID) && !defined(MODULE_BOMS)
#error "at least define one of HID or BOMS!"
#endif

#define USB_DESCRIPTOR_DEVICE_LEN         0x12
#define USB_DESCRIPTOR_CONFIG_LEN         0x09
#define USB_DESCRIPTOR_INTERFACE_LEN      0x09
#define USB_DESCRIPTOR_HID_LEN            0x09
#define USB_DESCRIPTOR_ENDPOINT_LEN       0x07

#define USB_HID_REQUEST_CODE_GET_REPORT	  0x01
#define USB_HID_REQUEST_CODE_GET_IDLE	  0x02
#define USB_HID_REQUEST_CODE_GET_PROTOCOL 0x03
#define USB_HID_REQUEST_CODE_SET_REPORT	  0x09
#define USB_HID_REQUEST_CODE_SET_IDLE	  0x0A
#define USB_HID_REQUEST_CODE_SET_PROTOCOL 0x0B

#define STRING_DESCRIPTOR_ALLOCATION      100
#define STRING_DESCRIPTOR_LEN             65

#define USB_DESCRIPTOR_TYPE_HID			  0x21
#define USB_DESCRIPTOR_TYPE_REPORT		  0x22
#define USB_DESCRIPTOR_TYPE_PHYSICAL	  0x23

#define DESCRIPTOR_QUALIFIER              const

#define USB_PID_MASS_STORAGE              0x0fd5
#define USB_PID_KEYBOARD                  0x0fda

#if defined(MODULE_BOMS) && !defined(MODULE_HID)
#define USB_PID_BOMS_HID                  USB_PID_MASS_STORAGE
#elif defined(MODULE_HID) && !defined(MODULE_BOMS)
#define USB_PID_BOMS_HID                  USB_PID_KEYBOARD
#else
#define USB_PID_BOMS_HID                  2
#endif

#define USB_BCD_VERSION_HID_1_1           0x0110
#define USB_HID_LANG_NOT_SUPPORTED		  0

#define USB_CONTROL_EP_SIZE               USBD_EP_SIZE_64
#define USB_CONTROL_EP_MAX_PACKET_SIZE    64

#define KEY_STARTUP_TIME                  2000

#define HID_EP_DATA_IN                    USBD_EP_1
#define HID_EP_DATA_SIZE                  8

#define BOMS_EP_DATA_IN 				  USBD_EP_2
#define BOMS_EP_DATA_OUT 				  USBD_EP_3
#define BOMS_EP_DATA_SIZE                 512

#define SWAP_16(x) \
    ((((x) & 0xff00) >> 8) | \
     (((x) & 0x00ff) << 8))
#define SWAP_32(x) \
    ((((x) & 0xff000000) >> 24) | \
     (((x) & 0x00ff0000) >>  8) | \
     (((x) & 0x0000ff00) <<  8) | \
     (((x) & 0x000000ff) << 24))

#if BYTE_ORDER == LITTLE_ENDIAN
#define CPU2BOMS_16(A) (A)
#define BOMS2CPU_16(A) (A)
#define CPU2BOMS_32(A) (A)
#define BOMS2CPU_32(A) (A)
#define CPU2SCSI_16(A) SWAP_16(A)
#define SCSI2CPU_16(A) SWAP_16(A)
#define CPU2SCSI_32(A) SWAP_32(A)
#define SCSI2CPU_32(A) SWAP_32(A)
#else
#define CPU2BOMS_16(A) SWAP_16(A)
#define BOMS2CPU_16(A) SWAP_16(A)
#define CPU2BOMS_32(A) SWAP_32(A)
#define BOMS2CPU_32(A) SWAP_32(A)
#define CPU2SCSI_16(A) (A)
#define SCSI2CPU_16(A) (A)
#define CPU2SCSI_32(A) (A)
#define SCSI2CPU_32(A) (A)
#endif

#define BOMS_USBS BOMS2CPU_32(BOMS_SIG_USBS) // Little Endian converted to CPU
#define BOMS_USBC BOMS2CPU_32(BOMS_SIG_USBC) // Little Endian converted to CPU

typedef struct PACK hid_report_structure_t
{
	union {
		struct {
			unsigned char kbdLeftControl :1;
			unsigned char kbdLeftShift :1;
			unsigned char kbdLeftAlt :1;
			unsigned char kbdLeftGUI :1;
			unsigned char kbdRightControl :1;
			unsigned char kbdRightShift :1;
			unsigned char kbdRightAlt :1;
			unsigned char kbdRightGUI :1;
		};
		unsigned char arrayMap;
	};

	unsigned char arrayNotUsed;  // [1]
	unsigned char arrayKeyboard; // [2]
	unsigned char arrayResv1;
	unsigned char arrayResv2;
	unsigned char arrayResv3;
	unsigned char arrayResv4;
	unsigned char arrayResv5;
} hid_report_structure_t;

typedef struct key_mappings
{
	char msg;
	uint8_t scan;
	uint8_t shift;
	uint8_t ctrl;
	uint8_t alt;
} key_mappings_t;

typedef struct PACK config_descriptor_boms_hid_ {
	USB_configuration_descriptor configuration;
    /* don't know why, must put BOMS ahead of HID */
#if defined(MODULE_BOMS)
	USB_interface_descriptor interface_boms;
	USB_endpoint_descriptor endpoint_in_boms;
	USB_endpoint_descriptor endpoint_out_boms;
#endif

#if defined(MODULE_HID)
	USB_interface_descriptor interface_hid;
    USB_hid_descriptor descriptor_hid;
	USB_endpoint_descriptor endpoint_in_hid;
#endif

} config_descriptor_boms_hid_t;

FTVOID ft9xx_apps_sys (FTU32 para);
FTU8 ft900_xdemo_usbd(FTVOID);
FTU8 ft9xx_xdemo_hid(FTU8 *pdata);
FTU8 ft9xx_xdemo_writeSD(FTU8 *pdata);
FTU8 ft9xx_xdemo_readSD(FTU8 *pdata);
