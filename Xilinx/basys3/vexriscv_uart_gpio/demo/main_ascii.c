// This file is Copyright (c) 2020 Florent Kermarrec <florent@enjoy-digital.fr>
// License: BSD

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>


#include <irq.h>
#include <libbase/uart.h>
#include <libbase/console.h>
#include <generated/csr.h>

/*-----------------------------------------------------------------------*/
/* Uart                                                                  */
/*-----------------------------------------------------------------------*/

static char *readstr(void)
{
	char c[2];
	static char s[64];
	static int ptr = 0;

	if(readchar_nonblock()) {
		c[0] = getchar();
		c[1] = 0;
		switch(c[0]) {
			case 0x7f:
			case 0x08:
				if(ptr > 0) {
					ptr--;
					fputs("\x08 \x08", stdout);
				}
				break;
			case 0x07:
				break;
			case '\r':
			case '\n':
				s[ptr] = 0x00;
				fputs("\n", stdout);
				ptr = 0;
				return s;
			default:
				if(ptr >= (sizeof(s) - 1))
					break;
				fputs(c, stdout);
				s[ptr] = c[0];
				ptr++;
				break;
		}
	}

	return NULL;
}

static char *get_token(char **str)
{
	char *c, *d;

	c = (char *)strchr(*str, ' ');
	if(c == NULL) {
		d = *str;
		*str = *str+strlen(*str);
		return d;
	}
	*c = 0;
	d = *str;
	*str = c+1;
	return d;
}

static void prompt(void)
{
	printf("\e[92;1mlitex-demo-app\e[0m> ");
}

/*-----------------------------------------------------------------------*/
/* Help                                                                  */
/*-----------------------------------------------------------------------*/

static void help(void)
{
	puts("\nLiteX minimal demo app built "__DATE__" "__TIME__"\n");
	puts("Available commands:");
	puts("help               - Show this command");
	puts("reboot             - Reboot CPU");
#ifdef CSR_LEDS_BASE
	puts("led                - Led demo");
#endif
	puts("donut              - Spinning Donut demo");
	puts("helloc             - Hello C");
#ifdef WITH_CXX
	puts("hellocpp           - Hello C++");
#endif
}



// Driver code to test above function
//int main()
//{
//	int i;
//	for (i = 0; i < 10; i++) {
//		// delay of one second
//		delay(1);
//		printf("%d seconds have passed\n", i + 1);
//	}
//	return 0;
//}


static void test_2(void)
{		
		uart_rxtx_write(0x48);
		uart_rxtx_write(0x65);
		uart_rxtx_write(0x6c);
		uart_rxtx_write(0x6c);
		uart_rxtx_write(0x6f);
		uart_rxtx_write(0x21);
		uart_rxtx_write(0x21);
		uart_rxtx_write(0x0a);
//		uart_rxtx_write(' ');
//		uart_rxtx_write('H');
//		uart_rxtx_write('e');
//		uart_rxtx_write('l');
//		uart_rxtx_write('l');
//		uart_rxtx_write('o');
//		uart_rxtx_write('_');
//		uart_rxtx_write('w');
//		uart_rxtx_write('o');
//		uart_rxtx_write('r');
//		uart_rxtx_write('l');
//		uart_rxtx_write('d');
//		uart_rxtx_write(' ');
}
/*----------------------------Manual test-------------------------------------------*/

void divideString(char* str, int n)
{
    int str_size = strlen(str);
    int i;
    int part_size;
 
    // Check if string can be divided in
    // n equal parts
    if (str_size % n != 0) {
        printf("Invalid Input: String size");
        printf(" is not divisible by n");
        return;
    }
 
    // Calculate the size of parts to
    // find the division points
    part_size = str_size / n;
    for (i = 0; i < str_size; i++) {
		printf('in the loop \n');
		printf(str[i]);
        if (i % part_size == 0)
	        uart_rxtx_write(str[i]);
			printf(str[i]);
		uart_rxtx_write(0x0a);

    }
}

static void test(void)
{
	uint32_t value, addr,rvalue,x;
	char strng[32];

	value=0x1;
    addr= 0x50000000;
	x=1;
	// enable sim_trace
	//csr_write_simple(1,0x82000000);
	sim_trace_enable_write(1);
	printf('check print working');

    for(x=0; x<10; x++)
    {
        csr_write_simple((0x30 +x ), addr+(4*x));
    }

//    for(x=0; x<10; x++)
//    {
		x=1;
		rvalue = csr_read_simple(addr+(4*x));
		printf(csr_read_simple(addr+(4*1)));
		printf(rvalue);
		
		itoa(rvalue, strng,10);
		printf(rvalue);
//		printf(strng);
//		divideString(strng,8);

		uart_rxtx_write(0x0a);
//		uart_rxtx_write(csr_read_simple(addr + (4*x)));
//    }



	sim_trace_enable_write(0);
	// call sim_finish
	//csr_write_simple(1,0x82001000);
	sim_finish_finish_write(1);

}
/*-----------------------------------------------------------------------*/
/* Commands                                                              */
/*-----------------------------------------------------------------------*/

static void reboot_cmd(void)
{
	ctrl_reset_write(1);
}

#ifdef CSR_LEDS_BASE
static void led_cmd(void)
{
	int i;
	printf("Led demo...\n");

	printf("Counter mode...\n");
	for(i=0; i<32; i++) {
		leds_out_write(i);
		busy_wait(100);
	}

	printf("Shift mode...\n");
	for(i=0; i<4; i++) {
		leds_out_write(1<<i);
		busy_wait(200);
	}
	for(i=0; i<4; i++) {
		leds_out_write(1<<(3-i));
		busy_wait(200);
	}

	printf("Dance mode...\n");
	for(i=0; i<4; i++) {
		leds_out_write(0x55);
		busy_wait(200);
		leds_out_write(0xaa);
		busy_wait(200);
	}
}
#endif

extern void donut(void);

static void donut_cmd(void)
{
	printf("Donut demo...\n");
	donut();
}

extern void helloc(void);

static void helloc_cmd(void)
{
	printf("Hello C demo...\n");
	helloc();
}

#ifdef WITH_CXX
extern void hellocpp(void);

static void hellocpp_cmd(void)
{
	printf("Hello C++ demo...\n");
	hellocpp();
}
#endif

/*-----------------------------------------------------------------------*/
/* Console service / Main                                                */
/*-----------------------------------------------------------------------*/

static void console_service(void)
{
	char *str;
	char *token;
	test();
	str = readstr();
	if(str == NULL) return;
	token = get_token(&str);
	if(strcmp(token, "help") == 0)
		help();
	else if(strcmp(token, "reboot") == 0)
		reboot_cmd();
#ifdef CSR_LEDS_BASE
	else if(strcmp(token, "led") == 0)
		led_cmd();
#endif
	else if(strcmp(token, "donut") == 0)
		donut_cmd();
	else if(strcmp(token, "helloc") == 0)
		helloc_cmd();
#ifdef WITH_CXX
	else if(strcmp(token, "hellocpp") == 0)
		hellocpp_cmd();
#endif
	prompt();
}

int main(void)
{
#ifdef CONFIG_CPU_HAS_INTERRUPT
	irq_setmask(0);
	irq_setie(1);
#endif
	uart_init();
//	test();
//	help();
//	prompt();
//	test_2();
	test();
//	test_2();
//	test();
//	test_2();

//	while(1) {
//		console_service();
//		}
	return 0;
}
