#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <string.h>
#include <inttypes.h>

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned long u32;

#define USB_EP_SIZE 64
#define USB_ENDPOINTS 7 // AtMegaxxU4
#define ISERIAL_MAX_LEN     20
#define CDC_INTERFACE_COUNT	2
#define CDC_ENPOINT_COUNT	3
#define CDC_ACM_INTERFACE	0	// CDC ACM
#define CDC_DATA_INTERFACE	1	// CDC Data
#define CDC_FIRST_ENDPOINT	1
#define CDC_ENDPOINT_ACM	(CDC_FIRST_ENDPOINT)							// CDC First
#define CDC_ENDPOINT_OUT	(CDC_FIRST_ENDPOINT+1)
#define CDC_ENDPOINT_IN		(CDC_FIRST_ENDPOINT+2)
#define INTERFACE_COUNT		(MSC_INTERFACE + MSC_INTERFACE_COUNT)
#define CDC_RX CDC_ENDPOINT_OUT
#define CDC_TX CDC_ENDPOINT_IN
#define IMANUFACTURER   1
#define IPRODUCT        2
#define ISERIAL         3
#define USB_CONFIG_BUS_POWERED                 0x80
#define USB_CONFIG_SELF_POWERED                0xC0
#define USB_CONFIG_REMOTE_WAKEUP               0x20
// bMaxPower in Configuration Descriptor
#define USB_CONFIG_POWER_MA(mA)                ((mA)/2)
#ifndef USB_CONFIG_POWER
 #define USB_CONFIG_POWER                      (500)
#endif
//	Standard requests
#define GET_STATUS			0
#define CLEAR_FEATURE		1
#define SET_FEATURE			3
#define SET_ADDRESS			5
#define GET_DESCRIPTOR		6
#define SET_DESCRIPTOR		7
#define GET_CONFIGURATION	8
#define SET_CONFIGURATION	9
#define GET_INTERFACE		10
#define SET_INTERFACE		11
// bmRequestType
#define REQUEST_HOSTTODEVICE	0x00
#define REQUEST_DEVICETOHOST	0x80
#define REQUEST_DIRECTION		0x80

#define REQUEST_STANDARD		0x00
#define REQUEST_CLASS			0x20
#define REQUEST_VENDOR			0x40
#define REQUEST_TYPE			0x60

#define REQUEST_DEVICE			0x00
#define REQUEST_INTERFACE		0x01
#define REQUEST_ENDPOINT		0x02
#define REQUEST_OTHER			0x03
#define REQUEST_RECIPIENT		0x03

#define USB_DEVICE_CLASS_COMMUNICATIONS        0x02
#define USB_DEVICE_CLASS_HUMAN_INTERFACE       0x03
#define USB_DEVICE_CLASS_STORAGE               0x08
#define USB_DEVICE_CLASS_VENDOR_SPECIFIC       0xFF

// HID subclass HID1.11 Page 8 4.2 Subclass
#define HID_SUBCLASS_NONE 0
#define HID_SUBCLASS_BOOT_INTERFACE 1

// HID Keyboard/Mouse bios compatible protocols HID1.11 Page 9 4.3 Protocols
#define HID_PROTOCOL_NONE 0
#define HID_PROTOCOL_KEYBOARD 1
#define HID_PROTOCOL_MOUSE 2

// usb_20.pdf Table 9.6 Standard Feature Selectors
#define DEVICE_REMOTE_WAKEUP                   1
#define ENDPOINT_HALT                          2
#define TEST_MODE                              3

#define USB_DEVICE_DESCRIPTOR_TYPE             1
#define USB_CONFIGURATION_DESCRIPTOR_TYPE      2
#define USB_STRING_DESCRIPTOR_TYPE             3
#define USB_INTERFACE_DESCRIPTOR_TYPE          4
#define USB_ENDPOINT_DESCRIPTOR_TYPE           5

// usb_20.pdf Figure 9-4. Information Returned by a GetStatus() Request to a Device
#define FEATURE_SELFPOWERED_ENABLED     (1 << 0)
#define FEATURE_REMOTE_WAKEUP_ENABLED   (1 << 1)

#define TRANSFER_PGM		0x80
#define TRANSFER_RELEASE	0x40
#define TRANSFER_ZERO		0x20

