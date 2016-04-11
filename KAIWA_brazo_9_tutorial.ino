//-------------------------------------------------------------------------------------
//                        Autor: Konrad Peschks
//-------------------------------------------------------------------------------------
//                        Fecha: octubre 2015
//-------------------------------------------------------------------------------------


//------------------------------------Declaracion de Librerias-------------------------------------------------------

#include <PS2X_lib.h> 

PS2X ps2x; // create PS2 Controller Class

#include <Servo.h>

Servo servo_base;                  //defino los alias de cada servo
Servo servo_hombro;
Servo servo_codo;
Servo servo_munhieca;
Servo servo_pinza;

//#define debug                  //comentando o descomentando esta linea activo el codigo para debug

//------------------------------------Declaraciones de variables-------------------------------------------------

int variable_control=0;            //es la variable que uso para saber que boton fue pulsado         
const int margen_joystick=10;      //margen que tiene el joystick para saber si esta corrido del centro
const int max_filas=25;             //defino la cantidad maxima de pasos que deseo guardar

const int limite_superior=200;         //limites superiores de todos los servos
const int limite_inferior=30;          //limite inferior de todos los servos

const int vel_movimiento=1;           //velocidad de movimiento de todos los servos

int variable_servo_base=100;          //defino las variables y los valores que tendran en el arranque
int variable_servo_hombro=100;
int variable_servo_codo=100;
int variable_servo_munhieca=100;
int variable_servo_pinza=100;

int matriz_posiciones[max_filas][5];      //defino la matriz donde voy a guardar los pasos

int aux_fila=0;
int aux_fila2=0;
int modo_carga=0;

#ifdef debug
const int tiempo_debug=10;
#endif

//-------------------------------------------------------------------------------------
//              Setup donde se inicializan librerias, pines y demas
//-------------------------------------------------------------------------------------

void setup()

{ 
#ifdef debug
 Serial.begin(57600);
#endif
          
ps2x.config_gamepad(A0,A2,A1,A3, true, true);   //setup pins and settings:  GamePad(clock, command, attention, data, Pressures?, Rumble?) check for error


servo_base.attach(4);      //defino pines de los servos
servo_hombro.attach(5);
servo_codo.attach(6);
servo_munhieca.attach(7);
servo_pinza.attach(8);

movimiento_servos();

reset_matriz();
}


//-------------------------------------------------------------------------------------
//                    Loop o programa principal
//-------------------------------------------------------------------------------------

void loop()
{

  lectura_control();
  ajuste_variable_servos(); 
  movimiento_servos();     
}

//-------------------------------------------------------------------------------------
//                   Declaracion de funciones
//-------------------------------------------------------------------------------------


//-------------------------------deteccion de limites de variables para servo-------------------------------------

void limites_servo(void)
{
   if(variable_servo_base>limite_superior)
   variable_servo_base=limite_superior;
  
    if(variable_servo_base<limite_inferior)
    variable_servo_base=limite_inferior;

     if(variable_servo_hombro>limite_superior)
    variable_servo_hombro=limite_superior;

 
     if(variable_servo_hombro<limite_inferior)
    variable_servo_hombro=limite_inferior;

    if(variable_servo_codo>limite_superior)
    variable_servo_codo=limite_superior;
  
     if(variable_servo_codo<limite_inferior)
    variable_servo_codo=limite_inferior;

     if(variable_servo_munhieca>limite_superior)
    variable_servo_munhieca=limite_superior;

     if(variable_servo_munhieca<limite_inferior)
    variable_servo_munhieca=limite_inferior;

    if(variable_servo_pinza>limite_superior)
   variable_servo_pinza=limite_superior;
 
    if(variable_servo_pinza<limite_inferior)
   variable_servo_pinza=limite_inferior;

    return;

}

//--------------------------------Ajuste variables servos-----------------------------------------------------

