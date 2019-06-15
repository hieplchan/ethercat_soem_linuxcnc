/**
  ******************************************************************************
  * @file    soem_linuxcnc.c
  * @author  hiep.lchan@gmail.com
	* @date    7-January-2018
  * @brief   Embedded SOEM EtherCAT master to LinuxCNC project.
  *          This file contains SOEM function to init EtherCAT communication
  *          and transfer processdata through EtherCAT
  *          using to build LinuxCNC HAL userspace component.
	*/

#include "soem_linuxcnc.h"

/*------------EtherCAT communication init------------
  * @brief  Initialize EtherCAT using SOEM
  * @param  ifname: name of ethernet driver on linux using ifconfig
  * @retval None
  */
void init_ethercat(char *ifname)
{
  int oloop, iloop, chk;

  /* initialise SOEM, bind socket to ifname */
  if (ec_init(ifname))
  {
    //printf("ec_init card mang %s thanh cong.\n",ifname);
    /* find and auto-config slaves */
    if (ec_config_init(FALSE) > 0)
    {
      //printf("Da cau hinh cho %d slave.\n",ec_slavecount);

      /*Mapping data*/
      ec_config_map(&IOmap);
      ec_configdc();

      for (int i = 0; i <= ec_slavecount; i++)
      {
        //printf("Slave %d: %s \n", i, ec_slave[i].name);
        //printf("Input: %d bytes\n", ec_slave[i].Ibytes);
        //printf("Output: %d bytes\n", ec_slave[i].Obytes);
      }
      /* wait for all slaves to reach SAFE_OP state */
      ec_statecheck(0, EC_STATE_SAFE_OP, EC_TIMEOUTSTATE * 4);

      oloop = ec_slave[0].Obytes;
      if ((oloop == 0) && (ec_slave[0].Obits > 0)) oloop = 1;
      if (oloop > 8) oloop = 8;
      iloop = ec_slave[0].Ibytes;
      if ((iloop == 0) && (ec_slave[0].Ibits > 0)) iloop = 1;
      if (iloop > 8) iloop = 8;

      //printf("segments : %d : %d %d %d %d\n",ec_group[0].nsegments ,ec_group[0].IOsegment[0],ec_group[0].IOsegment[1],ec_group[0].IOsegment[2],ec_group[0].IOsegment[3]);

      ec_slave[0].state = EC_STATE_OPERATIONAL;
      /* send one valid process data to make outputs in slaves happy*/
      ec_send_processdata();
      ec_receive_processdata(EC_TIMEOUTRET);
      /* request OP state for all slaves */
      ec_writestate(0);
      /* wait for all slaves to reach OP state */
      do
      {
         ec_send_processdata();
         ec_receive_processdata(EC_TIMEOUTRET);
         ec_statecheck(0, EC_STATE_OPERATIONAL, 50000);
      }
      while (ec_slave[0].state != EC_STATE_OPERATIONAL);

    }
    else
    {
      //printf("Khong co slave nao, kiem tra lai\n");
    }
  }
  else
  {
    //printf("Khong co card mang %s\nChay sudo hoac kiem tra lai ifconfig\n",ifname);
  }
}

/*------------Send byte through EtherCAT------------
  * @brief  Send 1 byte to EtherCAT send bufer
  * @param  slave_no: Number of slave
  * @param  byte_no: Location of byte in frame, test using salveinfo
  * @param  value: Value of byte
  * @retval None
  */
void set_output_byte(uint16 slave_no, uint8 byte_no, uint8 value)
{
   uint8 *data_ptr;

   data_ptr = ec_slave[slave_no].outputs;
   /* Move pointer to correct module index*/
   data_ptr += byte_no;
   /* Read value byte by byte since all targets can't handle misaligned
 addresses
    */
   *data_ptr++ = value;
}

/*------------Send Unsigned Integer 16 bit through EtherCAT------------
  * @brief  Send 1 uint16 to EtherCAT send bufer
  * @param  slave_no: Number of slave
  * @param  byte_no: Location of uint16 in frame, test using salveinfo
  * @param  value: Value of uint16
  * @retval None
  */
void set_output_uint16(uint16 slave_no, uint8 byte_no, uint16 value)
{
   uint8 *data_ptr;

   data_ptr = ec_slave[slave_no].outputs;
   /* Move pointer to correct module index*/
   data_ptr += byte_no;
   /* Read value byte by byte since all targets can't handle misaligned
 addresses
    */
   *data_ptr++ = (value >> 0) & 0xFF;
   *data_ptr++ = (value >> 8) & 0xFF;
}

/*------------Send Integer 16 bit through EtherCAT------------
  * @brief  Send 1 int16 to EtherCAT send bufer
  * @param  slave_no: Number of slave
  * @param  byte_no: Location of int16 in frame, test using salveinfo
  * @param  value: Value of int16
  * @retval None
  */