#define EP_TYPE_CONTROL				(0x00)
#define EP_TYPE_BULK_IN				((1<<EPTYPE1) | (1<<EPDIR))
#define EP_TYPE_BULK_OUT			(1<<EPTYPE1)
#define EP_TYPE_INTERRUPT_IN		((1<<EPTYPE1) | (1<<EPTYPE0) | (1<<EPDIR))
#define EP_TYPE_INTERRUPT_OUT		((1<<EPTYPE1) | (1<<EPTYPE0))
#define EP_TYPE_ISOCHRONOUS_IN		((1<<EPTYPE0) | (1<<EPDIR))
#define EP_TYPE_ISOCHRONOUS_OUT		(1<<EPTYPE0)
#define USB_VERSION 0x200
#define USB_VID 0x2341
#define USB_MANUFACTURER "Arduino LLC"
#define USB_PRODUCT     "USB IO Board"
#define USB_PID 0x0001

#define D_DEVICE(_class,_subClass,_proto,_packetSize0,_vid,_pid,_version,_im,_ip,_is,_configs) \
	{ 18, 1, USB_VERSION, _class,_subClass,_proto,_packetSize0,_vid,_pid,_version,_im,_ip,_is,_configs }

#define D_CONFIG(_totalLength,_interfaces) \
	{ 9, 2, _totalLength,_interfaces, 1, 0, USB_CONFIG_BUS_POWERED | USB_CONFIG_REMOTE_WAKEUP, USB_CONFIG_POWER_MA(USB_CONFIG_POWER) }

#define D_INTERFACE(_n,_numEndpoints,_class,_subClass,_protocol) \
	{ 9, 4, _n, 0, _numEndpoints, _class,_subClass, _protocol, 0 }

#define D_ENDPOINT(_addr,_attr,_packetSize, _interval) \
	{ 7, 5, _addr,_attr,_packetSize, _interval }

#define D_IAD(_firstInterface, _count, _class, _subClass, _protocol) \
	{ 8, 11, _firstInterface, _count, _class, _subClass, _protocol, 0 }

#define D_CDCCS(_subtype,_d0,_d1)	{ 5, 0x24, _subtype, _d0, _d1 }
#define D_CDCCS4(_subtype,_d0)		{ 4, 0x24, _subtype, _d0 }

#define lowByte(w) ((uint8_t) ((w) & 0xff))
#define highByte(w) ((uint8_t) ((w) >> 8))

#define D_HIDREPORT(length) { 9, 0x21, 0x01, 0x01, 0, 1, 0x22, lowByte(length), highByte(length) }

#define USB_ENDPOINT_IN(addr)                  (lowByte((addr) | 0x80))

#define USB_ENDPOINT_TYPE_MASK                 0x03
#define USB_ENDPOINT_TYPE_CONTROL              0x00
#define USB_ENDPOINT_TYPE_ISOCHRONOUS          0x01
#define USB_ENDPOINT_TYPE_BULK                 0x02
#define USB_ENDPOINT_TYPE_INTERRUPT            0x03

//	Device
typedef struct {
	u8 len;				// 18
	u8 dtype;			// 1 USB_DEVICE_DESCRIPTOR_TYPE
	u16 usbVersion;		// 0x200 or 0x210
	u8	deviceClass;
	u8	deviceSubClass;
	u8	deviceProtocol;
	u8	packetSize0;	// Packet 0
	u16	idVendor;
	u16	idProduct;
	u16	deviceVersion;	// 0x100
	u8	iManufacturer;
	u8	iProduct;
	u8	iSerialNumber;
	u8	bNumConfigurations;
} DeviceDescriptor;

//	Config
typedef struct {
	u8	len;			// 9
	u8	dtype;			// 2
	u16 clen;			// total length
	u8	numInterfaces;
	u8	config;
	u8	iconfig;
	u8	attributes;
	u8	maxPower;
} ConfigDescriptor;

typedef struct
{
	uint8_t bmRequestType;
	uint8_t bRequest;
	uint8_t wValueL;
	uint8_t wValueH;
	uint16_t wIndex;
	uint16_t wLength;
} USBSetup;

//	Interface
typedef struct
{
	u8 len;		// 9
	u8 dtype;	// 4
	u8 number;
	u8 alternate;
	u8 numEndpoints;
	u8 interfaceClass;
	u8 interfaceSubClass;
	u8 protocol;
	u8 iInterface;
} InterfaceDescriptor;

//	Endpoint
typedef struct
{
	u8 len;		// 7
	u8 dtype;	// 5
	u8 addr;
	u8 attr;
	u16 packetSize;
	u8 interval;
} EndpointDescriptor;

