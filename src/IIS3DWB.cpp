/*
  ******************************************************************************
  * @file    iis3dwb_reg.h
  * @author  Nirvik Adhikary & Rajiv Ranjan
  * @Date    06/07/2022
  * @brief   This file contains all the functions prototypes for the
  *          iis3dwb_reg.c driver.
  ******************************************************************************
  */
 /* Includes -----------------------------------------------------------------*/
  #include "Arduino.h"
  #include "IIS3DWB.h"
  #include <SPI.h>
  #include "iis3dwb_reg.h"
  #include "config.h"
  
 // iis3dwb_fs_xl_t scalling_factor; 
SPIClass vspi(FSPI);
/*============================================================================*/
/*                                CONSTRUCTORS                                */
/*============================================================================*/
/*!
///     @brief   IIS3DWB()
///          
///     @param   cs, chip select pin - active low
**/

   IIS3DWB::IIS3DWB(uint8_t cspin)
   {
     _cs=cspin;
     _csCONFIG();
   }

/*=============================================================================*/
/*                               PUBLIC FUNCTIONS                              */
/*=============================================================================*/
/**
 * @brief  initialize the sensor
 * @retval       none
 */
  void IIS3DWB::IIS3DWB_init()
  {
   vspi.begin(IIS3DWB_SCLK, IIS3DWB_MISO,IIS3DWB_MOSI, IIS3DWB_CS); //SCLK, MISO, MOSI, CS
   digitalWrite(_cs, HIGH);
   iis3dwb_reset_set( PROPERTY_ENABLE);
   iis3dwb_block_data_update_set( PROPERTY_ENABLE);
   iis3dwb_xl_data_rate_set( IIS3DWB_XL_ODR_26k7Hz);
   iis3dwb_xl_full_scale_set(IIS3DWB_16g ); 
   IIS3DWB::iis3dwb_xl_axis_selection_set(IIS3DWB_ENABLE_ALL);
   iis3dwb_xl_filt_path_on_out_set(IIS3DWB_LP_ODR_DIV_100);
   Serial.println("Device INITILIZED.....");
   Serial.println("Default scale = +-16g");
   delay(100);
  
  }
