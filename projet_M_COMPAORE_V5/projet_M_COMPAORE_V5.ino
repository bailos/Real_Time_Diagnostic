/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
                      /*:::::::::Auteur COMPAORE OUSMANE::::::::: */ 

// bibliothèques pour le lecteur de carte SD:
#include "FS.h"
#include <SPI.h>
#include <SD.h>

//bibliotheques des capteurs
#include <Adafruit_AHT10.h>
#include "Adafruit_VEML7700.h"
#include <ACS712.h>
//bibliotheque de la lcd
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

// bibliothèques pour la communication WiFi
#include "WiFi.h"
#include <WebServer.h>

// bibliothèque pour l'affichage de la date et de l'heure
#include <time.h>
#include <ezTime.h>
/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
                      /*:::::::::Configuration des fonctions::::::::: */ 

                      
LiquidCrystal_I2C lcd(0x27,20,4); 
Adafruit_VEML7700 veml = Adafruit_VEML7700();
Adafruit_AHT10 aht; 

// ssd (addresse)& mot de passe du réseau wifi
const char *ssid = "TECNOCAMON18P"; // changer pour le nom de votre reseau 
const char *password = "projet04"; // changer par le mot de passe de votre reseau 

// constantes
const int delaiEnMinutes = 1; // combien de minutes entre deux mesures consécutives?
const int stockage_max = 10; //nombre maximal de données stockées en mémoire, pour affichage dans la page web
const int decalage = 00;  // la valeur dépend de votre fuseau horaire. Essayez 2 pour la France.

// Declaratipn de  variables globales
unsigned long derniereMesure;  // derniere fois qu'une mesure a été prise
String mesures[stockage_max]; // tableau contenant les plus récentes mesures
time_t date_et_heure[stockage_max]; // tableau contenant le moment des plus récents scans
int nb_de_donnees = 0; // nombre de données déjà stockées dans mesures et dateHeure

File monFichier; // fichier sur la carte SD
 /** Fichier de sortie avec les mesures */
 struct tm * timeinfo;
  String contenu;
File file;

//Initialisation de lheure
Timezone Burkina;
String currentTimeString = "";
String minuteExtraZero = "";
String secondeExtraZero = "";
int currentTime[6] = {0, 0, 0, 0, 0, 0}; //Heure = 0, Minute = 1, Seconde = 0, Jour = 2, Mois = 3, Année = 4
char monthList[13][16] = {"", "janvier", "février", "mars", "avril", "mai", "juin", "juillet", "août", "septembre", "octobre", "novembre", "décembre"}; //Le premier est vide sinon janvier serait 0 au lieu de 1
int jourAJour = 0;
String interfaceErreur = "";
String  dataMessage1;
String lux ;
 String voltage ;
 String current ;
  String current2 ;
   String current3 ;
    String current4 ;
     String current5 ;
  String temperatureC ;
String dataMessage;

/** Nom du fichier de sortie */
// Un fichier format excel du "data" sera creer dans la carte SD
const char* OUTPUT_FILENAME = "dat.csv";
/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
                      /*:::::::::Configuration des entrees / sorties de la carte ::::::::: */ 

#define ACT_PIN 35
#define light_PIN 27
#define current_PIN 34
#define current2_PIN 33
#define current3_PIN 32
#define current4_PIN 26
#define current5_PIN 25

/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
                      /*:::::::::Configuration de la partie web ::::::::: */ 

WebServer serveur(80);

// Construction de la page web
void handle_root() {

  struct tm * timeinfo;
  String contenu;

  for (int i = 0; i < stockage_max; i++) {
    if (date_et_heure[i] != 0)
    {
      timeinfo = localtime(&date_et_heure[i]);
      contenu.concat( "<p> Date et heure: " + String(timeinfo->tm_mday) + "-" + String(timeinfo->tm_mon + 1) + "-" + String(timeinfo->tm_year + 1900) + "   " + String(timeinfo->tm_hour) + ":" + String(timeinfo->tm_min) + ":" + String(timeinfo->tm_sec) + "    Mesure: " + String(mesures[i]) + "</p>");
    
    }
  }

  serveur.send(200, "html", "<head>  <title>Data logger ESP</title> <meta http-equiv=Refresh content=30></head> "
               "<body><H1>" + String(stockage_max) + " plus r&eacute;centes mesures enregistr&eacute;es</H1>"
               "<p>" + contenu + "<p>"
               "<p>Créé par OUSMANE COMPAORE</p></body>");
  delay(100);
}
/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
                      /*:::::::::Fonction permettant d'obtenir le temps courant ::::::::: */ 

void getCurrentTime()
{
  currentTimeString = Burkina.dateTime();
  currentTime[0] = Burkina.hour();

  int temp = Burkina.minute();
  int seconde = Burkina.second();
  if (temp < 10)
    minuteExtraZero = "0";
  else
    minuteExtraZero = "";
  if (seconde < 10)
    secondeExtraZero = "0";
  else
    secondeExtraZero = "";

  currentTime[1] = temp;
  currentTime[2] = seconde;
  currentTime[3] = Burkina.day();
  currentTime[4] = Burkina.month();
  currentTime[5] = Burkina.year();
}


