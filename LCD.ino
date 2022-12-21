/***********************************
* LCD_print_HELLO();
* void LCD_print_HELLO(void)
************************************/
  void LCD_print_HELLO(void)
  {
    lcd.setCursor(0, 0);
    lcd.print("   Sensor pH");
    lcd.setCursor(0, 1);
    lcd.print("FIME-FACMED-UANL");
    delay(2000);
  }
/***********************************
* LCD_print_CALIB();
* void LCD_print_CALIB(void)
************************************/
  void LCD_print_CALIB(void)
  {
    lcd.setCursor(0, 0);//c,f
    lcd.print("Desea calibrar?");
    lcd.setCursor(0, 1);//c,f
    lcd.print("UP= si  DOWN= no");
  }
/***********************************
* Botones _LCD:
* read_LCD_buttons();
* int read_LCD_buttons(void)  
************************************/
int read_LCD_buttons(void)  
  { 
    int adc_key_in = analogRead(buttons_pin);      // Leemos A0
    // Y ahora los comparamos con un margen comodo
    if (adc_key_in < 60)        return button_right; 
    else if (adc_key_in < 200)  return button_up;
    else if (adc_key_in < 400)  return button_down;
    else if (adc_key_in < 600)  return button_left;
    else if (adc_key_in > 900)  return button_none;     // Ningun boton pulsado 
    //return button_none;  // Por si todo falla
  }
/***********************************
* LCD_print_ph_Value();
* void LCD_print_ph_Value(float ph_actual)
************************************/
  void LCD_print_ph_Value(float ph_actual)
  {
    lcd.setCursor(0, 0);
    lcd.print("Valor de Ph");
    lcd.setCursor(0, 1);
    lcd.print("Actual: ");
    lcd.setCursor(7, 1);
    lcd.print(ph_actual);
    LCDTime = millis();//Actualiza tiempo actual
  } 
/***********************************
* LCD_print_sensor_CLEAN();
* void LCD_print_sensor_CLEAN(void)
************************************/
void LCD_print_sensor_CLEAN(void)
{
  lcd.setCursor(0, 0);
  lcd.print("Limpia el sensor");
  lcd.setCursor(0, 1);
  lcd.print("UP= listo");
}
/***********************************
* LCD_print_ph4_WAIT();
* void LCD_print_ph4_WAIT(void)
************************************/
  void LCD_print_ph4_WAIT(void)
  {
    lcd.setCursor(0, 0);
    lcd.print("Coloca un buffer");
    lcd.setCursor(0, 1);
    lcd.print("de pH4");
    lcd.setCursor(7, 1);
    lcd.print("UP= listo");
  }

/***********************************
* LCD_print_1min_WAIT();
* void LCD_print_1min_WAIT(void)
************************************/
void LCD_print_1min_WAIT(void)
{
  lcd.clear();//limpiar
  lcd.setCursor(0, 0);
  lcd.print("Espera 1 min");
  lcd.setCursor(0, 1);
  lcd.print("por favor");
  delay(2000); 
  lcd.clear();//limpiar
}

/***********************************
* LCD_print_ph4_RUN();
* void LCD_print_ph4_RUN()(void)
************************************/
void LCD_print_ph4_RUN(int segundero, float pH_actual)
{
    lcd.setCursor(0, 0);
    lcd.print("Midiendo pH...");
    lcd.setCursor(0, 1);
    lcd.print("tiempo:");
    lcd.setCursor(7, 1);
    lcd.print(segundero);//Mostrar variable
    lcd.setCursor(10, 1);
    lcd.print("ph:");
    lcd.setCursor(13, 1);
    lcd.print(pH_actual);//Mostrar variable
}
/***********************************
* LCD_print_ph4_SHOW(); 
* void LCD_print_ph4_SHOW(void)
********************************** */
void LCD_print_ph4_SHOW(float ph4_actual)
{
    lcd.setCursor(0, 0);
    lcd.print("Actual:");
    lcd.setCursor(8, 0);
    lcd.print(ph4_actual);
    lcd.setCursor(0, 1);
    lcd.print("UP= si  DOWN= no");
}
/***********************************
* LCD_print_ph6_WAIT();
* void LCD_print_ph6_WAIT(void)
************************************/
  void LCD_print_ph6_WAIT(void)
  {
    lcd.setCursor(0, 0);
    lcd.print("Coloca un buffer");
    lcd.setCursor(0, 1);
    lcd.print("de pH6");
    lcd.setCursor(7, 1);
    lcd.print("UP= listo");
  }
/***********************************
* LCD_print_ph6_RUN();
* void LCD_print_ph6_RUN()(void)
************************************/
void LCD_print_ph6_RUN(int segundero, float pH_actual)
{
    lcd.setCursor(0, 0);
    lcd.print("Midiendo pH...");
    lcd.setCursor(0, 1);
    lcd.print("tiempo:");
    lcd.setCursor(7, 1);
    lcd.print(segundero);//Mostrar variable
    lcd.setCursor(10, 1);
    lcd.print("ph:");
    lcd.setCursor(13, 1);
    lcd.print(pH_actual);//Mostrar variable
}
/***********************************
* LCD_print_ph6_SHOW();
* void LCD_print_ph6_SHOW(void)
********************************** */
void LCD_print_ph6_SHOW(float ph6_actual)
{
    lcd.setCursor(0, 0);
    lcd.print("Actual:");
    lcd.setCursor(7, 0);
    lcd.print(ph6_actual);
    lcd.setCursor(0, 1);
    lcd.print("UP= si  DOWN= no");
}
/***********************************
* LCD_print_ph_OFFSET();
* void LCD_print_ph_OFFSET(void)
********************************** */
void LCD_print_ph_OFFSET(float ph_offset)
{
    lcd.setCursor(0, 0);
    lcd.print("OFFSET:");
    lcd.setCursor(7, 0);
    lcd.print(ph_offset);
    lcd.setCursor(0, 1);
    lcd.print("RIGHT=++ LEFT=--");
}
  

