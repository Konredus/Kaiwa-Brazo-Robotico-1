//-------------------------------------------------------------------------------------
//                        Autor: Konrad Peschka
//-------------------------------------------------------------------------------------
//                        Fecha: Septiembre 2016
//-------------------------------------------------------------------------------------


//------------------------------------Declaracion de Librerias-------------------------------------------------------
#include <Servo.h>

Servo servo_base;                  //defino los alias de cada servo
Servo servo_hombro;
Servo servo_codo;
Servo servo_pinza;

//#define debug                  //comentando o descomentando esta linea activo el codigo para debug

//------------------------------------Declaraciones de variables-------------------------------------------------

int variable_comando = 0;          //es la variable que uso para saber que boton fue pulsado
const int max_filas = 40;           //defino la cantidad maxima de pasos que deseo guardar
int matriz_posiciones[max_filas][4];      //defino la matriz donde voy a guardar los pasos

const int vel_movimiento = 8;         //velocidad de movimiento de todos los servos

int variable_servo_base;        //defino las variables y los valores que tendran en el arranque
int variable_servo_hombro;
int variable_servo_codo;
int variable_servo_pinza;


int aux_fila = 0;
int aux_fila2 = 0;
int variable_carga= 0;

#ifdef debug
const int tiempo_debug = 100;
#endif

const int Pote1 = A0; //pote para mover base
const int Pote2 = A2; //pote para mover hombro
const int Pote4 = A4; //pote para mover codo
const int Pote5 = A6; //pote para mover muñeca

const int Boton1 = 8; //boton para iniciar modo guardado
const int Boton2 = 9; //boton para guardar las secuencias
const int Boton3 = 10; //boton para iniciar modo automatico

const int Led1 = 12; //led para indicar modo guardado
const int Led2 = 13; //led para indicar modo secuencia automatica

int base_anterior = 0;
int hombro_anterior = 0;
int codo_anterior = 0;
int pinza_anterior = 0;

const int delta = 2; //diferencia minima para que tome potenciometro
//-------------------------------------------------------------------------------------
//              Setup donde se inicializan librerias, pines y demas
//-------------------------------------------------------------------------------------

void setup()

{
#ifdef debug
  Serial.begin(9600);
#endif

  pinMode(Boton1, INPUT);
 // digitalWrite(Boton1, HIGH); //activo pull up
  pinMode(Boton2, INPUT);
 // digitalWrite(Boton2, HIGH); //activo pull up
  pinMode(Boton3, INPUT);
 // digitalWrite(Boton3, HIGH); //activo pull up

  pinMode(Led1, OUTPUT);
  pinMode(Led2, OUTPUT);
  digitalWrite(Led1, LOW); //los leds arrancan apagados
  digitalWrite(Led2, LOW); //los leds arrancan apagados


  servo_base.attach(4);      //defino pines de los servos
  servo_hombro.attach(5);
  servo_codo.attach(6);
  servo_pinza.attach(7);

  lectura_potenciometros();

  reset_matriz();
}


//-------------------------------------------------------------------------------------
//                    Loop o programa principal
//-------------------------------------------------------------------------------------

void loop()
{
   lectura_pulsadores();
   if (variable_carga != 2)
    lectura_potenciometros();
    
  switch_case_comandos();

}

//-------------------------------------------------------------------------------------
//                   Declaracion de funciones
//-------------------------------------------------------------------------------------

//--------------------------------Ajuste variables servos-----------------------------------------------------

