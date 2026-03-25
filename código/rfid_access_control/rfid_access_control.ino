#include <MFRC522.h>
#include <SPI.h>
#include <LiquidCrystal.h>

#define SS_PIN 10
#define RST_PIN 9

MFRC522 mfrc522(SS_PIN, RST_PIN);

// LCD: RS, E, D4, D5, D6, D7
LiquidCrystal lcd(6, 7, 5, 4, 3, 2);

#define LED_NEGADO A0
#define LED_LIBERADO A1

void mensageminicial();

void setup() 
{
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();

  Serial.println("Aproxime o seu cartao do leitor...");
  Serial.println();

  pinMode(LED_NEGADO, OUTPUT);
  pinMode(LED_LIBERADO, OUTPUT);

  digitalWrite(LED_NEGADO, LOW);
  digitalWrite(LED_LIBERADO, LOW);

  lcd.begin(16, 2);
  mensageminicial();
}

void loop() 
{
  // Aguarda cartão
  if (!mfrc522.PICC_IsNewCardPresent()) 
    return;

  if (!mfrc522.PICC_ReadCardSerial()) 
    return;

  Serial.print("UID da tag: ");

  String conteudo = "";

  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);

    conteudo.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    conteudo.concat(String(mfrc522.uid.uidByte[i], HEX));
  }

  Serial.println();
  Serial.print("Mensagem: ");
  conteudo.toUpperCase();

  // TAG AUTORIZADA
  if (conteudo.substring(1) == "1F 45 CF C8") 
  {
    Serial.println("Acesso liberado");

    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Ola pessoal !");
    lcd.setCursor(0,1);
    lcd.print("Acesso liberado!");

    for (int i = 0; i < 6; i++) 
    {
      digitalWrite(LED_LIBERADO, HIGH);
      delay(250);
      digitalWrite(LED_LIBERADO, LOW);
      delay(250);
    }

    mensageminicial();
  }

  // TAG NEGADA
  if (conteudo.substring(1) == "59 19 47 05") 
  {
    Serial.println("Acesso negado");

    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Ola pessoal !");
    lcd.setCursor(0,1);
    lcd.print("Acesso Negado!");

    for (int i = 0; i < 6; i++) 
    {
      digitalWrite(LED_NEGADO, HIGH);
      delay(250);
      digitalWrite(LED_NEGADO, LOW);
      delay(250);
    }

    mensageminicial();
  }
}

void mensageminicial()
{
  lcd.clear();
  lcd.print("Aproxime o seu");
  lcd.setCursor(0,1);
  lcd.print("cartao do leitor");
}
