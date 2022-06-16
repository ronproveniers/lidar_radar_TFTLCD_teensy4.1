/*
   Lidar development on teensy 4.x
   use lidar  from EAI mb-1r2t
   main supply 5VDC
   uart 3V3 153600 BAUD

   serial protocol:
   SYNC   2 bytes 0xaa55
   CT|LSN 2 bytes ct= frame type 0= normal data type  |= 0 zero frame= start of measurement. LSN lenght of data in a frame
   FSA    2 bytes starting angle in this frame.
   LSA    2 bytes ending angle in this frame. from both the the difference anfle can be calculated and the (rotation) angle calculate of the data in the frame
   CS     2 bytes checksum XOR of all bytes
   after this the data follows. 3bytes (we have intensity byte
   S1     1 byte intensity
   S2     1 byte lsb distance
   S3     1 byte msb distance
   data(s1,s2,s3) repeated LSN times, so total bytes in data part of the frame is LSNx3 bytes

   formulae:

   intensity = uint16_t((S(1) & 0x03)<< 8 | S(0));
   distance  = uint16_t(S(2) << 8 | S(1)) >> 2

   Angle_FSA = (FSA >> 1) / 64;
   Angle_LSA = (LSA >> 1) / 64;
   angle_diff:　Angle_LSA - Angle_FSA If less than zero, diff_angle = (Angle_LSA- Angle_FSA) + 360, otherwise diff_Angle = Angle_LSA- Angle_FSA

   calculate angle per (s1,s2,s3) in one frame:
   Angle[i] = i* angle_diff / (LSN - 1) + Angle_FSA;// i- index of data in one frame

   there are 36 frames in one lidar rotation and 36 X 40=1440 data points of distance and intensity

   in null frame CT byte == rotation speed=>  a = float (rotation_speed); lidar_speed = a * 3.0000;
   if hispeed rotation the angel formulae must be adapted.

   use TFT_eSPI lib from github bodmer for 3.5 " tft lcd display with ILI9488 controoler 480x320
   use hardware serial 2 on teensy, enlarge tx rx buffers in teensyduion serial lib

   use A6 to measure 5VDC is on LIDAR
   WORKING on HISPEED!!!! else give wrong readings at around 330 degrees (distance =1000 all the time)!!
   TO DO:
  - implement alarm zones
  - determine if object is moving in a zone
  - object recognition

   https://github.com/Bodmer/TFT_eSPI

   https://github.com/YDLIDAR/YDLidar-SDK

   https://github.com/ardnew/XPT2046_Calibrated/blob/master/src/XPT2046_Calibrated.h    TO BE IMPLEMENTED when using touchsreen!!!

   to use TFT_eSPI for the 3.5" tft lcd display with ILI9488 driver, the following changes have to be made to the library (TFT_eSPI)

1- to the file User_Select_Setup.h (library TFT_eSPI)
 add this:
 #ifndef USER_SETUP_LOADED //  Lets PlatformIO users define settings in
                          //  platformio.ini, see notes in "Tools" folder.

// Only ONE line below should be uncommented.  Add extra lines and files as needed.
#include <../Ron_TFTeSPI_Setups/TFTeSPI_setup_teensy_ili9488.h> 

......

2-create a map in the library folder named: Ron_TFTeSPI_Setups
 
add the file : TFTeSPI_setup_teenst_ili9488.h in this map with the following content:

#define ILI9488_DRIVER
#define TFT_MISO 11
#define TFT_MOSI 12
#define TFT_SCLK 13
#define TFT_CS   10  // Chip select control pin
#define TFT_DC    9 // Data Command control pin
#define TFT_RST  -1  // Set TFT_RST to -1 if display RESET is connected to ESP32 board RST
#define TOUCH_CS 8     // Chip select pin (T_CS) of touch screen
#define LOAD_GLCD   // Font 1. Original Adafruit 8 pixel font needs ~1820 bytes in FLASH
#define LOAD_FONT2  // Font 2. Small 16 pixel high font, needs ~3534 bytes in FLASH, 96 characters
#define LOAD_FONT4  // Font 4. Medium 26 pixel high font, needs ~5848 bytes in FLASH, 96 characters
#define LOAD_FONT6  // Font 6. Large 48 pixel font, needs ~2666 bytes in FLASH, only characters 1234567890:-.apm
#define LOAD_FONT7  // Font 7. 7 segment 48 pixel font, needs ~2438 bytes in FLASH, only characters 1234567890:-.
#define LOAD_FONT8  // Font 8. Large 75 pixel font needs ~3256 bytes in FLASH, only characters 1234567890:-.
#define LOAD_GFXFF  // FreeFonts. Include access to the 48 Adafruit_GFX free fonts FF1 to FF48 and custom fonts

#define SMOOTH_FONT
#define SPI_FREQUENCY  30000000
#define SPI_READ_FREQUENCY  20000000
#define SPI_TOUCH_FREQUENCY  2500000
......

 thats it ...  

 the following libraries should be in the library folder now:
 SPI
 TFTeSPI
 XPT2046_touchscreen
 Ron_TFTeSPI_Setups

 make sue the arduino sketch has the follwing tabs:
 Free_Fonts.h
 defines.h
 variables.h
 radar
 touch
 ..

 used arduino ide 1.8.19
 teensyduino 1.56

 don't FORGET this is still under development!!!!!!
 Good Luck
 
*/

