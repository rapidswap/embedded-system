#include <stdio.h>

#include <stdlib.h>

#include <string.h>

#include <fcntl.h>

#include <unistd.h>

#include <sys/mman.h>

#include <linux/i2c-dev.h>

#include <sys/ioctl.h>

#include "gpiolib.c"




#define LED_NUM 8
#define BH1750_ADDR 0x23 // Replace with the actual I2C address of the sensor
int seg_pin[] = {26, 16, 20, 19, 13, 06, 05, 21};
				  // A B C D E F G DP
int digit[10][8] = {{1, 1, 1, 1, 1, 1, 0, 0}, //0
					{0, 1, 1, 0, 0, 0, 0, 0}, //1
					{1, 1, 0, 1, 1, 0, 1, 0}, //2
					{1, 1, 1, 1, 0, 0, 1, 0}, //3
					{0, 1, 1, 0, 0, 1, 1, 0}, //4
					{1, 0, 1, 1, 0, 1, 1, 0}, //5
					{1, 0, 1, 1, 1, 1, 1, 0}, //6
					{1, 1, 1, 0, 0, 1, 0, 0}, //7
					{1, 1, 1, 1, 1, 1, 1, 0}, //8
					{1, 1, 1, 1, 0, 1, 1, 0}}; //9
int ledControl(int gpio,  int onOff)
{
 	int fd;
 	char buf[128];

	gpioExport(gpio);

	gpioDirection(gpio, 1) ; // "out"

	gpioWrite(gpio, onOff);

	gpioUnexport(gpio);

	return 0;
}

void display_NUM(int num) {
	int i;
	
	for(i = 0; i<LED_NUM; i++){
		gpioWrite(seg_pin[i], digit[num][i]);
	}
		usleep(120000);

  }
  
  void dispOnOff(void)
  {
	  int i,j;
	  
	  for(i = 0; i< LED_NUM;i++){

			gpioWrite(seg_pin[i],1);
			sleep(1);

			gpioWrite(seg_pin[i],0);
		}

	}
		 

int main()

{

    int fd;

        int     luxRaw;

        float   lux;

    unsigned char buf[3] = {0};



    if ((fd = open("/dev/i2c-1", O_RDWR)) < 0) { // Open the I2C device file

        perror("open");

        exit(1);

    }



    if (ioctl(fd, I2C_SLAVE, BH1750_ADDR) < 0) { // Set the I2C slave address

        perror("ioctl");

        exit(1);

    }





    // Send measurement request to the sensor

    //buf[0] = 0x01; // Power On

    //buf[0] = 0x07; // Reset
 for (int i=0;i <LED_NUM; i++)
	{
		gpioExport(seg_pin[i]);
		gpioDirection(seg_pin[i],1);
	}
	
while(1){
    buf[0] = 0x23; // One time L-Resolution measurement

    if (write(fd, buf, 1) != 1) {

        perror("write");

        exit(1);

    }



    // Wait for measurement to be ready (typically takes 120ms)

    usleep(120000);



    // Read the measurement value from the sensor

    if (read(fd, buf, 2) != 2) {

        perror("read");

        exit(1);

    }



    // Convert the measurement value to lux

    luxRaw = (buf[0] << 8) | buf[1];

    lux = (float)luxRaw / 1.2; // Divide by 1.2 to get the actual lux value



    printf("Lux: %d(%7.3f) :: %x, %x\n", luxRaw, lux, buf[0], buf[1]);
    
    if(20>lux)
    {
		display_NUM(0);
	}
	else if(40 > lux && lux > 20)
    {
		display_NUM(1);
	} 
	else if(60 > lux && lux > 40)
    {
		display_NUM(2);
	} 
	else if(80 > lux && lux > 60)
    {
		display_NUM(3);
	} 
	else if(100 > lux && lux > 80)
    {
		display_NUM(4);
	} 
	else if(120 > lux && lux > 100)
    {
		display_NUM(5);
	} 
	else if(140 > lux && lux > 120)
    {
		display_NUM(6);
	} 
	else if(160 > lux && lux > 140)
    {
		display_NUM(7);
	} 
	else if(lux > 160)
    {
		display_NUM(8);
	} 
	 
    if(lux>200)// lux < 50 led On
    {
		ledControl(18,1);
		ledControl(15,0);
		ledControl(14,0);
		printf("BLUE\n");
	}
	else if (lux < 200 && lux >= 100){ //lux >51 led Off
		ledControl(15,1);
		ledControl(14,0);
		ledControl(18,0);
		printf("GREEN\n");
	}
	else if (lux < 100){
		ledControl(14,1);
		ledControl(15,0);
		ledControl(18,0);

		printf("RED\n");
	}

}
for (int i=0;i <LED_NUM; i++)
	{
		gpioUnexport(seg_pin[i]);
	}
    close(fd);



    return 0;

}