/**
  * @brief    determine the scalling range according to the raw input data of 3 axises
  * @param  val scalling factor
  * @return   scalling factor range
  */
  int32_t IIS3DWB:: auto_scale()
      { 
      iis3dwb_fs_xl_t val;  
      uint8_t ret;
      int16_t temp[3]={0,0,0};
      uint16_t abs_temp[3][128];
      uint16_t val_maximum[3],max_val;
      for (uint8_t i = 0; i <128; i++)
      {
        iis3dwb.iis3dwb_acceleration_raw_get(temp);
        abs_temp[0][i]=abs(temp[0]);
        abs_temp[1][i]=abs(temp[1]);
        abs_temp[2][i]=abs(temp[2]);
      }
      val_maximum[0]=max((abs_temp[0]),128);
      val_maximum[1]=max(abs_temp[1],128);
      val_maximum[2]=max(abs_temp[2],128);
      max_val=(max(val_maximum,3));
      Serial.print( "Maximum_X = "); Serial.print( val_maximum[0] * 0.000488f);Serial.println( "g ");
      Serial.print( "Maximum_Y = "); Serial.print( val_maximum[1] * 0.000488f);Serial.println( "g");
      Serial.print( "Maximum_Z = "); Serial.print( val_maximum[2] * 0.000488f);Serial.println( "g");
      Serial.print( "Maximum_X_Y_Z = ");     Serial.print( max_val* 0.000488f);Serial.println( "g");
      if((max_val>=0) && (max_val<=4096))
      {
        val=IIS3DWB_2g;
        //Serial.println( "Autoscale set = +-2g");
       
      }
      else  if((max_val>4096) && (max_val<=8192))
      {
        val=IIS3DWB_4g;
        //Serial.println( "Autoscale set = +-4g");
       
      }
      else  if((max_val>8192) && (max_val<=16384))
      {
        val=IIS3DWB_8g;
        //Serial.println( "Autoscale set = +-8g");
        
      }
      else  if((max_val>16384) && (max_val<=32768))
      {
        val=IIS3DWB_16g;
       // Serial.println( "Autoscale set = +-16g");
       
      }
      ret=iis3dwb.iis3dwb_xl_full_scale_set(val);
      return ret;
    }
       /**
  * @brief  Accelerometer full-scale selection.[get]
  *
  * @param  ctx    Read / write interface definitions.(ptr)
  * @param  val    Get the values of fs_xl in reg CTRL1_XL
  * @retval        Interface status (MANDATORY: return 0 -> no Error).
  *
  */
  int32_t IIS3DWB::iis3dwb_xl_full_scale_get(iis3dwb_fs_xl_t *val,uint8_t* _aes)
  {
    iis3dwb_ctrl1_xl_t ctrl1_xl;
    int32_t ret;

    ret = readByte(IIS3DWB_CTRL1_XL, (uint8_t *)&ctrl1_xl);

    switch (ctrl1_xl.fs_xl)
    {
      case IIS3DWB_2g:
        *val = IIS3DWB_2g;
        *_aes = 8;
        break;

      case IIS3DWB_16g:
        *val = IIS3DWB_16g;
         *_aes = 1;
        break;

      case IIS3DWB_4g:
        *val = IIS3DWB_4g;
         *_aes = 4;
        break;

      case IIS3DWB_8g:
        *val = IIS3DWB_8g;
         *_aes = 2;
        break;

      default:
        *val = IIS3DWB_2g;
         *_aes = 8;
        break;
    }

    return ret;
  }
  /**
    * @defgroup   determine the maximum value of an array.
    * @param  value pointer of the data buffer
    * @param size   size of the array
    * @brief      These function flashes the led for a perticuler time 
    * @{
    *
    */
  uint16_t IIS3DWB::max(uint16_t arr[],uint8_t size)
    {    
      uint16_t max=arr[0];      
      for(uint16_t i=0;i<size;i++)
      {    
      if(max<arr[i])
      {    
      max=arr[i];    
      }    
      }    
      return max;    
   }
  /**
    * @defgroup    IIS3DWB_Sensitivity
    * @brief       These functions convert raw-data into engineering units.
    * @{
    *
    */    
   float_t IIS3DWB::iis3dwb_from_fs2g_to_mg(int16_t lsb)
  {
    return ((float_t)lsb * 0.061f);
  }

  float_t IIS3DWB:: iis3dwb_from_fs4g_to_mg(int16_t lsb)
  {
    return ((float_t)lsb * 0.122f)/100;
  }

  float_t IIS3DWB:: iis3dwb_from_fs8g_to_mg(int16_t lsb)
  {
    return ((float_t)lsb * 0.244f)/100;
  }

  float_t IIS3DWB:: iis3dwb_from_fs16g_to_mg(int16_t lsb)
  {
    return ((float_t)lsb * 0.488f)/100;
  }

  float_t IIS3DWB:: iis3dwb_from_lsb_to_celsius(int16_t lsb)
  {
    return (((float_t)lsb / 256.0f) + 25.0f);
  }

  /**
   * @brief  check the devic has cennected or not
   * @param  timeout   checking time
   * @param  flag       device connection flag
   * @retval        Interface status (MANDATORY: return 0 -> no Error).
   *
   */
 int32_t IIS3DWB::iis3dwb_device_id_check(uint32_t timeout,uint8_t *flag)
{
float t1,t2,t3;
t1 = micros();
test_val=(uint8_t)iis3dwb_device_id_get();
while((test_val!=IIS3DWB_ID) && (t3<timeout))
 {
  test_val=(uint8_t)iis3dwb_device_id_get();
  iis3dwb_reset_set(PROPERTY_ENABLE );
  t2 = micros();
  t3 = (t2-t1)/1000000;
 }
if(test_val==0x7B)
 {
 Serial.println("Device FOUND!");
 Serial.println(test_val,HEX);
 *flag = 0;
 }
else
 {
 Serial.println(" Device NOT FOUND!");
 *flag = 1;
 ESP.deepSleep(SLEEP_DURATION);
 }
return 0;
}
 /**
   * @brief  Device Who am I.[get]
   * @param  buff   Buffer that stores data read
   * @retval        Interface status (MANDATORY: return 0 -> no Error).
   *
   */
  int32_t IIS3DWB::iis3dwb_device_id_get()
  {
    readByte( IIS3DWB_WHO_AM_I,&test_val);

    return test_val;
  }
  void IIS3DWB::offsetBias(float * accelBias)
  {
  int16_t temp[3] = {0, 0, 0};
    int32_t sum[3] = {0, 0, 0};

    Serial.println("Offset calibration starting......");
     Serial.println("Keep sensor flat and motionless!");
    for (uint8_t ii = 0; ii <128; ii++)
    {
      iis3dwb.iis3dwb_acceleration_raw_get(temp);
      sum[0] += temp[0];
      sum[1] += temp[1];
      sum[2] += temp[2];
      delayMicroseconds(100);
    }
    accelBias[0] = sum[0]  / 128.0f;
    accelBias[1] = sum[1]  / 128.0f;
    accelBias[2] = sum[2]  / 128.0f;
   Serial.println("Offset calibration done!");
   Serial.println("Device ready.");

  }
  /**
    * @brief  Block data update.[set]
    * @param  val    Change the values of bdu in reg CTRL3_C
    * @retval        Interface status (MANDATORY: return 0 -> no Error).
    *
    */
  int32_t IIS3DWB::iis3dwb_block_data_update_set(uint8_t val)
  {
  iis3dwb_ctrl3_c_t ctrl3_c;
    int32_t ret;

    ret = readByte( IIS3DWB_CTRL3_C,(uint8_t* )&ctrl3_c);

    if (ret == 0)
    {
      ctrl3_c.bdu = (uint8_t)val;
      ret = writeByte( IIS3DWB_CTRL3_C, (uint8_t* )&ctrl3_c);
    }
    return ret; 
  }

  /**
    * @brief  Software reset. Restore the default values in user registers.[set]
    * @param  val    Change the values of sw_reset in reg CTRL3_C
    * @retval        Interface status (MANDATORY: return 0 -> no Error).
    *
    */
  int32_t IIS3DWB::iis3dwb_reset_set( uint8_t val)
  {
  iis3dwb_ctrl3_c_t ctrl3_c;
    int32_t ret;

    ret = readByte( IIS3DWB_CTRL3_C,(uint8_t*)&ctrl3_c);

    if (ret == 0)
    {
      ctrl3_c.sw_reset = (uint8_t)val;
      ret = writeByte( IIS3DWB_CTRL3_C, (uint8_t*)&ctrl3_c);
    }

    return ret;
  }
  /**
    * @brief  select accelerometer axis.[set]
    * @param  val    Change the values of xl_axis_sel in reg CTRL6_C and
    *                the values of _1ax_to_3regout in reg CTRL4_C
    * @retval        Interface status (MANDATORY: return 0 -> no Error).
    *
    */
  int32_t IIS3DWB::iis3dwb_xl_axis_selection_set(iis3dwb_xl_axis_sel_t val)
  {

    iis3dwb_ctrl4_c_t ctrl4_c;
    iis3dwb_ctrl6_c_t ctrl6_c;
    int32_t ret;

    ret = readByte(IIS3DWB_CTRL4_C,(uint8_t* )&ctrl4_c);

    if (ret == 0)
    {
      ctrl4_c._1ax_to_3regout = ((uint8_t)val & 0x10U) >> 4;
      ret = writeByte( IIS3DWB_CTRL4_C,(uint8_t* )&ctrl4_c);
    }

    if (ret == 0)
    {
      ret = readByte( IIS3DWB_CTRL6_C,(uint8_t* )&ctrl6_c);
    }

    if (ret == 0)
    {
      ctrl6_c.xl_axis_sel = (uint8_t)val;
      ret = writeByte( IIS3DWB_CTRL6_C, (uint8_t* )&ctrl6_c);
    }

    return ret;
  }

  /**
    * @brief  Accelerometer UI data rate selection.[set]
    * @param  val    Change the values of xl_en in reg CTRL1_XL
    * @retval        Interface status (MANDATORY: return 0 -> no Error).
    *
    */
  int32_t IIS3DWB::iis3dwb_xl_data_rate_set(iis3dwb_odr_xl_t val)
  {
  iis3dwb_ctrl1_xl_t ctrl1_xl;
    int32_t ret;

    ret = readByte( IIS3DWB_CTRL1_XL,(uint8_t *)&ctrl1_xl);

    if (ret == 0)
    {
      ctrl1_xl.xl_en = (uint8_t)val;
      ret = writeByte( IIS3DWB_CTRL1_XL,
                              (uint8_t *)&ctrl1_xl);
    }

    return ret;
  }
  /**
    * @brief  Accelerometer full-scale selection[set]
    * @param  val    Change the values of fs_xl in reg CTRL1_XL
    * @retval        Interface status (MANDATORY: return 0 -> no Error).
    *
    */
  int32_t IIS3DWB::iis3dwb_xl_full_scale_set(iis3dwb_fs_xl_t val)
  {
    iis3dwb_ctrl1_xl_t ctrl1_xl;
    int32_t ret;

    ret = readByte(IIS3DWB_CTRL1_XL,(uint8_t* )&ctrl1_xl);

    if (ret == 0)
    {
      ctrl1_xl.fs_xl = (uint8_t)val;
      ret = writeByte( IIS3DWB_CTRL1_XL, (uint8_t* )&ctrl1_xl);
    }

    return ret;
  }

