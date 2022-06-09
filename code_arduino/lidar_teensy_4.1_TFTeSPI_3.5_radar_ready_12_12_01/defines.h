
#define CS_PIN  8
// lcd radar
#define RAD 156

//define states of FSM
#define frame_start           1
#define frame_sync            2
#define frame_msg_type        3
#define frame_msg_lengt       4
#define frame_start_a         5
#define frame_start_b         6
#define frame_start_c         7
#define frame_end_a           8
#define frame_end_b           9
#define frame_end_c           10
#define frame_cs1             11
#define frame_cs2             12
#define frame_load_frame_data 13
#define frame_end             14
#define frame_lidar_pwr_loss  15

#define sync1 0xaa
#define sync2 0x55
#define debug_pin1     16
#define debug_pin2     14
#define debug_pin3     15
#define lidar_rx_pin    9 //teensy4.x
#define lidar_tx_pin    10 // ,,
#define lidar_power_pin A6 //measure lidar unit power

#define BLACK       0x0000
#define NAVY        0x000F
#define DARKGREEN   0x03E0
#define DARKCYAN    0x03EF
#define MAROON      0x7800
#define PURPLE      0x780F
#define OLIVE       0x7BE0
#define LIGHTGREY   0xC618
#define DARKGREY    0x7BEF
#define BLUE        0x001F
#define GREEN       0x07E0
#define CYAN        0x07FF
#define RED         0xF800
#define MAGENTA     0xF81F
#define YELLOW      0xFFE0
#define WHITE       0xFFFF
#define ORANGE      0xFD20
#define GREENYELLOW 0xAFE5
#define PINK        0xF81F
#define G ((char)247)


//#define BLACK_SPOT


// define alarm area's in degrees
#define frame1_s  355
#define frame1_e  360
#define frame1a_s 0
#define frame1a_e 5
#define frame2_s  85
#define frame2_e  95
#define frame3_s  175
#define frame3_e  185
#define frame4_s  265
#define frame4_e  275


// define for button switch

// Switch position and size
#define FRAME_X 350
#define FRAME_Y 100
#define FRAME_W 120
#define FRAME_H 50

// Red zone size
#define REDBUTTON_X FRAME_X
#define REDBUTTON_Y FRAME_Y
#define REDBUTTON_W (FRAME_W/2)
#define REDBUTTON_H FRAME_H

// Green zone size
#define GREENBUTTON_X (REDBUTTON_X + REDBUTTON_W)
#define GREENBUTTON_Y FRAME_Y
#define GREENBUTTON_W (FRAME_W/2)
#define GREENBUTTON_H FRAME_H