void ajuste_variable_servos(void)
{
  switch (variable_control) 
  {
    case 0:     //no se presiono nada
    #ifdef debug
    Serial.println("Caso 0");
    delay(tiempo_debug);
    #endif
      break;
      
    case 1:    //boton start
    #ifdef debug
    Serial.println("Caso 1");
    delay(tiempo_debug);
    #endif
      break;
      
    case 2:    //boton select
    #ifdef debug
    Serial.println("Caso 2");
    delay(tiempo_debug);
    #endif
      break;
      
    case 3:    //boton L3
    #ifdef debug
    Serial.println("Caso 3");
    delay(tiempo_debug);
    #endif
      break;
      
    case 4:    //botonR3
    #ifdef debug
    Serial.println("Caso 4");
    delay(tiempo_debug);
    #endif
      break;
      
    case 5:    //boton L2
    #ifdef debug
    Serial.println("Caso 5");
    delay(tiempo_debug);
    #endif
      break;
      
    case 6:    //boton R2
    #ifdef debug
    Serial.println("Caso 6");
    delay(tiempo_debug);
    #endif
      break;
      
    case 7:    //boton Triangulo
    #ifdef debug
    Serial.println("Caso 7");
    delay(tiempo_debug);
    #endif

    reset_matriz();
    modo_carga=1;
    aux_fila=0;
    aux_fila2=0;
      break; 
         
    case 8:    //boton circulo
    #ifdef debug
    Serial.println("Caso 8");
    delay(tiempo_debug);
    #endif

    if(modo_carga==1)           //solo carga si el modo carga esta activo
    {
    cargar_matriz();              //carga valores de posicion en al matriz
    aux_fila++;                  //salta a la siguiente fila
    if(aux_fila>max_filas)               //si ingreso mas sobrepisa en la ultima fila
    aux_fila=max_filas;
    }

      break;
      
    case 9:    //boton cuadrado
    #ifdef debug
    Serial.println("Caso 9");
    delay(tiempo_debug);
    #endif
    
      break;
      
    case 10:    //boton x
    #ifdef debug
    Serial.println("Caso 10");
    delay(tiempo_debug);
    #endif

    if(modo_carga==1)       //finaliza la carga
    modo_carga=2;
    
      break;
      
    case 11:    //boton arriba
    #ifdef debug
    Serial.println("Caso 11");
    delay(tiempo_debug);
    #endif
      break;
      
    case 12:    //boton abajo
    #ifdef debug
    Serial.println("Caso 12");
    delay(tiempo_debug);
    #endif
      break;
      
    case 13:    //boton izquierda
    #ifdef debug
    Serial.println("Caso 13");
    delay(tiempo_debug);
    #endif
      break;
      
    case 14:    //boton derecha
    #ifdef debug
    Serial.println("Caso 14");
    delay(tiempo_debug);
    #endif
      break;
      
    case 15:    //boton joystick izquierdo abajo#ifdef debug
    
    variable_servo_munhieca=variable_servo_munhieca+vel_movimiento;
    limites_servo();

    #ifdef debug
    Serial.println("Caso 15");
    delay(tiempo_debug);
    #endif
    
      break;
      
    case 16:    //boton joystick izquierdo arriba
    
     variable_servo_munhieca=variable_servo_munhieca-vel_movimiento;
     limites_servo();

     #ifdef debug
    Serial.println("Caso 16");
    delay(tiempo_debug);
    #endif
    
      break;
      
    case 17:    //boton joystick izquierdo derecha
    
    variable_servo_codo=variable_servo_codo+vel_movimiento;
    limites_servo();

    #ifdef debug
    Serial.println("Caso 17");
    delay(tiempo_debug);
    #endif
    
      break;
      
    case 18:   //boton joystick izquierdo izquierda
    
    variable_servo_codo=variable_servo_codo-vel_movimiento;
    limites_servo();

    #ifdef debug
    Serial.println("Caso 18");
    delay(tiempo_debug);
    #endif
    
      break;
      
    case 19:   //boton joystick derecho abajo
    
    variable_servo_hombro=variable_servo_hombro+vel_movimiento;
    limites_servo();

    #ifdef debug
    Serial.println("Caso 19");
    delay(tiempo_debug);
    #endif
    
      break;
      
    case 20:    //boton joystick derecho arriba
    
    variable_servo_hombro=variable_servo_hombro-vel_movimiento;
    limites_servo();

     #ifdef debug
    Serial.println("Caso 20");
    delay(tiempo_debug);
    #endif
    
      break;
      
    case 21:    //boton joystick derecho derecha

    variable_servo_base=variable_servo_base+vel_movimiento;
    limites_servo();

     #ifdef debug
    Serial.println("Caso 21");
    delay(tiempo_debug);
    #endif
    
      break;
      
    case 22:    //boton joystick derecho izquierda
    variable_servo_base=variable_servo_base-vel_movimiento;
    limites_servo();
    
    #ifdef debug
    Serial.println("Caso 22");
    delay(tiempo_debug);
    #endif
      break;

    case 23:    //boton L1

    variable_servo_pinza=variable_servo_pinza+vel_movimiento;
    limites_servo();
    
    #ifdef debug
    Serial.println("Caso 23");
    delay(tiempo_debug);
    #endif
      break;

    case 24:    //boton R1
    variable_servo_pinza=variable_servo_pinza-vel_movimiento;   
    limites_servo();
    
    #ifdef debug
    Serial.println("Caso 24");
    delay(tiempo_debug);
    #endif

      break;


    case 25:    //boton R1
    
    #ifdef debug
    Serial.println("Caso 25");
    delay(tiempo_debug);
    #endif
    
    inicio_secuencia();
    
      break;
      
      
            
  }
}