/**
  * @brief  Accelerometer new data available.[get]
  * @param  val    Change the values of xlda in reg STATUS_REG
  * @retval        Interface status (MANDATORY: return 0 -> no Error).
  *
  */
  int32_t IIS3DWB::iis3dwb_xl_flag_data_ready_get(uint8_t *val)
  {
    iis3dwb_status_reg_t status_reg;
    int32_t ret;

    ret = readByte(IIS3DWB_STATUS_REG,(uint8_t* )&status_reg );
    *val = status_reg.xlda;

    return ret;
  }
/**
  * @brief  Linear acceleration output register. The value is expressed as a
  *         16-bit word in two’s complement.[get]
  * @param  buff   Buffer that stores data read
  * @retval        Interface status (MANDATORY: return 0 -> no Error).
  *
  */
  int32_t IIS3DWB::iis3dwb_acceleration_raw_get( int16_t *val)
  {
    uint8_t buff[6];
    int32_t ret;

    ret = readBytes(IIS3DWB_OUTX_L_A, buff, 6);
    val[0] = (int16_t)buff[1];
    val[0] = (val[0] * 256) + (int16_t)buff[0];
    val[1] = (int16_t)buff[3];
    val[1] = (val[1] * 256) + (int16_t)buff[2];
    val[2] = (int16_t)buff[5];
    val[2] = (val[2] * 256) + (int16_t)buff[4];

    return ret;
  }
  /**
    * @brief  Accelerometer filter selection on output.[set]
    * @param  val    Change filter selection on output.
    * @retval        Interface status (MANDATORY: return 0 -> no Error).
    *
    */
  int32_t IIS3DWB::iis3dwb_xl_filt_path_on_out_set(iis3dwb_filt_xl_en_t val)
  {
  iis3dwb_ctrl1_xl_t ctrl1_xl;
    iis3dwb_ctrl8_xl_t ctrl8_xl;
    int32_t ret;

    ret = readByte( IIS3DWB_CTRL1_XL,(uint8_t* )&ctrl1_xl);

    if (ret == 0)
    {
      ctrl1_xl.lpf2_xl_en = ((uint8_t)val & 0x80U) >> 7;
      ret = writeByte(IIS3DWB_CTRL1_XL,(uint8_t* )&ctrl1_xl);
    }

    if (ret == 0)
    {
      ret = readByte(IIS3DWB_CTRL8_XL,(uint8_t* )&ctrl8_xl);
    }

    if (ret == 0)
    {
      ctrl8_xl.fds = ((uint8_t)val & 0x10U) >> 4;
      ctrl8_xl.hp_ref_mode_xl = ((uint8_t)val & 0x20U) >> 5;
      ctrl8_xl.hpcf_xl = (uint8_t)val & 0x07U;
      ret = writeByte(IIS3DWB_CTRL8_XL, (uint8_t*)&ctrl8_xl);
    }

    return ret;
  }
   /**
  * @brief  Temperature new data available.[get]
  *
  * @param  spi_handle    Read / write interface definitions.(ptr)
  * @param  val    Change the values of tda in reg STATUS_REG
  * @retval        Interface status (MANDATORY: return 0 -> no Error).
  *
  */
