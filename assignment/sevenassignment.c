
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <sys/mman.h>

#include "gpiolib.c"




#define LED_NUM 8
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

int ledSW_Control(int gpio,  int onOff)
{
 	int fd;
 	char buf[128];
	char inCh;

	gpioExport(gpio);

	gpioDirection(gpio, 0) ; // "in"

	while(1) {
  		ledControl(21, 0);
		while(gpioRead(gpio) == 0) 
  			ledControl(21, 1);
	}

	gpioUnexport(gpio);

	return 0;
}

int pwmControl(int gpio,  int dutyRate, int dutyT)
{
 	int fd, cnt, dutyOn, dutyOff;
 	char buf[128];
	char inCh;

	printf("PWM duty(%d, %d) \n", dutyOn, dutyOff);

	gpioExport(gpio);

	gpioDirection(gpio, 1) ; // "out"

	cnt = 10;
	dutyOn = (dutyT*dutyRate/100)*1000 ; //nano unit
	dutyOff = (dutyT*(100-dutyRate)/100)*1000 ; //nano unit
	while(cnt--) {
  		ledControl(21, 1);
		usleep(dutyOn);
  		ledControl(21, 0);
		usleep(dutyOff);
	}

	gpioUnexport(gpio);

	//printf("PWM END !!!");

	return 0;
}

/* Function to play a note on the buzzer */
// note : freq(=W), T = 1/W 1초에 몇번떠는데 그것을 역수 주파수분의 1
// duration : time in msec
// period = 10**6 / note = T (in usec) 주기

void display_NUM(int num) {
	int i;
	
	for(i = 0; i<LED_NUM; i++){
		gpioWrite(seg_pin[i], digit[num][i]);
	}
		usleep(1000000);

  }
  void dispOnOff(void)
  {
	  int i,j;
	  
	  for(i = 0; i< LED_NUM-2;i++){

			gpioWrite(seg_pin[i],1);
			usleep(250000);

			gpioWrite(seg_pin[i],0);
		}

	}
		  

int main(void) {
	// gpio_set_value(BUZZER_PIN, 0); /* set the pin mode to output and turn off the buzzer */
	// usleep(1000000); /* pause for 1 second before playing the melody */
//{0,1,0,0,0,1,1,1}
//{5,6,13,19,21,20,16,26};
	int i,j;
	char ch;
	
	for (i=0;i <LED_NUM; i++)
	{
		gpioExport(seg_pin[i]);
		gpioDirection(seg_pin[i],1);
	}
	while(1){
		ch = getchar();
		if(ch == '0'){
			display_NUM(0);
			fflush(stdin);
		}
		else if(ch == '1'){
			display_NUM(1);
			fflush(stdin);
		}
		else if(ch == '2'){
			display_NUM(2);
			fflush(stdin);
		}
		else if(ch == '3'){
			display_NUM(3);
			fflush(stdin);
		}
		else if(ch == '4'){
			display_NUM(4);
			fflush(stdin);
		}
		else if(ch == '5'){
			display_NUM(5);
			fflush(stdin);
		}
		else if(ch == '6'){
			display_NUM(6);
			fflush(stdin);
		}
		else if(ch == '7'){
			display_NUM(7);
			fflush(stdin);
		}
		else if(ch == '8'){
			display_NUM(8);
			fflush(stdin);
		}
		else if(ch == '9'){
			display_NUM(9);
			fflush(stdin);
		}
		else if(ch == ' '){
			usleep(100000);
			fflush(stdin);
		}
		else if(ch == 'x'){
			for(i = 0; i< LED_NUM;i++){
				
				gpioWrite(seg_pin[i],0);
				
			}
			printf("END!!\n");
			break;
		}
		

		//usleep(100000); /* pause between notes to avoid overlapping */
  }

	
	/*while(1) //초의 일의 자리를 a를 입력하면 나오게하는 코드
	{
		dispOnOff();
		
		time_t current_time = time(NULL);
		struct tm *time_info = localtime(&current_time);
		int seconds = time_info->tm_sec;
		
		if(getchar() == 'a')
		{
			display_NUM((seconds % 10));
			break;
		}
		else
			continue;
	}*/
	for (i=0;i <LED_NUM; i++)
	{
		gpioUnexport(seg_pin[i]);
	}
	return 0;
}
	

//////////////////////////////////////////////////////
/* 
// Usage : ./ledgpio 21 1/0
int main(int argc, char **argv) 
{
 	int gno;
	int onOff, vol;
	char ch;

	printf("Start PWM !!!\n");

	vol = 100; // Duty Rate in Percent %
	while(vol) {
		pwmControl(21, vol, 33);
		vol = vol - 5;
	}

 	return 0;
}

 	if(argc != 3){
		printf("Usage: %s GPIO_NO\n", argv[0]);
		return -1;
 	}
 
 	gno = atoi(argv[1]);
 	onOff = atoi(argv[2]);

	ledSW_Control(20, 0);
*/
