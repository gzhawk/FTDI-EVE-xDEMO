
hid_report_structure_t report_buffer;
static uint8_t report_protocol = 0;
static uint8_t report_enable = 0;
static uint8_t report_idle = 0;
static uint8_t input_report = 0;
static volatile uint8_t scsi_sense = SCSI_SENSE_KEY_NO_SENSE;
static volatile uint16_t scsi_addn_sense = SCSI_SENSE_KEY_ADDN_NO_ERROR;
const TCHAR tstName[] = "SDTstFile";
boms_commandBlockWrapper_t BOMSCmd;
USBD_ctx usb_ctx_boms_hid;

DESCRIPTOR_QUALIFIER USB_device_descriptor device_descriptor_boms_hid =
{
    USB_DESCRIPTOR_DEVICE_LEN, /* bLength */
    USB_DESCRIPTOR_TYPE_DEVICE, /* bDescriptorType */
    USB_BCD_VERSION_2_0, /* bcdUSB */          // V2.0
    USB_CLASS_DEVICE, /* bDeviceClass */       // Defined in interface
    USB_SUBCLASS_DEVICE, /* bDeviceSubClass */ // Defined in interface
    USB_PROTOCOL_DEVICE, /* bDeviceProtocol */ // Defined in interface
    USB_CONTROL_EP_MAX_PACKET_SIZE, /* bMaxPacketSize0 */
    USB_VID_FTDI, /* idVendor */   // idVendor: 0x0403 (FTDI)
    USB_PID_BOMS_HID, /* idProduct */ 
    0x1234, /* bcdDevice */
    0x01, /* iManufacturer */      // Manufacturer
    0x02, /* iProduct */           // Product
    0x03, /* iSerialNumber */      // Serial Number
    0x01, /* windows only support 1 configuration */
};

DESCRIPTOR_QUALIFIER config_descriptor_boms_hid_t cfg_boms_hid = {
    {
        USB_DESCRIPTOR_CONFIG_LEN, /* configuration.bLength */
        USB_DESCRIPTOR_TYPE_CONFIGURATION, /* configuration.bDescriptorType */
        sizeof(config_descriptor_boms_hid_t), /* configuration.wTotalLength */
#if defined(MODULE_BOMS) && defined(MODULE_HID)
        0x02, /* configuration.bNumInterfaces */
#else
        0x01, /* configuration.bNumInterfaces */
#endif
        0x01, /* one configuration set to 1 */
        0, /* configuration.iConfiguration */
        USB_CONFIG_BMATTRIBUTES_RESERVED_SET_TO_1, /* configuration.bmAttributes */
        250,  /* 1==2mA, 250==500mA */
    },

#if defined(MODULE_BOMS)
    {
        USB_DESCRIPTOR_INTERFACE_LEN, /* interface.bLength */
        USB_DESCRIPTOR_TYPE_INTERFACE, /* interface.bDescriptorType */
        0, /* interface.bInterfaceNumber */
        0x00, /* interface.bAlternateSetting */
        0x02, /* interface.bNumEndpoints */
        USB_CLASS_MASS_STORAGE, /* interface.bInterfaceClass */ // HID Class
        USB_SUBCLASS_MASS_STORAGE_SCSI, /* interface.bInterfaceSubClass */
        USB_PROTOCOL_MASS_STORAGE_BOMS, /* interface.bInterfaceProtocol */
        0, /* interface.iInterface */
    },
    {
        USB_DESCRIPTOR_ENDPOINT_LEN, /* endpoint.bLength */
        USB_DESCRIPTOR_TYPE_ENDPOINT, /* endpoint.bDescriptorType */
        USB_ENDPOINT_DESCRIPTOR_EPADDR_IN | BOMS_EP_DATA_IN, /* endpoint.bEndpointAddress */
        USB_ENDPOINT_DESCRIPTOR_ATTR_BULK, /* endpoint.bmAttributes */
        BOMS_EP_DATA_SIZE, /* endpoint.wMaxPacketSize */
        0, /* ignored for bulk */
    },
    {
        USB_DESCRIPTOR_ENDPOINT_LEN, /* endpoint.bLength */
        USB_DESCRIPTOR_TYPE_ENDPOINT, /* endpoint.bDescriptorType */
        USB_ENDPOINT_DESCRIPTOR_EPADDR_OUT | BOMS_EP_DATA_OUT, /* endpoint.bEndpointAddress */
        USB_ENDPOINT_DESCRIPTOR_ATTR_BULK, /* endpoint.bmAttributes */
        BOMS_EP_DATA_SIZE, /* endpoint.wMaxPacketSize */
        0, /* ignored for bulk */
    },
#endif

#if defined(MODULE_HID)
    {
        // ---- INTERFACE DESCRIPTOR for Keyboard ----
        USB_DESCRIPTOR_INTERFACE_LEN, /* interface.bLength */
        USB_DESCRIPTOR_TYPE_INTERFACE, /* interface.bDescriptorType */
#if defined(MODULE_BOMS)
        1, /* interface.bInterfaceNumber */
#else
        0, /* interface.bInterfaceNumber */
#endif
        0x00, /* interface.bAlternateSetting */
        0x01, /* interface.bNumEndpoints */
        USB_CLASS_HID, /* interface.bInterfaceClass */ // HID Class
        USB_SUBCLASS_HID_BOOT_INTERFACE, /* interface.bInterfaceSubClass */ // Boot Protocol
        USB_PROTOCOL_HID_KEYBOARD, /* interface.bInterfaceProtocol */ // Keyboard
        0,/* interface.iInterface */
    },
    {
        // ---- HID DESCRIPTOR for Keyboard ----
        USB_DESCRIPTOR_HID_LEN, /* hid.bLength */
        USB_DESCRIPTOR_TYPE_HID, /* hid.bDescriptorType */
        USB_BCD_VERSION_HID_1_1, /* hid.bcdHID */
        USB_HID_LANG_NOT_SUPPORTED, /* hid.bCountryCode */
        0x01, /* hid.bNumDescriptors */
        USB_DESCRIPTOR_TYPE_REPORT, /* hid.bDescriptorType_0 */
        STRING_DESCRIPTOR_LEN, /* hid.wDescriptorLength_0 */
    },
    {
        // ---- ENDPOINT DESCRIPTOR for Keyboard ----
        USB_DESCRIPTOR_ENDPOINT_LEN, /* endpoint.bLength */
        USB_DESCRIPTOR_TYPE_ENDPOINT, /* endpoint.bDescriptorType */
        USB_ENDPOINT_DESCRIPTOR_EPADDR_IN | HID_EP_DATA_IN, /* endpoint.bEndpointAddress */
        USB_ENDPOINT_DESCRIPTOR_ATTR_INTERRUPT, /* endpoint.bmAttributes */
        HID_EP_DATA_SIZE, /* endpoint.wMaxPacketSize */
        0x0A, /* endpoint.bInterval */
    },
#endif


};

