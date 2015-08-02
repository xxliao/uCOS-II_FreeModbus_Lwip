/*
 * FreeModbus Libary: BARE Port
 * Copyright (C) 2006 Christian Walter <wolti@sil.at>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * File: $Id: portserial.c,v 1.1 2006/08/22 21:35:13 wolti Exp $
 */

#include "port.h"

/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"

#include "stm32f10x_usart.h"

/* ----------------------- static functions ---------------------------------*/
static void prvvUARTTxReadyISR( void );
static void prvvUARTRxISR( void );

/* ----------------------- Start implementation -----------------------------*/
void
vMBPortSerialEnable( BOOL xRxEnable, BOOL xTxEnable )
{
    /* If xRXEnable enable serial receive interrupts. If xTxENable enable
     * transmitter empty interrupts.
     */

	if(xRxEnable)
	{
		USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
		GPIO_ResetBits(GPIOA,GPIO_Pin_4);
	}
	else
	{
		USART_ITConfig(USART2, USART_IT_RXNE, DISABLE);
		GPIO_SetBits(GPIOA,GPIO_Pin_4);	
	}

	if(xTxEnable)
	{
		USART_ITConfig(USART2, USART_IT_TC, ENABLE);
		GPIO_SetBits(GPIOA,GPIO_Pin_4);
	}
	else
	{
	   USART_ITConfig(USART2, USART_IT_TC, DISABLE);
	   GPIO_ResetBits(GPIOA,GPIO_Pin_4);
	}
	
}

BOOL
xMBPortSerialInit( UCHAR ucPORT, ULONG ulBaudRate, UCHAR ucDataBits, eMBParity eParity )
{
	BOOL            bInitialized = TRUE;
	USART_InitTypeDef USART_InitStructure;
	USART_ClockInitTypeDef  USART_ClockInitStructure;

	USART_ClockInitStructure.USART_Clock = USART_Clock_Disable;
	USART_ClockInitStructure.USART_CPOL = USART_CPOL_Low;
	USART_ClockInitStructure.USART_CPHA = USART_CPHA_2Edge;
	USART_ClockInitStructure.USART_LastBit = USART_LastBit_Disable;

	USART_InitStructure.USART_BaudRate = ulBaudRate;
    switch ( eParity )
    {
    case MB_PAR_NONE:
        USART_InitStructure.USART_Parity = USART_Parity_No ;
				USART_InitStructure.USART_WordLength = USART_WordLength_8b;
				break;
    case MB_PAR_ODD:
        USART_InitStructure.USART_Parity = USART_Parity_Odd ;
				USART_InitStructure.USART_WordLength = USART_WordLength_9b;
        break;
    case MB_PAR_EVEN:
        USART_InitStructure.USART_Parity = USART_Parity_Even ;
				USART_InitStructure.USART_WordLength = USART_WordLength_9b;
        break;
    }
    if( bInitialized )
    {
        ENTER_CRITICAL_SECTION(  );

				USART_InitStructure.USART_StopBits = USART_StopBits_1;
				USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;					USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
				switch(ucPORT)
				{
		    case 1:
		        {
								USART_ClockInit(USART1, &USART_ClockInitStructure);
								USART_Init(USART1, &USART_InitStructure);
								USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
								USART_Cmd(USART1, ENABLE);
						}
						break;
		    case 2:
		        {
								USART_ClockInit(USART2, &USART_ClockInitStructure);
								USART_Init(USART2, &USART_InitStructure);
								USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
								USART_Cmd(USART2, ENABLE);
						}
		        break;
		    case 3:
		        {
								USART_ClockInit(USART3, &USART_ClockInitStructure);
								USART_Init(USART3, &USART_InitStructure);
								USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
								USART_Cmd(USART3, ENABLE);
						}
		        break;
	  	  }

        EXIT_CRITICAL_SECTION(  );
    }
    return bInitialized;


//    return FALSE;
}

void
vMBPortClose( void )
{
	USART_ITConfig(USART2, USART_IT_TC|USART_IT_RXNE, DISABLE);
	USART_Cmd(USART2, DISABLE);
}


BOOL
xMBPortSerialPutByte( CHAR ucByte )
{
    /* Put a byte in the UARTs transmit buffer. This function is called
     * by the protocol stack if pxMBFrameCBTransmitterEmpty( ) has been
     * called. */
	while( USART_GetFlagStatus(USART2,USART_FLAG_TC)!= SET);
		USART_SendData(USART2, ucByte);

	return TRUE;
}

BOOL
xMBPortSerialGetByte( CHAR * pucByte )
{
    /* Return the byte in the UARTs receive buffer. This function is called
     * by the protocol stack after pxMBFrameCBByteReceived( ) has been called.
     */
	*pucByte = USART_ReceiveData(USART2);
	
    return TRUE;
}

/* Create an interrupt handler for the transmit buffer empty interrupt
 * (or an equivalent) for your target processor. This function should then
 * call pxMBFrameCBTransmitterEmpty( ) which tells the protocol stack that
 * a new character can be sent. The protocol stack will then call 
 * xMBPortSerialPutByte( ) to send the character.
 */
static void prvvUARTTxReadyISR( void )
{
    pxMBFrameCBTransmitterEmpty(  );
}

/* Create an interrupt handler for the receive interrupt for your target
 * processor. This function should then call pxMBFrameCBByteReceived( ). The
 * protocol stack will then call xMBPortSerialGetByte( ) to retrieve the
 * character.
 */
static void prvvUARTRxISR( void )
{
    pxMBFrameCBByteReceived(  );
}


void USART2_IRQHandler(void) 
{  

	//发生接收中断 
	if(USART_GetITStatus(USART2, USART_IT_RXNE) == SET) 
	{   
		
		USART_ClearITPendingBit(USART2, USART_IT_RXNE); 
		prvvUARTRxISR();
		//清除中断标志位 
		
	}
	 //发生完成中断
	 if(USART_GetITStatus(USART2, USART_IT_TC) == SET)
	 {
	 //	USARTSendData(USART1,0xee);
		 prvvUARTTxReadyISR();
		 //清除中断标志
		//		 USART_ClearITPendingBit(USART2, USART_IT_TC);
	 }
 }

