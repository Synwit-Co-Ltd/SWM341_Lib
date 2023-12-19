#include "SWM341.h"

#include <string.h>


/* ע�⣺
 *	1����������������ֵ�ϴ����������������ޣ��� MstClk > 100000 ʱ��
 *	   ������������裨500KHz ʱ 3K����1MHz ʱ 1K���������� I2C ���Ƶ�ʻ�����趨ֵ
 *	2���ӻ�ģʽ֧�ֵ��������Ƶ���Ե�������ģʽ����˵�����ʱ��Ƶ������Ϊ���ʱ��
 *	   �ӻ��޷���ʱ��Ӧ ACK���������޷�����ִ��
 */


#define SLV_ADDR  0x6C

char mst_txbuff[4] = {0x37, 0x55, 0xAA, 0x78};
char mst_rxbuff[4] = {0};
char slv_txbuff[4] = {0};
char slv_rxbuff[5] = {0};	// ��һ�������ǵ�ַ

volatile uint32_t slv_rxindx = 0;
volatile uint32_t slv_txindx = 0;

void SerialInit(void);
void I2CMstInit(void);
void I2CSlvInit(void);

int main(void)
{
	uint32_t i;
	uint8_t ack;
	
	SystemInit();
	
	SerialInit();
	
	I2CMstInit();
	I2CSlvInit();
	
	while(1==1)
	{
		/*************************** Master Write ************************************/
		slv_rxindx = 0;
		
		ack = I2C_Start(I2C0, (SLV_ADDR << 1) | 0, 1);
		if(ack == 0)
		{
			printf("Slave send NACK for address\r\n");
			goto nextloop;
		}
		
		for(i = 0; i < 4; i++)
		{
			ack = I2C_Write(I2C0, mst_txbuff[i], 1);
			if(ack == 0)
			{
				printf("Slave send NACK for data\r\n");
				goto nextloop;
			}
		}
		
		I2C_Stop(I2C0, 1);
		
		printf("Master Send %X %X %X %X\r\n", mst_txbuff[0], mst_txbuff[1], mst_txbuff[2], mst_txbuff[3]);
		
		
		/********************************** Master Read *******************************/
		slv_txindx = 0;
		memcpy(slv_txbuff, &slv_rxbuff[1], 4);
		
		ack = I2C_Start(I2C0, (SLV_ADDR << 1) | 1, 1);
		if(ack == 0)
		{
			printf("Slave send NACK for address\r\n");
			goto nextloop;
		}
		
		for(i = 0; i < 3; i++)
		{
			mst_rxbuff[i] = I2C_Read(I2C0, 1, 1);
		}
		mst_rxbuff[i] = I2C_Read(I2C0, 0, 1);
		
		printf("Master Read %X %X %X %X\r\n", mst_rxbuff[0], mst_rxbuff[1], mst_rxbuff[2], mst_rxbuff[3]);
		
		if((mst_txbuff[0] == mst_rxbuff[0]) && (mst_txbuff[1] == mst_rxbuff[1]) && (mst_txbuff[2] == mst_rxbuff[2]) && (mst_txbuff[3] == mst_rxbuff[3]))
			printf("Success\r\n");
		else
			printf("Fail\r\n");

nextloop:
		I2C_Stop(I2C0, 1);
		for(i = 0; i < SystemCoreClock/8; i++) __NOP();
	}
}


void I2CMstInit(void)
{
	I2C_InitStructure I2C_initStruct;
	
	PORT_Init(PORTN, PIN5, PORTN_PIN5_I2C0_SCL, 1);
	PORTN->OPEND |= (1 << PIN5);	// ��©
	PORTN->PULLU |= (1 << PIN5);	// ����
	PORT_Init(PORTN, PIN4, PORTN_PIN4_I2C0_SDA, 1);
	PORTN->OPEND |= (1 << PIN4);
	PORTN->PULLU |= (1 << PIN4);
	
	I2C_initStruct.Master = 1;
	I2C_initStruct.MstClk = 100000;
	I2C_initStruct.Addr10b = 0;
	I2C_initStruct.TXEmptyIEn = 0;
	I2C_initStruct.RXNotEmptyIEn = 0;
	I2C_Init(I2C0, &I2C_initStruct);
	
	I2C_Open(I2C0);
}


