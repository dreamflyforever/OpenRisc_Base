#include "support.h"
#include "serial.h"
#define UART_BAUD_RATE	38400
#define UART_BASE			0x90000000
#define UART_IRQ			2

/* System Clock */
#define SYS_CLK	40000000 // 40MHz
#define TICKS_PER_SECOND 100
#define int32_t unsigned int 

unsigned int  strlen(const char *s)
{
    const char *sc;

    for (sc = s; *sc != '\0'; ++sc) /* nothing */
        ;

    return sc - s;
}


 void hw_console_output(const char* str)
 {
	while(*str){
		/* Transmit Character */
		if(*str == '\n'){
			/* Wait for transfer fifo empty*/
			while((REG8(UART_BASE + UART_LSR) & UART_LSR_THRE) != UART_LSR_THRE);
			REG8(UART_BASE + UART_TX) = '\r';
		}
		
		/* Wait for transfer fifo empty*/
		while((REG8(UART_BASE + UART_LSR) & UART_LSR_THRE) != UART_LSR_THRE);
		
		/* transfer a character */
		REG8(UART_BASE + UART_TX) = *str;
		/* point to next character */
		str++;
	}
 }

/* init console to support kprintf */
static void hw_console_init()
{
	int32_t divisor;
	float float_divisor;
 
	/* Reset receiver and transmiter */
	//start up FIFO ,clear RCVR FIFO and XMIT FIFO ,and set FIFO size for 4 bytes
	REG8(UART_BASE + UART_FCR) = UART_FCR_ENABLE_FIFO | UART_FCR_CLEAR_RCVR | UART_FCR_CLEAR_XMIT | UART_FCR_TRIGGER_4;
 
	/* Disable all interrupts */
	REG8(UART_BASE + UART_IER) = 0x0;
        
	/* Enable RX and TX interrupt */
	// REG8(UART_BASE + UART_IER) = UART_IER_RDI | UART_IER_THRI;
 
	/* Set 8 bit char, 1 stop bit, no parity */
	REG8(UART_BASE + UART_LCR) = UART_LCR_WLEN8 & ~(UART_LCR_STOP | UART_LCR_PARITY);
 
	/* Set baud rate */
	float_divisor = (float)SYS_CLK/(16 * UART_BAUD_RATE);
	float_divisor += 0.50f; // Ensure round up
	divisor = (int32_t)float_divisor;
        
	/* set corresponding register value */
	REG8(UART_BASE + UART_LCR) |= UART_LCR_DLAB;
	REG8(UART_BASE + UART_DLM) = (divisor >> 8) & 0x000000ff;
	REG8(UART_BASE + UART_DLL) = divisor & 0x000000ff;
	REG8(UART_BASE + UART_LCR) &= ~(UART_LCR_DLAB);
}

void hw_board_init()
{
	/* init console before using printf() */
	hw_console_init();	
	
	/* init UserISR interrupt routines */
}


void my_str_printf(char* p)
{
    int i;
    for(i = 0; i < strlen(p); i++ )
    {
        hw_console_output(p[i]);
    }    

}

#define va_end(ap) ( ap = (va_list)0 )

void my_printf(char *p,...)
{
    int i;  
    int x;           //放着要打印的整形
    char int_char[3]; //存放着整形的个、十、百。并且已经转化成字符
    
    va_list sec_addr = (char*)0; // 初始化指针为NULL

    va_start(sec_addr,p);     //目的是为了让sec_addr指向第二个参数的地址


    for(i=0; i < strlen(p); i++)
    {
        if(p[i] == '%')
        {
            if(p[i+1] == 's')
            {
                my_str_printf(va_arg(sec_addr,char *));  //每次执行都sec_addr指向下一参数地址
                i = i + 2;   //滤过 %s
            }        
            if(p[i+1] == 'd')
            {
                x = va_arg(sec_addr,int);

                int_char[0] = x/100 + '0'; // 百位
                if(int_char[0] == '0')
                {
                    ;
                }
                else 
                {
                    hw_console_output(int_char[0]);
                }

                int_char[1] = x/10 + '0';
                if(int_char[1] == '0')
                {
                    ;
                }
                else 
                {
                    hw_console_output(int_char[1]);
                }

                int_char[2] = x + '0';
                if(int_char[2] == '0')
                {
                    ;
                }
                else 
                {
                    hw_console_output(int_char[2]);
                }
                i = i + 2;    //滤过 %d
            }
        }     
        hw_console_output(p[i]);

    } 
    va_end( sec_addr);   
}

