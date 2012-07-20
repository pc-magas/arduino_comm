/*
The "Arduino Communication Library" is Under GNU Lesser GPL
LIbrary that Allows you to make your programm communicate With the Arduino
Copywright (C): 2011 Dimitrios Desyllas (pc_magas)
 
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

/**
\file Arduino.h
\brief Pc_maga's Arduino communication Libraries

The purspose of these Librareis are to Make a Communication sing the Seriela to USB Interface of the Arduino
The Liabraries Are Under GNU LESSER GPL v3: http://www.gnu.org/licenses/lgpl.html


*/
#ifndef ARDUINO_
#define ARDUINO_
#include <termios.h>
#include <unistd.h>   /* UNIX standard function definitions */
#include <stdlib.h>
#include <sys/types.h>
#include <fcntl.h>

/**
\fn int communication_begin(const char device[],unsigned int speed)

\brief With this function we initialise and begin the communication with the Arduino.
\warning returns -1 if error occured
\param device = The /dev/* path to the device that is connected the Arduino ex. /dev/tty/ACM3
\param speed =  The Baud rate that communications is set up
*/
int communication_begin(const char device[],unsigned int speed);/*With this we initialise and begin the communication with the Arduino*/

/**
\fn void communication_end(int id)
\brief This ends the communication with the Arduino
\param id = the result that returns the communication_begin()
*/

void communication_end(int id);//This ends the communication with the arduino

/**
\fnint int arduino_send_data(int devid,void * data,size_t size);
\brief With this function we send data to the arduino
\warfning returns what return and the write() function of the posix
\param devid = the device if from the communication_begin()
\param data = pointer to void (means any type) 
\param size the size in bytes
*/
int arduino_send_data(int devid,void * data,size_t size);

/**
\fn void * arduino_rcv_data(int devid,size_t size)
\brief With this function we recieve any type of data
\warning It may retyrn null if any error occured

@param devid = the device if from the communication_begin()
@param size  = the ammount of data in bytes we want to recieve

*/
void * arduino_rcv_data(int devid,size_t size);

/** 
\fn int arduino_send_char(int devid,char character)
\breif With this we send a character

\param devid = the device if from the communication_begin()
\param character = The Character we want to send

*/
int arduino_send_char(int devid,char character);

/**
\fn char arduino_rcv_char(int devid)
\breif With this we receive a character
\warning If any error occured return -1 else return the character we asked for

\param devid = the device if from the communication_begin()
\param ok = if we recieved or not the data
*/
char arduino_rcv_char(int devid);

/**
\fn int arduino_send_string(int devid,char string[])
\brief With this send a string to the arduino
\warning Return whatever returns write() funtion of the posix

\param devid = the device if from the communication_begin()
\param string = the string we need to send to the arduino
*/
int arduino_send_string(int devid,char string[]);

/**
\fn char * arduino_rcv_string(int devid,int length)
\breif With this we recieve a string from the Arduino
\warning If any error occured will return null else it will return the string we asked for

\param devid = the device if from the communication_begin()
\param length = the size we want to 
*/
char * arduino_rcv_string(int devid,int length);//with this we reicieve a string of a specific length
#endif