DESCRIPTOR_QUALIFIER uint8_t string_descriptor[STRING_DESCRIPTOR_ALLOCATION] =
{
		// String 0 is actually a list of language IDs supported by the real strings.
		4, USB_DESCRIPTOR_TYPE_STRING, 0x09, 0x04, // 0409 = English (US)

		// String 1 (Manufacturer): "FTDI"
		10, USB_DESCRIPTOR_TYPE_STRING, 'F', 0x00, 'T', 0x00, 'D', 0x00, 'I', 0,

		// String 2 (Product): "FT900 BOMS HID"
		30, USB_DESCRIPTOR_TYPE_STRING, 'F', 0x00, 'T', 0x00, '9', 0x00, '0',
		0x00, '0', 0x00, ' ', 0x00, 'B', 0, 'O', 0, 'M', 0, 'S', 0, ' ', 0, 'H', 0,
        'I', 0, 'D', 0,

		// String 3 (Serial Number): "FTBOMSHID123"
		28, USB_DESCRIPTOR_TYPE_STRING, 'F', 0, 'T', 0, 'B', 0, 'O', 0, 
        'M', 0, 'S', 0, ' ', 0, 'H', 0, 'I', 0, 'D', 0,'1', 0, '2', 0, '3', 0,

		// END OF STRINGS
		0
};

DESCRIPTOR_QUALIFIER uint8_t hid_report_descriptor_keyboard[STRING_DESCRIPTOR_LEN] =
{ 0x05, 0x01, 0x09, 0x06, 0xA1, 0x01, 0x05, 0x07, 0x19, 0xE0, 0x29, 0xE7, 0x15,
		0x00, 0x25, 0x01, 0x75, 0x01, 0x95, 0x08, 0x81, 0x02, 0x95, 0x01, 0x75,
		0x08, 0x81, 0x01, 0x95, 0x03, 0x75, 0x01, 0x05, 0x08, 0x19, 0x01, 0x29,
		0x03, 0x91, 0x02, 0x95, 0x05, 0x75, 0x01, 0x91, 0x01, 0x95, 0x06, 0x75,
		0x08, 0x15, 0x00, 0x26, 0xFF, 0x00, 0x05, 0x07, 0x19, 0x00, 0x2A, 0xFF,
		0x00, 0x81, 0x00, 0xC0, 
};

const key_mappings_t KMaps[] =
{
		{0x0d, 40, 0, 0, 0 }, // Enter key
		{0x7f, 42, 0, 0, 0 }, // <BS> Backspace CTRL+?
		{'a', 4, 0, 0, 0 }, {'A', 4, 1, 0, 0 },
		{'b', 5, 0, 0, 0 }, {'B', 5, 1, 0, 0 },
		{'c', 6, 0, 0, 0 }, {'C', 6, 1, 0, 0 },
		{'d', 7, 0, 0, 0 }, {'D', 7, 1, 0, 0 },
		{'e', 8, 0, 0, 0 }, {'E', 8, 1, 0, 0 },
		{'f', 9, 0, 0, 0 }, {'F', 9, 1, 0, 0 },
		{'g', 10, 0, 0, 0 }, {'G', 10, 1, 0, 0 },
		{'h', 11, 0, 0, 0 }, {'H', 11, 1, 0, 0 },
		{'i', 12, 0, 0, 0 }, {'I', 12, 1, 0, 0 },
		{'j', 13, 0, 0, 0 }, {'J', 13, 1, 0, 0 },
		{'k', 14, 0, 0, 0 }, {'K', 14, 1, 0, 0 },
		{'l', 15, 0, 0, 0 }, {'L', 15, 1, 0, 0 },
		{'m', 16, 0, 0, 0 }, {'M', 16, 1, 0, 0 },
		{'n', 17, 0, 0, 0 }, {'N', 17, 1, 0, 0 },
		{'o', 18, 0, 0, 0 }, {'O', 18, 1, 0, 0 },
		{'p', 19, 0, 0, 0 }, {'P', 19, 1, 0, 0 },
		{'q', 20, 0, 0, 0 }, {'Q', 20, 1, 0, 0 },
		{'r', 21, 0, 0, 0 }, {'R', 21, 1, 0, 0 },
		{'s', 22, 0, 0, 0 }, {'S', 22, 1, 0, 0 },
		{'t', 23, 0, 0, 0 }, {'T', 23, 1, 0, 0 },
		{'u', 24, 0, 0, 0 }, {'U', 24, 1, 0, 0 },
		{'v', 25, 0, 0, 0 }, {'V', 25, 1, 0, 0 },
		{'w', 26, 0, 0, 0 }, {'W', 26, 1, 0, 0 },
		{'x', 27, 0, 0, 0 }, {'X', 27, 1, 0, 0 },
		{'y', 28, 0, 0, 0 }, {'Y', 28, 1, 0, 0 },
		{'z', 29, 0, 0, 0 }, {'Z', 29, 1, 0, 0 },
		{'1', 30, 0, 0, 0 }, {'!', 30, 1, 0, 0 },
		{'2', 31, 0, 0, 0 }, {'@', 31, 1, 0, 0 },
		{'3', 32, 0, 0, 0 }, {'#', 32, 1, 0, 0 },
		{'4', 33, 0, 0, 0 }, {'$', 33, 1, 0, 0 },
		{'5', 34, 0, 0, 0 }, {'%', 34, 1, 0, 0 },
		{'6', 35, 0, 0, 0 }, {'^', 35, 1, 0, 0 },
		{'7', 36, 0, 0, 0 }, {'&', 36, 1, 0, 0 },
		{'8', 37, 0, 0, 0 }, {'*', 37, 1, 0, 0 },
		{'9', 38, 0, 0, 0 }, {'(', 38, 1, 0, 0 },
		{'0', 39, 0, 0, 0 }, {')', 39, 1, 0, 0 },
		{' ', 44, 0, 0, 0 },
		{'-', 45, 0, 0, 0 }, {'_', 45, 1, 0, 0 },
		{'=', 46, 0, 0, 0 }, {'+', 46, 1, 0, 0 },
		{'[', 47, 0, 0, 0 }, {'{', 47, 1, 0, 0 },
		{']', 48, 0, 0, 0 }, {'}', 48, 1, 0, 0 },
		{';', 51, 0, 0, 0 }, {':', 51, 1, 0, 0 },
        {'\'', 52, 0, 0, 0 },{'"', 52, 1, 0, 0 },
		{'`', 53, 0, 0, 0 }, {'~', 53, 1, 0, 0 },
		{',', 54, 0, 0, 0 }, {'<', 54, 1, 0, 0 },
		{'.', 55, 0, 0, 0 }, {'>', 55, 1, 0, 0 },
		{'/', 56, 0, 0, 0 }, {'?', 56, 1, 0, 0 },
		{'\\', 100, 0, 0, 0 }, {'|', 100, 1, 0, 0 },
		{ 0, 0, 0, 0 }, // end marker 
};

