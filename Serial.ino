/***********************************
* serial_print_pH();
* void serial_print_pH(void)
********************************** */
void serial_print_pH(void)
{
  Serial.print("Sensor: ");
  #if tipo_de_sensor == COMERCIAL
    Serial.println("COMERCIAL");
  #elif tipo_de_sensor == PROTOTIPO
    Serial.println("PROTOTIPO");
  #endif
  Serial.print("Voltage: ");//Palabra voltaje
  Serial.println(voltage,2);
  Serial.print("pH value: ");
  Serial.println(pH_actual,2);
  Serial.println( );
}
