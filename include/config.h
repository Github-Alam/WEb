/*
***********************************************************************************************
  * @file    config.h
  * @author  Nirvik Adhikary
  * @Date    26/09/2022
  * @brief   This file contains all the functions prototypes for the
  *          config.h driver.
***********************************************************************************************
*/

      /* Define to prevent recursive inclusion -------------------------------------*/
      #ifndef __CONFIG_H__
      #define __CONFIG_H__
      /* Includes ------------------------------------------------------------------*/
      #include <Arduino.h>
      #include  <EEPROM.h>
      #define FIRMWARE_VERSION             "SVMS_Ver_4.0_webserver_1.0"
      /***********************************eeprom section************************************************/
      //#define DefaultParamUpdate           1
      #define EEPROM_SIZE                  58
      #define INDEX_ADDRESS                0                                    //directory index 
      #define SSID_ADD                     INDEX_ADDRESS + 5                    //wifi-ssid address 
      #define PASS_ADD                     SSID_ADD + 11                        //wifi-password address 
      #define DATA_LENGTH_ADD              PASS_ADD + 11                        //datalength address 
      #define TX_COUNTER_ADD               DATA_LENGTH_ADD + 6                  //tx counter address 
      #define SLEEP_DUR_ADD                TX_COUNTER_ADD + 5                  // sleep duration address 
      #define CHUNK_NO_ADD                 SLEEP_DUR_ADD + 9                  //chunk no address
      #define SAMP_FREQ_ADD                CHUNK_NO_ADD + 2                               //sampling freq address
      #define DEFAULT_PARAM_ADD            56
      #define ST_DATA_SEND_ADD            57
      /*-------------------------------------SERVER INPUT PARAM--------------------------------------------------------------*/
      #define EEPROOM_WIFI_SSID_READ       EEPROM.readString(SSID_ADD)
      #define EEPROOM_WIFI_PASS_READ       EEPROM.readString(PASS_ADD)
      #define EEPROOM_DATALENGTH_READ      EEPROM.readString(DATA_LENGTH_ADD)
      #define EEPROOM_TX_COUNTER_READ      EEPROM.readString(TX_COUNTER_ADD)
      #define EEPROOM_SLEEP_DUR_READ       EEPROM.readString(SLEEP_DUR_ADD)
      #define EEPROOM_CHUNK_NO_READ        EEPROM.readString(CHUNK_NO_ADD)
      #define EEPROOM_SAMPLING_FREQ_READ   EEPROM.readString(SAMP_FREQ_ADD)
      /********************************accelerometer pins**********************************************/
      #define IIS3DWB_MISO                 2         
      #define IIS3DWB_MOSI                 7         
      #define IIS3DWB_SCLK                 6         
      #define IIS3DWB_CS                   10  
      /************************************/
      #define X_axis                       0
      #define Y_axis                       1
      #define Z_axis                       2
      /**********Server general parameters*************/
      #define GV                              "TMP;HUM;SF;SZ;ASF;AMS;RB"            //Temperature,humidity,sampling freq,sample size ,auto scale factor,active time ,remaining battery
      #define X_axis_key                      "XV"
      #define Y_axis_key                      "YV"
      #define Z_axis_key                      "ZV"
      #define x_api_key                       "XU8vMP" //"rtips"
      /**********************************/
      #define LED_PIN1                     18          //d2   green / wake up every one min status
      //#define LED_PIN2                     19          //d3   red / 
      //#define LED_PIN3                     3           //d4   blue / one time setup indication
      #define SETUP_PIN                    4           //one time setup pin
      #define TX_COUNTER                   EEPROOM_TX_COUNTER_READ.toInt()
      #define MAX_DATA_LENGTH              19200
      #define MAX_chunk_buff_LENGTH        ((MAX_DATA_LENGTH/2)*6)
      #define SENSOR_DATA_LENGTH           EEPROOM_DATALENGTH_READ.toInt()
      #define CHUNK_NUM                    EEPROOM_CHUNK_NO_READ.toInt()
      #define CHUNK_SIZE                   SENSOR_DATA_LENGTH / CHUNK_NUM
      #define CHUNK_BUFF_SIZE              CHUNK_SIZE*6
   
      
      #define SPI_10_MHz                   10000000
      #define SPI_5_MHz                    5000000
      #define SPI_1_MHz                    1000000
      #define SPI_FREQUENCY                SPI_5_MHz
      #define SLEEP_DURATION               EEPROOM_SLEEP_DUR_READ.toDouble()
      #define DATA_TRANS_LIMIT             6
      #define MAX_DAY                      200.0
      #define MAX_BATTERY_PERCENT          100.0
   
     /*10 MHz SPI */
      #define SPI_10_MHz_FS_6_4_kHz        131.30                                 //6.394 kHz
      #define SPI_10_MHz_FS_10_kHz         75.30                                  //9.997 kHz
      #define SPI_10_MHz_FS_13_3_kHz       50.00                                  //13.3 
      #define SPI_10_MHz_FS_26_6_kHz       13.25                               //26.667  kHz
      /*5 MHz SPI*/  
      #define SPI_5_MHz_FS_6_4_kHz         124.85                                //6.394 kHz
      #define SPI_5_MHz_FS_10_kHz          68.60                                 //9.997 kHz
      #define SPI_5_MHz_FS_13_3_kHz        44                                   //13.3 
      #define SPI_5_MHz_FS_26_6_kHz        7                                     //26.667  kHz
      /*1 MHz SPI*/     
      #define SPI_1_MHz_FS_6_4_kHz         71.5                                //6.394 kHz
      #define SPI_1_MHz_FS_10_kHz          15.30                                //9.997 kHz
      /*wifi and ftp server*/
      //################remote server####################################//
      #define WIFI_SSID                   EEPROOM_WIFI_SSID_READ.c_str()                                        //ssid  
      #define WIFI_PASSWORD               EEPROOM_WIFI_PASS_READ.c_str()
      /*wifi */
      //################remote server####################################//
      // #define WIFI_SSID "IOT"                                         //ssid  
      // #define WIFI_PASSWORD ""   
      //#define WIFI_SSID "CAPHHT"                                         //ssid  
      //#define WIFI_PASSWORD ""   
      // #define WIFI_SSID "CAP-Guest"                                         //ssid  
      // #define WIFI_PASSWORD ""   
      //################local wifi####################################// 
      // #define WIFI_SSID          "pktxkm"                                         //ssid  
      // #define WIFI_PASSWORD       "nirvik0000"    
      // #define WIFI_SSID          "Redmi9"                                         //ssid  
      // #define WIFI_PASSWORD       "You@1231" 

      #endif // (C) ICT_LAB *****End of __CONFIG_H__********************************/



