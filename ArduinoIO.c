/*
The "Arduino Communication Library" is Under GNU Lesser GPL
LIbrary that Allows you to make your programm communicate With the Arduino
Copywright (C): Dimitrios Desyllas (pc_magas)
 
This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/
#include"ArduinoIO.h"
#include<stdio.h>
/** With this function we begin the communication with the Arduino
*If any error occured will return -1
*@see int communication_begin(const char device[],unsigned int speed)
*@param device = the path to the file that indicates the arduino px /dev/ttyACM3
*@param speed = The speed rate of  arduino sends Data
*
*/

int communication_begin(const char device[],unsigned int speed)
{
	struct termios options; //Options for communication with the arduino
	speed_t speeds;	//Special type for setting the speed
	int dev;//thew device id that has arduino
	
	if((dev=open(device, O_RDWR | O_NOCTTY | O_NDELAY))==-1)
	/*
	*In unix like systems  like linux each device have a file in /dev so arduino has on
	*But we cannot just write and read on it we need a special way to read and write :S
	*/
	{
		fprintf(stderr,"Error with communicating with the device\n");
		return -1;
	}
	
	if(tcgetattr(dev,&options)<0)
	{
		fprintf(stderr,"Error with the atributes\n");
		return -1;
	}

	/*Setting the speed of communication*/
	switch(speed) 
	{
   	 	case 4800:   speeds=B4800;   break;
		
  		case 9600:   speeds=B9600;   break;
		
		#ifdef B14400
  		case 14400:  speeds=B14400;  break;
		#endif
		
    		case 19200:  speeds=B19200;  break;
		
		#ifdef B28800
  		case 28800:  speeds=B28800;  break;
		#endif
		
   		case 38400:  speeds=B38400;  break;
		
  		case 57600:  speeds=B57600;  break;
		
   		case 115200: speeds=B115200; break;
		
		default: speeds=speed;
  	}
	
	cfsetispeed(&options,speeds);//Here we set the input speed
	cfsetospeed(&options,speeds);//Here we set the output speed
	
	/* Setting communication Options*/

	options.c_cflag &= ~CRTSCTS;//Turn off flow control. As Arduino does not support any flow control
	
	/*Turn on  Receiver and the communication line belongs to us
	*more info: http://www.easysw.com/~mike/serial/serial.html
	*/
	options.c_cflag |= CREAD | CLOCAL;
	
	/*INPUT OUTPUT ATTRINUTES*/
	options.c_iflag &= ~(IXON | IXOFF | IXANY); //Arduino Does not support input flow control
	options.c_oflag &= ~OPOST; //We just send raw data

	/*When communicateing with arduino we DO NOT want to:
		(*)Process any related software singnal can ocuur
		(*)Repeat the input including erasing characters like BS,SP etc.
		(*)Just send a bunch of bits whithout any proccesion on it
	*/
	options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
	
	/*Setting the timing Options*/
	/*more info: http://unixwiz.net/techtips/termios-vmin-vtime.html*/
	options.c_cc[VMIN]  = 0;//No minimun data requirements to satisfy the input
	options.c_cc[VTIME] = 20;//Read expire every 0.2 seconds ( If u r Greek 0,2 deuterolepta)
	
	/* Checking if the attributes se set before can be accepted*/
	if( tcsetattr(dev,TCSANOW,&options)<0)
	{
		fprintf(stderr,"Error with the attributes of initialization\n");
		return -1;
	}
	return dev;
}

/**
* With this function I end the Communication with the arduino
*@see void communication_end(int id)
* @param id = the device Id we got from communication_begin()
*/
void communication_end(int id)
{
	close(id);//Just close it!
}

/**
*With this function we send data to the arduino
*@param devid = the device if from the communication_begin()
*@param data = pointer to void (means any type) 
*@ param size the size in bytes
*/
int arduino_send_data(int devid,void * data,size_t size)
{
	return write(devid,data,size);
}

/**
*With this function we recieve any type of data
*@param devid = the device if from the communication_begin()
*@param size  = the ammount of data in bytes we want to recieve
*/
void * arduino_rcv_data(int devid, size_t size)
{
	void * data=malloc(size);
	int error;
	if(data==NULL)
	{
		return NULL;
	}
	error=read(devid,data,size);
	if(error ==-1)
	{
		free(data);
		return NULL;
	}
	return data;
}

/** 
*With this we send a character
*
*@param devid = the device if from the communication_begin()
*@param character = The Character we want to send
*
*/
int arduino_send_char(int devid,char character)
{
	return arduino_send_data(devid,(void *)&character,sizeof(char));
}

/**
*With this we receive a character
*@param devid = the device if from the communication_begin()
*@param ok = if we recieved or not the data
*/
char arduino_rcv_char(int devid)
{
	char *data;
	data = (char *)arduino_rcv_data(devid , sizeof(char));
	if(data == NULL)
	{
		return -1;
	}
	return *data;
}

/**
*With this send a string to the arduino
*
*@param devid = the device if from the communication_begin()
*@param string = the string we need to send to the arduino
*/
int arduino_send_string(int devid,char string[])
{
	return write(devid,string,sizeof(string));
}

/**
*With this we recieve a string from the Arduino
*If any error occured will return null elseit will return the string we asked for
*
*@param devid = the device if from the communication_begin()
*@param length = the size we want to 
*/
char * arduino_rcv_string(int devid,int length)
{
	char *string=malloc(length*sizeof(char));
	int error;
	if(string == NULL)
	{
		return NULL;
	}
	error=read(devid,string,length);
	if(error == -1)
	{
		free(string);
		return NULL;
	}
	return string;
}


