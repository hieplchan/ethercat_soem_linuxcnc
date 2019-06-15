/**
  ******************************************************************************
  * @file    test.c
  * @author  hiep.lchan@gmail.com
	* @date    7-January-2018
  * @brief   Embedded SOEM EtherCAT master to LinuxCNC project.
  *          This file contains use for testing soem_linuxcnc.so
	*/

#include "soem_linuxcnc.h"

/*------------Test EtherCAT Communication------------
  * @brief  Test SOEM EtherCAT master
  * @retval None
  */
int main (void)
{
  struct Ethercat_actual_value actual_value;
  struct Ethercat_set_value set_value;
  boolean led1, led2;

  printf("SOEM (Simple Open EtherCAT Master)\n-----XMC4300-----\n");
  init_ethercat("enp4s0f2");

  led1 = 0;
  led2 = 0;

  set_value.LED = (uint8)(led1<<1) + (uint8)led2;
  for (int i = 0; i < 7; i++)
  {
    set_value.set_position[i] = (int16) -10.55 + i;
    set_value.set_velocity[i] = (int16) -20.55 + i;
    set_value.set_torque[i] = (int16) -30.55 + i;
  }

  while (1)
  {
    actual_value = ethecat_process_data(set_value);
    printf("Button: %d\n", actual_value.button);
    printf("POS1: %d\n", actual_value.actual_position[0]);
    printf("VEL1: %d\n", actual_value.actual_velocity[0]);
    printf("TORQUE: %d\n", actual_value.actual_torque[0]);

    printf("-------\n");
   }
}