int8_t req_cb_boms_hid(USB_device_request *req)
{
	int8_t status = USBD_ERR_NOT_SUPPORTED;
    FTU8 manLUN = 0;
    
    switch (req->bRequest)
    {
#if defined(MODULE_HID)
        case USB_HID_REQUEST_CODE_SET_IDLE:
            report_enable = 1;
            report_idle = req->wValue >> 8;
            USBD_transfer_ep0(USBD_DIR_IN, NULL, 0, 0);
            status = USBD_OK;
            break;

        case USB_HID_REQUEST_CODE_GET_IDLE:
            USBD_transfer_ep0(USBD_DIR_IN, &report_idle, 1, 1);
            // ACK packet
            USBD_transfer_ep0(USBD_DIR_OUT, NULL, 0, 0);
            status = USBD_OK;
            break;

        case USB_HID_REQUEST_CODE_SET_PROTOCOL:
            USBD_transfer_ep0(USBD_DIR_IN, NULL, 0, 0);
            report_protocol = req->wValue & 0xff;
            // ACK packet
            USBD_transfer_ep0(USBD_DIR_OUT, NULL, 0, 0);
            status = USBD_OK;
            break;

        case USB_HID_REQUEST_CODE_GET_PROTOCOL:
            USBD_transfer_ep0(USBD_DIR_IN, &report_protocol, 1, 1);
            // ACK packet
            USBD_transfer_ep0(USBD_DIR_OUT, NULL, 0, 0);
            status = USBD_OK;
            break;

        case USB_HID_REQUEST_CODE_GET_REPORT:
            USBD_transfer_ep0(USBD_DIR_IN, (uint8_t *) &report_buffer,
                    sizeof(report_buffer), sizeof(report_buffer));
            // ACK packet
            USBD_transfer_ep0(USBD_DIR_OUT, NULL, 0, 0);
            status = USBD_OK;
            break;

        case USB_HID_REQUEST_CODE_SET_REPORT:
            // dummy read of one byte
            USBD_transfer_ep0(USBD_DIR_OUT, &input_report, 1, 1);
            // Update LED status on GPIO P0
            //P0 = input_report;
            // Acknowledge SETUP
            USBD_transfer_ep0(USBD_DIR_IN, NULL, 0, 0);
            status = USBD_OK;
            break;
#endif
#if defined(MODULE_BOMS)
        case BOMS_REQUEST_GET_MAX_LUN:
            USBD_transfer_ep0(USBD_DIR_IN, &manLUN,
                    sizeof(manLUN), req->wLength);
            // ACK
            USBD_transfer_ep0(USBD_DIR_IN, NULL, 0, 0);

            status = USBD_OK;
            break;
        case BOMS_REQUEST_RESET:
            USBD_set_state(USBD_STATE_DEFAULT);
            // ACK
            USBD_transfer_ep0(USBD_DIR_IN, NULL, 0, 0);
            status = USBD_OK;
            break;
#endif
        default:
		    return USBD_ERR_NOT_SUPPORTED;
    }

	return status;
}

int8_t desp_cb_boms_hid(USB_device_request *req, uint8_t **buffer, uint16_t *len)
{
	uint8_t *src = NULL;
	uint16_t length = req->wLength;
	uint8_t hValue = req->wValue >> 8;
	uint8_t lValue = req->wValue & 0x00ff;
	uint16_t i;
	uint8_t slen;

	switch (hValue)
	{
	case USB_DESCRIPTOR_TYPE_DEVICE:
		src = (uint8_t *) &device_descriptor_boms_hid;
		if (length > sizeof(device_descriptor_boms_hid)) { // too many bytes requested
			length = sizeof(device_descriptor_boms_hid); // Entire structure.
        }
		break;

	case USB_DESCRIPTOR_TYPE_CONFIGURATION:
		src = (uint8_t *) &cfg_boms_hid;
		if (length > sizeof(cfg_boms_hid)) { // too many bytes requested
			length = sizeof(cfg_boms_hid); // Entire structure.
        }
		break;

	case USB_DESCRIPTOR_TYPE_STRING:
		i = 0;
		while ((slen = string_descriptor[i]) > 0)
		{
			src = (uint8_t *)&string_descriptor[i];
			if (lValue == 0)
			{
				break;
			}
			i += slen;
			lValue--;
		}
		if (lValue > 0)
		{
			// String not found
			return USBD_ERR_NOT_SUPPORTED;
		}
		if (length > slen)
		{
			length = slen;
		}
		break;
#if defined(MODULE_HID)
	case USB_DESCRIPTOR_TYPE_REPORT:
		src = (uint8_t *) &hid_report_descriptor_keyboard;
		if (length > sizeof(hid_report_descriptor_keyboard)) {
			length = sizeof(hid_report_descriptor_keyboard); // Entire structure.
		}
		break;
#endif
	default:
		return USBD_ERR_NOT_SUPPORTED;
	}

	*buffer = src;
	*len = length;

	return USBD_OK;
}