int32_t IIS3DWB::iis3dwb_temp_flag_data_ready_get(uint8_t *val)
{
  iis3dwb_status_reg_t status_reg;
  int32_t ret;

  ret = readByte(IIS3DWB_STATUS_REG,
                         (uint8_t *)&status_reg);
  *val = status_reg.tda;

  return ret;
}
      /**
  * @brief  Temperature data output register (r).
  *         L and H registers together express a 16-bit word in two’s
  *         complement.[get]
  *
  * @param  spi_handle    Read / write interface definitions.(ptr)
  * @param  buff   Buffer that stores data read
  * @retval        Interface status (MANDATORY: return 0 -> no Error).
  *
  */
int32_t IIS3DWB:: iis3dwb_temperature_raw_get( int16_t *val)
{
  uint8_t buff[2];
  int32_t ret;

  ret = readByte( IIS3DWB_OUT_TEMP_L, buff);
  *val = (int16_t)buff[1];
  *val = (*val * 256) + (int16_t)buff[0];

  return ret;
}
  /**
    * @brief  power down the sensor
    * @retval       none
    *
    */ 
      int32_t IIS3DWB:: iis3dwb_acc_power_down(void)
      { 
        int32_t ret;
        ret = iis3dwb_xl_data_rate_set(IIS3DWB_XL_ODR_OFF);
        return ret;
      }
 /*========================================================================*/
