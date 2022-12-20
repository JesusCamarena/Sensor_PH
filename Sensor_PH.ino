/*===================================================
+ Código para el uso del modulo de PH
+ Inital date: 10/12//22
+ Rev date: 19/12//22
+ Actual Rev: 1.9
+ Made by: Karla Rdz
+          Jesús Camarena 
+ Revised by: 
+ ===================================================*/

// Falta: 
// 1. Agregar pH6.
// 2. Agregar condiciones de alarma.
// 3. Guadar datos en memoria EEPROM
// 4. Eliminar rebotes
// 5. Verificar address

//----------------------Conexiones-------------------//
 /*=====================================  
 *    Source           ==>   Arduino     
 *     Vcc             ==>    Vcc (+5v)
 *     GND             ==>    GND (0v)
 ======================================= 
 *    Inputs           ==>   Arduino      
 *     BOTONES         ==>   A0      
 *     Sensor          ==>   A1
 =======================================
 *    Outputs          ==>   Arduino 
 *       rs            ==>   D8 
 *       en            ==>   D9
 *       d7            ==>   D7
 *       d6            ==>   D6
 *       d5            ==>   D5 
 *       d4            ==>   D4
 =======================================*/

//----------------------Bibliotecas-------------------//
#include <LiquidCrystal.h>
#include <EEPROM.h>

//----------------------Etiquetas---------------------/
//-Elegir sensor
#define COMERCIAL  0// sensor comercial de pH
#define PROTOTIPO  1// sensor prototipo
//#define tipo_de_sensor COMERCIAL
#define tipo_de_sensor PROTOTIPO

//-Constantes
#define debounce_time 300

//-General input

//-General output

//----------------------FSM--------------------------//
//Enumerar los estados del 0 al 10 (11)
enum states 
{
  STATE_HELLO,        //0
  STATE_CAL,          //1
  STATE_ERASE_DATA,
  STATE_CAL_PH4_CLEAN,
  STATE_CAL_PH4_WAIT, //2
  STATE_CAL_PH4_RUN,
  STATE_CAL_PH4_SHOW,
  STATE_CAL_PH4_SAVE,
  STATE_CAL_PH6_CLEAN,
  STATE_CAL_PH6_WAIT,
  STATE_CAL_PH6_RUN,
  STATE_CAL_PH6_SHOW,
  STATE_CAL_PH6_SAVE,
  STATE_CLEAN,
  STATE_RUN,
  STATE_ADD_OFFSET
} state ;

//----------------------Sensor de PH--------------------------//
//-Input
#define SensorPin A1 //conexion del sensor de PH
//-Constantes de la ecuacion de pH
#define OFFSET    0.31 //Compensacion de pH
//-Resultados
float pH_actual;
float voltage;   //PuntoDecimal 2 variables
//-Valores de ph4
float ph4_actual;
float ph4_offset = 0;
float const ph4_real = 4.00;
const int ph4_addr = 0; //Verificar
//-Valores de ph6
float ph6_actual;
float ph6_offset = 0;
float const ph6_real = 6.86;
const int ph6_addr = 1; //Verificar

//----------------------Filtro Media movil--------------------------//
//-Escalar a 0 a 5 volts
//-Constantes
#define mU 0.00488281// divison para converitr a voltaje /5/1204
#define bU 0 //Interseccion en Y
//-Variables
float Y  = 0; //Resultado
//-Diez muestras N = 10
float U  = 0; //Muestra actual
float U1 = 0; //Muestra anterior a la actual
float U2 = 0;
float U3 = 0;
float U4 = 0;
float U5 = 0;
float U6 = 0;
float U7 = 0;
float U8 = 0;
float U9 = 0; //Ultima muestra

//----------------------Medicion de tiempo--------------------------//
//-Constantes
#define SAMPLING_INTERVAL 20   //Cantidad de muestras
#define PRINT_INTERVAL    800  //Tiempo de impresión 
#define LCD_INTERVAL      1000 //Tiempo de impresión

