byte led_rouge = 6, led_verte = 7, bouton = 4, serrure = 8, buzz = 2;
byte cur_badge[5];
 
RFID rfid(SS_PIN, RST_PIN); 
    
void setup()
{ 
  Serial.begin(9600);
  SPI.begin(); 
  rfid.init();
  pinMode(led_rouge,OUTPUT);
  pinMode(led_verte,OUTPUT);
  pinMode(bouton, INPUT_PULLUP);
  pinMode(serrure, OUTPUT);
  pinMode(buzz, OUTPUT);
  
  if( !digitalRead(bouton))
  {
    for (int n=0; n<1024; n++)  
        EEPROM.write(n,0);
  
  digitalWrite(led_rouge,1);
  digitalWrite(led_verte,1);
  tone(buzz,2093,1000);
  delay(1000);
  digitalWrite(led_rouge,0);
  digitalWrite(led_verte,0);
  }
  else 
  {
    digitalWrite(led_verte,1);
    tone(buzz,2093, 150); delay(150);
    digitalWrite(led_verte,0); delay(150);
    tone(buzz, 2093, 150); digitalWrite(led_verte,1); delay(150);
    digitalWrite(led_verte,0);
  }
}
 
void new_badge()
{
  int start_mem = EEPROM.read(0)*5+1; sauvegardés,
    for(byte n= 0; n<5; n++)
    {
      EEPROM.write(start_mem+n, cur_badge[n]);
    }
    EEPROM.write(0, EEPROM.read(0)+1);
}
 
bool compare_badge()
{
  int nb_badge = EEPROM.read(0); 
  bool badgeOk = false; 
  /*Serial.println("Badge actuel: ");
  Serial.println(cur_badge[0]);
  Serial.println(cur_badge[1]);
  Serial.println(cur_badge[2]);
  Serial.println(cur_badge[3]);
  Serial.println(cur_badge[4]);
  Serial.println("FIN BADGE ACTUEL");*/
  for(int n = 0; n<nb_badge; n++)
  {
  /*Serial.print("Badge memoire lu numero: "); Serial.println(n+1);
  Serial.println(EEPROM.read(n*5+1));
  Serial.println(EEPROM.read(n*5+2));
  Serial.println(EEPROM.read(n*5+3));
  Serial.println(EEPROM.read(n*5+4));
  Serial.println(EEPROM.read(n*5+5));
  Serial.println("FIN BADGE MEMOIRE LU");*/
    badgeOk = EEPROM.read(n*5+1)==cur_badge[0] && EEPROM.read(n*5+2)==cur_badge[1] && EEPROM.read(n*5+3)==cur_badge[2] && EEPROM.read(n*5+4)==cur_badge[3] && EEPROM.read(n*5+5)==cur_badge[4];
    if (badgeOk) return true;
  }
  return false;
}
 
 
 
void loop()
{
    if (rfid.isCard()) {
        if (rfid.readCardSerial())
            {
                for(byte n=0; n<5; n++)
                {
                cur_badge[n] = rfid.serNum[n]; 
                } 
                /*Serial.println(rfid.serNum[0],DEC);
                Serial.println(rfid.serNum[1],DEC);
                Serial.println(rfid.serNum[2],DEC);
                Serial.println(rfid.serNum[3],DEC);
                Serial.println(rfid.serNum[4],DEC);*/
             
         if(compare_badge()) 
         {
          digitalWrite(led_verte,1);
          tone(buzz,523,50);
          delay(50);
          tone(buzz, 783, 50);
          delay(50);
          tone(buzz, 1046, 50);
          delay(50);
          tone(buzz, 1568, 50);
          delay(50);
          tone(buzz, 2093, 70);
          delay(250);
          digitalWrite(serrure, 1);
          delay(3000);
          digitalWrite(led_verte,0);
          digitalWrite(serrure, 0);
         }
         else
         {
          digitalWrite(led_rouge, 1);
          tone(buzz,370,50);
          delay(100);
          tone(buzz, 370, 300);
          delay(1000);
          digitalWrite(led_rouge, 0);
         }
       }
    }
 
          
    if(!digitalRead(bouton)) 
    {
       while(!digitalRead(bouton)) {} 
       delay(100);
       
       digitalWrite(led_rouge, 1); digitalWrite(led_verte,1);   
       while (!rfid.isCard() && digitalRead(bouton)) {}        
       if (rfid.readCardSerial())
       {
        for(byte n=0; n<5; n++)
            {cur_badge[n] = rfid.serNum[n];} 
        if(!compare_badge()) 
            {
              new_badge(); 
              digitalWrite(led_rouge, 0);
              digitalWrite(led_verte, 1);
              tone(buzz, 2093, 500);
              delay(200);
              digitalWrite(led_verte, 0);
              delay(200);
              digitalWrite(led_verte, 1);
              delay(200);
              digitalWrite(led_verte, 0);
              delay(200);
              digitalWrite(led_verte, 1);
              delay(200);
              digitalWrite(led_verte, 0);
            }
          else 
          {
              digitalWrite(led_verte, 0);
              digitalWrite(led_rouge, 1);
              tone(buzz,370,500);
              delay(200);
              digitalWrite(led_rouge, 0);
              delay(200);
              digitalWrite(led_rouge, 1);
              delay(200);
              digitalWrite(led_rouge, 0);
              delay(200);
              digitalWrite(led_rouge, 1);
              delay(200);
              digitalWrite(led_rouge, 0);
          }
        }
      else 
      {
        digitalWrite(led_rouge,0);
        digitalWrite(led_verte,0);
        while(!digitalRead(bouton)) {}
        delay(500);
      }
     }
    rfid.halt();
}