

void plotDistanceMap(float degree, float distance, uint16_t intensity, uint16_t color)
{
  float x, y;
  uint16_t color1 = color;
  if (distance < 500) {
  //  color1 = RED;
  }
  else if (distance > 700) {
 //   color1 = GREEN;
  }
  distance = distance * 4; //fuzz factor to adapt to 1 meter on display scale 1.2
  if(distance>1000){distance=1280;}
  x = (distance / 10) * cos(degree * M_PI / 180) + XX;
  y = (distance / 10) * sin(degree * M_PI / 180) + YY;
  tft.fillCircle(x, y, 1, color1);
}

void plot_degrees(uint8_t degree, uint16_t background_color, uint16_t foreground_color) // print angle
{
  tft.fillRoundRect(XX + 97, YY + 135, 90, 60, 5, background_color );
  tft.setCursor(XX + 99, YY + 97); tft.setTextSize(2); tft.setTextColor(foreground_color); tft.print((float)degree, 2); tft.print(G);
}

void plot_rotation_speed(uint16_t speed, uint16_t background_color, uint16_t foreground_color)
{
  tft.fillRoundRect(XX + 97, YY + 135, 200, 20, 5, background_color );
  tft.setCursor(XX + 97, YY + 137); tft.setTextSize(2); tft.setTextColor(foreground_color); tft.print( " rotation:"); tft.print(speed); tft.print("RPM");
}


void plot_text(char *text, uint16_t x, uint16_t y, uint16_t background_color, uint16_t foreground_color)
{
  tft.fillRoundRect(x, y, 90, 60, 5, background_color );
  tft.setCursor(x + 10, y + 10); tft.setTextSize(2); tft.setTextColor(foreground_color); tft.print(text);
}


void lidar_draw_raster2(void)
{
  //__________Drawing the edge circle for the radar_____________________
  tft.fillCircle(XX, YY, 158, WHITE);
  tft.fillCircle(XX, YY, 156, BLACK);
  //__________Drawing the white lines 10° for the radar_________________
  int i = 0; int angle = 0;
  float xi, yi;
  for (i = 0; i <= 37; i++) {
    xi = (XX) + (RAD) * cos(angle * M_PI / 180);
    yi = YY + (RAD) * sin(angle * M_PI / 180);
    tft.drawLine(xi, yi, XX, YY, WHITE);
    angle += 10;
  }
  //__________Drawing the white lines 5° for the radar__________________
  int ii = 0; angle = 0;
  float xii, yii;
  for (ii = 0; ii <= 74; ii++) {
    xii = (XX) + (RAD - 10) * cos(angle * M_PI / 180);
    yii = YY + (RAD - 10) * sin(angle * M_PI / 180);
    tft.drawLine(xii, yii, (XX), YY, WHITE);
    angle += 5;
  }
  //   //__________Drawing the edge circle for the radar_____________________
  tft.fillCircle(XX, YY, RAD - 15, GREENYELLOW);
  //   //__________Drawing the black circle for the moving dot_________________
  tft.fillCircle(XX, YY, RAD - 18, BLACK);
  //   //__________Drawing 4 major directions  for the RADAR______________________
  tft.setTextSize(1); tft.setTextColor(WHITE);
  tft.setCursor(XX - 178, YY - 4); tft.print("180");
  tft.setCursor(XX + 162, YY - 2); tft.print("360");
  tft.setCursor(XX - 5, YY - 155); tft.print("270");
  tft.setCursor(XX - 5, YY + 145); tft.print("90");
  //   //__________Drawing every 30° on the RADAR___________________________
  tft.setTextSize(1); tft.setTextColor(CYAN);
  tft.setCursor(XX + 140, YY + 80); tft.print("30");
  tft.setCursor(XX + 140, YY - 80); tft.print("330");
  tft.setCursor(XX - 160, YY + 78); tft.print("150");
  tft.setCursor(XX - 160, YY - 85); tft.print("210");
  tft.setCursor(XX + 83, YY + 138); tft.print("60");
  tft.setCursor(XX + 85, YY - 142); tft.print("300");
  tft.setCursor(XX - 100, YY + 140); tft.print("120");
  tft.setCursor(XX - 100, YY - 142); tft.print("240");
  tft.fillCircle(XX, YY, 4, GREEN);
  plot_x_y_axes();
  plot_distance_circles();
}

void plot_x_y_axes(void)
{
  tft.setTextSize(1);
  tft.setTextColor(WHITE);
  uint16_t xs, xe, ys, ye;
  xs = XX - RAD - 2 + 18; xe = XX + RAD - 18;
  ys = YY - RAD - 2 + 18; ye = YY + RAD - 18;
  tft.drawLine(xs, YY, xe, YY, GREEN);
  tft.drawLine(XX, ys , XX, ye, GREEN);
}


void plot_x_y_axes_extra(void)
{
  tft.setTextSize(1);
  tft.setTextColor(WHITE);
  uint16_t xs, xe, ys, ye, n = 0;
  xs = XX - RAD - 2 + 18; xe = XX + RAD - 18;
  ys = YY - RAD - 2 + 18; ye = YY + RAD - 18;
  for (int i = xs; i <= xe; i = i + ((xe - xs) / 19))
  {
    //   tft.drawLine(i, YY + 10, i, YY - 10, CYAN);
    tft.setCursor(i + 1, YY + 2);
    tft.print(scale_array[n++]);
  }
  for (int i = ys; i <= ye; i = i + ((ye - ys) / 19))
  {
    //    tft.drawLine(XX - 10, i, XX + 10, i, CYAN);
  }
  tft.drawLine(xs, YY, xe, YY, GREEN);
  tft.drawLine(XX, ys , XX, ye, GREEN);
}




void plot_distance_circles(void)
{
  float xs, xe;
  xs = 14; xe = RAD - 18;
  for (int i = xs; i <= xe; i = i + ((xe - xs) / 8.5))
  {
    tft.drawCircle(XX, YY, i, CYAN);
  }
}