int8_t vendor_req_cb(USB_device_request *req)
{
	return USBD_ERR_NOT_SUPPORTED;
}

void suspend_cb(uint8_t status)
{
	(void) status;
	return;
}

void resume_cb(uint8_t status)
{
	(void) status;
	return;
}

void reset_cb(uint8_t status)
{
	(void) status;

	USBD_set_state(USBD_STATE_DEFAULT);

	return;
}

void ep_cb(USBD_ENDPOINT_NUMBER ep_number)
{
    return;
}

void scsi_sense_not_ready(void)
{
	// Report failed due to medium not present to REQUEST SENSE.
	scsi_sense = SCSI_SENSE_KEY_NOT_READY;
	scsi_addn_sense = SCSI_SENSE_KEY_ADDN_NOT_READY_MEDIUM_NOT_PRESENT;
}

void boms_populate_status(uint8_t status,
		boms_commandBlockWrapper_t *BOMSCmd,
		boms_commandStatusWrapper_t *BOMSStatus)
{
	memset((void *)BOMSStatus, 0, sizeof(boms_commandStatusWrapper_t));
	BOMSStatus->dCSWTag = BOMSCmd->dCBWTag;
	BOMSStatus->dCSWSignature = BOMS_USBS;
	BOMSStatus->bCSWStatus = status;
	if (status != BOMS_STATUS_OK)
	{
		BOMSStatus->dCSWDataResidue = BOMSCmd->dCBWDataTransferLength;
	}
}

uint8_t boms_send_status(uint8_t status,
		boms_commandBlockWrapper_t *BOMSCmd)
{
	boms_commandStatusWrapper_t BOMSStatus;

	boms_populate_status(status, BOMSCmd, &BOMSStatus);

	while (USBD_ep_buffer_full(BOMS_EP_DATA_IN));
	USBD_transfer(BOMS_EP_DATA_IN, (uint8_t *)&BOMSStatus,
			sizeof(boms_commandStatusWrapper_t));

	return status;
}

int32_t scsi_test_unit_ready(boms_commandBlockWrapper_t *BOMSCmd)
{
	uint8_t boms_status;

	if (FT9xxFatFs.fs_type)
	{
		boms_status = BOMS_STATUS_OK;
	}
	else
	{
		boms_status = BOMS_STATUS_COMMAND_FAILED;
		scsi_sense_not_ready();
	}

	return boms_send_status(boms_status, BOMSCmd);
}

void scsi_sense_usb_fail(void)
{
	// Report failed due to a USB error to REQUEST SENSE.
	scsi_sense = SCSI_SENSE_KEY_ABORTED_COMMAND;
	scsi_addn_sense = SCSI_SENSE_KEY_ADDN_ABORTED_COMMAND_NO_ADDITIONAL_CODE;
}

void scsi_sense_sdhost_fail(int dir)
{
	// Report failed due to an SD card error to REQUEST SENSE.
	scsi_sense = SCSI_SENSE_KEY_MEDIUM_ERROR;
	if (dir == SDHOST_READ)
	{
		scsi_addn_sense = SCSI_SENSE_KEY_ADDN_MEDIUM_ERROR_UNRECOVERED_READ_ERROR;
	}
	else
	{
		scsi_addn_sense = SCSI_SENSE_KEY_ADDN_MEDIUM_ERROR_WRITE_FAULT;
	}
}

void scsi_sense_lba_fail(void)
{
	// Report failed due to an invalid LBA error to REQUEST SENSE.
	scsi_sense = SCSI_SENSE_KEY_ILLEGAL_REQUEST;
	scsi_addn_sense = SCSI_SENSE_KEY_ADDN_ILLEGAL_REQUEST_LBA_OUT_OF_RANGE;
}

void scsi_sense_lun_fail(void)
{
	// Report failed due to an invalid LUN error to REQUEST SENSE.
	scsi_sense = SCSI_SENSE_KEY_ILLEGAL_REQUEST;
	scsi_addn_sense = SCSI_SENSE_KEY_ADDN_ILLEGAL_REQUEST_INVALID_LUN;
}

void scsi_sense_illegal_request_command(void)
{
	// Report that the command is not supported to REQUEST SENSE.
	scsi_sense = SCSI_SENSE_KEY_ILLEGAL_REQUEST;
	scsi_addn_sense = SCSI_SENSE_KEY_ADDN_ILLEGAL_REQUEST_INVALID_COMMAND;
}

void scsi_sense_illegal_request_cdb(void)
{
	// Report that the command was incorrectly formatted to REQUEST SENSE.
	scsi_sense = SCSI_SENSE_KEY_ILLEGAL_REQUEST;
	scsi_addn_sense = SCSI_SENSE_KEY_ADDN_ILLEGAL_REQUEST_INVALID_FIELD_IN_CDB;
}

void scsi_sense_illegal_param_value(void)
{
	// Report that the command was incorrectly formatted to REQUEST SENSE.
	scsi_sense = SCSI_SENSE_KEY_ILLEGAL_REQUEST;
	scsi_addn_sense = SCSI_SENSE_KEY_ADDN_ILLEGAL_REQUEST_INVALID_PARM_VALUE;
}

