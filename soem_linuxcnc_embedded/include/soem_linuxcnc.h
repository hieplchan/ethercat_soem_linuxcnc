/**
  ******************************************************************************
  * @file    soem_linuxcnc.h
  * @author  hiep.lchan@gmail.com
	* @date    7-January-2018
  * @brief   Embedded SOEM EtherCAT master to LinuxCNC project.
  *          This file contains SOEM funtion header for using EtherCAT
	*/

#include <stdio.h>
#include <string.h>
#include <inttypes.h>

#include "ethercat.h"

char IOmap[4096];
int wkc;

/*-----ETHERCAT VARIABLE-----*/
struct Ethercat_set_value
{
  uint8 LED;
  int16 set_position[7];
  int16 set_velocity[7];
  int16 set_torque[7];
};

struct Ethercat_actual_value
{
  uint8 button;
  int16 actual_position[7];
  int16 actual_velocity[7];
  int16 actual_torque[7];
};

/*------------EtherCAT communication init------------
  * @brief  Initialize EtherCAT using SOEM
  * @param  ifname: name of ethernet driver on linux using ifconfig
  * @retval None
  */
void init_ethercat(char *ifname);

/*------------Send byte through EtherCAT------------
  * @brief  Send 1 byte to EtherCAT send bufer
  * @param  slave_no: Number of slave
  * @param  byte_no: Location of byte in frame, test using salveinfo
  * @param  value: Value of byte
  * @retval None
  */
void set_output_byte(uint16 slave_no, uint8 byte_no, uint8 value);

/*------------Send Unsigned Integer 16 bit through EtherCAT------------
  * @brief  Send 1 uint16 to EtherCAT send bufer
  * @param  slave_no: Number of slave
  * @param  byte_no: Location of uint16 in frame, test using salveinfo
  * @param  value: Value of uint16
  * @retval None
  */
void set_output_uint16(uint16 slave_no, uint8 byte_no, uint16 value);

/*------------Send Integer 16 bit through EtherCAT------------
  * @brief  Send 1 int16 to EtherCAT send bufer
  * @param  slave_no: Number of slave
  * @param  byte_no: Location of int16 in frame, test using salveinfo
  * @param  value: Value of int16
  * @retval None
  */
void set_output_int16(uint16 slave_no, uint8 byte_no, int16 value);

/*------------Receive byte through EtherCAT------------
  * @brief  Receive 1 byte of EtherCAT receive bufer
  * @param  slave_no: Number of slave
  * @param  byte_no: Location of byte in frame, test using salveinfo
  * @retval byte value
  */
uint8 get_input_byte(uint16 slave_no, uint8 byte_no);

/*------------Receive Unsigned Integer 16 bit through EtherCAT------------
  * @brief  Receive 1 uint16 of EtherCAT receive bufer
  * @param  slave_no: Number of slave
  * @param  byte_no: Location of uint16 in frame, test using salveinfo
  * @retval uin16 value
  */
uint16 get_input_uint16(uint16 slave_no, uint8 byte_no);

/*------------Receive Integer 16 bit through EtherCAT------------
  * @brief  Receive 1 int16 of EtherCAT receive bufer
  * @param  slave_no: Number of slave
  * @param  byte_no: Location of int16 in frame, test using salveinfo
  * @retval in16 value
  */
int16 get_input_int16(uint16 slave_no, uint8 byte_no);

/*------------Receive Process Data through EtherCAT------------
  * @brief  Receive all process data of 1  slave
  * @retval Ethercat_actual_value
  */
struct Ethercat_actual_value get_process_value(void);

/*------------Send Process Data through EtherCAT------------
  * @brief  Send all process data of 1  slave
  * @param  set_value: Value of send buffer
  * @retval None
  */
void set_process_value(struct Ethercat_set_value set_value);

/*------------Send and Receive Process Data through EtherCAT------------
  * @brief  Send and receive all process data of 1  slave
  * @param  set_value: Value of send buffer
  * @retval Ethercat_actual_value
  */
struct Ethercat_actual_value ethecat_process_data(struct Ethercat_set_value set_value);