typedef struct
{
  uint8_t len;      // 9
  uint8_t dtype;    // 0x21
  uint8_t addr;
  uint8_t versionL; // 0x101
  uint8_t versionH; // 0x101
  uint8_t country;
  uint8_t desctype; // 0x22 report
  uint8_t descLenL;
  uint8_t descLenH;
} HIDDescDescriptor;

typedef struct
{
  InterfaceDescriptor hid;
  HIDDescDescriptor   desc;
  EndpointDescriptor  in;
} HIDDescriptor;

const DeviceDescriptor USB_DeviceDescriptorIAD =
	D_DEVICE(0xEF,0x02,0x01,64,USB_VID,USB_PID,0x100,IMANUFACTURER,IPRODUCT,ISERIAL,1);

const u16 STRING_LANGUAGE[2] = {
	(3<<8) | (2+2),
	0x0409	// English
};

const u8 STRING_PRODUCT[] PROGMEM = USB_PRODUCT;
const u8 STRING_MANUFACTURER[] PROGMEM = USB_MANUFACTURER;

volatile u8 _usbConfiguration = 0;
volatile u8 _usbCurrentStatus = 0; // meaning of bits see usb_20.pdf, Figure 9-4. Information Returned by a GetStatus() Request to a Device
volatile u8 _usbSuspendState = 0; // copy of UDINT to check SUSPI and WAKEUPI bits

static inline void WaitIN(void)
{
	while (!(UEINTX & (1<<TXINI)))
		;
}

static inline void ClearIN(void)
{
	UEINTX = ~(1<<TXINI);
}

static inline void WaitOUT(void)
{
	while (!(UEINTX & (1<<RXOUTI)))
		;
}

static inline u8 WaitForINOrOUT()
{
	while (!(UEINTX & ((1<<TXINI)|(1<<RXOUTI))))
		;
	return (UEINTX & (1<<RXOUTI)) == 0;
}

static inline void ClearOUT(void)
{
	UEINTX = ~(1<<RXOUTI);
}

static inline void Recv(volatile u8* data, u8 count)
{
	while (count--)
		*data++ = UEDATX;

}

static inline u8 Recv8()
{
	return UEDATX;
}

static inline void Send8(u8 d)
{
	UEDATX = d;
}

static inline void SetEP(u8 ep)
{
	UENUM = ep;
}

static inline u8 FifoByteCount()
{
	return UEBCLX;
}

static inline u8 ReceivedSetupInt()
{
	return UEINTX & (1<<RXSTPI);
}

static inline void ClearSetupInt()
{
	UEINTX = ~((1<<RXSTPI) | (1<<RXOUTI) | (1<<TXINI));
}

static inline void Stall()
{
	UECONX = (1<<STALLRQ) | (1<<EPEN);
}

static inline u8 ReadWriteAllowed()
{
	return UEINTX & (1<<RWAL);
}

static inline u8 Stalled()
{
	return UEINTX & (1<<STALLEDI);
}

static inline u8 FifoFree()
{
	return UEINTX & (1<<FIFOCON);
}

static inline void ReleaseRX()
{
	UEINTX = 0x6B;	// FIFOCON=0 NAKINI=1 RWAL=1 NAKOUTI=0 RXSTPI=1 RXOUTI=0 STALLEDI=1 TXINI=1
}

static inline void ReleaseTX()
{
	UEINTX = 0x3A;	// FIFOCON=0 NAKINI=0 RWAL=1 NAKOUTI=1 RXSTPI=1 RXOUTI=0 STALLEDI=1 TXINI=0
}

static inline u8 FrameNumber()
{
	return UDFNUML;
}

static int _cmark;
static int _cend;
void InitControl(int end)
{
	SetEP(0);
	_cmark = 0;
	_cend = end;
}

static
int SendControl(u8 d)
{
	if (_cmark < _cend)
	{
		if (!WaitForINOrOUT())
			return 0;
		Send8(d);
		if (!((_cmark + 1) & 0x3F))
			ClearIN();	// Fifo is full, release this packet
	}
	_cmark++;
	return 1;
}

int USB_SendControl(u8 flags, const void* d, int len)
{
	int sent = len;
	const u8* data = (const u8*)d;
	int pgm = flags & TRANSFER_PGM;
	while (len--)
	{
		u8 c = pgm ? pgm_read_byte(data++) : *data++;
		if (!SendControl(c))
			return -1;
	}
	return sent;
}