void scsi_sense_illegal_request_parm(void)
{
	// Report that the command was incorrectly formatted to REQUEST SENSE.
	scsi_sense = SCSI_SENSE_KEY_ILLEGAL_REQUEST;
	scsi_addn_sense = SCSI_SENSE_KEY_ADDN_ILLEGAL_REQUEST_INVALID_FIELDS_IN_PARM;
}

void scsi_sense_ok(void)
{
	// Report no error to REQUEST SENSE.
	scsi_sense = SCSI_SENSE_KEY_NO_SENSE;
	scsi_addn_sense = SCSI_SENSE_KEY_ADDN_NO_ERROR;
}

int32_t scsi_inquiry(boms_commandBlockWrapper_t *BOMSCmd)
{
	int32_t status;
	uint8_t boms_status;

	boms_scsi_CRB_inquiry_t inq;

	memset((void *)&inq, 0, sizeof(boms_scsi_CRB_inquiry_t));

	// byte 0
    inq.peripheral_dev_type = 0;
    inq.peripheral_qual = 0;
    // byte 1
    inq.device_type_modifier = 0;
    inq.rmb = 1;
    // byte 2
    inq.ansi = 2;
    inq.ecma = 0;
    inq.iso = 0;
    // byte 3
    inq.resp_data_format = 2;
    inq.trm_iop = 0;
    inq.aen = 0;
    // byte 4
    inq.adnl_len = 0x1f;
    // byte 7
    inq.sft_re = 0;
    inq.cmd_que = 0;
    inq.linked = 0;
    inq.sync = 0;
    inq.wbus16 = 0;
    inq.wbus32 = 0;
    inq.rel_adr = 0;
    // bytes 8-15
    memcpy(&inq.vid, "FTDI    ", 8);
    // bytes 16-31
    memcpy(&inq.pid, "USB Dev 2 SDCard", 16);
    // bytes 32-35
    memcpy(&inq.rev, "1.0 ", 4);

    status = USBD_transfer(BOMS_EP_DATA_IN, (uint8_t *)&inq, BOMS_SCSI_INQUIRY_TRANSFER_LEN);

    if (status == BOMS_SCSI_INQUIRY_TRANSFER_LEN)
	{
		boms_status = BOMS_STATUS_OK;
	}
	else
	{
		boms_status = BOMS_STATUS_COMMAND_FAILED;
		scsi_sense_usb_fail();
	}

	return boms_send_status(boms_status, BOMSCmd);
}

int32_t scsi_request_sense(boms_commandBlockWrapper_t *BOMSCmd)
{
	int32_t status;
	uint8_t boms_status;

	boms_scsi_CRB_request_sense_t sense;

	memset((void *)&sense, 0, sizeof(boms_scsi_CRB_request_sense_t));

	// byte 0
	sense.rsp_code = 0x70;
	sense.valid = 0;
	// byte 1
	sense.segment = 0x00;
	// byte 2
	sense.sense_key = scsi_sense;
	sense.ili = 0;
	sense.eom = 0;
	sense.filemark = 0;
	// bytes 3-6
	sense.information = 0;
	// byte 7
	sense.adnl_len = 0x0a;
	// bytes 8-11
	sense.cmd_info = 0;
	// byte 12
	sense.adnl_sense_code = scsi_addn_sense >> 8;
	// byte 13
	sense.adnl_sense_code_qualifier = scsi_addn_sense & 0xff;
	// byte 14
	sense.fru = 0x00;
	// byte 15
	sense.sksv1 = 0;
	sense.sksv = 0;
	// byte 16
	sense.sksv2 = 0;
	// byte 17
	sense.sksv3 = 0;

	// Reset the sense key.
	scsi_sense_ok();

	status = USBD_transfer(BOMS_EP_DATA_IN, (uint8_t *) &sense, sizeof(boms_scsi_CRB_request_sense_t));
	if (status == sizeof(boms_scsi_CRB_request_sense_t))
	{
		boms_status = BOMS_STATUS_OK;
	}
	else
	{
		boms_status = BOMS_STATUS_COMMAND_FAILED;
		scsi_sense_usb_fail();
	}

	return boms_send_status(boms_status, BOMSCmd);
}

int32_t scsi_mode_sense(boms_commandBlockWrapper_t *BOMSCmd)
{
	int32_t status;
	uint8_t boms_status;

	// This is how we return the pages in the mode sense response.
	struct  {
		boms_scsi_CRB_mode_sense_t sense;
		boms_scsi_CRB_mode_sense_page_read_write_error_recovery_t recovery;
		boms_scsi_CRB_mode_sense_page_flexible_disk_t flexible;
		boms_scsi_CRB_mode_sense_page_removable_block_access_capabilities_t capab;
		boms_scsi_CRB_mode_sense_page_fault_failure_reporting_control_t reporting;
	} response;

	memset((void *)&response, 0, sizeof(response));

	response.sense.mode_data_length = sizeof(response) - 1;
	response.sense.medium_type_code = 0;
	response.sense.device_specific_parameters = 0;
	response.sense.block_descriptor_length = 0;

	response.recovery.page_code = 0x01;
	response.recovery.page_length = 0x0a;
	response.recovery.read_retry_count = 3;
	response.recovery.write_retry_count = 128;

	response.flexible.page_code = 0x05;
	response.flexible.page_length = 0x1e;
	response.flexible.transfer_rate = CPU2SCSI_16(5000);
	response.flexible.sectors_per_track = 16;
	response.flexible.bytes_per_sector = CPU2SCSI_16(16128);
	response.flexible.number_of_cylinders = CPU2SCSI_16(15);
	response.flexible.motor_on_delay = 5;
	response.flexible.motor_off_delay = 30;
	response.flexible.rotation_rate = CPU2SCSI_16(360);

	response.capab.page_code = 0x1b;
	response.capab.page_length = 0x0a;
	response.capab.total_luns = 1;

	response.reporting.page_code = 0x1c;
	response.reporting.page_length = 0x06;
	response.reporting.mrie = 0x05;
	response.reporting.interval_timer = CPU2SCSI_32(28);

	status = USBD_transfer(BOMS_EP_DATA_IN, (uint8_t *) &response, sizeof(response));
	if (status == sizeof(response))
	{
		boms_status = BOMS_STATUS_OK;
	}
	else
	{
		boms_status = BOMS_STATUS_COMMAND_FAILED;
		scsi_sense_usb_fail();
	}

	return boms_send_status(boms_status, BOMSCmd);
}

