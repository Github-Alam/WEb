/*
***********************************************************************************************
  * @file    peripheral.h
  * @author  Nirvik Adhikary  & Rajiv Ranjan
  * @Date    26/09/2022
  * @brief   This file contains all the functions prototypes for the
  *          peripheral.h driver.
***********************************************************************************************
*/
      #ifndef __PERIPHERAL_H__
      #define __PERIPHERAL_H__
      #include "IIS3DWB.h"
      #include "iis3dwb_reg.h"
      #include "config.h"
      #include "WiFi.h"
      #include <EEPROM.h>
      #include "WiFiClient.h"
      #include <HTTPClient.h>
      #include "ESP32_FTPClient.h"
      #include "ESPAsyncWebServer.h"
      #include <AsyncElegantOTA.h>
      #include "UUID.h"

      class PERIPHERAL
        {
      public:
      void   flashing_led(uint8_t led,uint8_t count);
      boolean OFFset_get(uint32_t startAddr);
      void SENSOR_data_write(uint8_t _axis,String _uuid,uint8_t* _success_flag,float active_time,float battery_lvl);
      float SENSOR_Temp_get(); 
      void parameter_setup();
      void Start_WiFi();
      void Start_WiFi_AP();
      void Stop_WiFi();
      void Stop_WiFi_AP();
      unsigned long getTime();
      const char * int2char_conv(uint16_t array[],char ch_arr[], uint16_t arr_size,uint16_t first_index, uint16_t last_index);
      float battery_level(uint16_t days);
      void introduction();
      void serverAction(uint8_t *flag);
      void OneTimeSend();
      void GPIO_Status( uint8_t* val);
      private:  
        };

    /**
      *@}
      *
      */

    #endif //(C) COPYRIGHT ICT_LAB --------- End of __PERIPHERAL_H__-------//

