#include  <LiquidCrystal.h>

#define pin2 2
#define pin3 3
#define rele 13
#define pinLuz A1
#define pinPir A2
#define pinTemp A3
#define pinHum A4
#define LCD A0

float temperatura = 0;
int humedad;
bool pir;
int luz;
int b = 0;

LiquidCrystal lcd(8,9,4,5,6,7);

void setup() 
{
	pinMode(pin2, INPUT);
	pinMode(pin3, INPUT);
	pinMode(pinLuz, INPUT);
	pinMode(pinPir, INPUT);
	pinMode(pinTemp, INPUT);
	pinMode(pinHum, INPUT);
	pinMode(LCD, INPUT);
  pinMode(rele, OUTPUT);


	lcd.begin(16, 2);
	lcd.setCursor(0,0);
	lcd.print("Comienzo");
	lcd.setCursor(0,1);
	  
	Serial.begin(9600);
  
  digitalWrite(rele, LOW);
  digitalWrite(pin2, HIGH);
  
	attachInterrupt(digitalPinToInterrupt(pin3), sobrecalentamiento, LOW);
  attachInterrupt(digitalPinToInterrupt(pin2), releLampara, FALLING);
}

void loop() 
{
  int x = 0; 
  x = analogRead(LCD);
  
  if (x < 1023)
  {
    ActualizarLcd();
  }
  else if (temperatura < 60)
  {
    PorSiempre(b);
  }
  else
  {
  digitalWrite(pin3, LOW);
  }

  int mov = MedirMov();

  if(mov == 1)
  {
    digitalWrite(pin2, LOW);
    Serial.print(" ESTADO PIN 2: ");
  Serial.println(digitalRead(2));
  }

  if(mov == 0)
  {
    digitalWrite(pin2, HIGH);
    digitalWrite(rele, LOW);
        Serial.print(" ESTADO PIN 2: ");
  Serial.println(digitalRead(2));
  }
  
  delay(200);

  Serial.println(" ");
	Serial.print(" Temperatura: ");
	Serial.print(MedirTemp());
	Serial.print(" Humedad: ");
	Serial.print(MedirHumedad());
	Serial.print(" Movimiento: ");
  Serial.print(MedirMov());
}

int ActualizarLcd()
{
  int x = 0;
	x = analogRead(LCD);
	lcd.setCursor(0,1);
	//digitalWrite(estadoLampara, LOW);
  Serial.println(" ");
  Serial.println(x);
  Serial.println(" ");

	if (x < 60)
	{
		//"Right" HUMEDAD
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Valor Humedad:");
    lcd.setCursor(0,1);
    lcd.print(MedirHumedad());
    lcd.print("%");
    b = 1;
	}
 
	else if (x < 200)
	{
		//Up LUZ
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Valor Luz:");
    lcd.setCursor(0,1);
    lcd.print(MedirLuz());
    lcd.setCursor(1,6);
    lcd.print("lm");

        if (luz < 10)
    {
    lcd.print(" - Oscuro");
    }

    else if (luz < 200)
    {
    lcd.print(" - Tenue");
    }

    else if (luz < 500)
    {
    lcd.print(" - Ilumunado");
    }

    else if (luz < 800)
    {
    lcd.print(" - Brillante");
    }

    else
    {
    lcd.print(" - Brillante++");
    }
    b = 2;
	} 


	else if (x < 400)
	{
		//Down PIR
		lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Sensor PIR:");
    lcd.setCursor(0,1);
    bool mov = MedirMov();
    if (mov == true)
    {
    lcd.print("Hay movimiento");
    }
    else
    {
    lcd.print("No hay movimiento");
    }
    b = 3;
	}


	else if (x < 600)
	{
		//Left TEMPERATURA
		lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Temperatura:");
    lcd.setCursor(0,1);
    lcd.print(MedirTemp());
    lcd.print(" C");
    b = 4;
	}

	else if (x < 800)
	{
		//Select
		lcd.clear();
    b = 5;
	}

}

void PorSiempre(int b)
{
  digitalWrite(pin3, HIGH);
  
	if(b == 1)
	{	//HUMEDAD
		lcd.clear();
		lcd.setCursor(0,0);
		lcd.print("Valor Humedad:");
		lcd.setCursor(0,1);
		lcd.print(MedirHumedad());
		lcd.print("%");
	}

	else if (b == 2)
	{	//LUZ
		lcd.clear();
		lcd.setCursor(0,0);
		lcd.print("Valor Luz:");
		lcd.setCursor(0,1);
		lcd.print(MedirLuz());
		lcd.setCursor(1,6);
		lcd.print("lm");
	    
		if (luz < 10)
		{
		lcd.print(" - Oscuro");
		}

		else if (luz < 200)
		{
		lcd.print(" - Tenue");
		}

		else if (luz < 500)
		{
		lcd.print(" - Ilumunado");
		}

		else if (luz < 800)
		{
		lcd.print(" - Brillante");
		}

		else
		{
		lcd.print(" - Brillante++");
		}
	}

	else if (b == 3)
	{	//MOVIMIENTO
		lcd.clear();
		lcd.setCursor(0,0);
		lcd.print("Sensor PIR:");
		lcd.setCursor(0,1);
		bool mov = MedirMov();
    if (mov == true)
    {
    lcd.print("Hay movimiento");
    }
    else
    {
    lcd.print("No hay movimiento");
    }
	}

	else if (b == 4)
	{	//TEMPERATURA
		lcd.clear();
		lcd.setCursor(0,0);
		lcd.print("Temperatura:");
		lcd.setCursor(0,1);
		lcd.print(MedirTemp());
		lcd.print(" C");
	}
}

int MedirHumedad()
{
    humedad = map(analogRead(humedad), 1023, 0, 0, 100);
    return humedad;
}

int MedirLuz()
{
    luz = analogRead(pinLuz);
    return luz;
}

bool MedirMov()
{
    pir = digitalRead(pinPir);
    return pir;
}

float MedirTemp()
{
    temperatura = analogRead(pinTemp);
    float temp = ((temperatura * 5000.0) / 1023) / 10;
    return temp;
}

//interrupciones

void sobrecalentamiento()
{
	lcd.clear();
	lcd.setCursor(0,0);
	lcd.print("Advertencia:");
	lcd.setCursor(0,1);
	lcd.print("Muy Caliente");
  digitalWrite(pin3, HIGH);
	Serial.println("");
	Serial.println("ENTRE A LA INTERRUPCION");
	Serial.println("");
 
}

void releLampara()
{
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Detectado");
  digitalWrite(rele, HIGH);
  Serial.println("RELE ENCENDIDO");
  Serial.println(rele);
}
