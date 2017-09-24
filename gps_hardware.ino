/*
 * Esempio di estrazione dei dati GPS con Arduino Mega 2560 v.3
 * Il modulo GPS usato in questa dimostrazione è un GY-GPS6MV1  
 * basato sul chip NEO-6M-0-001
 * https://www.u-blox.com/sites/default/files/products/documents/NEO-6_DataSheet_(GPS.G6-HW-09005).pdf
 * L'esempio è da prendersi solo come scopo dimostrativo e non richiede librerie esterne.
 * Lo sketch usa 5260 byte dello spazio disponibile per i programmi. 
 * Le variabili globali usano 663 byte di memoria dinamica.

 * (c) 2017 Umberto Genovese per https://uranopedia.com (Il Poliedrico)
 * Licensa GPL v.3  https://www.gnu.org/licenses/gpl-3.0.html
 * 
 */


#define GPS Serial3                       // STABILISCE QUALE INTERFACCIA DI COMUNICAZIONE SERIALE VIENE USATA
//                                           PER ARDINO MEGA POSSONO ESSERE USATE FINO A TRE DIVERSE PORTE HARDWARE
//                                           PER LE ALTRE VERSIONI SARà NECESSARIO RICORRERE ALLA LIBRERIA DI EMULAZIONE <SoftwareSerial.h>
//                                           IL PIN DEL MODULO RX VA COLLEGATO AL TX DEL SERIALE DI ARDUINO E IL TX ALL'RX

String NMEA;

int counter = 0;
int index = 0;
int field = 0;

String value ;

String Latitudine;
String Longitudine;
String Emisfero;
String Ora;
String Direzione;
String Fix;
String Satelliti;
String Scostamento;
String Quota;
String Geoide;
String Data;
String Ang_mag;

void setup() {
  Serial.begin(115200);                             // AVVIA LA COMUNICAZIONE SERIALE PER LA PROIEZIONE DEI DATI RACCOLTI
  GPS.begin(9600);                                  // AVVIA L'INTERFACCIA DI COMUNICAZIONE GPS ALLA CORRETTA VELOCITà DI LETTURA
}

void loop() {
  if (GPS.available()) {

    char c = GPS.read();                            // LEGGE I DATI 
    if (c != '\n') {                                // FINO AL FINE RIGA (\n) 
      NMEA += c;                                    // E LI CARICA NELLA STRINGA GLOBALE NMEA
    }
    else {
      counter = 0;
      index = 0;
      field = 0;
      if (NMEA.substring(0, 6) == "$GPGGA")         // VIENE SELEZIONATO IL CAMPO GPGCA
      {
        while (counter != -1)
        {
          counter = NMEA.indexOf(",", index);       // INDIVIDUA UNA SOTTOSTRINGA 
          value = NMEA.substring(index, counter );  // E LA IMMETTE NEL CAMPO TEMPORANEO value

          switch (field)                            // IN BASE ALLA POSIZIONE DELLA SOTTOSTRINGA AFFIDA IL 
          {                                         // CAMPO TEMPORANEO value ALLA VOCE CORRETTA
            case 1: Ora = value; break;
            case 2: Longitudine = value; break;
            case 3: Emisfero = value; break;
            case 4: Latitudine = value; break;
            case 5: Direzione = value; break;
            case 6: Fix = value; break;
            case 7: Satelliti  = value; break;
            case 8: Scostamento  = value; break;
            case 9: Quota = value; break;
            case 11: Geoide = value; break;

          }


          index = counter;
          index++;
          field ++;
        }

      } else if (NMEA.substring(0, 6) == "$GPRMC")  // LEGGE LA STRINGA GPRMC
      {                                             // ED ESEGUE IL MEDESIMO LAVORO DI ESTRAZIONE DEI DATI
        //Serial.println(NMEA);
        while (counter != -1)
        {
          counter = NMEA.indexOf(",", index);
          value = NMEA.substring(index, counter );

          switch (field)
          {
            case 9: Data = value; break;
              // case 10: Ang_mag = value; break;
          }


          index = counter;
          index++;
          field ++;
        }

      }
      //        ORA STAMPA SU MONITOR SERIALE I DATI RACCOLTI
      Serial.println ("-----------------  Inizio ---------------------");
      Serial.print("Data: "); Serial.println (Data);
      Serial.print("Ore: "); Serial.println(Ora);
      Serial.print("Latitudine: "); Serial.print(Latitudine);
      Serial.print("  "); Serial.println(Emisfero);
      Serial.print("Longitudine: "); Serial.print(Longitudine);
      Serial.print(" "); Serial.println(Direzione);
      Serial.print("Satelliti: "); Serial.println(Satelliti);
      Serial.print("Stato di aggangiamento: "); Serial.println(Fix);
      Serial.print("Altitudine: "); Serial.print(Quota); Serial.println( " metri");
      Serial.print("Geoide: "); Serial.println(Geoide);
      Serial.println ("-----------------  Fine ---------------------");
      Serial.println (" ");
      Serial.println (" ");
      NMEA = "";                                     // AZZERA LA STRINGA NMEA PER UN NUOVO CICLO
    }
  }
}