u8 _initEndpoints[USB_ENDPOINTS] =
{
	0,                      // Control Endpoint

	EP_TYPE_INTERRUPT_IN,   // CDC_ENDPOINT_ACM
	EP_TYPE_BULK_OUT,       // CDC_ENDPOINT_OUT
	EP_TYPE_BULK_IN,        // CDC_ENDPOINT_IN

	// Following endpoints are automatically initialized to 0
};

#define EP_SINGLE_64 0x32	// EP0
#define EP_DOUBLE_64 0x36	// Other endpoints
#define EP_SINGLE_16 0x12

static
void InitEP(u8 index, u8 type, u8 size)
{
	UENUM = index;
	UECONX = (1<<EPEN);
	UECFG0X = type;
	UECFG1X = size;
}

void USB_Flush(u8 ep)
{
	SetEP(ep);
	if (FifoByteCount())
		ReleaseTX();
}

static
void InitEndpoints()
{
	for (u8 i = 1; i < sizeof(_initEndpoints) && _initEndpoints[i] != 0; i++)
	{
		UENUM = i;
		UECONX = (1<<EPEN);
		UECFG0X = _initEndpoints[i];
#if USB_EP_SIZE == 16
		UECFG1X = EP_SINGLE_16;
#elif USB_EP_SIZE == 64
		UECFG1X = EP_DOUBLE_64;
#else
#error Unsupported value for USB_EP_SIZE
#endif
	}
	UERST = 0x7E;	// And reset them
	UERST = 0;
}

static const uint8_t _hidReportDescriptor[] PROGMEM = {
  //  Keyboard
  0x05, 0x01,                    // USAGE_PAGE (Generic Desktop)  // 47
  0x09, 0x06,                    // USAGE (Keyboard)
  0xa1, 0x01,                    // COLLECTION (Application)
  0x85, 0x08,                    //   REPORT_ID (2)
  0x05, 0x07,                    //   USAGE_PAGE (Keyboard)

  0x19, 0xe0,                    //   USAGE_MINIMUM (Keyboard LeftControl)
  0x29, 0xe7,                    //   USAGE_MAXIMUM (Keyboard Right GUI)
  0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
  0x25, 0x01,                    //   LOGICAL_MAXIMUM (1)
  0x75, 0x01,                    //   REPORT_SIZE (1)
  0x95, 0x08,                    //   REPORT_COUNT (8)
  0x81, 0x02,                    //   INPUT (Data,Var,Abs)

   /* 104 Keys as bitmap */
  0x05, 0x07,                     /*   USAGE_PAGE (Keyboard) */
  0x19, 0x00,						/*   Usage Minimum (0) */
  0x29, 103,		                /*   Usage Maximum (103) */
  0x15, 0x00,						/*   Logical Minimum (0) */
  0x25, 0x01,						/*   Logical Maximum (1) */
  0x75, 0x01,						/*   Report Size (1) */
  0x95, 104,			            /*   Report Count (104) */
  0x81, 0x02,						/*   Input (Data, Variable, Absolute) */

  0xc0,                          // END_COLLECTION
};

typedef struct
{
  uint8_t modifiers;
  uint8_t keys[104 / 8];
} KeyReport;

KeyReport _keyReport = {0};


//	Handle CLASS_INTERFACE requests
static int ClassInterfaceRequest(USBSetup setup) {
    return USB_SendControl(TRANSFER_PGM, _hidReportDescriptor, sizeof(_hidReportDescriptor));
}

static u8 SendInterfaces()
{
    uint8_t pluggedInterface = 0;
    uint8_t pluggedEndpoint = 0;
	HIDDescriptor hidInterface = {
		D_INTERFACE(pluggedInterface, 1, USB_DEVICE_CLASS_HUMAN_INTERFACE, HID_SUBCLASS_NONE, HID_PROTOCOL_NONE),
		D_HIDREPORT(sizeof(_hidReportDescriptor)),
		D_ENDPOINT(USB_ENDPOINT_IN(pluggedEndpoint), USB_ENDPOINT_TYPE_INTERRUPT, USB_EP_SIZE, 0x01)
	};
	USB_SendControl(0, &hidInterface, sizeof(hidInterface));
	return 1;
}

//	Construct a dynamic configuration descriptor
//	This really needs dynamic endpoint allocation etc
//	TODO
static
int SendConfiguration(int maxlen)
{
	//	Count and measure interfaces
	InitControl(0);
	u8 interfaces = SendInterfaces();
	ConfigDescriptor config = D_CONFIG(_cmark + sizeof(ConfigDescriptor),interfaces);

	//	Now send them
	InitControl(maxlen);
	USB_SendControl(0,&config,sizeof(ConfigDescriptor));
	SendInterfaces();
	return 1;
}

