/*
  ***********************************************************************************************
  * @file    peripheral.c
  * @author  Nirvik Adhikary &Rajiv Ranjan
  * @Date    26/09/2022
  * @brief   This file contains all the functions prototypes for the
  *          peripheral.c driver.
  ***********************************************************************************************
*/
#include "peripheral.h"
using namespace std;
const char *b;
IPAddress local_IP(10, 136, 53, 185);
IPAddress gateway(10, 136, 52, 1);
IPAddress subnet(255, 255, 254, 0); 
AsyncWebServer server(80);
// ESP32_FTPClient ftp (ftp_server,ftp_user,ftp_pass,5000, 2);
PERIPHERAL peri;
//***********************Ap_param*************************************//
  const char* ApSsid = "SVMS_VER_T";
  const char* ApPassword = "012345678";
/*************************web server*******************************/
 RTC_DATA_ATTR  const char* PARAM_INPUT_1 = "Wi-Fi ssid";
 RTC_DATA_ATTR  const char* PARAM_INPUT_2 = "Wi-Fi Password";
 RTC_DATA_ATTR  const char* PARAM_INPUT_3 = "Datalength";
 RTC_DATA_ATTR  const char* PARAM_INPUT_4 = "Tx counter";
 RTC_DATA_ATTR  const char* PARAM_INPUT_5 = "Sleep duration";
 RTC_DATA_ATTR  const char* PARAM_INPUT_6 = "Chunk no";
 RTC_DATA_ATTR  const char* PARAM_INPUT_7 = "Sampling freq";
 RTC_DATA_ATTR  const char* PARAM_INPUT_8 = "Current value Check";
 RTC_DATA_ATTR  const char* PARAM_INPUT_9 = "Preset value Check";
 RTC_DATA_ATTR  const char* PARAM_INPUT_10 = "Confirm";
 RTC_DATA_ATTR  const char* PARAM_INPUT_11 = "Restart";
 RTC_DATA_ATTR  const char* PARAM_INPUT_12 = "OTA";
 RTC_DATA_ATTR  const char* CNF_STATUS = "CNF";


  String inputMessage1,inputMessage2,inputMessage3,inputMessage4,inputMessage5,inputMessage6,inputMessage7,inputMessage8,inputMessage9,inputMessage10,inputMessage11,inputMessage12,CNF_STATUS13;
  String inputParam1,inputParam2,inputParam3,inputParam4,inputParam5,inputParam6,inputParam7,inputParam8,inputParam9,inputParam10,inputParam11,inputParam12,inputParam13;
  String presetValue1,presetValue2,presetValue3,presetValue4,presetValue5,presetValue6,presetValue7;
  bool check_1,check_2,check_3,check_4,check_5,check_6,check_7;

 //char DefaultParamUpdate = 'I';

