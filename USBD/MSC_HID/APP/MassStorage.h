#ifndef __MASSSTORAGE_H__
#define __MASSSTORAGE_H__


#define USBD_VID	0x1209
#define USBD_PID    0x5120


/* Endpoint Config */
#define EP_BULK_IN_NUM		1
#define EP_BULK_OUT_NUM		1

#define EP_CTRL_PKSZ		64
#define EP_BULK_IN_PKSZ		64
#define EP_BULK_OUT_PKSZ	64


/* Descriptor information */
#define USBD_SELF_POWERED	0
#define USBD_REMOTE_WAKEUP  0
#define USBD_MAX_POWER      50  // 50 * 2mA = 100mA
                   

/* MSC CBW/CSW Signature */
#define CBW_SIGNATURE       0x43425355
#define CSW_SIGNATURE       0x53425355

/* MSC UFI Command */
#define UFI_INQUIRY                             0x12
#define UFI_READ_CAPACITY                       0x25
#define UFI_READ_FORMAT_CAPACITY                0x23
#define UFI_READ_10                             0x28
#define UFI_READ_12                             0xA8
#define UFI_WRITE_10                            0x2A
#define UFI_WRITE_12                            0xAA
#define UFI_VERIFY_10                           0x2F
#define UFI_START_STOP                          0x1B
#define UFI_MODE_SENSE_6                        0x1A
#define UFI_MODE_SENSE_10                       0x5A
#define UFI_REQUEST_SENSE                       0x03
#define UFI_MODE_SELECT_6                       0x15
#define UFI_MODE_SELECT_10                      0x55
#define UFI_TEST_UNIT_READY                     0x00
#define UFI_PREVENT_ALLOW_MEDIUM_REMOVAL        0x1E


struct __attribute__((packed)) CBW 		// Command Block Wrapper
{
    uint32_t  dCBWSignature;
    uint32_t  dCBWTag;
    uint32_t  dCBWDataTransferLength;
    uint8_t   bmCBWFlags;
    uint8_t   bCBWLUN;
    uint8_t   bCBWCBLength;
    uint8_t   u8OPCode;
    uint8_t   u8LUN;
    uint8_t   au8Data[14];
};

struct __attribute__((packed)) CSW 		// Command Status Wrapper
{
    uint32_t  dCSWSignature;
    uint32_t  dCSWTag;
    uint32_t  dCSWDataResidue;
    uint8_t   bCSWStatus;
};


void MSC_Init(void);
void MSC_ProcessOUT(void);
void MSC_ProcessIN(void);


#endif
