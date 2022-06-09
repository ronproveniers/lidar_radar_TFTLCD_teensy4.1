

void touch(void)
{
  uint16_t x, y, z;
  // Serial.println("touch");
  if (ts.touched()) {
    TS_Point p = ts.getPoint();
    x = p.x; y = p.y; z = p.z;
  //  Serial.print("Pressure = ");
  //  Serial.println(z);
    //    Serial.print(", x = ");
    //    Serial.print(x);
    //    Serial.print(", y = ");
    //    Serial.print(y);
    //    //  delay(30);
    //    Serial.println();
    x = float(3860 - x) / float (7.4); y = (y - 600) / 10;
#ifdef BLACK_SPOT
    tft.fillCircle(x, y, 2, WHITE);
    delay(100);
    tft.fillCircle(x, y, 2, BLACK);
#endif
    if (SwitchOn)
    {
      if ((x > REDBUTTON_X) && (x < (REDBUTTON_X + REDBUTTON_W))) {
        if ((y + 20 > REDBUTTON_Y) && (y + 20 <= (REDBUTTON_Y + REDBUTTON_H))) {
          //         Serial.println("Red btn hit");
          redBtn();
        }
      }
    }
    else //Record is off (SwitchOn == false)
    {
      if ((x > GREENBUTTON_X) && (x < (GREENBUTTON_X + GREENBUTTON_W))) {
        if ((y + 20 > GREENBUTTON_Y) && (y + 20 <= (GREENBUTTON_Y + GREENBUTTON_H))) {
          //         Serial.println("Green btn hit");
          greenBtn();
        }
      }
    }
    //   Serial.println(SwitchOn);
  }
}
//------------------------------------------------------------------------------------------

void touch_calibrate()
{
  uint16_t calData[5] = {285, 3349, 478, 2571, 5};
  // tft.setTouch(calData);
  //  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  //  tft.println("Calibration complete!");
  // Serial.print(calData[0]); Serial.print(":"); Serial.print(calData[1]); Serial.print(":"); Serial.print(calData[2]);
  // Serial.print(":"); Serial.print(calData[3]); Serial.print(":"); Serial.print(calData[4]); Serial.print(":"); Serial.print(calData[5]);
}



void drawFrame()
{
  tft.drawRoundRect(FRAME_X, FRAME_Y, FRAME_W, FRAME_H, 6, TFT_BLACK);
  tft.setCursor(FRAME_X,FRAME_Y-20);
  tft.setTextColor(GREEN);
  tft.setTextSize(2);
  tft.print("Alarm zone");
}

// Draw a red button
void redBtn()
{
  tft.fillRoundRect(REDBUTTON_X, REDBUTTON_Y, REDBUTTON_W, REDBUTTON_H, 6, TFT_RED);
  tft.fillRoundRect(GREENBUTTON_X, GREENBUTTON_Y, GREENBUTTON_W, GREENBUTTON_H, 6, TFT_DARKGREY);
  drawFrame();
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(2);
  tft.setTextDatum(MC_DATUM);
  tft.drawString("ON", GREENBUTTON_X + (GREENBUTTON_W / 2), GREENBUTTON_Y + (GREENBUTTON_H / 2));
  SwitchOn = false;
}

// Draw a green button
void greenBtn()
{
  tft.fillRoundRect(GREENBUTTON_X, GREENBUTTON_Y, GREENBUTTON_W, GREENBUTTON_H, 6, TFT_GREEN);
  tft.fillRoundRect(REDBUTTON_X, REDBUTTON_Y, REDBUTTON_W, REDBUTTON_H, 6, TFT_DARKGREY);
  drawFrame();
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(2);
  tft.setTextDatum(MC_DATUM);
  tft.drawString("OFF", REDBUTTON_X + (REDBUTTON_W / 2) + 1, REDBUTTON_Y + (REDBUTTON_H / 2));
  SwitchOn = true;
}