#include <SPI.h>
#include <TFT_eSPI.h>
#include <XPT2046_Touchscreen.h>
#include "Free_Fonts.h"
#include "defines.h"
#include "variables.h"
TFT_eSPI tft = TFT_eSPI();
XPT2046_Touchscreen ts(CS_PIN);

void setup() {
  pinMode(debug_pin1, OUTPUT);
  pinMode(debug_pin2, OUTPUT);
  pinMode(debug_pin3, OUTPUT);
  pinMode(lidar_power_pin, INPUT);
  frame_state = frame_start;
  Serial.begin(230400);
  Serial2.begin(153600);//used for lidar TX
  Serial2.setTimeout(1);
  digitalWriteFast(debug_pin1, LOW);
  digitalWriteFast(debug_pin2, LOW);
  digitalWriteFast(debug_pin3, LOW);
  tft.begin();
  tft.setRotation(1);
  tft.fillScreen(BLACK);
  tft.setTextSize(2);
  tft.setCursor(320, 4);
  tft.println("LIDAR plot");
  tft.setCursor(320, 24);
  tft.println("max 1 meter");
  tft.setCursor(320, 44);
  tft.println("10 cm/mark");
  lidar_draw_raster2();
  ts.begin();
  ts.setRotation(3);
  redBtn();
  Serial.println("lidar startup message");
  delay(1000);
}