int32_t scsi_prevent_allow_removal(boms_commandBlockWrapper_t *BOMSCmd)
{
	if (BOMSCmd->CBWCB.removal.prevent)
	{
		scsi_sense_illegal_request_cdb();
		// Not supported.
		return boms_send_status(BOMS_STATUS_COMMAND_FAILED, BOMSCmd);
	}

	return boms_send_status(BOMS_STATUS_OK, BOMSCmd);
}

int32_t scsi_read_capacity(boms_commandBlockWrapper_t *BOMSCmd)
{
	size_t status;
	uint8_t boms_status = BOMS_STATUS_OK;
	boms_scsi_CRB_read_capacity_16_t cap_16;
	boms_scsi_CRB_read_capacity_t cap_10;
	size_t len;

	uint8_t *cap;

	if (!FT9xxFatFs.fs_type)
	{
		boms_status = BOMS_STATUS_COMMAND_FAILED;
		scsi_sense_not_ready();
		USBD_stall_endpoint(BOMS_EP_DATA_IN);
	}

	if (boms_status == BOMS_STATUS_OK)
	{
		if (BOMSCmd->CBWCB.read_capacity.op_code == 0x9e)
		{
			cap = (uint8_t *)&cap_16;
			len = sizeof(cap_16);
			memset(cap, 0, sizeof(cap_16));

			cap_16.block_len_l = CPU2SCSI_32(0x200);
			cap_16.lba_l = CPU2SCSI_32(sdhost_get_capacity());
		}
		else
		{
			cap = (uint8_t *)&cap_10;
			len = sizeof(cap_10);

			cap_10.block_len = CPU2SCSI_32(0x200);
			cap_10.lba = CPU2SCSI_32(sdhost_get_capacity());
		}

		status = USBD_transfer(BOMS_EP_DATA_IN, (uint8_t *)cap, len);
		if (status == len)
		{
			boms_status = BOMS_STATUS_OK;
		}
		else
		{
			boms_status = BOMS_STATUS_COMMAND_FAILED;
			scsi_sense_usb_fail();
		}
	}

	return boms_send_status(boms_status, BOMSCmd);
}

int32_t scsi_read_format_capacity(boms_commandBlockWrapper_t *BOMSCmd)
{
	size_t status;
	uint8_t boms_status = BOMS_STATUS_OK;
	boms_scsi_CRB_read_format_capacity_t fcap;
	size_t len;
	uint32_t blocks;
	uint32_t block_size;
	uint32_t erase_block_count;

	if (!FT9xxFatFs.fs_type)
	{
		boms_status = BOMS_STATUS_COMMAND_FAILED;
		scsi_sense_not_ready();
		USBD_stall_endpoint(BOMS_EP_DATA_IN);
	}

	if (boms_status == BOMS_STATUS_OK)
	{
		memset(&fcap, 0, sizeof(fcap));

		// Maximum length of return data.
		len = SCSI2CPU_16(BOMSCmd->CBWCB.read_fcapacity.alloc_length);
		if (len > sizeof(boms_scsi_CRB_read_format_capacity_t))
		{
			len = sizeof(boms_scsi_CRB_read_format_capacity_t);
		}

		// Capacity is highest addressable sector.
		blocks = sdhost_get_capacity() + 1;
		block_size = sdhost_get_block_size();
		erase_block_count = sdhost_get_erase_block_count();

		if ((block_size) && (erase_block_count))
		{
			blocks = blocks / erase_block_count;

			fcap.cap_list = CPU2SCSI_32(8);
			fcap.blocks = CPU2SCSI_32(blocks);

#if BYTE_ORDER == LITTLE_ENDIAN
			// Shift to compensate for 24 bits from 32 being used.
			block_size = (erase_block_count * block_size) << 8;
#else
			block_size = (erase_block_count * block_size);
#endif

			fcap.block_len = CPU2SCSI_32(block_size);
			fcap.code = 2;

			status = USBD_transfer(BOMS_EP_DATA_IN, (uint8_t *)&fcap, len);
			if (status == len)
			{
				boms_status = BOMS_STATUS_OK;
			}
			else
			{
				boms_status = BOMS_STATUS_COMMAND_FAILED;
				scsi_sense_usb_fail();
			}
		}
		else
		{
			boms_status = BOMS_STATUS_COMMAND_FAILED;
			scsi_sense_not_ready();
			USBD_stall_endpoint(BOMS_EP_DATA_IN);
		}
	}

	return boms_send_status(boms_status, BOMSCmd);
}