// HTML web page to handle input fields
  const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html><head>
    <title>SVMS WEBSERVER</title>
    <meta charset="UTF-8">
    <style>
      * {
        box-sizing:border-box;
      }
      form {color:rgb(172, 208, 245);
            text-align: center;}
      input[type=text], select, textarea {
              width: 40%;
              padding: 15px;
              border: 1px solid #cd72da;
              border-style:hidden;
              border-radius: 10px;
              resize: vertical;
              background-color: #e5e8eb;
            }
       input:hover{background-color: #a1f7dd;color: rgb(31, 27, 27);}     
      label {
              padding: 15px 12px 12px 0;
              display: inline-block;
              color: rgb(240, 238, 236);
            }
      .container {
        border-radius: 10px;
        background-color: #5e7579;
        padding: 30px;
      }    
      .col-30 {
          float:left;
          width: 30%;
          margin-top: 5px;
        }
  
      .col-75 {
          float: left;
          width: 5%;
          margin-top: 6px;
        }
  
      /* Clear floats after the columns */
      .row:after {
        content: "";
        display: table;
        clear: both;
      }
      a {
        text-decoration: none;
        display: inline-block;
        padding: 8px 16px;
        text-align: right;
      }
      
      a:hover {
        background-color: #ddd;
        color: black;
      }
      .round {
        border-radius: 50%;
      }
      .Home_page 
               {
                  background-color: #eb688f;
                  color: rgb(243, 239, 239);
                  padding: 70px 150px;
                  text-align: center;
                  border-style:dashed;
               }
      .next {
        background-color: #04AA6D;
        color: white;
      }
      button{text-decoration: none;display: inline-block;padding: 15px 20px;text-align: right;border-radius: 50%;background-color: #81dbf1; }
      button:hover{background-color: #47be9b;color: rgb(245, 240, 240);}
               .round {border-radius: 20%;}
               .Home_page {background-color: #ae8ee2;color: rgb(19, 9, 9);padding: 10px;text-align: right;}
               .next {background-color: #0483aa;color: white;}
    footer 
      {
      text-align: center;
      padding: 10px;
      background-color: rgb(58, 82, 80);
      color: rgb(53, 70, 63);
      border-radius: 10px;
      }
      /* Responsive layout - when the screen is less than 600px wide, make the two columns stack on top of each other instead of next to each other */
      @media screen and (max-width: 250px) {
        .col-25, .col-75, input[type=submit] {
          width: 100%;
          margin-top: 0;
        }
      }
      h5   {text-align: center;}
      h2   {background-color: azure;text-align: center;border-color:rgb(107, 187, 233);border-style:double;border-width:4px;}
      h3   {text-align: center;}
      body {background-color: rgb(39, 90, 90);}
      p    {color: rgb(63, 147, 243);}
      /*********************************tooltip**************************************/
      .tooltip {
        position: relative;
        display:contents;
        border-bottom: 1px dotted rgb(231, 156, 156);
      }
      
      .tooltip .tooltiptext {
        visibility: hidden;
        width: 150px;
        height: fit-content;
        background-color: rgb(218, 232, 243);
        color: #161414;
        text-align: center;
        border-radius: 6px;
        padding: 5px 0;
      
        /* Position the tooltip */
        position:fixed;
        z-index: 1;
      }
      .tooltip:hover .tooltiptext {
        visibility: visible;
      }
      /*********************************************************************************/
    </style>
    <title>ESP Input Form</title>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    </head>
    <body>
    <h2><p>WELCOME TO SVMS</p></h2>
    <h3><p style="color: rgb(56, 206, 69)">PLEASE ENTER THE PARAMETERS:</p></h3>
    <h5><i style="color: rgb(231, 53, 97)">&#x1F4E2 WIFI_SSID and WIFI_PASSWORD both should be less or equal of 10 characters!</i></h5>
    <div class="container">
    <form action="/get">
      <div class="row">
        <div class="col-30">
          <label for="fname">Wi-Fi ssid:</label>
        </div>
        <!--<div class="col-75">-->
     <input  type="text" name="Wi-Fi ssid" placeholder="Enter WiFi ssid">
     <button type="submit" name="Wi-Fi ssid" formmethod="get" style='font-size:20px;'>&#128228</button> 
    <!-- <input type="submit" value="&#x1F4E4"> -->
    </div>
    </form><br>
    
    <form action="/get">
      <div class="row">
        <div class="col-30">
          <label for="fname">Wi-Fi Password:</label>
        </div>
          <!--<div class="col-75">-->
     <input  type="text" name="Wi-Fi Password" placeholder="Enter WiFi password">
    <!-- <input type="submit" value="&#x1F4E4"> -->
   <button type="submit" name="Wi-Fi Password" formmethod="get" style='font-size:20px;'>&#128228</button> 
    </div>
    </form><br> 
    
    <form action="/get">
      <div class="row">
        <div class="col-30">
          <label for="fname">Data Transmition per Day </label>
        </div>
         <!--<div class="col-75">-->
     <!--<input  type="text" name="Tx counter" placeholder="From 1 to 240"> -->
     <div class="tooltip"><form>
        <!--<label for="options">Choose an option:</label>--> 
        
         <select id="options" name="Tx counter" >
            
         <!--<option> Choose an option</option>-->
 
             <option value="1440"> 1 </option>
             <option value="720"> 2 </option>
             <option value="480"> 3 </option>
             <option value="360"> 4 </option>
             <option value="240"> 6 </option>
             <option value="180"> 8 </option>
             <option value="120"> 12 </option>
             <option value="60"> 24 </option>
         </select>
     </form>
        <span class="tooltiptext">Choose an option</span>
      </div>
    <!-- <input type="submit" value="&#x1F4E4"> -->
    <button type="submit" name="Tx counter" formmethod="get" style='font-size:20px;'>&#128228</button>
    </div>
    </form><br> 

  <form action="/get">
    <div class="row">
      <div class="col-30">
        <label for="fname">Save</label>
      </div>
        <!--<div class="col-75">-->
   <button onclick="doubleConfirmation()" type="submit" name="Confirm" formmethod="get" style='font-size:20px;'>&#128228</button>
  </div>
  </form><br>

  <form action="/get">
    <div class="row">
      <div class="col-30">
        <label for="fname">Previous Configurations</label>
      </div>
        <!--<div class="col-75">-->
   <button type="submit" name="Preset value Check" formmethod="get" style='font-size:20px;'>&#128229</button>
  </div>
  </form><br>

  <form action="/get">
    <div class="row">
      <div class="col-30">
        <label for="fname">Exit</label>
      </div>
        <!--<div class="col-75">-->
          <!--<button onclick="doubleConfirmation()">Click me</button>-->
  <!-- <button onclick="doubleConfirmation()" type="submit" name="Restart" formmethod="get" style='font-size:20px;'>&#128228</button> -->
  <form id="myForm" action="Restart" method="get">
    <!-- Your form content here -->

    <button onclick="confirmSubmit()" type="submit" name="Restart formmethod="get" style='font-size:20px;'>&#128228;</button>
</form>

<script>
    function confirmSubmit() {
        if (confirm('Are you sure you want to proceed?')) {
            document.getElementById("myForm").submit(); // Submit the form
        }
    }
</script>

  </div>
  </form><br>

  <form action="/get">
    <div class="row">
      <div class="col-30">
       <!-- <label for="fname">Timer</label>-->
        <span style='font-size:50px;'>&#128338;</span>
      </div>
  
  <head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Timer with Professional Icon</title>
    <!--<link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/5.15.4/css/all.min.css">-->
    
  </head>
  
    <div class="timer-container">
        <i class="fas fa-clock timer-icon"></i>
        <div class="timer" id="timer">02:00</div>
    </div>
  
    <script>
      // Function to start the timer
      function startTimer(duration) {
          var startTime = Date.now();
          localStorage.setItem('startTime', startTime);

          var timer = setInterval(function() {
              var currentTime = Date.now();
              var elapsedTime = currentTime - startTime;
              var remainingTime = duration - elapsedTime;

              // Check if time is up
              if (remainingTime <= 0) {
                  clearInterval(timer);
                  localStorage.removeItem('startTime');
                  document.getElementById('timer').innerHTML = "Time's up!";
              } else {
                  var minutes = Math.floor((remainingTime / 1000 / 60) % 60);
                  var seconds = Math.floor((remainingTime / 1000) % 60);
                  document.getElementById('timer').innerHTML = minutes + "m " + seconds + "s";
              }
          }, 1000);
      }

      // Check if timer is already running
      var startTime = localStorage.getItem('startTime');
      if (startTime) {
          var currentTime = Date.now();
          var elapsedTime = currentTime - startTime;
          var remainingTime = 2 * 60 * 1000 - elapsedTime;

          if (remainingTime > 0) {
              startTimer(remainingTime);
          } else {
              localStorage.removeItem('startTime');
              document.getElementById('timer').innerHTML = "Time's up!";
          }
      } else {
          startTimer(2 * 60 * 1000); // Start timer for 2 minutes (2 * 60 * 1000 milliseconds)
      }
  </script>
  </div>
  </form><br>
  </div>
  </div>
  <!-- <footer>
  
    <img src="automation-division-tata-steel-logo.png" alt="automation-division-tata-steel-logo" width="650" height="120">
  </footer> -->

  <script>
    function doubleConfirmation() {
        // First confirmation
        if (confirm("Are you sure you want to proceed?"))
        {
             // Action to be taken after double confirmation
               // alert("Action executed!");         
        }
    }
</script>

  </body>
  </html>)rawliteral";
/*========================================================================*/
/*                           PUBLIC FUNCTIONS                             */
/*========================================================================*/
    /**
  * @defgroup   led flash.
  * @param  led gpio pin number
  * @param  count number of time the led will flash
  * @brief      These function flashes the led for a perticuler time 
  * @{
  *
  */
void PERIPHERAL::flashing_led(uint8_t led,uint8_t count)
{
    pinMode(led,OUTPUT);
    for(int i=0;i<count;i++)
    {
      digitalWrite(led,HIGH);
      delay(15);
      digitalWrite(led,LOW);
      delay(25);
    }
    pinMode(led,INPUT);
}
  /**
  * @defgroup   OFFset value is calibrated and value is written into FRAM.
  * @param  startAdr  FRAM address to which values is to be write.
  * @return  0: error
  *          1: ok
  */
boolean PERIPHERAL::OFFset_get(uint32_t startAddr )
{
  float accelBias[3] = {0.0f, 0.0f, 0.0f};                 // offset biases for the accel 
  iis3dwb.offsetBias(accelBias);
  
  return true;
}

/**
* @defgroup   calculate the temperature 
* @param  null
* @return  temperature value in degree C
*/
   float PERIPHERAL::SENSOR_Temp_get()
   {
      uint8_t res;
      static int16_t data_raw_temperature;
      static float acceleration_mg[3];
      float temperature_degC;
      iis3dwb.iis3dwb_temp_flag_data_ready_get(&res);
      if (res) 
      {
      /** Read temperature data */
      memset(&data_raw_temperature, 0x00, sizeof(int16_t));
      iis3dwb.iis3dwb_temperature_raw_get(&data_raw_temperature);
      temperature_degC = iis3dwb.iis3dwb_from_lsb_to_celsius(data_raw_temperature);
      return temperature_degC;
      }
     else
     {
     return false;
     }
   }

  /**
  * @defgroup   write data over FTP server via wifi
  * @param  _axis   the perticular axis of accelerometer
  * @param   dir    directory name 
  * @param  file    file name
  * @param  active_time total active time of system
  * @param battery_lvl remaining battery level 
  * @return  none
  */
void PERIPHERAL:: SENSOR_data_write(uint8_t _axis,String _uuid,uint8_t* _success_flag,float active_time,float battery_lvl)
{ 
  iis3dwb_fs_xl_t Auto_sc_val;
  uint8_t resp = 0, chunk_num = 0, acc_aes = 0;
  HTTPClient http; 
  int httpResponseCode,httpResponseCode_border;
  float t1,t2,t3,temperature;
  int16_t first_index,last_index;
  static int16_t data_raw_accel[3];
  int16_t accelBias[3] = {0, 0, 0};
  static int16_t sensor_read_buff[MAX_DATA_LENGTH]={0};
  static char converted_buff[MAX_chunk_buff_LENGTH]={'\0'};
   String device_id =WiFi.macAddress();
 // char *converted_buff_ptr = (char *)malloc(CHUNK_BUFF_SIZE * sizeof(char));           //memory alocate 
  //int16_t *sensor_read_buff_ptr = (int16_t *)malloc(SENSOR_DATA_LENGTH * sizeof(int16_t));           //memory alocate 
  
  
  iis3dwb.iis3dwb_xl_full_scale_get(&Auto_sc_val,&acc_aes);
  iis3dwb.iis3dwb_xl_flag_data_ready_get(&resp);             //data ready flag output status
  if (resp)
  {
    memset(data_raw_accel,0x00,3*sizeof(int16_t));            //memory alocate 

   //t1=micros();
  for(uint16_t i=0;i<SENSOR_DATA_LENGTH;i++)
  { 
  iis3dwb.iis3dwb_acceleration_raw_get(data_raw_accel);        //raw data read
  sensor_read_buff[i]=data_raw_accel[_axis];
  delayMicroseconds(SPI_5_MHz_FS_6_4_kHz);                    
  } 
  //temperature = SENSOR_Temp_get();
  temperature = random(27,30);
// t2=micros();
 // Serial.printf("%d numbers of data taken from Accelerometer sensor\r\n",SENSOR_DATA_LENGTH);
  for(uint8_t j=0; j<CHUNK_NUM; j++)
  {
  first_index=CHUNK_SIZE*j;
  last_index =first_index + CHUNK_SIZE;
  chunk_num += 1;
  //Serial.println("Offset removed...");
  // t1=micros(); 
  int2char_conv((uint16_t*)sensor_read_buff,converted_buff,CHUNK_SIZE,first_index,last_index);  //convert int to string 
 //  t2=micros();
 // Serial.printf("%d numbers of data converted into string...\r\n",CHUNK_SIZE);  
  
  String serverPath = "http://151.0.42.101/FFTApi/SaveVibration?deviceId=" + device_id + "&sampleSize=" + (String)SENSOR_DATA_LENGTH + "&axis=" + (String)_axis + "&sFreq=" + EEPROOM_SAMPLING_FREQ_READ + "&timestamp=" + _uuid + "&chunkNum=" + (String)chunk_num + "&totChunks=" + (String)CHUNK_NUM + "&reserve1=&reserve2=&autoScale=" + (String)Auto_sc_val + "&activeTime=" + (String)active_time + "&remBatt=" + (String)battery_lvl + "&temp10=" + (String)temperature + "0"; 

  http.begin(serverPath.c_str());
  http.addHeader("Content-Type" , "text/plain");
  httpResponseCode = http.POST(converted_buff);
  Serial.print("HTTP Response code: ");
  Serial.println(httpResponseCode); 
  memset(converted_buff,'\0',CHUNK_BUFF_SIZE);  
 // Serial.println("converted_buff buffer cleared."); 
  

  // t3= (t2-t1)/1000;
  // Serial.print("conversion time in ms= ");Serial.println(t3); 
  }
  memset(sensor_read_buff,0,SENSOR_DATA_LENGTH); 
  //Serial.println("sensor_read_buff cleared.");
  }
    if (httpResponseCode == 200)
    {
      *_success_flag = 1;
      //Serial.println("Total data sent");
    }
    else
    {
      *_success_flag = 0;
    };                                                       
  // t2=micros();
  // t3= (t2-t1)/1000000;
  // Serial.print("conversion time in sec= ");Serial.println(t3); 
  }
  
/**
  * @defgroup   one time offset setup and make index number zero
  * @param  null
  * @return  none
  */
    void PERIPHERAL :: parameter_setup()
    {
      uint8_t status_flag,serverFlag = 0, Led_Status = 0;
      pinMode(SETUP_PIN,INPUT);
      if (digitalRead(SETUP_PIN) == LOW)
        delay(20);                                                       // for Debouncing check
      if (digitalRead(SETUP_PIN) == LOW)
      {
        if(( EEPROM.readChar(DEFAULT_PARAM_ADD) != 'I') && (digitalRead(SETUP_PIN) == LOW))
        {
          EEPROM.writeString(SSID_ADD,"IOT");
          EEPROM.writeString(PASS_ADD,"");
          EEPROM.writeString(DATA_LENGTH_ADD,"19200");
          EEPROM.writeString(TX_COUNTER_ADD,"240");
          EEPROM.writeString(SLEEP_DUR_ADD,"60000000");
          EEPROM.writeString(CHUNK_NO_ADD,"3");
          EEPROM.writeString(SAMP_FREQ_ADD,"6400");
          EEPROM.writeChar(DEFAULT_PARAM_ADD,'I');
          EEPROM.commit();
         
          presetValue1 = EEPROM.readString(SSID_ADD);
          presetValue2 = EEPROM.readString(PASS_ADD);
          presetValue3 = EEPROM.readString(DATA_LENGTH_ADD);
          presetValue4 = EEPROM.readString(TX_COUNTER_ADD);
          presetValue5 = EEPROM.readString(SLEEP_DUR_ADD);
          presetValue6 = EEPROM.readString(CHUNK_NO_ADD);
          presetValue7 = EEPROM.readString(SAMP_FREQ_ADD);
          Serial.print("PRESET PARAMETERS LIST >> \r\n"); Serial.print("Wi-Fi ssid = ");Serial.println(presetValue1);Serial.print("Wi-Fi password = ");Serial.println(presetValue2);Serial.print("Datalength = ");Serial.println(presetValue3);Serial.print("TX counter = ");Serial.println(presetValue4);Serial.print("Sleep duration in ms = ");Serial.println(presetValue5);Serial.print("Number of data-chunk = ");Serial.println(presetValue6);Serial.print("Sampling frequency = ");Serial.println(presetValue7);
        }
        introduction();
        Serial.print("flagf= ");Serial.println(serverFlag);
        iis3dwb.iis3dwb_acc_power_down();                                //power down the sensor
        //flashing_led(LED_PIN1,4);
        while (serverFlag != 1)
        {
          if (Led_Status==0)
          {
            Led_Status=1;
            flashing_led(LED_PIN1,5);
          }  
          Start_WiFi_AP();                                                 //Start the wifi access point
          server.begin();                                                  //start the server to handle client
          serverAction(&serverFlag);                                       //perform server action
          Serial.print("flagf= ");Serial.println(serverFlag);

        }
        flashing_led(LED_PIN1,4);
      }
      /*else if( DefaultParamUpdate != 'I')
      {
        EEPROM.writeString(SSID_ADD,"CAP-Guest");
        EEPROM.writeString(PASS_ADD,"");
        EEPROM.writeString(DATA_LENGTH_ADD,"19200");
        EEPROM.writeString(TX_COUNTER_ADD,"240");
        EEPROM.writeString(SLEEP_DUR_ADD,"60000000");
        EEPROM.writeString(CHUNK_NO_ADD,"3");
        EEPROM.writeString(SAMP_FREQ_ADD,"6400");
        EEPROM.commit();
        DefaultParamUpdate = 'A';
        presetValue1 = EEPROM.readString(SSID_ADD);
        presetValue2 = EEPROM.readString(PASS_ADD);
        presetValue3 = EEPROM.readString(DATA_LENGTH_ADD);
        presetValue4 = EEPROM.readString(TX_COUNTER_ADD);
        presetValue5 = EEPROM.readString(SLEEP_DUR_ADD);
        presetValue6 = EEPROM.readString(CHUNK_NO_ADD);
        presetValue7 = EEPROM.readString(SAMP_FREQ_ADD);
        Serial.print("PRESET PARAMETERS LIST >> \r\n"); Serial.print("Wi-Fi ssid = ");Serial.println(presetValue1);Serial.print("Wi-Fi password = ");Serial.println(presetValue2);Serial.print("Datalength = ");Serial.println(presetValue3);Serial.print("TX counter = ");Serial.println(presetValue4);Serial.print("Sleep duration in ms = ");Serial.println(presetValue5);Serial.print("Number of data-chunk = ");Serial.println(presetValue6);Serial.print("Sampling frequency = ");Serial.println(presetValue7);
        
      }*/
      
    }
  /**
      * @defgroup  start the wifi connection
      * @param  null
      * @return none
      */
      void PERIPHERAL :: Start_WiFi()
      {
      uint8_t retry_counter=0;
      String ssid,pass;
      if (!WiFi.config(local_IP, gateway, subnet))                                              //configure wifi for static ip
      {
        Serial.println("STA Failed to configure");
      }
      // ssid =  EEPROM.readString(SSID_ADD);
       pass =  EEPROM.readString(PASS_ADD);
       
       if (pass == "NULL")
       {
         pass = "";          
       }    
      const char* pass_ep = pass.c_str();     
      WiFi.begin(WIFI_SSID, pass_ep);              //wifi initialization with ssid and password 
      while((WiFi.status() != WL_CONNECTED ))
      {
      delay(1000);
      if((WiFi.status() != WL_CONNECTED )&& retry_counter<3)       //connection retry for 3 time otherwise system will enter in deep sleep mode
      {
      WiFi.begin(WIFI_SSID, pass_ep);  
      delay(1000);
      }
      else
      {
      break;
      }
      retry_counter ++;
      }
      }
      /**
      * @defgroup  stop the wifi connection
      * @param  null
      * @return none
      */
      void PERIPHERAL :: Stop_WiFi()
      { 
      Serial.println("Disconnecting the WiFi connection....");
      WiFi.disconnect(); 
      Serial.println("WiFi connection Disconnected");
      }
      
//  /**
//     * @defgroup   int to char convertion 
//     * @param  array  arry of int data buff
//     * @param ch_arr  converted character data buff
//     * @param arr_size  size of buffer
//     * @return  const character pointer of converted buffer
//     */
//        const char * PERIPHERAL :: int2char_conv(uint16_t array[],char ch_arr[], uint16_t arr_size,uint16_t first_index, uint16_t last_index)
//         {
//           uint32_t j = 0;

//           for(uint16_t i=first_index; i<last_index; i++)
//             {
//               uint8_t len=5;
//               j+=4;
//               while(len)
//                 {
//                   ch_arr[j] = 48+ array[i]%10;
//                   array[i] = array[i]/10;
//                   j--;
//                   len--;
//                 }
//               j+=6;
//               ch_arr[j] =',';
//               j++;
//             }
//             ch_arr[j] ='\0';
          
//           return ch_arr;
//         }
    /**
   /**
    * @defgroup   int to char convertion 
    * @param  array  arry of int data buff
    * @param ch_arr  converted character data buff
    * @param arr_size  size of buffer
    * @return  const character pointer of converted buffer
    */
       const char * PERIPHERAL :: int2char_conv(uint16_t array[],char ch_arr[], uint16_t arr_size,uint16_t first_index, uint16_t last_index)
        {
          uint32_t j = 1;
          ch_arr[0] ='\'';
          for(uint16_t i=first_index; i<last_index; i++)
            {
              uint8_t len=5;
              j+=4;
              while(len)
                {
                  ch_arr[j] = 48+ array[i]%10;
                  array[i] = array[i]/10;
                  j--;
                  len--;
                }
              j+=6;
              ch_arr[j] =',';
              j++;
            }
            ch_arr[j] ='\'';
            j++;
            ch_arr[j] ='\0';
          return ch_arr;
        }
/* @defgroup   battery_level calculation 
* @param    days number of days device ran
* @return  remaining battery level
*/
  float PERIPHERAL :: battery_level(uint16_t days)
  {
    float battery_level = 0.0;
    battery_level = (100.0 -((MAX_BATTERY_PERCENT/ MAX_DAY) * days)) ;
    Serial.print("battery_level = "); Serial.println(battery_level);
    return battery_level;
  }
   /**
  * @brief    introduction
  * @return   none
  */
  void PERIPHERAL:: introduction()
  {
    Serial.println("+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++");
    Serial.println("");
    Serial.println("Welcome to SVMS!");
    Serial.print("Firmware version = ");Serial.println(FIRMWARE_VERSION);
    Serial.print("Target ssid = ");Serial.println(WIFI_SSID);
    Serial.println("+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++");
    Serial.println("");
  }
     /**
  * @brief    Start wifi in ap mode
  * @return   none
  */
   void PERIPHERAL:: Start_WiFi_AP()
   {
    Serial.println("Setting AP (Access Point)…");
    // Remove the password parameter, if you want the AP (Access Point) to be open
    WiFi.softAP(ApSsid, ApPassword);
    IPAddress IP = WiFi.softAPIP();
    Serial.println("AP IP address: ");
    Serial.println(IP);
   }
        /**
  * @brief    stop wifi ap 
  * @return   none
  */
    void PERIPHERAL ::  Stop_WiFi_AP()
    {
      Serial.println("Shutting down the ap connection...");
      WiFi.softAPdisconnect();
    }
           /**
  * @brief   webserver 
  * @return   none
  */
    void PERIPHERAL :: serverAction(uint8_t *flag)
    {
     
     // Send web page with input fields to client
      server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
      {
        request->send_P(200, "text/html", index_html);
      });

      // Send a GET request to <ESP_IP>/get?input1=<inputMessage>
      server.on("/get", HTTP_GET, [] (AsyncWebServerRequest *request) 
      {
        
        uint8_t flagCount = 0;
        // GET input1 value on <ESP_IP>/get?input1=<inputMessage>
        if (request->hasParam(PARAM_INPUT_1)) 
        {
          inputMessage1 = request->getParam(PARAM_INPUT_1)->value();
          inputParam1 = PARAM_INPUT_1;
          if (inputMessage1.length() > 10)
          {
            request->send(200, "text/html", "INVALID PARAMETER LENGTH!<!DOCTYPE HTML><html><head><title>SVMS WEBSERVER</title><meta charset='UTF-8'><style> a {text-decoration: none;display: inline-block;padding: 8px 16px;text-align: right;}a:hover{background-color: #ddd;color: black;}.round {border-radius: 50%;}.Home_page {background-color: #4e6d52;color: rgb(240, 234, 234);padding: 10px;text-align: right;}.next {background-color: #04AA6D;color: white;}</style></head><body><div align = 'center'><br><a  href='/\' class='Home_page'>&laquo;Return to the Home page</a></div></body>");
            Serial.println("INVALID PARAMETER LENGTH!"); 
          }
          else
          {
            request->send(200, "text/html", "HTTP GET request sent to your ESP on input field (" + inputParam1 + ") with value: " + inputMessage1 + "<!DOCTYPE HTML><html><head><title>SVMS WEBSERVER</title><meta charset='UTF-8'><style> a {text-decoration: none;display: inline-block;padding: 8px 16px;text-align: right;}a:hover{background-color: #ddd;color: black;}.round {border-radius: 50%;}.Home_page {background-color: #4e6d52;color: rgb(240, 234, 234);padding: 10px;text-align: right;}.next {background-color: #04AA6D;color: white;}</style></head><body><div align = 'center'><br><a  href='/\' class='Home_page'>&laquo;Return to the Home page</a></div></body>");
            Serial.print("input Message = "); Serial.println(inputMessage1);
          }
          

        }
        // GET input2 value on <ESP_IP>/get?input2=<inputMessage>
        else if (request->hasParam(PARAM_INPUT_2)) 
        {
          inputMessage2 = request->getParam(PARAM_INPUT_2)->value();
          inputParam2 = PARAM_INPUT_2;
             if (inputMessage2.length() > 10)
          {
            request->send(200, "text/html", "INVALID PARAMETER LENGTH!<!DOCTYPE HTML><html><head><title>SVMS WEBSERVER</title><meta charset='UTF-8'><style> a {text-decoration: none;display: inline-block;padding: 8px 16px;text-align: right;}a:hover{background-color: #ddd;color: black;}.round {border-radius: 50%;}.Home_page {background-color: #4e6d52;color: rgb(240, 234, 234);padding: 10px;text-align: right;}.next {background-color: #04AA6D;color: white;}</style></head><body><div align = 'center'><br><a  href='/\' class='Home_page'>&laquo;Return to the Home page</a></div></body>");
            Serial.println("INVALID PARAMETER LENGTH!"); 
          }
          else
          {
            request->send(200, "text/html", "HTTP GET request sent to your ESP on input field (" + inputParam2 + ") with value: " + inputMessage2 + "<!DOCTYPE HTML><html><head><title>SVMS WEBSERVER</title><meta charset='UTF-8'><style> a {text-decoration: none;display: inline-block;padding: 8px 16px;text-align: right;}a:hover{background-color: #ddd;color: black;}.round {border-radius: 50%;}.Home_page {background-color: #4e6d52;color: rgb(240, 234, 234);padding: 10px;text-align: right;}.next {background-color: #04AA6D;color: white;}</style></head><body><div align = 'center'><br><a  href='/\' class='Home_page'>&laquo;Return to the Home page</a></div></body>");
            Serial.print("input Message = "); Serial.println(inputMessage2); 
          }
        
        }
        // GET input3 value on <ESP_IP>/get?input3=<inputMessage>
        // else if (request->hasParam(PARAM_INPUT_3)) 
        // {
        //   inputMessage3 = request->getParam(PARAM_INPUT_3)->value();
        //   inputParam3 = PARAM_INPUT_3;
        //   request->send(200, "text/html", "HTTP GET request sent to your ESP on input field (" + inputParam3 + ") with value: " + inputMessage3 + "<!DOCTYPE HTML><html><head><title>SVMS WEBSERVER</title><meta charset='UTF-8'><style> a {text-decoration: none;display: inline-block;padding: 8px 16px;text-align: right;}a:hover{background-color: #ddd;color: black;}.round {border-radius: 50%;}.Home_page {background-color: #4e6d52;color: rgb(240, 234, 234);padding: 10px;text-align: right;}.next {background-color: #04AA6D;color: white;}</style></head><body><div align = 'center'><br><a  href='/\' class='Home_page'>&laquo;Return to the Home page</a></div></body>");
        //   Serial.print("input Message = "); Serial.println(inputMessage3);
        // }
        // GET input3 value on <ESP_IP>/get?input3=<inputMessage>
        else if (request->hasParam(PARAM_INPUT_4)) 
        {
          inputMessage4 = request->getParam(PARAM_INPUT_4)->value();
          inputParam4 = PARAM_INPUT_4;
          request->send(200, "text/html", "HTTP GET request sent to your ESP on input field (" + inputParam4 + ") with value: " + inputMessage4 + "<!DOCTYPE HTML><html><head><title>SVMS WEBSERVER</title><meta charset='UTF-8'><style> a {text-decoration: none;display: inline-block;padding: 8px 16px;text-align: right;}a:hover{background-color: #ddd;color: black;}.round {border-radius: 50%;}.Home_page {background-color: #4e6d52;color: rgb(240, 234, 234);padding: 10px;text-align: right;}.next {background-color: #04AA6D;color: white;}</style></head><body><div align = 'center'><br><a  href='/\' class='Home_page'>&laquo;Return to the Home page</a></div></body>");
          Serial.print("input Message = "); Serial.println(inputMessage4);       
        }
        // GET input3 value on <ESP_IP>/get?input3=<inputMessage>
        // else if (request->hasParam(PARAM_INPUT_5)) 
        // {
        //   inputMessage5 = request->getParam(PARAM_INPUT_5)->value();
        //   inputParam5 = PARAM_INPUT_5;
        //   request->send(200, "text/html", "HTTP GET request sent to your ESP on input field (" + inputParam5 + ") with value: " + inputMessage5 + "<!DOCTYPE HTML><html><head><title>SVMS WEBSERVER</title><meta charset='UTF-8'><style> a {text-decoration: none;display: inline-block;padding: 8px 16px;text-align: right;}a:hover{background-color: #ddd;color: black;}.round {border-radius: 50%;}.Home_page {background-color: #4e6d52;color: rgb(240, 234, 234);padding: 10px;text-align: right;}.next {background-color: #04AA6D;color: white;}</style></head><body><div align = 'center'><br><a  href='/\' class='Home_page'>&laquo;Return to the Home page</a></div></body>");
        //   Serial.print("input Message = "); Serial.println(inputMessage5);
        // }
        // // GET input3 value on <ESP_IP>/get?input3=<inputMessage>
        // else if (request->hasParam(PARAM_INPUT_6)) 
        // {
        //   inputMessage6 = request->getParam(PARAM_INPUT_6)->value();
        //   inputParam6 = PARAM_INPUT_6;
        //   request->send(200, "text/html", "HTTP GET request sent to your ESP on input field (" + inputParam6 + ") with value: " + inputMessage6 + "<!DOCTYPE HTML><html><head><title>SVMS WEBSERVER</title><meta charset='UTF-8'><style> a {text-decoration: none;display: inline-block;padding: 8px 16px;text-align: right;}a:hover{background-color: #ddd;color: black;}.round {border-radius: 50%;}.Home_page {background-color: #4e6d52;color: rgb(240, 234, 234);padding: 10px;text-align: right;}.next {background-color: #04AA6D;color: white;}</style></head><body><div align = 'center'><br><a  href='/\' class='Home_page'>&laquo;Return to the Home page</a></div></body>");
        //   Serial.print("input Message = "); Serial.println(inputMessage6);
        // }
        // // GET input3 value on <ESP_IP>/get?input3=<inputMessage>
        // else if (request->hasParam(PARAM_INPUT_7)) 
        // {
        //   inputMessage7 = request->getParam(PARAM_INPUT_7)->value();
        //   inputParam7 = PARAM_INPUT_7;
        //   request->send(200, "text/html", "HTTP GET request sent to your ESP on input field (" + inputParam7 + ") with value: " + inputMessage7 + "<!DOCTYPE HTML><html><head><title>SVMS WEBSERVER</title><meta charset='UTF-8'><style> a {text-decoration: none;display: inline-block;padding: 8px 16px;text-align: right;}a:hover{background-color: #ddd;color: black;}.round {border-radius: 50%;}.Home_page {background-color: #4e6d52;color: rgb(240, 234, 234);padding: 10px;text-align: right;}.next {background-color: #04AA6D;color: white;}</style></head><body><div align = 'center'><br><a  href='/\' class='Home_page'>&laquo;Return to the Home page</a></div></body>");
        //   Serial.print("input Message = "); Serial.println(inputMessage7);
        // }
          else if (request->hasParam(PARAM_INPUT_8)) 
        {
          inputMessage8 = request->getParam(PARAM_INPUT_8)->value();
          inputParam8 = PARAM_INPUT_8;
          //request->send(200, "text/html", "PRESET PARAMETERS LIST:<br>" + (String)PARAM_INPUT_1 + " = " + inputMessage1 + "<br>" + (String)PARAM_INPUT_2 + " = " + inputMessage2 + "<br>" + (String)PARAM_INPUT_3 + " = " + inputMessage3 + "<br>" + (String)PARAM_INPUT_4 + " = " + inputMessage4 + "<br>" + (String)PARAM_INPUT_5 + " = " + inputMessage5 + "<br>" + (String)PARAM_INPUT_6 + " = " + inputMessage6 + "<br>" + (String)PARAM_INPUT_7 + " = " + inputMessage7  + "<br>" + "<!DOCTYPE HTML><html><head><title>SVMS WEBSERVER</title><meta charset='UTF-8'><style> a {text-decoration: none;display: inline-block;padding: 8px 16px;text-align: right;}a:hover{background-color: #ddd;color: black;}.round {border-radius: 50%;}.Home_page {background-color: #4e6d52;color: rgb(240, 234, 234);padding: 10px;text-align: right;}.next {background-color: #04AA6D;color: white;}</style></head><body><div align = 'center'><br><a  href='/\' class='Home_page'>&laquo;Return to the Home page</a></div></body>");
          request->send(200, "text/html", "PRESET PARAMETERS LIST:<br>" + (String)PARAM_INPUT_1 + " = " + inputMessage1 + "<br>" + (String)PARAM_INPUT_2 + " = " + inputMessage2 + "<br>" + (String)PARAM_INPUT_4 + " = " + inputMessage4 +  "<br>" + "<!DOCTYPE HTML><html><head><title>SVMS WEBSERVER</title><meta charset='UTF-8'><style> a {text-decoration: none;display: inline-block;padding: 8px 16px;text-align: right;}a:hover{background-color: #ddd;color: black;}.round {border-radius: 50%;}.Home_page {background-color: #4e6d52;color: rgb(240, 234, 234);padding: 10px;text-align: right;}.next {background-color: #04AA6D;color: white;}</style></head><body><div align = 'center'><br><a  href='/\' class='Home_page'>&laquo;Return to the Home page</a></div></body>");
          Serial.print("input Message = "); Serial.println(inputMessage8);
        }
         else if (request->hasParam(PARAM_INPUT_9)) 
        {
          inputMessage9 = request->getParam(PARAM_INPUT_9)->value();
          inputParam9 = PARAM_INPUT_9;
          presetValue1 = EEPROM.readString(SSID_ADD);
          presetValue2 = EEPROM.readString(PASS_ADD);
          //presetValue3 = EEPROM.readString(DATA_LENGTH_ADD);
          presetValue4 = EEPROM.readString(TX_COUNTER_ADD);
          // presetValue5 = EEPROM.readString(SLEEP_DUR_ADD);
          // presetValue6 = EEPROM.readString(CHUNK_NO_ADD);
          // presetValue7 = EEPROM.readString(SAMP_FREQ_ADD);
          // Serial.print("PRESET PARAMETERS LIST >> \r\n"); Serial.print("Wi-Fi ssid = ");Serial.println(presetValue1);Serial.print("Wi-Fi password = ");Serial.println(presetValue2);Serial.print("Datalength = ");Serial.println(presetValue3);Serial.print("TX counter = ");Serial.println(presetValue4);Serial.print("Sleep duration in ms = ");Serial.println(presetValue5);Serial.print("Number of data-chunk = ");Serial.println(presetValue6);Serial.print("Sampling frequency = ");Serial.println(presetValue7);
          // request->send(200, "text/html", "PRESET PARAMETERS LIST:<br>" + (String)PARAM_INPUT_1 + " = " + presetValue1 + "<br>" + (String)PARAM_INPUT_2 + " = " + presetValue2 + "<br>" + (String)PARAM_INPUT_3 + " = " + presetValue3 + "<br>" + (String)PARAM_INPUT_4 + " = " + presetValue4 + "<br>" + (String)PARAM_INPUT_5 + " = " + presetValue5 + "<br>" + (String)PARAM_INPUT_6 + " = " + presetValue6 + "<br>" + (String)PARAM_INPUT_7 + " = " + presetValue7  + "<br>" + "<!DOCTYPE HTML><html><head><title>SVMS WEBSERVER</title><meta charset='UTF-8'><style> a {text-decoration: none;display: inline-block;padding: 8px 16px;text-align: right;}a:hover{background-color: #ddd;color: black;}.round {border-radius: 50%;}.Home_page {background-color: #4e6d52;color: rgb(240, 234, 234);padding: 10px;text-align: right;}.next {background-color: #04AA6D;color: white;}</style></head><body><div align = 'center'><br><a  href='/\' class='Home_page'>&laquo;Return to the Home page</a></div></body>");
          Serial.print("PRESET PARAMETERS LIST >> \r\n"); Serial.print("Wi-Fi ssid = ");Serial.println(presetValue1);Serial.print("Wi-Fi password = ");Serial.println(presetValue2);Serial.print("TX counter = ");Serial.println(presetValue4);
          request->send(200, "text/html", "PRESET PARAMETERS LIST:<br>" + (String)PARAM_INPUT_1 + " = " + presetValue1 + "<br>" + (String)PARAM_INPUT_2 + " = " + presetValue2 + "<br>"  + (String)PARAM_INPUT_4 + " = " + presetValue4 +  "<br>" + "<!DOCTYPE HTML><html><head><title>SVMS WEBSERVER</title><meta charset='UTF-8'><style> a {text-decoration: none;display: inline-block;padding: 8px 16px;text-align: right;}a:hover{background-color: #ddd;color: black;}.round {border-radius: 50%;}.Home_page {background-color: #4e6d52;color: rgb(240, 234, 234);padding: 10px;text-align: right;}.next {background-color: #04AA6D;color: white;}</style></head><body><div align = 'center'><br><a  href='/\' class='Home_page'>&laquo;Return to the Home page</a></div></body>");
        }
         else if (request->hasParam(PARAM_INPUT_10)) 
        {
          inputMessage10 = request->getParam(PARAM_INPUT_10)->value();
          inputParam10 = PARAM_INPUT_10;
        
          check_1= inputMessage1.isEmpty();    check_2= inputMessage2.isEmpty();    check_3= inputMessage3.isEmpty();    check_4= inputMessage4.isEmpty();    check_5= inputMessage5.isEmpty();    check_6= inputMessage6.isEmpty();    check_7= inputMessage7.isEmpty();
          if ((check_1 != true) && (inputMessage1.length() <= 10) )
          {
            EEPROM.writeString(SSID_ADD,inputMessage1);
           
          }
          if ((check_2 != true) && (inputMessage2.length() <= 10))
          {
            EEPROM.writeString(PASS_ADD,inputMessage2);
      
          }
          // if (check_3 != true )
          // {
          //   EEPROM.writeString(DATA_LENGTH_ADD,inputMessage3);
          
          // }
          if (check_4 != true )
          {
            EEPROM.writeString(TX_COUNTER_ADD,inputMessage4);
          
          }
          // if (check_5 != true )
          // {
          //   EEPROM.writeString(SLEEP_DUR_ADD,inputMessage5);
            
          // }
          // if (check_6 != true )
          // {
          //   EEPROM.writeString(CHUNK_NO_ADD,inputMessage6);
          
          // }
          // if (check_7 != true )
          // {
          //   EEPROM.writeString(SAMP_FREQ_ADD,inputMessage7);
         
          // }
          EEPROM.writeChar(ST_DATA_SEND_ADD,'I');
          EEPROM.commit();

          request->send(200, "text/html", "PARAMETERS LIST UPDATED WITH NEW VALUES.<br><!DOCTYPE HTML><html><head><title>SVMS WEBSERVER</title><meta charset='UTF-8'><style> a {text-decoration: none;display: inline-block;padding: 8px 16px;text-align: right;}a:hover{background-color: #ddd;color: black;}.round {border-radius: 50%;}.Home_page {background-color: #4e6d52;color: rgb(240, 234, 234);padding: 10px;text-align: right;}.next {background-color: #04AA6D;color: white;}</style></head><body><div align = 'center'><br><a  href='/\' class='Home_page'>&laquo;Return to the Home page</a></div></body>");
        }
          else if (request->hasParam(PARAM_INPUT_11)) 
        {
          inputMessage11 = request->getParam(PARAM_INPUT_11)->value();
          inputParam11 = PARAM_INPUT_11;
          request->send(200, "text/html", "Restarting device <br> <!DOCTYPE HTML><html><head><title>SVMS WEBSERVER</title><meta charset='UTF-8'><style> a {text-decoration: none;display: inline-block;padding: 8px 16px;text-align: right;}a:hover{background-color: #ddd;color: black;}.round {border-radius: 50%;}.Home_page {background-color: #4e6d52;color: rgb(240, 234, 234);padding: 10px;text-align: right;}.next {background-color: #04AA6D;color: white;}</style></head><body><div align = 'center'><br><a  href='/\' class='Home_page'>&laquo;Return to the Home page</a></div></body>");
          ESP.restart();
        }
        else if (request->hasParam(PARAM_INPUT_12)) 
        { 
          // String ip;
          // IPAddress ipa;
          inputMessage12 = request->getParam(PARAM_INPUT_12)->value();
          inputParam12 = PARAM_INPUT_12;
          // peri.Start_WiFi();
          // if((WiFi.status() == WL_CONNECTED ))                                                //chexk the wifi connection ststus
          // {
          //  ipa =WiFi.localIP(); 
          //  ip = ipa.toString();
          // Serial.print("ip= ");Serial.println(ip);
          // AsyncElegantOTA.begin(&server);    // Start ElegantOTA
          // server.begin();
          // }
          AsyncElegantOTA.begin(&server);    // Start ElegantOTA
          server.begin();
          request->send(200, "text/html", "<!DOCTYPE HTML><html><head><title>SVMS WEBSERVER</title><meta charset='UTF-8'><style>p {background-color: #321c46;color: rgb(7, 243, 58);padding: 70px 150px;text-align: center;border-style:dotted;}body{background-color: #1c1f1e;}a {text-decoration: none;display: inline-block;padding: 8px 16px;text-align: right;}a:hover{background-color: #a1f7dd;color: rgb(9, 45, 248);}.Home_page{background-color: #eb688f;color: rgb(243, 239, 239);padding: 70px 150px;text-align: center;border-style:dashed;}.next {background-color: #11335f;color: rgb(255, 255, 255);padding: 70px 150px;text-align: center;border-style: dashed;}</style></head><body><h4><p><i>OTA HAS INITIATED...PLEASE VISIT OTA PAGE!</i></p></h4><br><br><br><br><div align = 'center'><br><br><br><br><br><br><a href='http://192.168.4.1/update' class='next'>**GO TO OTA PAGE**</a><br><br><br><br><br></div><br><br><br><br><div align = 'center'><br><a  href='/\' class='Home_page'>RETURN TO HOME PAGE</a><br></div></body>");
       
        }
        else 
        {
           request->send(200, "text/html", " <!DOCTYPE HTML><html><head><title>SVMS WEBSERVER</title><meta charset='UTF-8'><style> a {text-decoration: none;display: inline-block;padding: 8px 16px;text-align: right;}a:hover{background-color: #ddd;color: black;}.round {border-radius: 50%;}.Home_page {background-color: #4e6d52;color: rgb(240, 234, 234);padding: 10px;text-align: right;}.next {background-color: #04AA6D;color: white;}</style></head><body><div align = 'center'><br><a  href='/\' class='Home_page'>&laquo;Return to the Home page</a></div></body>");
        }
  
       
      });
        
      delay(180*1000);
      *flag = 1;
    }
               /**
  * @brief   web view 
  * @return   none
  */

//"<br><a style='border:2px; background-color:'#2bc9b4'' href=\"/\" >Return to Home Page</a>"

           /**
  * @brief   OneTimeSend and flash EEPROM memory
  * @return   none
  */
    void PERIPHERAL :: OneTimeSend(void)
    {
      if ((digitalRead(SETUP_PIN) == LOW) && ( EEPROM.readChar(ST_DATA_SEND_ADD) == 'I'))
        delay(20);                                                       // for Debouncing check
      if ((digitalRead(SETUP_PIN) == LOW) && ( EEPROM.readChar(ST_DATA_SEND_ADD) == 'I'))
      {
        uint8_t status_flag;  
        uint8_t success_flag=2;  
        PERIPHERAL peripheral; 
        UUID uuid; 
        uint32_t val = random(0,200);

        EEPROM.writeChar(ST_DATA_SEND_ADD,'F');           
        EEPROM.commit();
        iis3dwb.IIS3DWB_init();                                                                //if sensor found then initialize the sensor 
        iis3dwb.iis3dwb_device_id_check(2,&status_flag);                                      //check the connected sensor id 
        iis3dwb.auto_scale();
        peripheral.Start_WiFi();
        if((WiFi.status() == WL_CONNECTED ))                                                //chexk the wifi connection ststus
        {
          delay(50);
          peripheral.flashing_led(LED_PIN1,2); 
          uuid.seed(val);
          uuid.generate();
          String gen_uuid =  uuid.toCharArray();
          peripheral.SENSOR_data_write(X_axis,gen_uuid,&success_flag,2,02);                       
          iis3dwb.iis3dwb_acc_power_down();
          peripheral.Stop_WiFi();
          ESP.deepSleep(SLEEP_DURATION);
        }
        iis3dwb.iis3dwb_acc_power_down();
        Serial.println("Wifi not available,going into deep sleep mode ");
        ESP.deepSleep(SLEEP_DURATION);
      }
      //flash EEPROM memory 
      if((digitalRead(GPIO_NUM_20) == LOW) && (digitalRead(SETUP_PIN) == LOW))
        delay(20);                                                               // for Debouncing check
      if((digitalRead(GPIO_NUM_20) == LOW) && (digitalRead(SETUP_PIN) == LOW))
        {
          EEPROM.writeString(SSID_ADD,"");
          EEPROM.writeString(PASS_ADD,"");
          EEPROM.writeString(DATA_LENGTH_ADD,"");
          EEPROM.writeString(TX_COUNTER_ADD,"");
          EEPROM.writeString(SLEEP_DUR_ADD,"");
          EEPROM.writeString(CHUNK_NO_ADD,"");
          EEPROM.writeString(SAMP_FREQ_ADD,"");
          EEPROM.writeChar(DEFAULT_PARAM_ADD,'0');
          EEPROM.writeChar(ST_DATA_SEND_ADD,'0');
          EEPROM.commit();
         
          presetValue1 = EEPROM.readString(SSID_ADD);
          presetValue2 = EEPROM.readString(PASS_ADD);
          presetValue3 = EEPROM.readString(DATA_LENGTH_ADD);
          presetValue4 = EEPROM.readString(TX_COUNTER_ADD);
          presetValue5 = EEPROM.readString(SLEEP_DUR_ADD);
          presetValue6 = EEPROM.readString(CHUNK_NO_ADD);
          presetValue7 = EEPROM.readString(SAMP_FREQ_ADD);
          Serial.print("PRESET PARAMETERS LIST >> \r\n"); Serial.print("Wi-Fi ssid = ");Serial.println(presetValue1);Serial.print("Wi-Fi password = ");Serial.println(presetValue2);Serial.print("Datalength = ");Serial.println(presetValue3);Serial.print("TX counter = ");Serial.println(presetValue4);Serial.print("Sleep duration in ms = ");Serial.println(presetValue5);Serial.print("Number of data-chunk = ");Serial.println(presetValue6);Serial.print("Sampling frequency = ");Serial.println(presetValue7);
        }
    }


    void PERIPHERAL :: GPIO_Status( uint8_t* val)
    {
      if (digitalRead(IIS3DWB_MISO) == LOW)
        delay(10);                                                       // for Debouncing check
      if (digitalRead(IIS3DWB_MISO) == LOW)
      {
        *val++;
      }
    }