void loop() {
  if (Serial2.available())
  {
    //    digitalWriteFast(debug_pin3, HIGH);
    //    delayMicroseconds(10);
    //    digitalWriteFast(debug_pin3, LOW);
    //FSM decoding lidar data
    switch (frame_state)
    {
      case frame_start://find syncs
        lidar_read = Serial2.read();
        if (lidar_read == sync1)
        {
          frame_state = frame_sync;
          break;
        }
        break;

      case frame_sync://find syncs
        lidar_read = Serial2.read();
        if (lidar_read == sync2)
        {
          frame_state = frame_msg_type;
          digitalWriteFast(debug_pin1, HIGH);
          break;
        }
        break;

      case frame_msg_type://msg type
        lidar_read = Serial2.read();
        msg_type = lidar_read & 0x01;
        if (msg_type == 1) {
          digitalWriteFast(debug_pin2, HIGH);
          delayMicroseconds(10);
          digitalWriteFast(debug_pin2, LOW);

          rotation_speed = lidar_read;
          test_count++;
          cpu_load = (idle_count / 350.00) * 100.00 * idle_delay;
          Serial.print(" cpu load : "); Serial.println(cpu_load, 2);
          idle_count = 0;
          if (test_count > 6)
          {
            tft.fillCircle(XX, YY, 135, BLACK);
            tft.fillCircle(XX, YY, 4, GREEN);
            plot_distance_circles();
            plot_x_y_axes();
            test_count = 0;
          }
          plot_rotation_speed(lidar_speed, LIGHTGREY , YELLOW);
          buffer_pointer = 0;
          //       Serial.print(" frame count: "); Serial.println(frame_count-1);
          frame_count = 0;
        }
        else
        {
          frame_count++;
        }
        frame_state = frame_msg_lengt;
        break;

      case frame_msg_lengt://message lenght
        lidar_read = Serial2.read();
        frame_lenght = lidar_read * 3;
        buffer_pointer = 0;
        frame_state = frame_start_a;
        break;

      case frame_start_a://FSA
        lidar_read = Serial2.read();
        frame_state = frame_start_b;
        break;

      case frame_start_b:
        lidar_read1 = Serial2.read();
        frame_state = frame_start_c;
        break;

      case frame_start_c:
        start_angle = (lidar_read1 << 8) + lidar_read;
        frame_state = frame_end_a;
        break;

      case frame_end_a://LSA
        lidar_read = Serial2.read();
        frame_state = frame_end_b;
        break;

      case frame_end_b:
        lidar_read1 = Serial2.read();
        frame_state = frame_end_c;
        break;

      case frame_end_c:
        end_angle = (lidar_read1 << 8) + lidar_read;
        Angle_FSA = (start_angle >> 1) / 64;
        Angle_LSA = (end_angle >> 1) / 64;
        Angle_diff = Angle_LSA - Angle_FSA;
        if (Angle_diff < 0)
        {
          Angle_diff = Angle_diff + 360;
        }
        frame_state = frame_cs1;
        break;

      case frame_cs1://cs
        lidar_read = Serial2.read();
        frame_state = frame_cs2;
        break;

      case frame_cs2://cs
        if (Serial2.available() )
        {
          lidar_read1 = Serial2.read();
          frame_checksum = (lidar_read1 << 8) + lidar_read;
          frame_state = frame_load_frame_data;
          break;
        }

      case  frame_load_frame_data:// check for end of frame, if not keep up loading data
        if (buffer_pointer >= frame_lenght - 1 ) {
          digitalWriteFast(debug_pin1, LOW);
          frame_state = frame_end;
          break;
        }
        lidar_read = Serial2.read();
        buffer[buffer_pointer] = lidar_read;
        buffer_pointer++;
        break;

      case frame_end:// do some data processing
               if (( msg_type == 0) && (frame_lenght > 2)) { // only if normal frame
           for (int i = 0; i < frame_lenght; i = i + 3)
          {
            data = (buffer[i + 2] << 8 | buffer[i + 1]);
            intensity = ((buffer[i + 1] & 0x03) << 8 |  buffer[i]);
            distance = data >> 2;
            Angle = i * Angle_diff / (frame_lenght - 1) + Angle_FSA;
            if (Angle >= 360) {
              Angle -= 360;
            }
            if ((Angle > 200) && (Angle < 220)) {
              digitalWriteFast(debug_pin3, HIGH);
              delayMicroseconds(10);
              digitalWriteFast(debug_pin3, LOW);
        //      Serial.print(buffer[i + 2],HEX);Serial.print(" ");Serial.println(buffer[i + 1],HEX);
              Serial.print("angle: ");Serial.print(Angle,2);Serial.print(" distance: ");Serial.println(distance,2);
            }

            //            // process 4 alarm area's
            //            if ((distance < 1180) && (Angle >= frame1_s) && (Angle <= frame1_e) && (intensity > 10))
            //            {
            //              //   Serial.print("angle: "); Serial.print(Angle, 2);
            //              //   Serial.print("  distance: "); Serial.print(float(distance), 2);
            //              //   Serial.print("  intensity: "); Serial.println(intensity, DEC);
            //              if (distance < 800)
            //              {
            //           //     object = (object * .1) + 0.9 * (1180 - distance) * intensity / 100;
            //              }
            //         //     plotDistanceMap(Angle, distance, intensity, CYAN);
            //            }
            //            if ((distance < 1180) && (Angle >= frame1a_s) && (Angle <= frame1a_e) && (intensity > 100))
            //            {
            //              //   Serial.print("angle: "); Serial.print(Angle, 2);
            //              //   Serial.print("  distance: "); Serial.print(float(distance), 2);
            //              //   Serial.print("  intensity: "); Serial.println(intensity, DEC);
            //              if (distance < 800)
            //              {
            //                object = (object * .1) + 0.9 * (1180 - distance) * intensity / 100;
            //              }
            //           //   plotDistanceMap(Angle, distance, intensity, CYAN);
            //            }
            //            if ((distance < 1180) && (Angle >= frame2_s) && (Angle <= frame2_e) && (intensity > 100))
            //            {
            //              if (distance < 800)
            //              {
            //                object = (object * .1) + 0.9 * (1180 - distance) * intensity / 100;
            //              }
            //          //    plotDistanceMap(Angle, distance, intensity, CYAN);
            //            }
            //
            //            if ((distance < 1180) && (Angle >= frame3_s ) && (Angle <= frame3_e) && (intensity > 100))
            //            {
            //              if (distance < 800)
            //              {
            //                object = (object * .1) + 0.9 * (1180 - distance) * intensity / 100;
            //              }
            //         //     plotDistanceMap(Angle, distance, intensity, CYAN);
            //            }
            //
            //            if ((distance < 1180) && (Angle > frame4_s ) && (Angle <= frame4_e ) && (intensity > 100))
            //            {
            //              if (distance < 800)
            //              {
            //                object = (object * .1) + 0.9 * (1180 - distance) * intensity / 100;
            //              }
            //          //    plotDistanceMap(Angle, distance, intensity, CYAN);
            //            }
            //
            //            //  Serial.print("  object: "); Serial.print(object, 2); Serial.print(" object_saved : "); Serial.println(object_saved, 2);
            //            if ((object - object_saved) >= 1600) {
            //        //      Serial.println("  object moving ");
            //            }
            //            object_saved = object;
            //            if ((distance < 1180) && (Angle > frame4_s ) && (Angle <= frame4_e ) && (intensity > 100))
            //            {
            //              //   Serial.print("angle: "); Serial.print(Angle, 2);
            //              //   Serial.print("  distance: "); Serial.print(float(distance), 2);
            //              //   Serial.print("  intensity: "); Serial.println(intensity, DEC);
            //          //    plotDistanceMap(Angle, distance, intensity, CYAN);
            //            }
            if (SwitchOn == 0)
            {
              //    if ((distance < 1180) && (Angle >= 0 ) && (Angle <= 360) && (intensity > 10))
              {
 //               Serial.print("angle: "); Serial.print(Angle, 2);
 //               Serial.print("  distance: "); Serial.print(float(distance), 2);
 //               Serial.print("  intensity: "); Serial.println(intensity, DEC);
                plotDistanceMap(Angle, distance, intensity, CYAN);
              }
            }
          }
        }
        if (msg_type == 1) {
          a = float (rotation_speed); lidar_speed = a * 3.0000;
        }
        frame_lenght = 0;
        buffer_pointer = 0;
        frame_state = frame_start;
        break;

      case frame_lidar_pwr_loss:
        Serial.println("lidar power loss!!!");
        if (analogRead(lidar_power_pin) >= 400) {
          Serial.println("lidar power restored");
          frame_state = frame_start;
          break;
        }
        else
        {
          delay(1000);
        }
        break;
      default:
        break;
    }
  }
  else
  {
    idle_count++;
       touch();
       delay(idle_delay);
  }
}