/*                           PRIVATE FUNCTIONS                            */
/*========================================================================*/

/**
  *    @brief   _csCONFIG()
  *             initialize the chip select line
  */
  void IIS3DWB:: _csCONFIG()
  {
   pinMode(_cs, OUTPUT);
  }
/**
  * @brief  Accelerometer registor read byte.
  * @param  reg   register address
  * @param value value of the register
  * @retval      the value of register.
  *
  */
 int32_t IIS3DWB::readByte(uint8_t reg,uint8_t* value) 
  { 
    
    vspi.beginTransaction(SPISettings(SPI_FREQUENCY, MSBFIRST, SPI_MODE0));
    digitalWrite(_cs, LOW); 
    vspi.transfer((reg & 0x7F) | 0x80);
    
    *value = vspi.transfer(0);
    
    digitalWrite(_cs, HIGH);
    vspi.endTransaction();
 
    return 0;
  }

     /**
    * @brief  Accelerometer registor read byte.
    * @param  reg  register address from where data will be read.
    * @param dest  the buffer where value of the register will store.
    * @param count how many to read.
    * @retval      the value of register.
    *
    */
  int32_t IIS3DWB::readBytes(uint8_t reg,uint8_t * dest,uint8_t count) 
  {  
   
    vspi.beginTransaction(SPISettings(SPI_FREQUENCY, MSBFIRST, SPI_MODE0));
    digitalWrite(_cs, LOW);
    vspi.transfer((reg & 0x7F) | 0x80);
    
    for (uint8_t ii = 0; ii < count; ii++)
    {
      dest[ii] = vspi.transfer(0);
    }
    
    digitalWrite(_cs, HIGH);
    vspi.endTransaction();
    

    return 0;
  }

/**
  * @brief  Accelerometer registor write byte.
  * @param reg  register address to where data will be written.
  * @param value the value will be written
  *
  */
  int32_t IIS3DWB::writeByte(uint8_t reg, uint8_t* value) 
  {  
   
    vspi.beginTransaction(SPISettings(SPI_FREQUENCY, MSBFIRST, SPI_MODE0));
    digitalWrite(_cs, LOW); 
    vspi.transfer(reg & 0x7F);
    vspi.transfer((*value));
    digitalWrite(_cs, HIGH);
    vspi.endTransaction();
    
    return 0;
  }
 