void switch_case_comandos(void)
{
  switch (variable_comando)
  {
    case 0:   //no hace nada
#ifdef debug
      Serial.println("Caso 0");
      delay(tiempo_debug);
#endif
      digitalWrite(Led1, LOW);
      digitalWrite(Led2, LOW);
      break;
    case 1:    //se activa el modo carga
#ifdef debug
      Serial.println("Caso 1");
      delay(tiempo_debug);
#endif
      digitalWrite(Led1, HIGH);
      digitalWrite(Led2, LOW);
      reset_matriz();
      aux_fila = 0;
      aux_fila2 = 0;
      variable_comando=2;
      break;

    case 2:    // Cargando datos en la matriz
#ifdef debug
      Serial.println("Caso 2");
      delay(tiempo_debug);
#endif
      digitalWrite(Led1, HIGH);
      digitalWrite(Led2, LOW); 
      if (variable_carga == 1)        //solo carga si el modo carga esta activo
      {
        digitalWrite(Led1, HIGH);
        digitalWrite(Led2, HIGH); 
        delay(100);
        cargar_matriz();              //carga valores de posicion en al matriz
        aux_fila++;                  //salta a la siguiente fila
        if (aux_fila > max_filas)            //si ingreso mas sobrepisa en la ultima fila
          aux_fila = max_filas;
        variable_carga = 0;
      }
      
      break;

    case 3:    //inicio modo secuencia automatica
#ifdef debug
      Serial.println("Caso 3");
      delay(tiempo_debug);
#endif
      digitalWrite(Led1, LOW);
      digitalWrite(Led2, HIGH);
      variable_carga = 2;
      break;

    case 4:    //Inicio secuencia automatica
#ifdef debug
      Serial.println("Caso 4");
      delay(tiempo_debug);
#endif
      digitalWrite(Led1, LOW);
      digitalWrite(Led2, HIGH);
      inicio_secuencia();
      break;
  }
}

//-------------------------incio secuencia--------------------------------------------

void inicio_secuencia(void)
{
  if (aux_fila == 0)    //significa que no fue cargado ningun valor
    return;

  int variable;          //variable auciliar que cree para esta funcion

  variable = matriz_posiciones[aux_fila2][0];
  if (variable_servo_base != variable)
  {
    mueve_base( variable,  variable_servo_base);
    variable_servo_base = variable;
  }

  variable = matriz_posiciones[aux_fila2][1];
  if (variable_servo_hombro != variable)
  {
    mueve_hombro( variable, variable_servo_hombro);
    variable_servo_hombro = variable;
  }

  variable = matriz_posiciones[aux_fila2][2];
  if (variable_servo_codo != variable)
  {
    mueve_codo(variable, variable_servo_codo);
    variable_servo_codo = variable;
  }

  variable = matriz_posiciones[aux_fila2][3];
  if (variable_servo_pinza != variable)
  {
    mueve_pinza(variable, variable_servo_pinza);
    variable_servo_pinza = variable;
  }


#ifdef debug
  Serial.print("base");
  Serial.print(variable_servo_base);
  Serial.print("hombro");
  Serial.print(variable_servo_hombro);
  Serial.print("codo");
  Serial.print(variable_servo_codo);
  Serial.print("pinza");
  Serial.println(variable_servo_pinza);
  delay(tiempo_debug);
#endif

  aux_fila2++;
  if (aux_fila2 > aux_fila - 1)
    aux_fila2 = 0;

}


//-----------------------------carga valores a la matriz------------------------------------

void cargar_matriz(void)
{
  matriz_posiciones[aux_fila][0] = variable_servo_base;
  matriz_posiciones[aux_fila][1] = variable_servo_hombro;
  matriz_posiciones[aux_fila][2] = variable_servo_codo;
  matriz_posiciones[aux_fila][3] = variable_servo_pinza;
}


//-----------------------------------funcion que resetea la matriz----------------------------

void reset_matriz(void)
{
  for (int i = 0; i < max_filas; i++)
  {
    matriz_posiciones[i][0] = 0;
    matriz_posiciones[i][1] = 0;
    matriz_posiciones[i][2] = 0;
    matriz_posiciones[i][3] = 0;
  }
}

//-------------------------------------------------------------------------------------
void lectura_pulsadores(void)
{
  /*
  if (digitalRead(Boton1) == HIGH) //cuando se preciona Boton 1
  {
    delay(50);
    while (digitalRead(Boton1) == HIGH)
        {
      #ifdef debug
    Serial.println("Esperando Boton 1 ");
    delay(50);
     #endif
    }
    variable_comando = 1;
    variable_carga = 0;
#ifdef debug
    Serial.println("Boton 1");
    delay(tiempo_debug);
#endif
    delay(50);
  }
*/
  if (digitalRead(Boton2) == HIGH) //cuando se preciona Boton 2
  {
    delay(50);
    while (digitalRead(Boton2) == HIGH)
        {
      #ifdef debug
    Serial.println("Esperando Boton 2 ");
    delay(50);
     #endif
    }
    variable_comando = 2;
    variable_carga = 1;
#ifdef debug
    Serial.println("Boton 2");
    delay(tiempo_debug);
#endif
    delay(50);
  }

  if (digitalRead(Boton3) == HIGH) //cuando se preciona Boton 3
  {
    delay(50);
    while (digitalRead(Boton3) == HIGH)
    {
      #ifdef debug
    Serial.println("Esperando Boton 3 ");
    delay(50);
     #endif
    }
    variable_comando  = 3;
#ifdef debug
    Serial.println("Boton 3");
    delay(tiempo_debug);
#endif
    delay(50);
  }

  if (variable_carga == 2)       //si el modo carga es 2 inicio con la secuencia
    variable_comando = 4;
}

