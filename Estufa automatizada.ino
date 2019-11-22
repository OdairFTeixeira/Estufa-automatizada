
/////////////////////   MODBUS   //////////////////////

 #include <SimpleModbusSlave.h>
 
 /*REGISTRADORES  DAS VARIAVEIS  DO ARDUINO */
enum {  
        MB_40000,       //A0      
        MB_40001,       //A1        
        MB_40002,       //A2
        MB_40003,       //A3
        MB_40004,       //A4
        MB_40005,       //A5
        MB_40006,       //A6
        MB_40007,       //A7
        MB_40008,       //PWM  PIN_06
        MB_40009,       //PWM  PIN_09
        MB_40010,       //PWM  PIN_10
        MB_40011,       //PWM  PIN_11               
        MB_40012,        //PIN_07                
        MB_40013,        //PIN_08
        MB_40014,        //PIN_12           
        MB_40015,        //PIN_13
        MB_40016,      //PIN_02
        MB_40017,      //PIN_03
        MB_40018,      //PIN_04
        MB_40019,      //PIN_05
  MB_REGS=20,
};

 unsigned int holdingRegs[MB_REGS]; // function 3 and 16 register array*/
 
/*====================================== LDR ======================================*/
int lux = 0;
int valorLux = 0;
int leds = 13;
/*================================== VENTILADOR ===================================*/
int temp = 2;
int valorTemp = 0;
int vent = 12;
/*==================================== BOMBA =====================================*/
int umi = 1;
int valorUmi = 0;
int bomba = 11;
int simuTemperatura = 40;
int memoria = 1;
void setup() {
 /////////////////////   MODBUS   ////////////////////// 
  modbus_configure(&Serial, 9600, SERIAL_8N2, 1, 10, MB_REGS, holdingRegs);
  modbus_update_comms(9600, SERIAL_8N2, 1);
  holdingRegs[MB_40003] = 1;

  Serial.begin(9600);
  pinMode(13, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(11, OUTPUT);
}

void loop() {
/*====================================== LDR ======================================*/
  Serial.print("Valor Luminosidade: ");
  Serial.println(valorLux);
  valorLux = analogRead(lux);
  valorLux = map(valorLux, 150, 1023, 0, 100);

  if (valorLux > 85){
    digitalWrite(leds, HIGH);  }
    else {  digitalWrite(leds, LOW);  }

/*================================== VENTILADOR ===================================*/
  Serial.print("Valor Temperatura: ");
  Serial.println(valorTemp);
  valorTemp = analogRead(temp);
  valorTemp = map(valorTemp, 20, 358, 0, 100);

  if (simuTemperatura > 35){        //CONFERIR VALOR PARA SAÍDA
    digitalWrite(vent, LOW); }
    else {  digitalWrite(vent, HIGH); }

 delay(500);      //DELAY PARA SERIAL, RETIRAR APÓS TÉRMINO

/*==================================== BOMBA =====================================*/
  valorUmi = analogRead(umi);
  valorUmi = map(valorUmi, 0, 1023, 0, 100); 
  Serial.print("Valor Umidade: ");
  Serial.println(valorUmi);

  if (valorUmi < 70){
    digitalWrite(11, HIGH); }
    
  if (valorUmi > 70){
    digitalWrite(11, LOW);  }
    
  modbus_update();
  holdingRegs[MB_40000] = valorUmi;
  holdingRegs[MB_40001] = valorTemp;
  holdingRegs[MB_40002] = valorLux;

  memoria =  holdingRegs[MB_40003];
  while(memoria == 0) {
    modbus_update();
    memoria =  holdingRegs[MB_40003];
    digitalWrite(11, memoria);
    memoria =  holdingRegs[MB_40003];
    digitalWrite(11, memoria);
    
  }
}