int32_t scsi_read(boms_commandBlockWrapper_t *BOMSCmd)
{
	uint8_t bufBOMSData[BOMS_EP_DATA_SIZE];
	int32_t status = BOMS_EP_DATA_SIZE;
	uint8_t boms_status = BOMS_STATUS_OK;
	uint32_t sector;
	int count;
	SDHOST_STATUS sdret = SDHOST_OK;

	if (!FT9xxFatFs.fs_type)
	{
		boms_status = BOMS_STATUS_COMMAND_FAILED;
		scsi_sense_not_ready();
		USBD_stall_endpoint(BOMS_EP_DATA_IN);
	}

	if (boms_status == BOMS_STATUS_OK)
	{
		sector = SCSI2CPU_32(BOMSCmd->CBWCB.read.lba);
		count = SCSI2CPU_16(BOMSCmd->CBWCB.read.length);

		if (count == 0)
		{
			boms_status = BOMS_STATUS_COMMAND_FAILED;
			scsi_sense_illegal_param_value();
			USBD_stall_endpoint(BOMS_EP_DATA_IN);
		}
		// Check range of LBA - capacity is highest addressable sector.
		else if ((count + sector) > (sdhost_get_capacity() + 1))
		{
			boms_status = BOMS_STATUS_COMMAND_FAILED;
			scsi_sense_lba_fail();
			USBD_stall_endpoint(BOMS_EP_DATA_IN);
		}
		// Check supported LUN
		else if (BOMSCmd->CBWCB.read.lun != 0)
		{
			boms_status = BOMS_STATUS_COMMAND_FAILED;
			scsi_sense_lun_fail();
			USBD_stall_endpoint(BOMS_EP_DATA_IN);
		}
		else
		{
			while (count--)
			{
				if (sdret == SDHOST_OK)
				{
					sdret = sdhost_transfer_data(SDHOST_READ, (void*)bufBOMSData, BOMS_EP_DATA_SIZE, sector++);
				}

				status = USBD_transfer_ex(BOMS_EP_DATA_IN, (uint8_t *)bufBOMSData,
							BOMS_EP_DATA_SIZE, 1, 0);
			}

			if (status != BOMS_EP_DATA_SIZE)
			{
				boms_status = BOMS_STATUS_COMMAND_FAILED;
				scsi_sense_usb_fail();
			}
			else if (sdret != SDHOST_OK)
			{
				boms_status = BOMS_STATUS_COMMAND_FAILED;
				scsi_sense_sdhost_fail(SDHOST_READ);
			}
			else
			{
				boms_status = BOMS_STATUS_OK;
			}
		}
	}

	return boms_send_status(boms_status, BOMSCmd);
}

int32_t scsi_write(boms_commandBlockWrapper_t *BOMSCmd)
{
	uint8_t bufBOMSData[BOMS_EP_DATA_SIZE];
	int32_t status = BOMS_EP_DATA_SIZE;
	uint8_t boms_status = BOMS_STATUS_OK;
	uint32_t sector;
	int count;
	SDHOST_STATUS sdret = SDHOST_OK;

	if (!FT9xxFatFs.fs_type)
	{
		boms_status = BOMS_STATUS_COMMAND_FAILED;
		scsi_sense_not_ready();
		USBD_stall_endpoint(BOMS_EP_DATA_OUT);
	}

	if (boms_status == BOMS_STATUS_OK)
	{
		sector = SCSI2CPU_32(BOMSCmd->CBWCB.write.lba);
		count = SCSI2CPU_16(BOMSCmd->CBWCB.write.length);

		if (count == 0)
		{
			boms_status = BOMS_STATUS_COMMAND_FAILED;
			scsi_sense_illegal_param_value();
			USBD_stall_endpoint(BOMS_EP_DATA_IN);
		}
		// Check range of LBA - capacity is highest addressable sector.
		else if ((count + sector) > (sdhost_get_capacity() + 1))
		{
			boms_status = BOMS_STATUS_COMMAND_FAILED;
			scsi_sense_lba_fail();
			USBD_stall_endpoint(BOMS_EP_DATA_OUT);
		}
		// Check supported LUN
		else if (BOMSCmd->CBWCB.write.lun != 0)
		{
			boms_status = BOMS_STATUS_COMMAND_FAILED;
			scsi_sense_lun_fail();
			USBD_stall_endpoint(BOMS_EP_DATA_OUT);
		}
		else
		{
			while (count--)
			{
				status = USBD_transfer(BOMS_EP_DATA_OUT, (uint8_t *)bufBOMSData,
						BOMS_EP_DATA_SIZE);//, 1, 0);

				if (sdret == SDHOST_OK)
				{
					if (status == BOMS_EP_DATA_SIZE)
					{
						sdret = sdhost_transfer_data(SDHOST_WRITE, (void*)bufBOMSData, BOMS_EP_DATA_SIZE, sector++);
					}
				}
			}

			if (status != BOMS_EP_DATA_SIZE)
			{
				boms_status = BOMS_STATUS_COMMAND_FAILED;
				scsi_sense_usb_fail();
			}
			else if (sdret != SDHOST_OK)
			{
				boms_status = BOMS_STATUS_COMMAND_FAILED;
				scsi_sense_sdhost_fail(SDHOST_WRITE);
			}
			else
			{
				boms_status = BOMS_STATUS_OK;
			}
		}

	}

	return boms_send_status(boms_status, BOMSCmd);
}