//--------------------------Lectura Potenciometros--------------------------------

void lectura_potenciometros(void)
{

  variable_servo_base = map(analogRead(Pote1), 0, 1023, 0, 180);
  if (((variable_servo_base - base_anterior) > delta) || ((base_anterior - variable_servo_base) > delta))
  {
    mueve_base(variable_servo_base, base_anterior);
    base_anterior = variable_servo_base;
  }

  variable_servo_hombro = map(analogRead(Pote2), 0, 1023, 0, 180);
  if (((variable_servo_hombro - hombro_anterior) > delta) || ((hombro_anterior - variable_servo_hombro ) > delta))
  {
    mueve_hombro(variable_servo_hombro , hombro_anterior);
    hombro_anterior = variable_servo_hombro ;
  }

  variable_servo_codo = map(analogRead(Pote4), 0, 1023, 0, 180);
  if (((variable_servo_codo - codo_anterior) > delta) || ((codo_anterior - variable_servo_codo) > delta))
  {
    mueve_codo(variable_servo_codo, codo_anterior);
    codo_anterior = variable_servo_codo;
  }

  variable_servo_pinza = map(analogRead(Pote5), 0, 1023, 0, 180);
  if (((variable_servo_pinza - pinza_anterior) > delta) || ((pinza_anterior - variable_servo_pinza) > delta))
  {
    mueve_pinza(variable_servo_pinza, pinza_anterior);
    pinza_anterior = variable_servo_pinza;
  }

#ifdef debug
  Serial.print("base: ");
  Serial.print(variable_servo_base);
  Serial.print(" hombro: ");
  Serial.print(variable_servo_hombro);
  Serial.print(" codo: ");
  Serial.print(variable_servo_codo);
  Serial.print(" pinza: ");
  Serial.println(variable_servo_pinza);
   delay(tiempo_debug);
#endif
}

//-----------------------------------Movimiento servos-------------------------------------------------

void mueve_base(int fin, int inicio)
{
  if (inicio < fin)
  {
    for (int i = inicio; i < fin; i++)
    {
      servo_base.write(i);
      delay(vel_movimiento);
    }
  }

  if (inicio > fin)
  {
    for (int i = inicio; i > fin; i--)
    {
      servo_base.write(i);
      delay(vel_movimiento);
    }
  }
}


void mueve_hombro(int fin, int inicio)
{
  if (inicio < fin)
  {
    for (int i = inicio; i < fin; i++)
    {
      servo_hombro.write(i);
      delay(vel_movimiento);
    }
  }

  if (inicio > fin)
  {
    for (int i = inicio; i > fin; i--)
    {
      servo_hombro.write(i);
      delay(vel_movimiento);
    }
  }
}


void mueve_codo(int fin, int inicio)
{
  if (inicio < fin)
  {
    for (int i = inicio; i < fin; i++)
    {
      servo_codo.write(i);
      delay(vel_movimiento);
    }
  }

  if (inicio > fin)
  {
    for (int i = inicio; i > fin; i--)
    {
      servo_codo.write(i);
      delay(vel_movimiento);
    }
  }
}

void mueve_pinza(int fin, int inicio)
{
  if (inicio < fin)
  {
    for (int i = inicio; i < fin; i++)
    {
      servo_pinza.write(i);
      delay(vel_movimiento);
    }
  }

  if (inicio > fin)
  {
    for (int i = inicio; i > fin; i--)
    {
      servo_pinza.write(i);
      delay(vel_movimiento);
    }
  }
}
