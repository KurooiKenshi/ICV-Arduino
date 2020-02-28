  #include <Servo.h>
#include <DS1307.h>
#include <RTClib.h>
#include <EEPROM.h>
#include <HX711.h>

HX711 scale;

int chipSelect = 2;
struct openCdt //Estrutura de condição de abertura, representa os elementos que controlam a abertura e fechamento do alimentador.
{
  int mn;
  int hr;
  int peso;
  bool rdy;
};typedef struct openCdt openCdt;

class Relogio //Classe que controla o RTC.
{ 
  private:
    DS1307 rtc = DS1307(16,5);
    DateTime now;

  public:
    Relogio(){}
    
    Time getTempo()
    {
      return rtc.getTime();
    }

    String getTempoStr()
    {
      return rtc.getTimeStr();
    }

    String getDataStr()
    {
      return rtc.getDateStr();
    }

    String getDDSStr() // Dia da semana
    {
      return rtc.getDOWStr();
    }

    String getMesStr()
    {
      return rtc.getMonthStr();
    }

    void adjustTime()
    {
      now = DateTime(F(__DATE__), F(__TIME__));
      
      rtc.setTime((uint8_t)now.hour(),(uint8_t)now.minute(),(uint8_t)now.second());
      rtc.setDate((uint8_t)now.day(),(uint8_t)now.month(),(uint16_t)now.year());
      rtc.setDOW((uint8_t)now.dayOfTheWeek());
    }

  bool verificaHora(uint8_t hora, uint8_t minu)
  {
    if(hora == getTempo().hour && minu == getTempo().min)
    {
      return true;
    }
    else
    {
      return false;
    }
  }
    
};

class Motor //Classe que controla o servo motor.
{
  private:
    bool aberto;
    Servo servo1;
  
  public:
    Motor()
    {
      aberto = false;
      servo1.attach(D2);
      servo1.write(180);
    }

    bool isOpen()
    {
      return aberto;
    }

    void abre()
    {
      if(!isOpen()){
        servo1.write(0);
        delay(1000);
        aberto = 1;
      }
      else{
        Serial.print("A porta já está aberta!\n");
      }
    }

    void fecha()
    {
       if(isOpen()){
            servo1.write(180);
          delay(1000);
          aberto=0;
        }  
        
        else{
          Serial.print("A porta já está fechada!\n");
        }
        
    }

    void abreConditional(openCdt cdt, Relogio r)
    {
      if(r.verificaHora(cdt.hr, cdt.mn))
      {
        abre();
      }
      else
      {
        fecha();
      }
    }

};

class MEMNV //Classe que controla a memória EEPROM.
{
  private:

  public:
    MEMNV()
    {
      EEPROM.begin(512);
    }

    void addHora(byte hr, byte mn, int peso)
    {
      byte p = (byte)(peso/10);
      
      int qtd = EEPROM.read(1);

      EEPROM.write((qtd*3)+2,hr);
      EEPROM.write((qtd*3)+3,mn);
      EEPROM.write((qtd*3)+4,p);
      qtd++;
      EEPROM.write(1,qtd);
      EEPROM.commit();
    }

    openCdt getHora(int qtd)
    {
      openCdt temp;

      temp.hr = EEPROM.read(((qtd)*3)+2);
      temp.mn = EEPROM.read(((qtd)*3)+3);
      temp.peso = (EEPROM.read(((qtd)*3)+4))*10;
      temp.rdy = true;
      

      return temp;
    }
  
    void clearMem()
    {
      EEPROM.write(1,0);
      EEPROM.commit();
    }
};

void calibrarBalanca()
{
  scale.begin(D3, D4); 
  scale.set_scale(447500); 

  delay(2000);
  scale.tare(); 
  //scale.power_down();
  Serial.println("Balanca Zerada");
}

int getPeso()
{
  
  int medida = (int)(scale.get_units(1)*1000);
  scale.power_down();
  delay(1000);
  scale.power_up();
  return medida;
}
  
MEMNV mem;
Motor M = Motor();
Relogio R;

openCdt *cdtVet;//Vetor de condições de abertura, ainda não alocado.

int qtd; //Quantidade de condições de abertura.

void setup() {
  
  
  
  //--------------------------------SERIAL---------------------------------------
  Serial.begin(9600); //Inicia o monitor serial
  while (!Serial) {}
  //----------------------------------------------------------------------------- 
  
  R.adjustTime();//Ajusta o tempo do relógio de acordo com o sistema.


  
  //O código na sessão a seguir corresponde às configurações do alimentador, aqui serão adicionadas as condições de abertura e também é possivel limpar a lista de condições.
    mem.clearMem();
    mem.addHora(15, 45, 250);
    mem.addHora(15, 47, 250);

  //-------------------------------------------------------------------------------------------------------------------------------------------------------------------------

  qtd = EEPROM.read(1); //Lê, na memória, quantas condições de abertura foram armazenadas.

  cdtVet = (openCdt*)malloc(sizeof(openCdt)*qtd); //Aloca a quantida de posições no vetor de condições necessárias.

  for(int i = 0;i<qtd;i++) //Lê as condições da memória para o vetor de condições.
  {
    cdtVet[i] = mem.getHora(i);
  }
  calibrarBalanca();
}










void loop() 
{
  for(int i = 0;i<qtd;i++) //Compara as condições de abertura para verificar qual ação executar.
  {
      if(R.verificaHora((uint8_t)cdtVet[i].hr,(uint8_t)cdtVet[i].mn) && getPeso()<cdtVet[i].peso && cdtVet[i].rdy)
      {
          M.abre();
          while(getPeso()<cdtVet[i].peso){yield();}
          M.fecha();
          for(int j = 0;j<qtd;j++)
          {
            cdtVet[j].rdy = true;
          }
          cdtVet[i].rdy = false;
      }
  }
  delay(1000);

  
}