// Send a USB descriptor string. The string is stored in PROGMEM as a
// plain ASCII string but is sent out as UTF-16 with the correct 2-byte
// prefix
static int USB_SendStringDescriptor(const u8*string_P, u8 string_len, uint8_t flags) {
        SendControl(2 + string_len * 2);
        SendControl(3);
        int pgm = flags & TRANSFER_PGM;
        for(u8 i = 0; i < string_len; i++) {
                int r = SendControl(pgm ? pgm_read_byte(&string_P[i]) : string_P[i]);
                r &= SendControl(0); // high byte
                if(!r) {
                        return 0;
                }
        }
        return 1;
}

static
int SendDescriptor(USBSetup setup)
{
	u8 t = setup.wValueH;
	if (USB_CONFIGURATION_DESCRIPTOR_TYPE == t)
		return SendConfiguration(setup.wLength);

	InitControl(setup.wLength);
    int ret = USB_SendControl(TRANSFER_PGM, _hidReportDescriptor, sizeof(_hidReportDescriptor));
	if (ret != 0) {
		return (ret > 0 ? 1 : 0);
	}

	const u8* desc_addr = 0;
	if (USB_DEVICE_DESCRIPTOR_TYPE == t)
	{
		desc_addr = (const u8*)&USB_DeviceDescriptorIAD;
	}
	else if (USB_STRING_DESCRIPTOR_TYPE == t)
	{
		if (setup.wValueL == 0) {
			desc_addr = (const u8*)&STRING_LANGUAGE;
		}
		else if (setup.wValueL == IPRODUCT) {
			return USB_SendStringDescriptor(STRING_PRODUCT, strlen(USB_PRODUCT), TRANSFER_PGM);
		}
		else if (setup.wValueL == IMANUFACTURER) {
			return USB_SendStringDescriptor(STRING_MANUFACTURER, strlen(USB_MANUFACTURER), TRANSFER_PGM);
		}
		else if (setup.wValueL == ISERIAL) {
			char name[ISERIAL_MAX_LEN];
            int descriptorSize = sizeof(_hidReportDescriptor);
            name[0] = 'H';
            name[1] = 'I';
            name[2] = 'D';
            name[3] = 'A' + (descriptorSize & 0x0F);
            name[4] = 'A' + ((descriptorSize >> 4) & 0x0F);
			return USB_SendStringDescriptor((uint8_t*)name, strlen(name), 0);
		}
		else
			return 0;
	}

	if (desc_addr == 0)
		return 0;
	u8 desc_length = pgm_read_byte(desc_addr);

	USB_SendControl(TRANSFER_PGM,desc_addr,desc_length);
	return 1;
}


//	Endpoint 0 interrupt
ISR(USB_COM_vect)
{
    SetEP(0);
	if (!ReceivedSetupInt())
		return;

	USBSetup setup;
	Recv((u8*)&setup,8);
	ClearSetupInt();

	u8 requestType = setup.bmRequestType;
	if (requestType & REQUEST_DEVICETOHOST)
		WaitIN();
	else
		ClearIN();

    int ok = 1;
	if (REQUEST_STANDARD == (requestType & REQUEST_TYPE))
	{
		//	Standard Requests
		u8 r = setup.bRequest;
		u16 wValue = setup.wValueL | (setup.wValueH << 8);
		if (GET_STATUS == r)
		{
			if (requestType == (REQUEST_DEVICETOHOST | REQUEST_STANDARD | REQUEST_DEVICE))
			{
				Send8(_usbCurrentStatus);
				Send8(0);
			}
			else
			{
				// TODO: handle the HALT state of an endpoint here
				// see "Figure 9-6. Information Returned by a GetStatus() Request to an Endpoint" in usb_20.pdf for more information
				Send8(0);
				Send8(0);
			}
		}
		else if (CLEAR_FEATURE == r)
		{
			if((requestType == (REQUEST_HOSTTODEVICE | REQUEST_STANDARD | REQUEST_DEVICE))
				&& (wValue == DEVICE_REMOTE_WAKEUP))
			{
				_usbCurrentStatus &= ~FEATURE_REMOTE_WAKEUP_ENABLED;
			}
		}
		else if (SET_FEATURE == r)
		{
			if((requestType == (REQUEST_HOSTTODEVICE | REQUEST_STANDARD | REQUEST_DEVICE))
				&& (wValue == DEVICE_REMOTE_WAKEUP))
			{
				_usbCurrentStatus |= FEATURE_REMOTE_WAKEUP_ENABLED;
			}
		}
		else if (SET_ADDRESS == r)
		{
			WaitIN();
			UDADDR = setup.wValueL | (1<<ADDEN);
		}
		else if (GET_DESCRIPTOR == r)
		{
			ok = SendDescriptor(setup);
		}
		else if (SET_DESCRIPTOR == r)
		{
			ok = 0;
		}
		else if (GET_CONFIGURATION == r)
		{
			Send8(1);
		}
		else if (SET_CONFIGURATION == r)
		{
			if (REQUEST_DEVICE == (requestType & REQUEST_RECIPIENT))
			{
				InitEndpoints();
				_usbConfiguration = setup.wValueL;
			} else
				ok = 0;
		}
		else if (GET_INTERFACE == r)
		{
		}
		else if (SET_INTERFACE == r)
		{
		}
	}
	else
	{
		InitControl(setup.wLength);		//	Max length of transfer
		ok = ClassInterfaceRequest(setup);
	}

	if (ok)
		ClearIN();
	else
	{
		Stall();
	}
}