//-------------------------incio secuencia--------------------------------------------

void inicio_secuencia(void)
{
    if(aux_fila==0)       //significa que no fue cargado ningun valor
    return;
    
    int variable;          //variable auciliar que cree para esta funcion
  
    variable=matriz_posiciones[aux_fila2][0];
    while(variable_servo_base!=variable)
    {
      if(variable>variable_servo_base)
      variable_servo_base++;
      else
      variable_servo_base--;
      movimiento_servos();
    }

    variable=matriz_posiciones[aux_fila2][1];
    while(variable_servo_hombro!=variable)
    {
      if(variable>variable_servo_hombro)
      variable_servo_hombro++;
      else
      variable_servo_hombro--;
      movimiento_servos();
    }

    variable=matriz_posiciones[aux_fila2][2];
    while(variable_servo_codo!=variable)
    {
      if(variable>variable_servo_codo)
      variable_servo_codo++;
      else
      variable_servo_codo--;
      movimiento_servos();
    }

    variable=matriz_posiciones[aux_fila2][3];
    while(variable_servo_munhieca!=variable)
    {
      if(variable>variable_servo_munhieca)
      variable_servo_munhieca++;
      else
      variable_servo_munhieca--;
      movimiento_servos();
    }

    variable=matriz_posiciones[aux_fila2][4];
    while(variable_servo_pinza!=variable)
    {
      if(variable>variable_servo_pinza)
      variable_servo_pinza++;
      else
      variable_servo_pinza--;
      movimiento_servos();
    }
    

    #ifdef debug
        Serial.print("base");
        Serial.print(variable_servo_base);
        Serial.print("hombro");
        Serial.print(variable_servo_hombro);
        Serial.print("codo");
        Serial.print(variable_servo_codo);
        Serial.print("munhieca");
        Serial.print(variable_servo_munhieca);
        Serial.print("pinza");
        Serial.print(variable_servo_pinza);
    #endif

  aux_fila2++;
  if(aux_fila2>aux_fila-1)
  aux_fila2=0;
   
}


//-----------------------------carga valores a la matriz------------------------------------

void cargar_matriz(void)
{

   limites_servo();
  
   matriz_posiciones[aux_fila][0]=variable_servo_base;
   matriz_posiciones[aux_fila][1]=variable_servo_hombro;
   matriz_posiciones[aux_fila][2]=variable_servo_codo;
   matriz_posiciones[aux_fila][3]=variable_servo_munhieca;
   matriz_posiciones[aux_fila][4]=variable_servo_pinza;

}


//-----------------------------------funcion que resetea la matriz----------------------------

void reset_matriz(void)
{

  for(int i=0; i<max_filas;i++)
  {
  matriz_posiciones[i][0]=0;
  matriz_posiciones[i][1]=0;
  matriz_posiciones[i][2]=0;
  matriz_posiciones[i][3]=0;
  matriz_posiciones[i][4]=0;
  }
}


//-----------------------------------Movimiento servos--------------------------------------------------

void movimiento_servos(void)
{

  servo_base.write(variable_servo_base);        //muevo los servos a valor medio de inicio
  delay(3);
  servo_hombro.write(variable_servo_hombro);
  delay(3);
  servo_codo.write(variable_servo_codo);
  delay(3);
  servo_munhieca.write(variable_servo_munhieca);
  delay(3);
  servo_pinza.write(variable_servo_pinza);
  delay(3);

  #ifdef debug
  Serial.println("Movimiento Servo");
  delay(tiempo_debug);
  #endif

}

