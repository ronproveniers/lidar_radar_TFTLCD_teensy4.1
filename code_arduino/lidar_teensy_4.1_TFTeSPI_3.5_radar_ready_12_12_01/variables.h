//TFT LCD 480x320 3.5" ILI9488
uint16_t XX = 180 , YY = 160 ; // middle of the tft screen
uint16_t X0 = 0 , Y0 = 0 ;     // start position of the tft screen
uint16_t X1 = 0 ;              //second start position on the tft
uint16_t count = 0;
int scan_diameter = 135;



uint8_t lidar_read;
uint8_t lidar_read1;
static uint16_t frame_state = frame_start;
uint16_t buffer[2000];
uint16_t frame_lenght = 0;
uint16_t  buffer_pointer = 0;
uint16_t char_count = 0;
uint16_t start_angle;
uint16_t end_angle;
uint16_t frame_checksum;
uint16_t data;
uint8_t msg_type;
float distance;
uint16_t intensity;
float Angle_FSA;
float Angle_LSA;
float Angle_diff;
float Angle;
uint16_t frame_count = 0;
uint8_t rotation_speed = 0;
float a;
float lidar_speed;
float value_smooth = 0;
float smooth_cooficient = .6;
float real_average = 0;
uint16_t test_count = 0;
String scale_array[10] = {"100", "90", "80", "70", "60", "50", "40", "30", "20", "10"};
bool SwitchOn = 0;

long idle_count = 0;
float cpu_load = 0;
uint16_t idle_delay = 1;


float object = 0;
float object_saved = 0;