//-Variables
unsigned long samplingTime = 0;// medir tiempo 2^32  //0.5
unsigned long printTime    = 0;//TiempoActual
unsigned long LCDTime      = 0;//TiempoActual
unsigned long segundos     = 0;//Cuenta el tiempo Actual + 1s
int segundero = 0;

//----------------------LCD--------------------------//
//-General Pins
#define buttons_pin A0
#define rs 8 
#define en 9
#define d7 7
#define d6 6
#define d5 5
#define d4 4

//-Array LCD
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
//-Buttons States
int lcd_key     = 0;//init
enum btnStates 
{
  button_right,
  button_up,
  button_down,
  button_left,
  button_select,
  button_none
};
//-Function()
//int read_LCD_buttons();  // para leer los botones
// borrar
//----------------------SETUP()--------------------------//
void setup() 
{
  //--Serial
  Serial.begin(9600);  //9600 baudios comunicación
  Serial.println("Fecha: 24/11/22"); //Test the serial monitor
  //--LCD
  lcd.begin(16, 2); // Set up the number of columns and rows on the LCD.
}//Fin setup

//----------------------LOOP()--------------------------//
void loop()
{
  //Toma el tiempo actual desde que se inicio el programa
  unsigned long actualTime = millis();
//----------------------FSM-----------------------------//
switch (state) 
  {
//----------------------STATE 1 - HELLO--------------------------//
//--Estado Inicial
  case STATE_HELLO:
    LCD_print_HELLO(); //Imprime en pantalla
//-Transitions
    state = STATE_CAL;  //Cambia de estado
    lcd.clear();//Limpiar pantalla
  break;// Fin STATE_HELLO

//----------------------STATE 2 - ¿Calibrar?--------------------------//
//--Pregunta si desea calibrar
  case STATE_CAL:
    LCD_print_CALIB(); //Imprime en pantalla
//-Transitions
    lcd_key = read_LCD_buttons(); //Leer botones 
    if (lcd_key == button_up)     //button presionado
    {
      delay(debounce_time);     //Debounce
      state = STATE_ERASE_DATA; //Cambia de estado
      lcd.clear();//Limpiar
    }  
    else if (lcd_key == button_down) //button presionado
    {
      delay(debounce_time); //Debounce
      state = STATE_RUN;    //Cambia de estado
      lcd.clear();          //Limpiar
    } 
    else state = STATE_CAL; //Continua en el mismo estado
  break;// Fin STATE_CAL

//----------------------STATE 3 - Borrar--------------------------//
  case STATE_ERASE_DATA:
      //- Borra los offset ya guardados
      //EEPROM.put(ph4_addr, 0);//Addr, Value
      //EEPROM.put(ph6_addr, 0);//Addr, Value
  //-Transitions
      state = STATE_CAL_PH4_CLEAN; //Cambia de estado
  break;// STATE_ERASE_DATA

//----------------------STATE 4 - PH4 Limpar--------------------------//
  case STATE_CAL_PH4_CLEAN:
      LCD_print_sensor_CLEAN(); //Imprime en pantalla
      lcd.clear();//Limpiar
  //-Transitions
      state = STATE_CAL_PH4_WAIT; //Cambia de estado
  break;// STATE_CAL_PH4_CLEAN

//----------------------STATE 5 - PH4 Espera--------------------------//
//--Espera a que coloquen el pH4
  case STATE_CAL_PH4_WAIT:
    LCD_print_ph4_WAIT(); //Imprime en pantalla
//-Transitions
    lcd_key = read_LCD_buttons();//Leer botones button presionado
    if (lcd_key == button_up)//button presionado
    {
      delay(debounce_time); //Debounce
      state = STATE_CAL_PH4_RUN; //Cambio de estado
      LCD_print_1min_WAIT(); //Imprime en pantalla
    }  
    else state = STATE_CAL_PH4_WAIT; //Continua en el mismo estado
  break;// Fin STATE_CAL_PH4_WAIT

//----------------------STATE 6 - PH4 Medicion--------------------------//
//--Calcula el pH del momento
  case STATE_CAL_PH4_RUN:
    LCD_print_ph4_RUN(segundero, pH_actual);//Imprime en pantalla
    //--Ecuacion de ph
    //--Tiempo para tomar muestras 20ms
    if((actualTime - samplingTime) > SAMPLING_INTERVAL)
    {
      ph_equation();// Ecuación para medir el nivel de pH
      samplingTime = millis();//Actualiza tiempo actual
    }//Fin if para mostrar muestras

    //--Tiempo para imprimir muestras 800ms
    if((actualTime - printTime) > PRINT_INTERVAL)
    {
      serial_print_pH();   //Imprime el nivel de pH
      printTime = millis();//Actualiza tiempo actual
    }//Fin if

//-Transitions
    if (actualTime >= segundos)// ve cuando pasa un segundo
    {
      segundos = actualTime + 1000; //Aumenta un segundo al tiempo actual
      segundero++;// Incrementa el segundero
      if (segundero>=10) //Si pasa el tiempo requerido pasa de estado
      {
        //ph4_actual = float (EEPROM.write(ph4_addr, pH_actual));// Guarda el ph actual
        state = STATE_CAL_PH4_SHOW; //Cambia de estado
        lcd.clear();   //Limpiar
        segundero = 0; //Reinicia el segundero
      }//Fin if_
    }//Fin if_
  break;// Fin STATE_CAL_PH4_RUN

//----------------------STATE 7 - PH4 Mostrar--------------//
//--Muestra el OFFSET de pH
  case STATE_CAL_PH4_SHOW:
  // mostrar la ultima medida y la guarda
  // para restarla o sumarla al offset calibrado
    ph4_offset = ph4_real - ph4_actual;//Calcula el offset
    LCD_print_ph4_SHOW_OFFSET(ph4_offset);//Imprime en pantalla
    //-Transition
    lcd_key = read_LCD_buttons(); //Leer botones 
    if (lcd_key == button_up) // button presionado
    {
      delay(debounce_time); //Debounce
      state = STATE_CAL_PH4_SAVE; //Cambia de estado
      lcd.clear();//Limpiar
    }  
    else if (lcd_key == button_down) //button presionado
    {
      delay(debounce_time); //Debounce
      state = STATE_CAL_PH4_WAIT; //Cambia de estado
      lcd.clear();//Limpiar
    } 
    else state = STATE_CAL_PH4_SHOW; //Continua en el mismo estado
  break;// Fin STATE_CAL_PH4_SHOW

//----------------------STATE 8 - pH4 Guardar---------------//
//--Guarda los datos del pH4
  case STATE_CAL_PH4_SAVE:
   //Guardar offset
   //EEPROM.put(ph4_addr,ph4_offset);
    state = STATE_CAL_PH6_CLEAN;
  break;// Fin STATE_CAL_PH4_SHOW

//----------------------STATE 9 - PH6 Limpar--------------//
  case STATE_CAL_PH6_CLEAN:
      LCD_print_sensor_CLEAN(); //Imprime en pantalla
      lcd.clear();//Limpiar
  //-Transitions
      state = STATE_CAL_PH6_WAIT; //Cambia de estado
  break;// STATE_CAL_PH6_CLEAN

//----------------------STATE 10 - PH6 Esperar------------//
//--Espera a que coloquen el pH6
  case STATE_CAL_PH6_WAIT:
    LCD_print_ph6_WAIT(); //Imprime en pantalla
    //-Transition
    lcd_key = read_LCD_buttons();//Leer botones button presionado
    if (lcd_key == button_up)//button presionado
    {
      delay(debounce_time); //Debounce
      state = STATE_CAL_PH6_RUN; //Cambio de estado
      LCD_print_sensor_CLEAN(); //Imprime en pantalla
      LCD_print_1min_WAIT(); //Imprime en pantalla
    }  
    else state = STATE_CAL_PH6_WAIT; //Continua en el mismo estado
  break;// Fin STATE_CAL_PH6_WAIT

//----------------------STATE 11 - PH6 Medicion--------------------------//
//--Calcula el pH del momento
  case STATE_CAL_PH6_RUN:
    LCD_print_ph6_RUN(segundero, pH_actual);//Imprime en pantalla
    //--Ecuacion de ph
    //--Tiempo para tomar muestras 20ms
    if((actualTime - samplingTime) > SAMPLING_INTERVAL)
    {
      ph_equation();// Ecuación para medir el nivel de pH
      samplingTime = millis();//Actualiza tiempo actual
    }//Fin if para mostrar muestras

    //--Tiempo para imprimir muestras 800ms
    if((actualTime - printTime) > PRINT_INTERVAL)
    {
      serial_print_pH();   //Imprime el nivel de pH
      printTime = millis();//Actualiza tiempo actual
    }//Fin if

  //-Transition
    if (actualTime >= segundos)// ve cuando pasa un segundo
    {
      segundos = actualTime + 1000; //Aumenta un segundo al tiempo actual
      segundero++;// Incrementa el segundero
      if (segundero>=10) //Si pasa el tiempo requerido pasa de estado
      {
        //ph4_actual = float (EEPROM.write(ph4_addr, pH_actual));// Guarda el ph actual
        state = STATE_CAL_PH6_SHOW; //Cambia de estado
        lcd.clear(); //Limpiar
        segundero = 0; //Reinicia el segundero
      }//Fin if_
    }//Fin if_
  break;// Fin STATE_CAL_PH6_RUN

//----------------------STATE 12 - PH6 Mostrar---------------------//
//--Muestra el OFFSET de pH
  case STATE_CAL_PH6_SHOW:
  // mostrar la ultima medida y la guarda
  // para restarla o sumarla al offset calibrado
    ph6_offset = ph6_real - ph6_actual;//Calcula el offset
    LCD_print_ph6_SHOW_OFFSET(ph6_offset);//Imprime en pantalla
    //-Transition
    lcd_key = read_LCD_buttons(); //Leer botones 
    if (lcd_key == button_up) // button presionado
    {
      delay(debounce_time); //Debounce
      state = STATE_CAL_PH6_SAVE; //Cambia de estado
      lcd.clear();//Limpiar
    }  
    else if (lcd_key == button_down) //button presionado
    {
      delay(debounce_time); //Debounce
      state = STATE_CAL_PH6_WAIT; //Cambia de estado
      lcd.clear();//Limpiar
    } 
    else state = STATE_CAL_PH6_SHOW; //Continua en el mismo estado
  break;// Fin STATE_CAL_PH6_SHOW

//----------------------STATE 13 PH6 Guardar--------------------------//
//--Guarda los datos del pH6
  case STATE_CAL_PH6_SAVE:
    //EEPROM.put(ph6_addr,ph6_offset);
    state = STATE_CLEAN;
  break;// Fin STATE_CAL_PH6_SAVE

//----------------------STATE 14 - Limpar--------------//
  case STATE_CLEAN:
      LCD_print_sensor_CLEAN(); //Imprime en pantalla
      lcd.clear();//Limpiar
  //-Transitions
      state = STATE_RUN; //Cambia de estado
  break;// STATE_CLEAN

//----------------------STATE 15 - RUN --------------------------//
//--Muestra el OFFSET de pH
  case STATE_RUN:
    //--Ecuacion de ph
    //--Tiempo para tomar muestras 20ms
    if((actualTime - samplingTime) > SAMPLING_INTERVAL)
    {
      ph_equation();// Ecuación para medir el nivel de pH
      samplingTime = millis();//Actualiza tiempo actual
    }//Fin if para mostrar muestras

    //--Tiempo para imprimir muestras 800ms
    if((actualTime - printTime) > PRINT_INTERVAL)
    {
      serial_print_pH();   //Imprime el nivel de pH
      printTime = millis();//Actualiza tiempo actual
    }//Fin if

    //--Tiempo para imprimir en LCD
    if((actualTime - LCDTime) > LCD_INTERVAL)
    {
      LCD_print_ph_Value();
    }//Fin if

  break;// Fin STATE_RUN
  case STATE_ADD_OFFSET:

  break;// Fin STATE_ADD_OFFSET
  default:
    // statements
  break;
}//Fin cases estados
}//Fin loop

//----------------------FUNCTIONS()--------------------------//

