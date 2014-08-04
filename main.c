/*
 * HardWare: OpenRISC
 * Make it easy to run main-function
 * By Shanjin Yang, 2013-9
 *
 */
extern void hw_console_output(const char* str);
#define kprintf hw_console_output

int main()
{
    while(1)
    {
       kprintf("hello\n"); 
    };
    return 0;
}