void set_output_int16(uint16 slave_no, uint8 byte_no, int16 value)
{
   uint8 *data_ptr;

   data_ptr = ec_slave[slave_no].outputs;
   /* Move pointer to correct module index*/
   data_ptr += byte_no;
   /* Read value byte by byte since all targets can't handle misaligned
 addresses
    */
   *data_ptr++ = (value >> 0) & 0xFF;
   *data_ptr++ = (value >> 8) & 0xFF;
}

/*------------Receive byte through EtherCAT------------
  * @brief  Receive 1 byte of EtherCAT receive bufer
  * @param  slave_no: Number of slave
  * @param  byte_no: Location of byte in frame, test using salveinfo
  * @retval byte value
  */
uint8 get_input_byte(uint16 slave_no, uint8 byte_no)
{
  uint8 *u8;
  uint8 byte_in;

  u8 = (uint8*) (ec_slave[slave_no].inputs + byte_no);
  byte_in = *u8;

  return byte_in;
}

/*------------Receive Unsigned Integer 16 bit through EtherCAT------------
  * @brief  Receive 1 uint16 of EtherCAT receive bufer
  * @param  slave_no: Number of slave
  * @param  byte_no: Location of uint16 in frame, test using salveinfo
  * @retval uin16 value
  */
uint16 get_input_uint16(uint16 slave_no, uint8 byte_no)
{
  uint16 *u16;
  uint16 uint16_in;

  u16 = (uint16*) (ec_slave[slave_no].inputs + byte_no);
  uint16_in = *u16;

  return uint16_in;
}

/*------------Receive Integer 16 bit through EtherCAT------------
  * @brief  Receive 1 int16 of EtherCAT receive bufer
  * @param  slave_no: Number of slave
  * @param  byte_no: Location of int16 in frame, test using salveinfo
  * @retval in16 value
  */
int16 get_input_int16(uint16 slave_no, uint8 byte_no)
{
  int16 *u16;
  int16 int16_in;

  u16 = (int16*) (ec_slave[slave_no].inputs + byte_no);
  int16_in = *u16;

  return int16_in;
}

/*------------Receive Process Data through EtherCAT------------
  * @brief  Receive all process data of 1  slave
  * @retval Ethercat_actual_value
  */
struct Ethercat_actual_value get_process_value(void)
{
  struct Ethercat_actual_value get_data;

  get_data.button =  get_input_byte(0, 0x2A);

  for (int i = 0; i < 7; i++)
  {
    //Get Position
    get_data.actual_position[i] =  get_input_int16(0, 0x00 + i*2);
    //Get Velocity
    get_data.actual_velocity[i] =  get_input_int16(0, 0x0E + i*2);
    //Get Torque
    get_data.actual_torque[i] =  get_input_int16(0, 0x1C + i*2);
  }

  return get_data;
}

/*------------Send Process Data through EtherCAT------------
  * @brief  Send all process data of 1  slave
  * @param  set_value: Value of send buffer
  * @retval None
  */
void set_process_value(struct Ethercat_set_value set_value)
{
  //Set LED
  set_output_byte(0, 0x2A, set_value.LED);

  for (int i = 0; i < 7; i++)
  {
    //Set Position
    set_output_int16(0, 0x00 + i*2, set_value.set_position[i]);
    //Set Velocity
    set_output_int16(0, 0x0E + i*2, set_value.set_velocity[i]);
    //Set Torque
    set_output_int16(0, 0x1C + i*2, set_value.set_torque[i]);
  }
}

/*------------Send and Receive Process Data through EtherCAT------------
  * @brief  Send and receive all process data of 1  slave
  * @param  set_value: Value of send buffer
  * @retval Ethercat_actual_value
  */
struct Ethercat_actual_value ethecat_process_data(struct Ethercat_set_value set_value)
{
  struct Ethercat_actual_value get_data;
  /*-----SET OUTPUT-----*/
  //Set LED
  set_output_byte(0, 0x2A, set_value.LED);
  for (int i = 0; i < 7; i++)
  {
    //Set Position
    set_output_int16(0, 0x00 + i*2, set_value.set_position[i]);
    //Set Velocity
    set_output_int16(0, 0x0E + i*2, set_value.set_velocity[i]);
    //Set Torque
    set_output_int16(0, 0x1C + i*2, set_value.set_torque[i]);
  }

  /*-----SEND - RECEIVE PROCESS DATA-----*/
  ec_send_processdata();
  ec_receive_processdata(EC_TIMEOUTRET);

  /*-----GET INPUT-----*/
  get_data.button =  get_input_byte(0, 0x2A);
  for (int i = 0; i < 7; i++)
  {
    //Get Position
    get_data.actual_position[i] =  get_input_int16(0, 0x00 + i*2);
    //Get Velocity
    get_data.actual_velocity[i] =  get_input_int16(0, 0x0E + i*2);
    //Get Torque
    get_data.actual_torque[i] =  get_input_int16(0, 0x1C + i*2);
  }
  return get_data;
}