void I2CSlvInit(void)
{
	I2C_InitStructure I2C_initStruct;
	
	PORT_Init(PORTC, PIN5, PORTC_PIN5_I2C1_SCL, 1);
	PORTC->OPEND |= (1 << PIN5);	// ��©
	PORTC->PULLU |= (1 << PIN5);	// ����
	PORT_Init(PORTC, PIN4, PORTC_PIN4_I2C1_SDA, 1);
	PORTC->OPEND |= (1 << PIN4);
	PORTC->PULLU |= (1 << PIN4);
	
	I2C_initStruct.Master = 0;
	I2C_initStruct.MstClk = 0;
	I2C_initStruct.Addr10b = 0;
	I2C_initStruct.SlvAddr = SLV_ADDR;
	I2C_initStruct.TXEmptyIEn = 1;
	I2C_initStruct.RXNotEmptyIEn = 1;
	I2C_initStruct.SlvSTADetIEn  = 1;
	I2C_initStruct.SlvSTODetIEn  = 1;
	I2C_Init(I2C1, &I2C_initStruct);
	
	I2C_Open(I2C1);
}

void I2C1_Handler(void)
{
	uint32_t i;
	
	if(I2C1->IF & I2C_IF_RXSTA_Msk)					//�յ���ʼλ
	{
		I2C1->IF = (1 << I2C_IF_RXSTA_Pos);
		
		slv_rxindx = 0;
		
		I2C1->TR = (1 << I2C_TR_TXCLR_Pos);			//������ʱ�޷�д��
		I2C1->TXDATA = slv_txbuff[0];
		slv_txindx = 1;
	}
	else if(I2C1->IF & I2C_IF_RXNE_Msk)				//���ռĴ����ǿ�
	{
		slv_rxbuff[slv_rxindx] = I2C1->RXDATA;
		if(slv_rxindx < 4) slv_rxindx++;
	}
	else if(I2C1->IF & I2C_IF_TXE_Msk)				//���ͼĴ�����
	{
		I2C1->TXDATA = slv_txbuff[slv_txindx];
		if(slv_txindx < 3) slv_txindx++;
	}
	else if(I2C1->IF & I2C_IF_RXSTO_Msk)			//�յ�ֹͣλ
	{
		I2C1->IF = (1 << I2C_IF_RXSTO_Pos);
		
		for(i = 0; i < 4; i++) slv_txbuff[i] = slv_rxbuff[1+i];
	}
}


void SerialInit(void)
{
	UART_InitStructure UART_initStruct;
	
	PORT_Init(PORTM, PIN0, PORTM_PIN0_UART0_RX, 1);	//GPIOM.0����ΪUART0��������
	PORT_Init(PORTM, PIN1, PORTM_PIN1_UART0_TX, 0);	//GPIOM.1����ΪUART0�������
 	
 	UART_initStruct.Baudrate = 57600;
	UART_initStruct.DataBits = UART_DATA_8BIT;
	UART_initStruct.Parity = UART_PARITY_NONE;
	UART_initStruct.StopBits = UART_STOP_1BIT;
	UART_initStruct.RXThresholdIEn = 0;
	UART_initStruct.TXThresholdIEn = 0;
	UART_initStruct.TimeoutIEn = 0;
 	UART_Init(UART0, &UART_initStruct);
	UART_Open(UART0);
}

/****************************************************************************************************************************************** 
* ��������: fputc()
* ����˵��: printf()ʹ�ô˺������ʵ�ʵĴ��ڴ�ӡ����
* ��    ��: int ch		Ҫ��ӡ���ַ�
*			FILE *f		�ļ����
* ��    ��: ��
* ע������: ��
******************************************************************************************************************************************/
int fputc(int ch, FILE *f)
{
	UART_WriteByte(UART0, ch);
	
	while(UART_IsTXBusy(UART0));
 	
	return ch;
}