void scsi_proc(void)
{
    if (USBD_ep_buffer_full(BOMS_EP_DATA_OUT)) {
        if (sizeof(BOMSCmd) == USBD_transfer(BOMS_EP_DATA_OUT, (uint8_t *)&BOMSCmd, sizeof(BOMSCmd))) {
            if (BOMSCmd.dCBWSignature == BOMS_USBC) {
                if (BOMSCmd.CBWCB.unit_ready.op_code == BOMS_SCSI_TEST_UNIT_READY) // SCSI Test Unit Ready
                {
                    scsi_test_unit_ready(&BOMSCmd);
                } 
                else if (BOMSCmd.CBWCB.request_sense.op_code == BOMS_SCSI_READ) // SCSI Read
                {
                    scsi_read(&BOMSCmd);
                }
                else if (BOMSCmd.CBWCB.request_sense.op_code == BOMS_SCSI_WRITE) // SCSI Write
                {
                    scsi_write(&BOMSCmd);
                }
                else if (BOMSCmd.CBWCB.inquiry.op_code == BOMS_SCSI_INQUIRY) // SCSI Inquiry
                {
                    scsi_inquiry(&BOMSCmd);
                }
                else if (BOMSCmd.CBWCB.request_sense.op_code == BOMS_SCSI_REQUEST_SENSE) // SCSI Request Sense
                {
                    scsi_request_sense(&BOMSCmd);
                }
                else if (BOMSCmd.CBWCB.read_capacity.op_code == BOMS_SCSI_READ_CAPACITY) // SCSI Read Capacity (10)
                {
                    scsi_read_capacity(&BOMSCmd);
                }
                else if (BOMSCmd.CBWCB.read_capacity.op_code == BOMS_SCSI_READ_FORMAT_CAPACITY) // SCSI Read Format Capacity (10)
                {
                    scsi_read_format_capacity(&BOMSCmd);
                }
                else if (BOMSCmd.CBWCB.mode_sense.op_code == BOMS_SCSI_MODE_SENSE) // SCSI Mode Sense
                {
                    scsi_mode_sense(&BOMSCmd);
                }
                else if (BOMSCmd.CBWCB.request_sense.op_code == BOMS_SCSI_READ_CAPACITY_16) // SCSI Read Capacity (16)
                {
                    scsi_read_capacity(&BOMSCmd);
                }
                else if (BOMSCmd.CBWCB.request_sense.op_code == BOMS_SCSI_PREVENT_ALLOW_REMOVAL) // SCSI Prevent Allow Removal
                {
                    scsi_prevent_allow_removal(&BOMSCmd);
                }
                else
                {
                    // Send IN a zero length packet.
                    if (BOMSCmd.dCBWDataTransferLength)
                    {
                        USBD_stall_endpoint(BOMS_EP_DATA_IN);
                    }
                    boms_send_status(BOMS_STATUS_COMMAND_FAILED, &BOMSCmd);
                    scsi_sense_illegal_request_command();
                }
            }
            else
            {
                // Was not a USBC signature.
                boms_send_status(BOMS_STATUS_PHASE_ERROR, &BOMSCmd);
                scsi_sense_illegal_request_cdb();
            }
        }else{
            // Wrong command size.
            boms_send_status(BOMS_STATUS_COMMAND_FAILED, &BOMSCmd);
            scsi_sense_illegal_request_parm();
        }
    }
}


FTU8 ft900_xdemo_usbd_init(FTVOID)
{
    memset(&usb_ctx_boms_hid, 0, sizeof(usb_ctx_boms_hid));

    usb_ctx_boms_hid.standard_req_cb = NULL;
    usb_ctx_boms_hid.get_descriptor_cb = desp_cb_boms_hid;
    usb_ctx_boms_hid.class_req_cb = req_cb_boms_hid;
    usb_ctx_boms_hid.vendor_req_cb = vendor_req_cb;
    usb_ctx_boms_hid.suspend_cb = suspend_cb;
    usb_ctx_boms_hid.resume_cb = resume_cb;
    usb_ctx_boms_hid.reset_cb = reset_cb;
    usb_ctx_boms_hid.lpm_cb = NULL;
    usb_ctx_boms_hid.speed = USBD_SPEED_HIGH;
    usb_ctx_boms_hid.ep0_size = USB_CONTROL_EP_SIZE;

    USBD_initialise(&usb_ctx_boms_hid);

    while (USBD_connect() != USBD_OK);

#if defined(MODULE_HID)
    USBD_create_endpoint(HID_EP_DATA_IN, USBD_EP_INT,
            USBD_DIR_IN, USBD_EP_SIZE_8, 
            USBD_DB_OFF, NULL);

    memset(&report_buffer, 0, sizeof(report_buffer));

    FTPRINT("\n HID inited.");
#endif
#if defined(MODULE_BOMS)
    USBD_create_endpoint(BOMS_EP_DATA_IN, USBD_EP_BULK,
            USBD_DIR_IN,USBD_EP_SIZE_512, 
            USBD_DB_ON, NULL);
    USBD_create_endpoint(BOMS_EP_DATA_OUT, USBD_EP_BULK, 
            USBD_DIR_OUT,USBD_EP_SIZE_512, 
            USBD_DB_ON, NULL);

    memset((void *)&BOMSCmd, 0, sizeof(BOMSCmd));    

    scsi_sense_not_ready(); 

    FTPRINT("\n BOMS inited.");
#endif

    return 1;
}

FTU8 ft900_xdemo_usbd(FTVOID)
{
    static FTU8 inited = 0;

    if (!inited) {
        inited = ft900_xdemo_usbd_init();
    }

#if defined(MODULE_BOMS)
    scsi_proc();
#endif

    return (USBD_OK == USBD_process())?1:0;
}

FTU8 ft9xx_xdemo_hid (FTU8 *pdata)
{
    FTU8 i;
    static FTU8 pressed = 0;

    if (USBD_get_state() != USBD_STATE_CONFIGURED) {
        return DATA4MCU;
    }

    if (!report_enable) {
        return DATA4MCU;
    }

    if (!pressed && *pdata) {
        for (i = 0; KMaps[i].msg; i++) {
            if (*pdata == KMaps[i].msg) {
                report_buffer.arrayKeyboard = KMaps[i].scan;
                report_buffer.kbdRightShift = KMaps[i].shift;
                pressed = *pdata;
                break;
            }
        }
    }

    if (0 == KMaps[i].msg || !pressed) {
        return DATA4MCU;
    } else {
        USBD_transfer(HID_EP_DATA_IN, (uint8_t *) &report_buffer, HID_EP_DATA_SIZE);
        while(!USBD_ep_buffer_full(HID_EP_DATA_IN));
        if (*pdata == 0) {
            /* terminal end sent, should not return */
            pressed = 0;
            return DATA4UI;
        } else {
            /* make it coming back again to send terminal end */
            *pdata = 0;
            memset(&report_buffer, 0, sizeof(report_buffer));
            return DATA4MCU;
        }
    }
}

FTVOID ft9xx_apps_sys (FTU32 para)
{
    data_pool_st *p = (data_pool_st *)para;

    if (!ft900_xdemo_usbd()){
        return;
    }

    if (!p || (DATA4MCU != p->direction)) {
        return;
    } 

    switch (p->type) {
        case UI_KB2PC:
            p->direction = ft9xx_xdemo_hid(p->data);
            break;
        default:
            break;
    }
    return;
}
