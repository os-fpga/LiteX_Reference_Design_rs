// This file is Copyright (c) 2020 Florent Kermarrec <florent@enjoy-digital.fr>
// License: BSD

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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



/*----------------------------Manual test-------------------------------------------*/



static void test(void)
{
	uint32_t value_1,value_2,value_3, addr,rvalue;
	value_1 =0xdeadbeef;
	value_2 =0xaaaaaaaa;
	value_3 =0x55555555;
	addr= 0xf0020000;
	// enable sim_trace
	//csr_write_simple(1,0x82000000);
	sim_trace_enable_write(1);
	// write to Peripheral IP reg

	rvalue = csr_read_simple(addr);
	csr_write_simple(value_1,addr);
	//printf("Writing value 0x%08lx at 0x%08lx\n\n",value, addr);
	printf("Writing value 0x%08lx for 0xf0021008\n\n",value_1);
	// Read from Peripheral IP reg
	rvalue = csr_read_simple(addr);

	csr_write_simple(value_2 ,addr+4);
	//printf("Writing value 0x%08lx at 0x%08lx\n\n",value, addr);
	printf("Writing value 0x%08lx for 0xf0021008\n\n",value_2);
	// Read from Peripheral IP reg
	rvalue = csr_read_simple(addr+4);
	printf("Reading value 0x%08lx before writing \n\n",rvalue);
	//csr_write_simple(value,addr);
	csr_write_simple(value_3,addr+8);
	//printf("Writing value 0x%08lx at 0x%08lx\n\n",value, addr);
	printf("Writing value 0x%08lx for 0xf0021008\n\n",value_3);
	// Read from Peripheral IP reg
	// rvalue = csr_read_simple(addr);
	rvalue = csr_read_simple(addr+8);
	//printf("Reading value 0x%08lx at 0x%08lx\n\n",rvalue, addr);
	printf("Reading value 0x%08lx after writing\n\n",rvalue);
	
	// disable sim_trace
	//csr_write_simple(0,0x82000000);
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

	help();
	prompt();

	while(1) {
		console_service();
	}

	return 0;
}
