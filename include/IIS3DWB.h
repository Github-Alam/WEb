/*
  ******************************************************************************
  * @file    iis3dwb_reg.h
  * @author  Nirvik Adhikary & Rajiv Ranjan
  * @Date    06/07/2022
  * @brief   This file contains all the functions prototypes for the
  *          iis3dwb_reg.c driver.
  ******************************************************************************
  */

#ifndef IIS3DWB_h
#define IIS3DWB_h
/* Includes ------------------------------------------------------------------*/
#include "Arduino.h"
#include <SPI.h>
#include "iis3dwb_reg.h"

class IIS3DWB
{
  public:
  IIS3DWB(uint8_t cspin);
  void    IIS3DWB_init();
  void    offsetBias(float * accelBias);
  int32_t auto_scale();
  uint16_t max(uint16_t * value,uint8_t size);
  float_t iis3dwb_from_fs2g_to_mg(int16_t lsb);
  float_t iis3dwb_from_fs4g_to_mg(int16_t lsb);
  float_t iis3dwb_from_fs8g_to_mg(int16_t lsb);
  float_t iis3dwb_from_fs16g_to_mg(int16_t lsb);
  float_t iis3dwb_from_lsb_to_celsius(int16_t lsb);
  int32_t iis3dwb_reset_set( uint8_t val);
  int32_t iis3dwb_device_id_get();
  int32_t iis3dwb_device_id_check(uint32_t timeout,uint8_t *flag);
  int32_t iis3dwb_block_data_update_set(uint8_t val);
  int32_t iis3dwb_xl_data_rate_set(iis3dwb_odr_xl_t val);
  int32_t iis3dwb_xl_axis_selection_set(iis3dwb_xl_axis_sel_t val);
  int32_t iis3dwb_xl_full_scale_set(iis3dwb_fs_xl_t val);
  int32_t iis3dwb_xl_full_scale_get(iis3dwb_fs_xl_t *val,uint8_t* _aes);
  int32_t iis3dwb_xl_flag_data_ready_get(uint8_t *val);
  int32_t iis3dwb_acceleration_raw_get( int16_t *val);
  int32_t iis3dwb_xl_filt_path_on_out_set(iis3dwb_filt_xl_en_t val);
  int32_t iis3dwb_acc_power_down(void);

  int32_t iis3dwb_temp_flag_data_ready_get(uint8_t *val);
  int32_t iis3dwb_temperature_raw_get( int16_t *val);
  
  private:
  uint8_t _cs, _mode;
  void _csCONFIG();
  int32_t writeByte(uint8_t reg, uint8_t *value);
  int32_t readByte(uint8_t reg,uint8_t* value);
  int32_t readBytes(uint8_t reg,uint8_t * dest,uint8_t count); 
  float _aRes;
  uint8_t test_val;
};

#if !defined(NO_GLOBAL_INSTANCES) && !defined(NO_GLOBAL_IIS3DWB)
extern IIS3DWB iis3dwb;
#endif
#endif/************************ (C) ICT_LAB *****END OF FILE*** IIS3DWB_h****/
