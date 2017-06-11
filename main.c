#include "stdio.h"
#include "unistd.h"
#include "fcntl.h"
#include "sys/mman.h"
#include "hwlib.h"
#include "socal/socal.h"
#include "socal/hps.h"
#include "socal/alt_gpio.h"
#include "hps_0.h"
#include "indicator.h"

#define HW_REGS_BASE (ALT_STM_OFST)
#define HW_REGS_SPAN (0X04000000)
#define HW_REGS_MASK (HW_REGS_SPAN - 1)

#define read_reg_addr  0
#define write_reg_addr 32

int main(int argc, char **argv)
{
	void *virtual_base;
	void *virtual_pio_pin_addr;
	void *virtual_pio_led_addr;
	void *virtual_pio_button_addr;
	int fd;
	int vilue=0;
	int a_ctrl=5;
	int f_ctrl=20;
	int w_ctrl=0;
	int wave_ctrl=4;
	int pw_ctrl=5;
	int i=0;
	
	if( ( fd = open("/dev/mem",(O_RDWR | O_SYNC))) == -1)
	{
		printf("ERROR:could not open \"/dev/mem\"...\n");
		return (1);
	}
	
	virtual_base = mmap(NULL,HW_REGS_SPAN,(PROT_READ | PROT_WRITE),MAP_SHARED,fd,HW_REGS_BASE);
	
	if(virtual_base == MAP_FAILED)
	{
		printf("ERROR:mmap() failed...\n");
		close (fd);
		return (1);
	}
	
	virtual_pio_pin_addr = virtual_base +((unsigned long)(ALT_LWFPGASLVS_OFST+LED_PIO_BASE)&(unsigned long)(HW_REGS_MASK));//硬件地址映射
	virtual_pio_led_addr = virtual_base +((unsigned long)(ALT_LWFPGASLVS_OFST+DDS_PIO_BASE)&(unsigned long)(HW_REGS_MASK));
	virtual_pio_button_addr = virtual_base +((unsigned long)(ALT_LWFPGASLVS_OFST+BUTTON_PIO_BASE)&(unsigned long)(HW_REGS_MASK));
	
	while ( 1 )
	{
		// *(uint32_t *) (virtual_pio_pin_addr + read_reg_addr)=0x3ff;
		// *(uint32_t *) (virtual_pio_led_addr + read_reg_addr)=0x00;
		// usleep(500*1000);
		// *(uint32_t *) (virtual_pio_pin_addr + read_reg_addr)=0x00;
		// *(uint32_t *) (virtual_pio_led_addr + read_reg_addr)=0x1f;
		// usleep(500*1000);
		
	   //vilue = *(uint32_t *) (virtual_pio_pin_addr + write_reg_addr);
		// if( vilue & 0x7ffc00)
		// {
			// switch(vilue & 0x7ffc00)
			// /***** 波形选择 *****/
			// case 0x400000:{
				// if( wave_ctrl < 4)
					// wave_ctrl++;
				// else
					// wave_ctrl = 0;
			// }
			// /***** 幅值选择 *****/
			// case 0x200000:{
				// if( a_ctrl < 10)
					// a_ctrl++;
				// else
					// a_ctrl = 10;
			// }
			// case 0x100000:{
				// if( a_ctrl > 1)
					// a_ctrl--;
				// else
					// a_ctrl= 1;
			// }
			// /****** 相位选择 ****/
			// case 0x080000:{
				// if( w_ctrl < 93)
					// w_ctrl += 6;
				// else
					// w_ctrl = w_ctrl -93;
			// }
			// case 0x040000:{
				// if( w_ctrl >= 6)
					// w_ctrl -= 6;
				// else
					// w_ctrl = w_ctrl + 93;
			// }
			// /****** 频率选择 *****/
			// case 0x020000:{               //+20Hz
				// if( f_ctrl > 0)
					// f_ctrl = 1000000*f_ctrl/(100*20*f_ctrl+1000000);
				// else
					// f_ctrl = 5000;
			// }
			// case 0x010000:{               //+200Hz
				// if( f_ctrl > 0)
					// f_ctrl = 1000000*f_ctrl/(100*200*f_ctrl+1000000);
				// else
					// f_ctrl = 500;
			// }
			// case 0x008000:{               //+1Hz
				// if( f_ctrl > 0)
					// f_ctrl = 1000000*f_ctrl/(100*1*f_ctrl+1000000);
				// else
					// f_ctrl = 100000;
			// }
			// case 0x004000:{
				// if( f_ctrl > 0)
					// f_ctrl = 1000000*f_ctrl/(1000000-100*20*f_ctrl)
				// else
					// f_ctrl = 5000;
			// }
			// case 0x002000:{
				// if(f_ctrl > 0)
					// f_ctrl = 1000000*f_ctrl/(1000000-100*200*f_ctrl)
				// else
					// f_ctrl = 500;
			// }
			// case 0x001000:{
				// if( f_ctrl > 0)
					// f_ctrl = 1000000*f_ctrl/(1000000-100*f_ctrl)
				// else
					// f_ctrl = 100000;
			// }
			// /******* 脉宽选择 ******/
			// case 0x000800:{
				// if(pw_ctrl < 9)
					// pw_ctrl++;
				// else
					// pw_ctrl = 9;
			// }
			// case 0x000400:{
				// if(pw_ctrl > 1)
					// pw_ctrl--;
				// else
					// pw_ctrl = 1;
			// }
		// }
		
		vilue = *(uint32_t *) (virtual_pio_button_addr + write_reg_addr);
		switch(vilue&0x0f)
		{
			case 0x01:
			{
			     if(pw_ctrl < 9)
					 pw_ctrl++;
				 else
					 pw_ctrl = 9;
				 break;
			}
			case 0x02:
			{
				if(pw_ctrl > 1)
					 pw_ctrl--;
				 else
					 pw_ctrl = 1;
				 break;
			}
			// case 0x04:
			// case 0x08:
			// defaule　:
			
		}
		
		
		
		switch(wave_ctrl)
		{
			case 0:{
				*(uint32_t *) (virtual_pio_pin_addr + read_reg_addr) = ( a_ctrl * sin_data[i+w_ctrl]/10) & 0x3ff;
				break;
			}
			case 1:{
				*(uint32_t *) (virtual_pio_pin_addr + read_reg_addr) = ( a_ctrl * tri_data[i+w_ctrl]/10) & 0x3ff;
				break;
			}
			case 2:{
				*(uint32_t *) (virtual_pio_pin_addr + read_reg_addr) = ( a_ctrl * tra_data[i+w_ctrl]/10) & 0x3ff;
				break;
			}
			case 3:{
				*(uint32_t *) (virtual_pio_pin_addr + read_reg_addr) = ( a_ctrl * saw_data[i+w_ctrl]/10) & 0x3ff;
				break;
			}
			case 4:{
				if((( i + w_ctrl)/100) < pw_ctrl)
					*(uint32_t *) (virtual_pio_pin_addr + read_reg_addr) = 0;
				else
					*(uint32_t *) (virtual_pio_pin_addr + read_reg_addr) = a_ctrl * 0x3ff/10;
				break;
			}
			default :{
				*(uint32_t *) (virtual_pio_pin_addr + read_reg_addr) = a_ctrl * sin_data[i+w_ctrl]/10;
				break;
			} 
		}
		usleep(f_ctrl);
		i++;
		
	}
	
	return (1);
}