ISR(USB_GEN_vect)
{
	u8 udint = UDINT;
	UDINT &= ~((1<<EORSTI) | (1<<SOFI)); // clear the IRQ flags for the IRQs which are handled here, except WAKEUPI and SUSPI (see below)

	//	End of Reset
	if (udint & (1<<EORSTI))
	{
		InitEP(0,EP_TYPE_CONTROL,EP_SINGLE_64);	// init ep0
		_usbConfiguration = 0;			// not configured yet
		UEIENX = 1 << RXSTPE;			// Enable interrupts for ep0
	}

	//	Start of Frame - happens every millisecond so we use it for TX and RX LED one-shot timing, too
	if (udint & (1<<SOFI))
	{
		USB_Flush(CDC_TX);				// Send a tx frame if found

		// check whether the one-shot period has elapsed.  if so, turn off the LED
	}

	// the WAKEUPI interrupt is triggered as soon as there are non-idle patterns on the data
	// lines. Thus, the WAKEUPI interrupt can occur even if the controller is not in the "suspend" mode.
	// Therefore the we enable it only when USB is suspended
	if (udint & (1<<WAKEUPI))
	{
		UDIEN = (UDIEN & ~(1<<WAKEUPE)) | (1<<SUSPE); // Disable interrupts for WAKEUP and enable interrupts for SUSPEND

		//TODO
		// WAKEUPI shall be cleared by software (USB clock inputs must be enabled before).
		//USB_ClockEnable();
		UDINT &= ~(1<<WAKEUPI);
		_usbSuspendState = (_usbSuspendState & ~(1<<SUSPI)) | (1<<WAKEUPI);
	}
	else if (udint & (1<<SUSPI)) // only one of the WAKEUPI / SUSPI bits can be active at time
	{
		UDIEN = (UDIEN & ~(1<<SUSPE)) | (1<<WAKEUPE); // Disable interrupts for SUSPEND and enable interrupts for WAKEUP

		//TODO
		//USB_ClockDisable();

		UDINT &= ~((1<<WAKEUPI) | (1<<SUSPI)); // clear any already pending WAKEUP IRQs and the SUSPI request
		_usbSuspendState = (_usbSuspendState & ~(1<<WAKEUPI)) | (1<<SUSPI);
	}
}


int SendReport(uint8_t id, const void* data, int len)
{
	int ret = USB_Send(0, &id, 1);
	if (ret < 0) return ret;
	int ret2 = USB_Send(0 | TRANSFER_RELEASE, data, len);
	if (ret2 < 0) return ret2;
	return ret + ret2;
}

#include <avr/io.h>
#include <util/delay.h>

int main()
{
	DDRB=0xFF;

  _keyReport.keys[0] = 0xFF;
  _keyReport.keys[1] = 0xFF;
  _keyReport.keys[2] = 0xFF;
  _keyReport.keys[3] = 0xFF;
    while (1) {
      SendReport(8, &_keyReport, sizeof(KeyReport));
		PORTB = 0xFF; // ON
        _delay_ms(1000);
		PORTB = 0x00; // OFF
        _delay_ms(1000);
	}
    return 0;
}
