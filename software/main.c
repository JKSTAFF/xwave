/*
  2016年9月13日
  第二次改写




*/
#include "stdio.h"
#include "unistd.h"
#include "fcntl.h"
#include "sys/mman.h"
#include "hwlib.h"
#include "soc_cv_av/socal/socal.h"
#include "soc_cv_av/socal/hps.h"
#include "soc_cv_av/socal/alt_gpio.h"
#include "hps_0.h"
#include "indicator.h"

#define HW_REGS_BASE (ALT_STM_OFST)
#define HW_REGS_SPAN (0X04000000)
#define HW_REGS_MASK (HW_REGS_SPAN - 1)

#define read_reg_addr  0
#define write_reg_addr 0

int main(int argc, char **argv)
{
	void *virtual_base;
	void *virtual_data_addr;//数据地址
	void *virtual_led_addr;//LED指示灯地址
	void *virtual_button_addr;//按键地址
	//void *virtual_rst_addr;
	
	int fd;
	int vilue=0; //按键值
	int a_ctrl=5; //幅值控制
	double f_ctrl=100;//频率控制
	int w_ctrl=0;//相位控制
	int wave_ctrl=1;//波形控制
	int pw_ctrl=5;//脉宽控制
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
	
	//FPGA端地址映射
	virtual_data_addr = virtual_base +((unsigned long)(ALT_LWFPGASLVS_OFST+DDS_PIO_BASE)&(unsigned long)(HW_REGS_MASK));
	virtual_button_addr = virtual_base +((unsigned long)(ALT_LWFPGASLVS_OFST+KEY_PIO_BASE)&(unsigned long)(HW_REGS_MASK));
	virtual_led_addr = virtual_base +((unsigned long)(ALT_LWFPGASLVS_OFST+LED_PIO_BASE)&(unsigned long)(HW_REGS_MASK));
	//virtual_rst_addr = virtual_base +((unsigned long)(ALT_LWFPGASLVS_OFST+BUTTON_PIO_BASE)&(unsigned long)(HW_REGS_MASK));
	
	printf("triwave\n");
	printf("freq:%.2lfHz\n",10000/f_ctrl);
	printf("phas:%d\n",w_ctrl);
	printf("volt:%d\n",a_ctrl);
	*(uint32_t *) (virtual_led_addr + read_reg_addr) = 1;
	while(1)
	{
		vilue = (*(uint32_t *) (virtual_button_addr + write_reg_addr))&0x1fff;
		/******* 按键操作 *******/
	    if(vilue&0x1fff)
		{
			switch(vilue&0x1fff)
		{
			case 0x1000:{
				if(wave_ctrl < 4)
					wave_ctrl++;
				else
					wave_ctrl = 0;
				switch(wave_ctrl)
				{
					case 0:{
						*(uint32_t *) (virtual_led_addr + read_reg_addr) = 1;
						printf("sin wave\n");
						break;
					}
					case 1:{
						*(uint32_t *) (virtual_led_addr + read_reg_addr) = 2;
						printf("triwave\n");
						break;
					}
					case 2:{
						*(uint32_t *) (virtual_led_addr + read_reg_addr) = 4;
						printf("trawave\n");
						break;
					}
					case 3:{
						*(uint32_t *) (virtual_led_addr + read_reg_addr) = 8;
						printf("sawwave\n");
						break;
					}
					case 4:{
						*(uint32_t *) (virtual_led_addr + read_reg_addr) = 16;
						printf("squwave\n");
						break;
					}
					i=0;
				}
				break;
			}//波形选择 
			case 0x0800:{
				if(a_ctrl < 10)
					a_ctrl++;
				else
					a_ctrl = 10;
				printf("volt:%d\n",a_ctrl);
				break;
			}//幅值选择 +
			case 0x0400:{
				if(a_ctrl > 1)
					a_ctrl--;
				else
					a_ctrl = 1;
				printf("volt:%d\n",a_ctrl);
				break;
			}//幅值选择 -
			case 0x0200:{
				if(w_ctrl < 96)
					w_ctrl = w_ctrl + 4;
				else
					w_ctrl = w_ctrl - 96;
				printf("phas:%d\n",w_ctrl);
				break;
			}//相位选择 +
			case 0x0100:{
				if(w_ctrl > 3)
					w_ctrl = w_ctrl - 4;
				else
					w_ctrl = w_ctrl + 96;
				printf("phas:%d\n",w_ctrl);
				break;
			}//相位选择 -
			case 0x0080:{
				f_ctrl = 10000*f_ctrl/(10000+f_ctrl);
				printf("freq:%.2lfHz\n",10000/f_ctrl);
				// if(f_ctrl < 1)
					// flg = 1;
				// else
					// flg = 0;
				break;
			}//频率选择 1+
			case 0x0040:{
				f_ctrl = 10000*f_ctrl/(10000+10*f_ctrl);
				printf("freq:%.2lfHz\n",10000/f_ctrl);
				// if(f_ctrl < 1)
					// flg = 1;
				// else
					// flg = 0;
				break;
			}//频率选择 10+
			case 0x0020:{
				f_ctrl = 10000*f_ctrl/(10000+100*f_ctrl);
				printf("freq:%.2lfHz\n",10000/f_ctrl);
				// if(f_ctrl < 1)
					// flg = 1;
				// else
					// flg = 0;
				break;
			}//频率选择 100+
			case 0x0010:{
				f_ctrl = 10000*f_ctrl/(10000-f_ctrl);
				printf("freq:%.2lfHz\n",10000/f_ctrl);
				// if(f_ctrl < 1)
					// flg = 1;
				// else
					// flg = 0;
				break;
			}//频率选择 1-
			case 0x0008:{
				f_ctrl = 10000*f_ctrl/(10000-10*f_ctrl);
				printf("freq:%.2lfHz\n",10000/f_ctrl);
				// if(f_ctrl < 1)
					// flg = 1;
				// else
					// flg = 0;
				break;
			}//频率选择 10-
			case 0x0004:{
				f_ctrl = 10000*f_ctrl/(10000-100*f_ctrl);
				printf("freq:%.2lfHz\n",10000/f_ctrl);
				// if(f_ctrl < 1)
					// flg = 1;
				// else
					// flg = 0;
				break;
			}//频率选择 100-
			case 0x0002:{
				if(pw_ctrl < 9)
					pw_ctrl++;
				else 
					pw_ctrl = 9;
				if(wave_ctrl==4)
					printf("duty：%d\n",pw_ctrl);
				break;
			}//脉宽选择 +
			case 0x0001:{
				if(pw_ctrl > 1)
					pw_ctrl--;
				else
					pw_ctrl = 1;
				if(wave_ctrl==4)
					printf("duty：%d\n",pw_ctrl);
				break;
			}//脉宽选择 -
		}
		}			
	
	/********** 波形输出 *************/
		switch(wave_ctrl)
		{
			case 0:{
				*(uint32_t *) (virtual_data_addr + read_reg_addr) = (int)(( a_ctrl * sin_data[i+w_ctrl]/10) & 0x3ff);
				break;
			}//输出正弦波
			case 1:{
				// while(1)
				{
					*(uint32_t *) (virtual_data_addr + read_reg_addr) = (int)(( a_ctrl * tri_data[i+w_ctrl]/10) & 0x3ff);
					//usleep((int)f_ctrl);
					// if(i+w_ctrl < 99)
						// i++;
					// else
						// i=-w_ctrl;
				}
				break;
			}//输出三角波
			case 2:{
				*(uint32_t *) (virtual_data_addr + read_reg_addr) = (int)(( a_ctrl * tra_data[i+w_ctrl]/10) & 0x3ff);
				break;
			}//输出梯形波
			case 3:{
				*(uint32_t *) (virtual_data_addr + read_reg_addr) = (int)(( a_ctrl * saw_data[i+w_ctrl]/10) & 0x3ff);
				break;
			}//输出锯齿波
			case 4:{
				if((( i + w_ctrl)/10) > pw_ctrl)
					*(uint32_t *) (virtual_data_addr + read_reg_addr) = 0;
				else
					*(uint32_t *) (virtual_data_addr + read_reg_addr) = (int)(a_ctrl * 0x3ff/10);
				break;
			}//输出脉冲信号
			default :{
				*(uint32_t *) (virtual_data_addr + read_reg_addr) =(int) (a_ctrl * sin_data[i+w_ctrl]/10);
				break;
			} //默认输出正弦波
		}
		
		usleep((int)f_ctrl);
		
		if(i+w_ctrl < 99)
			i++;
		else
			i=-w_ctrl;
		
		/************ 频率优化 *****************/
		//1、方案一
		/********** 低频处理 *********/
		// if(flg==0 && i+w_ctrl < 99){
			// i++;
			// usleep((int)f_ctrl);
		// }
		// else if(flg==0 && i+w_ctrl >= 99){
			// i=-w_ctrl;
			// usleep((int)f_ctrl);
		// }
		
		// /*********** 高频处理 ********/
		// else if(flg==1 && i+w_ctrl < 90){
			// i = i + 10;
			// usleep((int)(f_ctrl*10);
		// }
		// else{
			// i = i - 90 - w_ctrl;
			// usleep((int)(f_ctrl*10);
		// }
		
		//方案二
		// switch(flg)
		// {
			// case 0:{
				// switch(i+w_ctrl < 99)
				// {
					// case 1:{
						// i++;
						// usleep((int)f_ctrl);
						// break;
					// }
					// case 0:{
						// i=-w_ctrl;
						// usleep((int)f_ctrl);
						// break;
					// }
				// }
			// }
			// case 1:{
				// switch(i+w_ctrl < 90)
				// {
					// case 1:{
						// i = i + 10;
						// usleep((int)(f_ctrl*10);
						// break;
					// }
					// case 0:{
						// i = i - 90 - w_ctrl;
						// usleep((int)(f_ctrl*10);
						// break;
					// }
				// }
			// }	
		// }
		
	}
	
	return (1);
}