/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
                      /*:::::::::Ecrire des informations dans un fichier stocke sur la carte SD ::::::::: */ 

void writeFile(fs::FS &fs, const char * path, const char * message) {
  Serial.printf("Writing file: %s\n", path);

  File file = fs.open(path, FILE_WRITE);
  if(!file) {
    Serial.println("Failed to open file for writing");
    return;
  }
  if(file.print(message)) {
    Serial.println("File written");
  } else {
    Serial.println("Write failed");
  }
  file.close();
}

/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
                      /*:::::::::Fonction permettant l'ajout d'information dans le fichier stocker sur la SD cart ::::::::: */ 

void appendFile(fs::FS &fs, const char * path, const char * message) {
  Serial.printf("Ecriture dans le fichier: %s\n", path);

  File file = fs.open(path, FILE_APPEND);
  if (!file) {
    Serial.println("Echec de l'ouverture du fichier");
    return;
  }
  if (file.print(message)) {
    Serial.println("Modification du fichier reussie.");
  } else {
    Serial.println("Echec de la modification du fichier");
  }
  file.close();
}


/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
                      /*:::::::::Fonction de paramettrage::::::::: */ 

void setup()
{
   // Initialisation de l'afficheur LCD
  lcd.init();
  lcd.backlight();
  lcd.clear();
  
// Definir les entrees des capteurs 
pinMode(ACT_PIN, INPUT);
pinMode(light_PIN, INPUT);
pinMode(current2_PIN, INPUT);
pinMode(current3_PIN, INPUT);
pinMode(current4_PIN, INPUT);
pinMode(current5_PIN, INPUT);
pinMode(current_PIN, INPUT);
 //Configuration du port serie sur 115200 bauds   
  Serial.begin(115200);
  Serial.println();
 Serial.println("Adafruit AHT10 demo!");

  if (! aht.begin()) {
    Serial.println("Could not find AHT10? Check wiring");
    while (1) delay(10);
  }
  Serial.println("AHT10 found");
   if (!veml.begin()) {
    Serial.println("Sensor not found");
    while (1);
  }
  Serial.println("Sensor found");
  // Se connecter au reseau wifi 
  Serial.println("Initialisation terminée.");
  WiFi.begin(ssid, password);
  Serial.print("Connexion au reseau WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }

  Serial.println();
// Afficher le ssid et l'adresse IP de la page web
  Serial.print("Nom du reseau WiFi: ");
  Serial.println(ssid);
  Serial.print("Adresse IP de la page web: ");
  Serial.println(WiFi.localIP());

  serveur.on("/", handle_root);

  serveur.begin();
  Serial.println("Le serveur web est en fonction.");
Serial.println("Burkina:" + UTC.dateTime());
  Timezone Burkina;
  Burkina.setLocation(F("Africa/Abidjan"));

  configTime(decalage * 3600, 0, "ca.pool.ntp.org");  //serveurs NTP canadiens
  // en Europe, essayez europe.pool.ntp.org ou fr.pool.ntp.org

  Serial.print("Attente date et heure");


  Serial.println();
  Serial.println("Pret.");

  Serial.print("Initialisation de la carte SD...");
// Activer la broche d'amoncage de la carte SD avec le GPIO 
  if (!SD.begin(5)) {  //  CS du lecteur de carte branché à GPIO5
    Serial.println("echec!");
    while (1);
  }
  Serial.println("reussie");
  // Stocker les information sur le fichier au format CSV avec colonne JOUR--mois-annee-heure---minutes----secondes-- UTC---;lux;---voltage;----current;---Temperature
File file = SD.open("/data9.CSV");
  if(!file) {
    Serial.println("File doens't exist");
    Serial.println("Creating file...");
    writeFile(SD, "/data9.CSV", "JOUR;--mois;-annee;-heure;---minutes;----secondes;-- UTC;---;lux;---voltage;----current;---Temperature \r\n");
  }
  else {
    Serial.println("File already exists");  
  }
  file.close();

  }


/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
                      /*:::::::::Instruction a executer en boucle  ::::::::: */ 

void loop()
{
  sensors_event_t humidity, temp;
  aht.getEvent(&humidity, &temp);// Recuperation de l'humidite et la temperature 
  

temperatureC =temp.temperature; //Recuperation de la temperature fourni par le capteur 
  
     lux = veml.readLux(VEML_LUX_AUTO);// Recuperation de l'ensoleillement fournit par le capteur 
 
 voltage = (float)analogRead(ACT_PIN); // Recuperation de la tension generale a etalonnes selon le capteur 

 //Recuperation des differents courants a etalonne selon le capteur 
  current= (float)analogRead(current_PIN);
  current2 = (float)analogRead(current2_PIN);

current3 = (float)analogRead(current3_PIN);

current4 = (float)analogRead(current4_PIN);

current5 = (float)analogRead(current5_PIN);

/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
                      /*:::::::::Affichage des grandeurs sur moniteur serie   ::::::::: */ 

   Serial.print(" Date et heure");Serial.print(Burkina.dateTime());
 Serial.print(" --Lux = ");Serial.print(lux);
 Serial.print("--voltage : "); Serial.print(voltage); 
 Serial.print("--IG: "); Serial.print(current);
  Serial.print("--I2: "); Serial.print(current2);
   Serial.print("--I3: "); Serial.print(current3);
    Serial.print("--I4: "); Serial.print(current4);
     Serial.print("--I5: "); Serial.print(current5);
 Serial.print("--Temperature: ");Serial.println(temp.temperature); 
String tp = temperatureC ;
/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
                      /*:::::::::Ecrire les donnees sur un afficheurs LCD  ::::::::: */ 

 lcd.setCursor(0, 0);
   lcd.print("lheure");
   lcd.scrollDisplayRight();
  //  delay(1000);
   lcd.setCursor(7, 0);
  lcd.print(Burkina.hour());Serial.print(":");
  lcd.scrollDisplayRight();
  // delay(1000);
  lcd.setCursor(10, 0);
   lcd.print(Burkina.minute());Serial.print(":");
     lcd.scrollDisplayRight();
  // delay(1000);
   lcd.setCursor(14, 0);
    lcd.print(Burkina.second());
      lcd.scrollDisplayRight();
  // delay(1000);
  lcd.setCursor(0,1);
  lcd.print("Lux=");lcd.print(lux);
    lcd.scrollDisplayRight();
  // delay(1000);
   lcd.setCursor(10,1);
  lcd.print("tp=");lcd.print(tp);Serial.println("%");
    lcd.scrollDisplayRight();
  // delay(1000);
   lcd.setCursor(0,2);
  lcd.print("UG=");lcd.print(voltage);
    lcd.scrollDisplayRight();
  // delay(1000);
  lcd.setCursor(11,2);
  lcd.print("IG=");lcd.print(current);
    lcd.scrollDisplayRight();
  // delay(1000);
 lcd.setCursor(0, 3);
  lcd.print("I2=");lcd.print(current2);
  lcd.scrollDisplayRight();
  // delay(1000);
    lcd.setCursor(11, 3);
  lcd.print("I3=");lcd.print(current3);
    lcd.scrollDisplayRight();
  // delay(1000);
  
//  lcd.print("I4=");lcd.print(current4);
//  lcd.print("I5=");lcd.print(current5);S
  
  delay(50000);
   String nouvelleMesure ;
   
  time_t heureMesure;
  struct tm * timeinfo;

  if ((millis() - derniereMesure) >= delaiEnMinutes * 60000) // c'est le moment de prendre une nouvelle mesure
  { 
   
   nouvelleMesure = ";" + String(lux) + "lm ;" + String(voltage ) + "mv ;" + String(current) + "mA ;" + String(current2) + "mA ;" +  String(current3) + "mA ;" + String(current4) + "mA ;" + String(current5) + "mA ;" 
                  + String(temperatureC) +"_degrees C_""\r\n";

 dataMessage1 = String( Burkina.dateTime())+ "§§§" + nouvelleMesure ;

  Serial.print("Save data: ");
  Serial.println(dataMessage1);
  appendFile(SD, "/data9.CSV", dataMessage1.c_str());
 // mise à jour du fichier sur la carte SD
    char message [100];
//  sprintf (message, "Date et heure:  %d-%d-%d    %d:%d:%d    Mesure: %d-%d-%d-%d \n", timeinfo->tm_mday, timeinfo->tm_mon + 1, timeinfo->tm_year + 1900, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec, nouvelleMesure );
//appendFile(SD, "/data3.CSV", message);
    time(&heureMesure);  // quelle heure est-il?
    timeinfo = localtime(&heureMesure);


    // Stockage des 10 mesures les plus récentes pour affichage dans la page web
    if (nb_de_donnees < stockage_max) { // il y a encore des lignes vides pour l'affichage web
      mesures[nb_de_donnees] = nouvelleMesure;
      date_et_heure[nb_de_donnees] = heureMesure;  // quelle heure est-il?
      nb_de_donnees++;
    }
    else // la page web est pleine
    {
      for (int i = 0; i < stockage_max - 1; i++) { // on décale toutes les infos d'une ligne
        mesures[i] = mesures[i + 1];
        date_et_heure[i] = date_et_heure[i + 1];
      }
      mesures[stockage_max - 1] = nouvelleMesure;  // on enregistre la nouvelle info
      date_et_heure[stockage_max - 1] = heureMesure;  // quelle heure est-il?
    }

    derniereMesure = millis();
  }

  serveur.handleClient();
    events();
  if (minuteChanged())
  {
    getCurrentTime();
}
 
}
