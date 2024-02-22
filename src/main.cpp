        
       /*
  ***********************************************************************************************
  * @file    main.c 
  * @author  Nirvik Adhikary
  * @Date    29/11/2023
  * @brief   This file contains main function of DN sir ICT internal server code final 31/01/24
  ***********************************************************************************************
*/
  #include"peripheral.h" 
  
  // NTP server to request epoch time
  const char* ntpServer = "pool.ntp.org";
  UUID uuid;
  IIS3DWB iis3dwb(IIS3DWB_CS);
  PERIPHERAL peripheral;
  RTC_DATA_ATTR uint16_t Wake_up_counter             = 0;
  RTC_DATA_ATTR float system_active_time            = 0;
  RTC_DATA_ATTR uint32_t successfully_transmit_no   = 0;
  RTC_DATA_ATTR uint32_t total_transmit_attempt_no  = 0;
  RTC_DATA_ATTR uint8_t transmit_count              = 0;
  RTC_DATA_ATTR uint16_t days_count                 = 0;
  uint8_t status_flag;

  /**
  * @defgroup   get epoch time
  * @return  epoch time
  */  
  unsigned long PERIPHERAL :: getTime() 
  {
    time_t now;
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) 
    {
      Serial.println("Failed to obtain time");
      return(0);
    }
    time(&now);
    return now;
  }

  void setup()
  {
    
    uint8_t success_flag_gen,success_flag_x,success_flag_y,success_flag_z;
    uint8_t retry_counter=0;
    uint32_t f_count =0;
    uint32_t val = random(0,200);
    const char* dir_name;
    float t1,t2,battery_lvl;

    Serial.begin(115200);                                                                   //begin serial communication    
    Serial.println("Woke up from deep sleep mode"); 
    EEPROM.begin(EEPROM_SIZE); 
    peripheral.OneTimeSend();
    peripheral.parameter_setup();
    peripheral.flashing_led(LED_PIN1,2); 
    Wake_up_counter+=1;
    Serial.print("Wake_up_counter = "); Serial.println(Wake_up_counter);
    if(Wake_up_counter>=TX_COUNTER)
    {                                                                      
      iis3dwb.IIS3DWB_init();                                                                //if sensor found then initialize the sensor 
      iis3dwb.iis3dwb_device_id_check(2,&status_flag);                                      //check the connected sensor id 
      iis3dwb.auto_scale();
      peripheral.Start_WiFi();
      if((WiFi.status() == WL_CONNECTED ))                                                //chexk the wifi connection ststus
      {
        delay(50);
        t1=micros(); 
        peripheral.flashing_led(LED_PIN1,2); 
        uuid.seed(val);
        uuid.generate();
        String gen_uuid =  uuid.toCharArray();
        if (transmit_count >= DATA_TRANS_LIMIT)
        { 
          transmit_count = 0;
          days_count += 1;
          Serial.print("Days_count = "); Serial.println(days_count);
        }
        battery_lvl = peripheral.battery_level(days_count);                               //battery lvl calculation
        total_transmit_attempt_no = total_transmit_attempt_no + 1; 
        peripheral.SENSOR_data_write(X_axis,gen_uuid,&success_flag_x,system_active_time,battery_lvl);                       
        peripheral.SENSOR_data_write(Y_axis,gen_uuid,&success_flag_y,system_active_time,battery_lvl); 
        peripheral.SENSOR_data_write(Z_axis,gen_uuid,&success_flag_z,system_active_time,battery_lvl); 
        iis3dwb.iis3dwb_acc_power_down();
        peripheral.Stop_WiFi();

        if ((success_flag_gen == 1) && (success_flag_x == 1) && (success_flag_y == 1) && (success_flag_z == 1))
        {
          successfully_transmit_no = successfully_transmit_no +1;
        }
        Wake_up_counter = 0;
        transmit_count += 1;
        t2=micros();
        system_active_time = (t2-t1)/1000; // in msec
        ESP.deepSleep(SLEEP_DURATION);
      }
      iis3dwb.iis3dwb_acc_power_down();
      Wake_up_counter = 0;
      Serial.println("Wifi not available,going into deep sleep mode ");
      ESP.deepSleep(SLEEP_DURATION);
    }
    ESP.deepSleep(SLEEP_DURATION);
    Serial.println("Wake-up_counter is less than tx counter.going into deep sleep mode" );
  }
  void loop()
  {
    
  }