//-------------------------------------------------------------------------------------
void lectura_control(void)
{
         ps2x.read_gamepad(false, 0);          //read controller and set large motor to spin at 'vibrate' speed 
         
//-------------------------------------lectura de los Botones------------------------------------------------
        variable_control=0;

        if(ps2x.ButtonPressed(PSB_START))   
        {
          #ifdef debug
          Serial.println("Start");
          #endif    
          variable_control=1;     
          return;       
        }
         
        if(ps2x.ButtonPressed(PSB_SELECT))
        {
          #ifdef debug
          Serial.println("Select");
          #endif
          variable_control=2;
          return; 
        }
         

        if(ps2x.ButtonPressed(PSB_L3))
        {
          #ifdef debug
          Serial.println("L3");
          #endif
          variable_control=3;
          return; 
        }
         
         
        if(ps2x.ButtonPressed(PSB_R3))
        {
          #ifdef debug
          Serial.println("R3");
          #endif
          variable_control=4;
          return; 
        }
         
         
        if(ps2x.ButtonPressed(PSB_L2))
        {
          #ifdef debug
          Serial.println("L2");
          #endif
          variable_control=5;
          return; 
        }
         
         
        if(ps2x.ButtonPressed(PSB_R2))
        {
          #ifdef debug
          Serial.println("R2");
          #endif
          variable_control=6;
          return; 
        }
         
         
         if(ps2x.ButtonPressed(PSB_GREEN))  //triangulo
         {
          #ifdef debug
          Serial.println("Triangulo");
          #endif
           variable_control=7;
           return; 
         }
         
          
         if(ps2x.ButtonPressed(PSB_RED))   //circulo
         {
          #ifdef debug
          Serial.println("Circulo");
          #endif
          variable_control=8;
          return; 
         }
         
         
         if(ps2x.ButtonPressed(PSB_PINK))   //cuadrado
         {
          #ifdef debug
          Serial.println("Cuadrado");
          #endif
           variable_control=9;
           return; 
         }
         
          
         if(ps2x.ButtonPressed(PSB_BLUE))   //x
         {
          #ifdef debug
          Serial.println("X");
          #endif
          variable_control=10;
          return; 
         }
          
        

        if(ps2x.ButtonPressed(PSB_PAD_UP))       //boton arriba   
        {
          #ifdef debug
          Serial.println("Arriba");
          #endif   
          variable_control=11;  
          return;     
        }
         
         
        if(ps2x.ButtonPressed(PSB_PAD_DOWN))     //boton abajo  
        {
          #ifdef debug
          Serial.println("Abajo");
          #endif  
          variable_control=12; 
          return;           
        }
         
         
        if(ps2x.ButtonPressed(PSB_PAD_LEFT))   //boton izquierda     
        {
          #ifdef debug
          Serial.println("Izquierda");
          #endif  
          variable_control=13;
          return;          
        }
         
         
        if(ps2x.ButtonPressed(PSB_PAD_RIGHT))     //boton derecha   
        {
          #ifdef debug
          Serial.println("Derecha");
          #endif   
          variable_control=14;  
          return;    
        }

        if(ps2x.Button(PSB_L1))
        {
          #ifdef debug
          Serial.println("L1");
          #endif
          variable_control=23;
          return; 
        }
         
         
        if(ps2x.Button(PSB_R1))
        {
          #ifdef debug
          Serial.println("R1");
          #endif
          variable_control=24;
          return; 
        }
 

      if(modo_carga==2)          //si el modo carga es 2 inicio con la secuencia
      variable_control=25;


//------------------------------------Lectura de Los analogicos-------------------------------------------------   
    
      #ifdef debug

        Serial.print("Stick Values:");
        Serial.print(ps2x.Analog(PSS_LY), DEC); //Left stick, Y axis. Other options: LX, RY, RX  
        Serial.print(",");
        Serial.print(ps2x.Analog(PSS_LX), DEC); 
        Serial.print(",");
        Serial.print(ps2x.Analog(PSS_RY), DEC); 
        Serial.print(",");
        Serial.println(ps2x.Analog(PSS_RX), DEC);

      #endif
        
     if(ps2x.Analog(PSS_LY)>(128+margen_joystick))   //me fijo si joystick Izquierdo para abajo
     {
     variable_control=17;
     return; 
     }
     if(ps2x.Analog(PSS_LY)<(128-margen_joystick))   //me fijo si joystick Izquierdo para arriba
     {
     variable_control=18;
     return; 
     }
     if(ps2x.Analog(PSS_LX)>(128+margen_joystick))   //me fijo si joystick Izquierdo para derecha
     {
     variable_control=15;
     return; 
     }
     if(ps2x.Analog(PSS_LX)<(128-margen_joystick))  //me fijo si joystick Izquierdo para izquierda
     {
     variable_control=16;
     return; 
     }
     if(ps2x.Analog(PSS_RY)>(128+margen_joystick))  //me fijo si joystick derecho para abajo
     {
     variable_control=19;
     return; 
     }
     if(ps2x.Analog(PSS_RY)<(128-margen_joystick))  //me fijo si joystick derecho para arriba
     {
     variable_control=20;
     return; 
     }
     if(ps2x.Analog(PSS_RX)>(128+margen_joystick))  //me fijo si joystick derecho para derecha
     {
     variable_control=21;
     return; 
     }
     if(ps2x.Analog(PSS_RX)<(128-margen_joystick))  //me fijo si joystick derecho para izquierda
     {
     variable_control=22;
     return; 
     }


}

