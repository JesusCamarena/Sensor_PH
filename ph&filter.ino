/***********************************
* Filtro de media movil:
* float filtro(void)
* filtro();
************************************/
float filtro(void)
{
  //Funcion 
//Convertir de binario a voltaje
  U = analogRead(SensorPin)*mU + bU;
  // La salida es igual al promedio de las ultimas
  // 10 entradas
  Y = 0.1*(U+U1+U2+U3+U4+U5+U6+U7+U8+U9);
  // Una cadena de registros para almacenar las
  // ultimas 10 muestras
  U9=U8;
  U8=U7;
  U7=U6;
  U6=U5;
  U5=U4;
  U4=U3;
  U3=U2;
  U2=U1;
  U1=U;
  return Y;
}

/***********************************
* ph_equation:
* void ph_equation(void)
* ph_equation();
********************************** */
void ph_equation(void)
{
  voltage = filtro(); //Obtiene el voltaje mediante el filtro de media movil
    switch (tipo_de_sensor)
    {
      case COMERCIAL:
        //y = mx + b
        ph_actual = -5.8752* voltage + 21.799; //Ecuacion actual de ph para sensor comercial
        ph_actual+= offset; //Compensacion de calibracion 
      break;

      case PROTOTIPO:
        //Ecuaciones h para sensor prototipo
        //Rango de 4 a 6.8 pH
        if (voltage >= 2.39 && voltage <= 4.64)
        {
          ph_actual = -1.7217* voltage + 11.003;
        }
        //Rango de 6.8 a 9 pH
        else if (voltage >= 2.24 && voltage < 2.39)
        {
          ph_actual = -15.597* voltage + 44.394;
        }
        else
        {
          ph_actual = 0;
        }
        //Compensacion de calibracion 
        ph_actual+= offset;
      break;
    }//Fin switch
}



