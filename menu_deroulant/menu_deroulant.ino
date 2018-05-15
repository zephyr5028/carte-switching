/* inclut la librairie LiquidCrystal pour le Lcd + clavier */
#include <LiquidCrystal.h>
#include "Menu.h" // fichier d'entête avec les types pour le menu
#include <Wire.h>
#include "Adafruit_MCP23017.h"

// Basic pin reading and pullup test for the MCP23017 I/O expander
// public domain!
// Connect pin #12 of the expander to Analog 5 (i2c clock)
// Connect pin #13 of the expander to Analog 4 (i2c data)
// Connect pins #15, 16 and 17  (address selection)
// Connect pin #9 of the expander to 5V (power)
// Connect pin #10 of the expander to ground (common ground)
// Connect pin #18 through a ~10kohm resistor to 5V (reset pin, active low)

Adafruit_MCP23017 mcpP1; // address 001
Adafruit_MCP23017 mcpP2; // address 010
Adafruit_MCP23017 mcpP3; // address 011

/* Objet Lcd sur les broches utilisées par le shield Lcd + clavier */
static LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

void doMainMenuAction (byte selectedMenuItem);
void doManuelMenuAction (byte selectedMenuItem);
void doAutoMenuAction (byte selectedMenuItem);
void displayChoice (byte selectedMenuItem, const char** items);

/* Menu principal */
static const char* MAIN_MENU_ITEMS[] = {
  "1-Test Manuel",
  "2-Test Auto."
};

static const Menu_t MAIN_MENU = {
  "Votre choix ?", // titre du menu
  MAIN_MENU_ITEMS, // tableau de choix
  2,               // nombre de choix possibles
  &doMainMenuAction // fonction appelée lorsque l'utilisateur validera son choix
};

/* Sous menu pour Date Heure */
static const char* MANUEL_MENU_ITEMS [] = {
  "M1-swit._dso_nor",
  "M2-swit._dso_alt",
  "M3-ps_valid_io2",
  "M4-ps_valid_io1",
  "M5-io2_valid",
  "M6-io1_valid",
  "M7-error_reset",
  "M8-dsi_asoff1",
  "M9-cpu_valid",
  "M10-cmd_ora",
  "M11-cmd_nor",
  "M12-cmd_alt"
};

static const Menu_t MANUEL_MENU = {
  "Votre choix ?", // titre du sous-menu
  MANUEL_MENU_ITEMS, // tableau de choix
  12,                 // nombre de choix possibles
  &doManuelMenuAction //fonction appelée lorsque l'utilisateur validera son choix
};

/* Sous menu pour Lumiere */
static const char* AUTO_MENU_ITEMS [] = {
  "A1-switch_dso",
  "A2-cde_val_swit"
};

static const Menu_t AUTO_MENU = {
  "Votre choix ?", // titre du sous-menu
  AUTO_MENU_ITEMS, // tableau de choix
  2,                 // nombre de choix possibles
  &doAutoMenuAction //fonction appelée lorsque l'utilisateur validera son choix
};


/** Setup **/
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  /* con figuration du Lcd */
  lcd.begin(16, 2); // 2 lignes de 16 caractères

  /* initialisation des boiters mcp23017 */
  mcpP1.begin(1);      // addresse 1
  mcpP2.begin(2);      // addresse 2
  mcpP3.begin(3);      // addresse 3

  // P3 MCP23017 setup gpio
#define MCP3_B2 0
#define MCP3_B4 1
#define MCP3_B6 2
#define MCP3_B1 3
#define MCP3_B3 4
#define MCP3_B5 5
#define MCP3_C2 6
#define MCP3_C4 7
#define MCP3_C6 8
#define MCP3_A2 9
#define MCP3_A4 10
#define MCP3_A6 11
#define MCP3_D1 12
#define MCP3_D6 13
#define MCP3_D2 14
#define MCP3_D3 15

  mcpP3.pinMode(MCP3_B2, INPUT); // entree B2
  mcpP3.pullUp(MCP3_B2, HIGH); // entree with turn on a 100K pullup internally
  mcpP3.pinMode(MCP3_B4, INPUT); // entree B4
  mcpP3.pullUp(MCP3_B4, HIGH); // entree with turn on a 100K pullup internally
  mcpP3.pinMode(MCP3_B6, INPUT); // entree B6
  mcpP3.pullUp(MCP3_B6, HIGH); // entree with turn on a 100K pullup internally
  mcpP3.pinMode(MCP3_B1, INPUT); // entree B1
  mcpP3.pullUp(MCP3_B1, HIGH); // entree with turn on a 100K pullup internally
  mcpP3.pinMode(MCP3_B3, INPUT); // entree B3
  mcpP3.pullUp(MCP3_B3, HIGH); // entree with turn on a 100K pullup internally
  mcpP3.pinMode(MCP3_B5, INPUT); // entree B5
  mcpP3.pullUp(MCP3_B5, HIGH); // entree with turn on a 100K pullup internally
  mcpP3.pinMode(MCP3_C2, INPUT); // entree C2
  mcpP3.pullUp(MCP3_C2, HIGH); // entree with turn on a 100K pullup internally
  mcpP3.pinMode(MCP3_C4, INPUT); // entree C4
  mcpP3.pullUp(MCP3_C4, HIGH); // entree with turn on a 100K pullup internally
  mcpP3.pinMode(MCP3_C6, INPUT); // entree C6
  mcpP3.pullUp(MCP3_C6, HIGH); // entree with turn on a 100K pullup internally
  mcpP3.pinMode(MCP3_A2, INPUT); // entree A2
  mcpP3.pullUp(MCP3_A2, HIGH); // entree with turn on a 100K pullup internally
  mcpP3.pinMode(MCP3_A4, INPUT); // entree A4
  mcpP3.pullUp(MCP3_A4, HIGH); // entree with turn on a 100K pullup internally
  mcpP3.pinMode(MCP3_A6, INPUT); // entree A6
  mcpP3.pullUp(MCP3_A6, HIGH); // entree with turn on a 100K pullup internally

  mcpP3.pinMode(MCP3_D1, OUTPUT); // sortie
  mcpP3.pinMode(MCP3_D6, OUTPUT); // sortie
  mcpP3.pinMode(MCP3_D2, OUTPUT); // sortie
  mcpP3.pinMode(MCP3_D3, OUTPUT); // sortie

  // P2 MCP23017 setup gpio
#define MCP2_B2 0
#define MCP2_B3 1
#define MCP2_A2 2
#define MCP2_C2 3
#define MCP2_C3 4
#define MCP2_C6 5
#define MCP2_A5 6
#define MCP2_D5 7
#define MCP2_C5 8
#define MCP2_B5 9
#define MCP2_A4 10
#define MCP2_C4 11
#define MCP2_A6 12
#define MCP2_A3 13
#define MCP2_D3 14

  mcpP2.pinMode(MCP2_B2, INPUT); // entree B2
  mcpP2.pullUp(MCP2_B2, HIGH); // entree with turn on a 100K pullup internally

  mcpP2.pinMode(MCP2_B3, OUTPUT); // sortie
  mcpP2.pinMode(MCP2_A2, OUTPUT); // sortie
  mcpP2.pinMode(MCP2_C2, OUTPUT); // sortie
  mcpP2.pinMode(MCP2_C3, OUTPUT); // sortie
  mcpP2.pinMode(MCP2_C6, OUTPUT); // sortie
  mcpP2.pinMode(MCP2_A5, OUTPUT); // sortie
  mcpP2.pinMode(MCP2_D5, OUTPUT); // sortie
  mcpP2.pinMode(MCP2_C5, OUTPUT); // sortie
  mcpP2.pinMode(MCP2_B5, OUTPUT); // sortie
  mcpP2.pinMode(MCP2_A4, OUTPUT); // sortie
  mcpP2.pinMode(MCP2_C4, OUTPUT); // sortie
  mcpP2.pinMode(MCP2_A6, OUTPUT); // sortie
  mcpP2.pinMode(MCP2_A3, OUTPUT); // sortie
  mcpP2.pinMode(MCP2_D3, OUTPUT); // sortie

  // P1 MCP23017 setup gpio
#define MCP1_A6 0
#define MCP1_B4 1
#define MCP1_D5 2
#define MCP1_B5 3
#define MCP1_C6 4
#define MCP1_D6 5
#define MCP1_A4 6
#define MCP1_B2 7
#define MCP1_C1 8
#define MCP1_B3 9
#define MCP1_D4 10
#define MCP1_C4 11
#define MCP1_D3 12
#define MCP1_B1 13
#define MCP1_A2 14
#define MCP1_D1 15

  mcpP1.pinMode(MCP1_A6, INPUT); // entree A6
  mcpP1.pullUp(MCP1_A6, HIGH); // entree with turn on a 100K pullup internally
  mcpP1.pinMode(MCP1_B4, INPUT); // entree B4
  mcpP1.pullUp(MCP1_B4, HIGH); // entree with turn on a 100K pullup internally
  mcpP1.pinMode(MCP1_D5, INPUT); // entree D5
  mcpP1.pullUp(MCP1_D5, HIGH); // entree with turn on a 100K pullup internally
  mcpP1.pinMode(MCP1_B5, INPUT); // entree B5
  mcpP1.pullUp(MCP1_B5, HIGH); // entree with turn on a 100K pullup internally
  mcpP1.pinMode(MCP1_C6, INPUT); // entree C6
  mcpP1.pullUp(MCP1_C6, HIGH); // entree with turn on a 100K pullup internally
  mcpP1.pinMode(MCP1_D6, INPUT); // entree D6
  mcpP1.pullUp(MCP1_D6, HIGH); // entree with turn on a 100K pullup internally
  mcpP1.pinMode(MCP1_A4, INPUT); // entree A4
  mcpP1.pullUp(MCP1_A4, HIGH); // entree with turn on a 100K pullup internally
  mcpP1.pinMode(MCP1_B2, INPUT); // entree B2 sans pullup
  mcpP1.pinMode(MCP1_C1, INPUT); // entree C1
  mcpP1.pullUp(MCP1_C1, HIGH); // entree with turn on a 100K pullup internally
  mcpP1.pinMode(MCP1_B3, INPUT); // entree B3 sans pullup
  mcpP1.pinMode(MCP1_D4, INPUT); // entree D4 sans pullup
  mcpP1.pinMode(MCP1_C4, INPUT); // entree C4 sans pullup

  mcpP1.pinMode(MCP1_D3, OUTPUT); // sortie
  mcpP1.pinMode(MCP1_B1, OUTPUT); // sortie
  mcpP1.pinMode(MCP1_A2, OUTPUT); // sortie
  mcpP1.pinMode(MCP1_D1, OUTPUT); // sortie

  // P3 arduino setup gpio
#define ARDP3_D4 2
#define ARDP3_D5 3
#define ARDP2_B1 11
#define ARDP2_D1 12
#define ARDP2_A1 13
#define ARDP2_C1 15
#define ARDP1_C2 16
#define ARDP1_D2 17


}

/** Programme principale */
void loop() {

  /* affiche le menu principale */
  displayMenu ( MAIN_MENU );

  /*Demo pour montrer la sortie du menu */
  lcd.clear();
  lcd.print(F("Menu le retour"));
  delay(1000); // attente avant de réouvrir le menu
}

/**  Fonction retournant le bouton appuyé si existant */
Button_t readPushButton(void) {

  /* lecture de l'entrée A0 */
  unsigned int val = analogRead(A0);

  /* test suivant les fourchettes de valeurs */
  if (val > 1000) return BP_NONE;
  if (val < 50) return BP_RIGHT;
  if (val < 195) return BP_UP;
  if (val < 380) return BP_DOWN;
  if (val < 555) return BP_LEFT;
  if (val < 790) return BP_SELECT;

  /* par defaut aucun bouton n'est appuyé */
  return BP_NONE;
}

/** affiche le menu passé en argument */
void displayMenu (const Menu_t &menu ) {

  /* variable pour le menu */
  byte selectedMenuItem = 0; // choix seclectionné
  byte shouldExitMenu = false; // devient true quand l'utilisateur valide
  Button_t buttonPressed; // contient le bouton appuyé

  /* tant que l'utilisateur ne veut pas quitter le menu */
  while (!shouldExitMenu) {

    /* affiche le menu */
    lcd.clear();
    lcd.print(menu.prompt); // titre
    lcd.setCursor(0, 1);
    lcd.print(menu.items[selectedMenuItem]); // choix dans le tableau

    /* attend le relâchement du bouton */
    while (readPushButton() != BP_NONE);

    /* attend l'appui sur un bouton */
    while ((buttonPressed = readPushButton()) == BP_NONE);

    /* anti rebond pour le bouton */
    delay(30);

    /* attend le relâchement du du bouton */
    while (readPushButton() != BP_NONE);

    // évite qu'un appui prolongé ne fasse plusieurs appels

    /* gére l'appui sur le bouton */
    switch (buttonPressed) {
      case BP_UP: // Bouton haut = choix précédent
        /* si existance choix précédent */
        if (selectedMenuItem > 0) {

          /*passe au choix précédent */
          selectedMenuItem--;
        } else {
          selectedMenuItem = menu.nbItems - 1;
        }
        break;

      case BP_DOWN: // Bouton bas = choix suivant
        /* si existance choix précédent */
        if (selectedMenuItem < (menu.nbItems - 1)) {

          /*passe au choix suivant */
          selectedMenuItem++;
        } else {
          selectedMenuItem = 0;
        }
        break;

      case BP_LEFT: // Bouton gauche = sortie du menu
        /* affiche le menu principale */
        displayMenu ( MAIN_MENU );
        break;

      case BP_SELECT: //bouton select = validation du choix
        menu.callbackFnct(selectedMenuItem);
        shouldExitMenu = true; //sortie du menu une fois l'action choisie
        break;

      case BP_RIGHT: // bouton droit = validation du choix
        menu.callbackFnct(selectedMenuItem);
        shouldExitMenu = true; //sortie du menu une fois l'action choisie
        break;
    }
  }
}

/** affiche le choix de l'utilisateur */
void doMainMenuAction (byte selectedMenuItem) {
  /* gère le choix de l'utilisateur */
  switch (selectedMenuItem) {
    case 0:
      /* affiche le sous_menu pour le test manuel */
      displayMenu(MANUEL_MENU);
      break;
    case 1:
      /* affiche le sous_menu pour test automatique */
      displayMenu(AUTO_MENU);
      break;
    default:
      //
      break;
  }
}

/**  affiche le choix de l'utilisateur */
void doManuelMenuAction (byte selectedMenuItem) {
  byte buttonPressed;
  bool etat;
  int tab[5] = {0};
  int i;
  switch (selectedMenuItem) {
    case 0:
      /* test des switchs dso normal -  configuration */
      commandesOn();// toutes les commandes sur on
      //gestion lcd
      lcd.clear();
      lcd.print(F("swit._dso_nor"));
      do {
        // test d'un dso : colonne lcd, objet Connecteur Sortie, numero Dso Entree, numeroDso Sortie
        testDsoObjetArduino(0, mcpP3, ARDP3_D4, MCP3_B2); //dso 14
        // test d'un dso : colonne lcd, objet Connecteur Entree, objet Connecteur Sortie, numero Dso Entree, numeroDso Sortie
        testDsoObjetMcp23017(1, mcpP2, mcpP3, MCP2_B3, MCP3_B4); //dso 13
        testDsoObjetMcp23017(2, mcpP2, mcpP3, MCP2_A2, MCP3_B6); //dso 12
        testDsoObjetArduino(3, mcpP3, ARDP3_D5, MCP3_B1); //dso 11
        testDsoObjetMcp23017(4, mcpP2, mcpP3, MCP2_C2, MCP3_B3); //dso 13
        testDsoObjetMcp23017(5, mcpP2, mcpP3, MCP2_C3, MCP3_B5); //dso 12
        buttonPressed = readPushButton();
      }
      /* attend l'appui sur le bouton gauche ou SELECT */
      while (buttonPressed != BP_RIGHT &&  buttonPressed != BP_SELECT);
      break;
    case 1:
      /* test des switchs dso alternate - configuration */
      commandesOn();// toutes les commandes sur on
      // gestion lcd
      lcd.clear();
      lcd.print(F("swit._dso_alt"));
      do {
        // test d'un dso : colonne lcd, objet Connecteur Entree, objet Connecteur Sortie, numero Dso Entree, numeroDso Sortie
        testDsoObjetMcp23017(0, mcpP3, mcpP1, MCP3_D1, MCP1_A6); //dso 30
        testDsoObjetMcp23017(1, mcpP3, mcpP3, MCP3_D6, MCP3_C2); //dso 29
        testDsoObjetMcp23017(2, mcpP2, mcpP3, MCP2_C6, MCP3_C4); //dso 28
        testDsoObjetMcp23017(3, mcpP2, mcpP3, MCP2_A5, MCP3_C6); //dso 27
        testDsoObjetMcp23017(4, mcpP3, mcpP2, MCP3_D2, MCP2_B2); //dso 24
        testDsoObjetMcp23017(5, mcpP2, mcpP1, MCP1_D5, MCP1_B4); //dso 23
        testDsoObjetMcp23017(6, mcpP2, mcpP1, MCP2_C5, MCP1_D5); //dso 22
        testDsoObjetMcp23017(7, mcpP2, mcpP1, MCP2_B5, MCP1_B5); //dso 21
        testDsoObjetMcp23017(8, mcpP2, mcpP1, MCP2_A4, MCP1_C6); //dso 20
        testDsoObjetMcp23017(9, mcpP3, mcpP1, MCP3_D3, MCP1_D6); //dso 19
        testDsoObjetMcp23017(10, mcpP2, mcpP1, MCP2_C4, MCP1_A4); //dso 18
        testDsoObjetMcp23017(11, mcpP2, mcpP3, MCP2_A6, MCP3_A2); //dso 17
        testDsoObjetMcp23017(12, mcpP2, mcpP3, MCP2_A3, MCP3_A4); //dso 16
        testDsoObjetMcp23017(13, mcpP2, mcpP3, MCP2_D3, MCP3_A6); //dso 15
        buttonPressed = readPushButton();
      }
      /* attend l'appui sur le bouton gauche ou SELECT */
      while (buttonPressed != BP_RIGHT &&  buttonPressed != BP_SELECT);
      break;
    case 2:
      /* test des commandes valid - configuration */
      commandesOn();// toutes les commandes sur on
      etat = true;
      for (i = 0; i <= 4; i++) {
        tab[i] = 0;
      }
      // gestion lcd
      lcd.clear();
      lcd.print(F("test_ps_valid_io2"));
      do {
        /* test des commandes valid et switch */
        // test avec commande : Hight
        digitalWrite(ARDP2_B1, HIGH);
        etat = true;
        delay(10);
        testSortiesValidSwitch (etat, tab); //test des sorties switch et valid
        // test avec commande  : low
        digitalWrite(ARDP2_B1, LOW);
        etat = false;
        delay (10);
        testSortiesValidSwitch (etat, tab); //test des sorties switch et valid
        buttonPressed = readPushButton();
      }
      /* attend l'appui sur le bouton gauche ou SELECT */
      while (buttonPressed != BP_RIGHT &&  buttonPressed != BP_SELECT);
      break;
    case 3:
      /* test des commandes valid - configuration */
      commandesOn();// toutes les commandes sur on
      etat = true;
      for (i = 0; i <= 4; i++) {
        tab[i] = 0;
      }
      // gestion lcd
      lcd.clear();
      lcd.print(F("test_ps_valid_io1"));
      do {
        /* test des commandes valid et switch */
        // test avec commande : Hight
        digitalWrite(ARDP2_D1, HIGH);
        etat = true;
        delay(10);
        // test des sorties commandes : colonne lcd, objet Connecteur Sortie, cde Sortie, etat de la sortie
        testSortiesValidSwitch (etat, tab); //test des sorties switch et valid
        // test avec commande  : low
        digitalWrite(ARDP2_D1, LOW);
        etat = false;
        delay (10);
        // test des sorties commandes : colonne lcd, objet Connecteur Sortie, cde Sortie, etat de la sortie
        testSortiesValidSwitch (etat, tab); //test des sorties switch et valid
        buttonPressed = readPushButton();
      }
      /* attend l'appui sur le bouton gauche ou SELECT */
      while (buttonPressed != BP_RIGHT &&  buttonPressed != BP_SELECT);
      break;
    case 4:
      /* test des commandes valid - configuration */
      commandesOn();// toutes les commandes sur on
      etat = true;
      for (i = 0; i <= 4; i++) {
        tab[i] = 0;
      }
      // gestion lcd
      lcd.clear();
      lcd.print(F("test_io2_valid"));
      do {
        /* test des commandes valid et switch */
        // test avec commande : Hight
        digitalWrite(ARDP2_A1, HIGH);
        etat = true;
        delay(10);
        // test des sorties commandes : colonne lcd, objet Connecteur Sortie, cde Sortie, etat de la sortie
        testSortiesValidSwitch (etat, tab); //test des sorties switch et valid
        // test avec commande  : low
        digitalWrite(ARDP2_A1, LOW);
        etat = false;
        delay (10);
        // test des sorties commandes : colonne lcd, objet Connecteur Sortie, cde Sortie, etat de la sortie
        testSortiesValidSwitch (etat, tab); //test des sorties switch et valid
        buttonPressed = readPushButton();
      }
      /* attend l'appui sur le bouton gauche ou SELECT */
      while (buttonPressed != BP_RIGHT &&  buttonPressed != BP_SELECT);
      break;
    case 5:
      /* test des commandes valid - configuration */
      commandesOn();// toutes les commandes sur on
      etat = true;
      for (i = 0; i <= 4; i++) {
        tab[i] = 0;
      }
      // gestion lcd
      lcd.clear();
      lcd.print(F("test_io1_valid"));
      do {
        /* test des commandes valid et switch */
        // test avec commande : Hight
        digitalWrite(ARDP2_C1, HIGH);
        etat = true;
        delay(10);
        // test des sorties commandes : colonne lcd, objet Connecteur Sortie, cde Sortie, etat de la sortie
        testSortiesValidSwitch (etat, tab); //test des sorties switch et valid
        // test avec commande  : low
        digitalWrite(ARDP2_C1, LOW);
        etat = false;
        delay (10);
        // test des sorties commandes : colonne lcd, objet Connecteur Sortie, cde Sortie, etat de la sortie
        testSortiesValidSwitch (etat, tab); //test des sorties switch et valid
        buttonPressed = readPushButton();
      }
      /* attend l'appui sur le bouton gauche ou SELECT */
      while (buttonPressed != BP_RIGHT &&  buttonPressed != BP_SELECT);
      break;
    case 6:
      /* affiche le choix de l'utilisateur */
      displayChoice (selectedMenuItem, MANUEL_MENU_ITEMS);
      break;
    case 7:
      /* test de la commande asoff1 - configuration */
      commandesOn();// toutes les commandes sur on
      etat = true;
      for (i = 0; i <= 4; i++) {
        tab[i] = 0;
      }
      // gestion lcd
      lcd.clear();
      lcd.print(F("test_dsi_asoff"));
      do {
        /* test des commandes valid et switch */
        // test avec commande : Hight
        mcpP1.digitalWrite(MCP1_B1, HIGH);
        etat = true;
        delay(10);
        // test des sorties commandes : colonne lcd, objet Connecteur Sortie, cde Sortie, etat de la sortie
        testSortiesValidSwitch (etat, tab); //test des sorties switch et valid
        // test avec commande  : low
        mcpP1.digitalWrite(MCP1_B1, LOW);
        etat = false;
        delay (10);
        // test des sorties commandes : colonne lcd, objet Connecteur Sortie, cde Sortie, etat de la sortie
        testSortiesValidSwitch (etat, tab); //test des sorties switch et valid
        buttonPressed = readPushButton();
      }
      /* attend l'appui sur le bouton gauche ou SELECT */
      while (buttonPressed != BP_RIGHT &&  buttonPressed != BP_SELECT);
      break;
    case 8:
      /* test des commandes valid - configuration */
      commandesOn();// toutes les commandes sur on
      etat = true;
      for (i = 0; i <= 4; i++) {
        tab[i] = 0;
      }
      // gestion lcd
      lcd.clear();
      lcd.print(F("test_cpu_valid"));
      do {
        /* test des commandes valid et switch */
        // test avec commande : Hight
        mcpP1.digitalWrite(MCP1_A2, HIGH);
        etat = true;
        delay(10);
        // test des sorties commandes : colonne lcd, objet Connecteur Sortie, cde Sortie, etat de la sortie
        testSortiesValidSwitch (etat, tab); //test des sorties switch et valid
        // test avec commande  : low
        mcpP1.digitalWrite(MCP1_A2, LOW);
        etat = false;
        delay (10);
        // test des sorties commandes : colonne lcd, objet Connecteur Sortie, cde Sortie, etat de la sortie
        testSortiesValidSwitch (etat, tab); //test des sorties switch et valid
        buttonPressed = readPushButton();
      }
      /* attend l'appui sur le bouton gauche ou SELECT */
      while (buttonPressed != BP_RIGHT &&  buttonPressed != BP_SELECT);
      break;
    case 9:
      /* test des commandes - configuration */
      commandesOn();// toutes les commandes sur on
      etat = true;
      for (i = 0; i <= 4; i++) {
        tab[i] = 0;
      }
      // gestion lcd
      lcd.clear();
      lcd.print(F("test_cmd_ora"));
      do {
        /* test des commandes valid et switch */
        // test avec commande : Hight
        mcpP1.digitalWrite(MCP1_D1, HIGH);
        etat = true;
        delay(10);
        // test des sorties commandes : colonne lcd, objet Connecteur Sortie, cde Sortie, etat de la sortie
        testSortiesValidSwitch (etat, tab); //test des sorties switch et valid
        // test avec commande  : low
        mcpP1.digitalWrite(MCP1_D1, LOW);
        etat = false;
        delay (10);
        // test des sorties commandes : colonne lcd, objet Connecteur Sortie, cde Sortie, etat de la sortie
        testSortiesValidSwitch (etat, tab); //test des sorties switch et valid
        buttonPressed = readPushButton();
      }
      /* attend l'appui sur le bouton gauche ou SELECT */
      while (buttonPressed != BP_RIGHT &&  buttonPressed != BP_SELECT);
      break;
    case 10:
      /* test des commandes - configuration */
      commandesOn();// toutes les commandes sur on
      etat = true;
      for (i = 0; i <= 4; i++) {
        tab[i] = 0;
      }
      // gestion lcd
      lcd.clear();
      lcd.print(F("test_cmd_nor"));
      do {
        /* test des commandes valid et switch */
        // test avec commande : Hight
        digitalWrite(ARDP1_C2, HIGH);
        etat = true;
        delay(10);
        // test des sorties commandes : colonne lcd, objet Connecteur Sortie, cde Sortie, etat de la sortie
        testSortiesValidSwitch (etat, tab); //test des sorties switch et valid
        // test avec commande  : low
        digitalWrite(ARDP1_C2, LOW);
        etat = false;
        delay (10);
        // test des sorties commandes : colonne lcd, objet Connecteur Sortie, cde Sortie, etat de la sortie
        testSortiesValidSwitch (etat, tab); //test des sorties switch et valid
        buttonPressed = readPushButton();
      }
      /* attend l'appui sur le bouton gauche ou SELECT */
      while (buttonPressed != BP_RIGHT &&  buttonPressed != BP_SELECT);
      break;
    case 11:
      /* test des commandes - configuration */
      commandesOn();// toutes les commandes sur on
      etat = true;
      for (i = 0; i <= 4; i++) {
        tab[i] = 0;
      }
      // gestion lcd
      lcd.clear();
      lcd.print(F("test_cmd_alt"));
      do {
        /* test des commandes valid et switch */
        // test avec commande : Hight
        digitalWrite(ARDP1_D2, HIGH);
        etat = true;
        delay(10);
        // test des sorties commandes : colonne lcd, objet Connecteur Sortie, cde Sortie, etat de la sortie
        testSortiesValidSwitch (etat, tab); //test des sorties switch et valid
        // test avec commande  : low
        digitalWrite(ARDP1_D2, LOW);
        etat = false;
        delay (10);
        // test des sorties commandes : colonne lcd, objet Connecteur Sortie, cde Sortie, etat de la sortie
        testSortiesValidSwitch (etat, tab); //test des sorties switch et valid
        buttonPressed = readPushButton();
      }
      /* attend l'appui sur le bouton gauche ou SELECT */
      while (buttonPressed != BP_RIGHT &&  buttonPressed != BP_SELECT);
      break;
    default:
      //
      break;
  }
}

/**  affiche le choix de l'utilisateur */
void doAutoMenuAction (byte selectedMenuItem) {
  byte buttonPressed;
  byte etat;
  int tab[5] = {0};
  int i;
  switch (selectedMenuItem) {
    case 0:
      commandesOn();// toutes les commandes sur on
      /* test des switchs dso normal -  configuration */
      //gestion lcd
      lcd.clear();
      lcd.print(F("swit._dso_nor"));
      do {
        // test d'un dso : colonne lcd, objet Connecteur Sortie, numero Dso Entree, numeroDso Sortie
        testDsoObjetArduino(0, mcpP3, ARDP3_D4, MCP3_B2); //dso 14
        // test d'un dso : colonne lcd, objet Connecteur Entree, objet Connecteur Sortie, numero Dso Entree, numeroDso Sortie
        testDsoObjetMcp23017(1, mcpP2, mcpP3, MCP2_B3, MCP3_B4); //dso 13
        testDsoObjetMcp23017(2, mcpP2, mcpP3, MCP2_A2, MCP3_B6); //dso 12
        testDsoObjetArduino(3, mcpP3, ARDP3_D5, MCP3_B1); //dso 11
        testDsoObjetMcp23017(4, mcpP2, mcpP3, MCP2_C2, MCP3_B3); //dso 13
        testDsoObjetMcp23017(5, mcpP2, mcpP3, MCP2_C3, MCP3_B5); //dso 12
        buttonPressed = readPushButton();
      }
      /* attend l'appui sur le bouton gauche ou SELECT */
      while (buttonPressed != BP_RIGHT &&  buttonPressed != BP_SELECT);
      delay(100);
      /* test des switchs dso alternate - configuration */
      // gestion lcd
      lcd.clear();
      lcd.print(F("swit._dso_alt"));
      do {
        // test d'un dso : colonne lcd, objet Connecteur Entree, objet Connecteur Sortie, numero Dso Entree, numeroDso Sortie
        testDsoObjetMcp23017(0, mcpP3, mcpP1, MCP3_D1, MCP1_A6); //dso 30
        testDsoObjetMcp23017(1, mcpP3, mcpP3, MCP3_D6, MCP3_C2); //dso 29
        testDsoObjetMcp23017(2, mcpP2, mcpP3, MCP2_C6, MCP3_C4); //dso 28
        testDsoObjetMcp23017(3, mcpP2, mcpP3, MCP2_A5, MCP3_C6); //dso 27
        testDsoObjetMcp23017(4, mcpP3, mcpP2, MCP3_D2, MCP2_B2); //dso 24
        testDsoObjetMcp23017(5, mcpP2, mcpP1, MCP1_D5, MCP1_B4); //dso 23
        testDsoObjetMcp23017(6, mcpP2, mcpP1, MCP2_C5, MCP1_D5); //dso 22
        testDsoObjetMcp23017(7, mcpP2, mcpP1, MCP2_B5, MCP1_B5); //dso 21
        testDsoObjetMcp23017(8, mcpP2, mcpP1, MCP2_A4, MCP1_C6); //dso 20
        testDsoObjetMcp23017(9, mcpP3, mcpP1, MCP3_D3, MCP1_D6); //dso 19
        testDsoObjetMcp23017(10, mcpP2, mcpP1, MCP2_C4, MCP1_A4); //dso 18
        testDsoObjetMcp23017(11, mcpP2, mcpP3, MCP2_A6, MCP3_A2); //dso 17
        testDsoObjetMcp23017(12, mcpP2, mcpP3, MCP2_A3, MCP3_A4); //dso 16
        testDsoObjetMcp23017(13, mcpP2, mcpP3, MCP2_D3, MCP3_A6); //dso 15
        buttonPressed = readPushButton();
      }
      /* attend l'appui sur le bouton gauche ou SELECT */
      while (buttonPressed != BP_RIGHT &&  buttonPressed != BP_SELECT);
      break;
    case 1:
      /* test des commandes valid - configuration */
      commandesOn();// toutes les commandes sur on
      etat = true;
      for (i = 0; i <= 4; i++) {
        tab[i] = 0;
      }
      // gestion lcd
      lcd.clear();
      lcd.print(F("test_ps_val_io2"));
      do {
        /* test des commandes valid et switch */
        // test avec commande : Hight
        digitalWrite(ARDP2_B1, HIGH);
        etat = true;
        delay(10);
        // test des sorties commandes : colonne lcd, objet Connecteur Sortie, cde Sortie, etat de la sortie
        testSortiesValidSwitch (etat, tab); //test des sorties switch et valid
        // test avec commande  : low
        digitalWrite(ARDP2_B1, LOW);
        etat = false;
        delay (10);
        // test des sorties commandes : colonne lcd, objet Connecteur Sortie, cde Sortie, etat de la sortie
        testSortiesValidSwitch (etat, tab); //test des sorties switch et valid
        buttonPressed = readPushButton();
      }
      /* attend l'appui sur le bouton gauche ou SELECT */
      while (buttonPressed != BP_RIGHT &&  buttonPressed != BP_SELECT);
      delay(200);
      /* test des commandes valid - configuration */
      etat = true;
      for (i = 0; i <= 4; i++) {
        tab[i] = 0;
      }
      // gestion lcd
      lcd.clear();
      lcd.print(F("test_ps_val_io1"));
      do {
        /* test des commandes valid et switch */
        // test avec commande : Hight
        digitalWrite(ARDP2_D1, HIGH);
        etat = true;
        delay(10);
        // test des sorties commandes : colonne lcd, objet Connecteur Sortie, cde Sortie, etat de la sortie
        testSortiesValidSwitch (etat, tab); //test des sorties switch et valid
        // test avec commande  : low
        digitalWrite(ARDP2_D1, LOW);
        etat = false;
        delay (10);
        // test des sorties commandes : colonne lcd, objet Connecteur Sortie, cde Sortie, etat de la sortie
        testSortiesValidSwitch (etat, tab); //test des sorties switch et valid
        buttonPressed = readPushButton();
      }
      /* attend l'appui sur le bouton gauche ou SELECT */
      while (buttonPressed != BP_RIGHT &&  buttonPressed != BP_SELECT);
      delay(200);
      /* test des commandes valid - configuration */
      etat = true;
      for (i = 0; i <= 4; i++) {
        tab[i] = 0;
      }
      // gestion lcd
      lcd.clear();
      lcd.print(F("test_io2_valid"));
      do {
        /* test des commandes valid et switch */
        // test avec commande : Hight
        digitalWrite(ARDP2_A1, HIGH);
        etat = true;
        delay(10);
        // test des sorties commandes : colonne lcd, objet Connecteur Sortie, cde Sortie, etat de la sortie
        testSortiesValidSwitch (etat, tab); //test des sorties switch et valid
        // test avec commande  : low
        digitalWrite(ARDP2_A1, LOW);
        etat = false;
        delay (10);
        // test des sorties commandes : colonne lcd, objet Connecteur Sortie, cde Sortie, etat de la sortie
        testSortiesValidSwitch (etat, tab); //test des sorties switch et valid
        buttonPressed = readPushButton();
      }
      /* attend l'appui sur le bouton gauche ou SELECT */
      while (buttonPressed != BP_RIGHT &&  buttonPressed != BP_SELECT);
      delay(200);
      /* test des commandes valid - configuration */
      etat = true;
      for (i = 0; i <= 4; i++) {
        tab[i] = 0;
      }
      // gestion lcd
      lcd.clear();
      lcd.print(F("test_io1_valid"));
      do {
        /* test des commandes valid et switch */
        // test avec commande : Hight
        digitalWrite(ARDP2_C1, HIGH);
        etat = true;
        delay(10);
        // test des sorties commandes : colonne lcd, objet Connecteur Sortie, cde Sortie, etat de la sortie
        testSortiesValidSwitch (etat, tab); //test des sorties switch et valid
        // test avec commande  : low
        digitalWrite(ARDP2_C1, LOW);
        etat = false;
        delay (10);
        // test des sorties commandes : colonne lcd, objet Connecteur Sortie, cde Sortie, etat de la sortie
        testSortiesValidSwitch (etat, tab); //test des sorties switch et valid
        buttonPressed = readPushButton();
      }
      /* attend l'appui sur le bouton gauche ou SELECT */
      while (buttonPressed != BP_RIGHT &&  buttonPressed != BP_SELECT);
      delay(200);
      /* test de la commande asoff1 - configuration */
      etat = true;
      for (i = 0; i <= 4; i++) {
        tab[i] = 0;
      }
      // gestion lcd
      lcd.clear();
      lcd.print(F("test_dsi_asoff"));
      do {
        /* test des commandes valid et switch */
        // test avec commande : Hight
        mcpP1.digitalWrite(MCP1_B1, HIGH);
        etat = true;
        delay(10);
        // test des sorties commandes : colonne lcd, objet Connecteur Sortie, cde Sortie, etat de la sortie
        testSortiesValidSwitch (etat, tab); //test des sorties switch et valid
        // test avec commande  : low
        mcpP1.digitalWrite(MCP1_B1, LOW);
        etat = false;
        delay (10);
        // test des sorties commandes : colonne lcd, objet Connecteur Sortie, cde Sortie, etat de la sortie
        testSortiesValidSwitch (etat, tab); //test des sorties switch et valid
        buttonPressed = readPushButton();
      }
      /* attend l'appui sur le bouton gauche ou SELECT */
      while (buttonPressed != BP_RIGHT &&  buttonPressed != BP_SELECT);
      delay(200);
      /* test des commandes valid - configuration */
      etat = true;
      for (i = 0; i <= 4; i++) {
        tab[i] = 0;
      }
      // gestion lcd
      lcd.clear();
      lcd.print(F("test_cpu_valid"));
      do {
        /* test des commandes valid et switch */
        // test avec commande : Hight
        mcpP1.digitalWrite(MCP1_A2, HIGH);
        etat = true;
        delay(10);
        // test des sorties commandes : colonne lcd, objet Connecteur Sortie, cde Sortie, etat de la sortie
        testSortiesValidSwitch (etat, tab); //test des sorties switch et valid
        // test avec commande  : low
        mcpP1.digitalWrite(MCP1_A2, LOW);
        etat = false;
        delay (10);
        // test des sorties commandes : colonne lcd, objet Connecteur Sortie, cde Sortie, etat de la sortie
        testSortiesValidSwitch (etat, tab); //test des sorties switch et valid
        buttonPressed = readPushButton();
      }
      /* attend l'appui sur le bouton gauche ou SELECT */
      while (buttonPressed != BP_RIGHT &&  buttonPressed != BP_SELECT);
      delay(200);
      /* test des commandes - configuration */
      etat = true;
      for (i = 0; i <= 4; i++) {
        tab[i] = 0;
      }
      // gestion lcd
      lcd.clear();
      lcd.print(F("test_cmd_ora"));
      do {
        /* test des commandes valid et switch */
        // test avec commande : Hight
        mcpP1.digitalWrite(MCP1_D1, HIGH);
        etat = true;
        delay(10);
        // test des sorties commandes : colonne lcd, objet Connecteur Sortie, cde Sortie, etat de la sortie
        testSortiesValidSwitch (etat, tab); //test des sorties switch et valid
        // test avec commande  : low
        mcpP1.digitalWrite(MCP1_D1, LOW);
        etat = false;
        delay (10);
        // test des sorties commandes : colonne lcd, objet Connecteur Sortie, cde Sortie, etat de la sortie
        testSortiesValidSwitch (etat, tab); //test des sorties switch et valid
        buttonPressed = readPushButton();
      }
      /* attend l'appui sur le bouton gauche ou SELECT */
      while (buttonPressed != BP_RIGHT &&  buttonPressed != BP_SELECT);
      delay(200);
      /* test des commandes - configuration */
      etat = true;
      for (i = 0; i <= 4; i++) {
        tab[i] = 0;
      }
      // gestion lcd
      lcd.clear();
      lcd.print(F("test_cmd_nor"));
      do {
        /* test des commandes valid et switch */
        // test avec commande : Hight
        digitalWrite(ARDP1_C2, HIGH);
        etat = true;
        delay(10);
        // test des sorties commandes : colonne lcd, objet Connecteur Sortie, cde Sortie, etat de la sortie
        testSortiesValidSwitch (etat, tab); //test des sorties switch et valid
        // test avec commande  : low
        digitalWrite(ARDP1_C2, LOW);
        etat = false;
        delay (10);
        // test des sorties commandes : colonne lcd, objet Connecteur Sortie, cde Sortie, etat de la sortie
        testSortiesValidSwitch (etat, tab); //test des sorties switch et valid
        buttonPressed = readPushButton();
      }
      /* attend l'appui sur le bouton gauche ou SELECT */
      while (buttonPressed != BP_RIGHT &&  buttonPressed != BP_SELECT);
      delay(200);
      /* test des commandes - configuration */
      etat = true;
      for (i = 0; i <= 4; i++) {
        tab[i] = 0;
      }
      // gestion lcd
      lcd.clear();
      lcd.print(F("test_cmd_alt"));
      do {
        /* test des commandes valid et switch */
        // test avec commande : Hight
        digitalWrite(ARDP1_D2, HIGH);
        etat = true;
        delay(10);
        // test des sorties commandes : colonne lcd, objet Connecteur Sortie, cde Sortie, etat de la sortie
        testSortiesValidSwitch (etat, tab); //test des sorties switch et valid
        // test avec commande  : low
        digitalWrite(ARDP1_D2, LOW);
        etat = false;
        delay (10);
        // test des sorties commandes : colonne lcd, objet Connecteur Sortie, cde Sortie, etat de la sortie
        testSortiesValidSwitch (etat, tab); //test des sorties switch et valid
        buttonPressed = readPushButton();
      }
      /* attend l'appui sur le bouton gauche ou SELECT */
      while (buttonPressed != BP_RIGHT &&  buttonPressed != BP_SELECT);
      break;
    default:
      //
      break;
  }
}

/** affiche le choix de l'utilisateur */
void displayChoice (byte selectedMenuItem, const char** items) {
  /* affiche le choix de l'utilisateur */
  lcd.clear();
  lcd.print(F("Vous avez choisi :"));
  lcd.setCursor(0, 1);
  lcd.print(items[selectedMenuItem]);
  /* attend l'appui sur le bouton gauche ou SELECT */
  byte buttonPressed;
  do {
    buttonPressed = readPushButton();
  }
  while (buttonPressed != BP_LEFT && buttonPressed != BP_SELECT && buttonPressed != BP_RIGHT);
}

/**routine de test pour un dso avec mcp23017 **/
void testDsoObjetMcp23017 (byte lcdCursor, class Adafruit_MCP23017 objetConnectEntree , class Adafruit_MCP23017 objetConnectSortie, int dsoIn, int dsoOut)
{
  int resultat = 2; // test du dso niveau haut et bas
  lcd.setCursor(lcdCursor, 1);
  // ecriture sortie mcp23017        delay(10);
  objetConnectEntree.digitalWrite(dsoIn, LOW);
  delay(10);
  // lecture et affichage de la lecture de l'entree du mcp23017
  if (objetConnectSortie.digitalRead(dsoOut)) {
    resultat -= 1 ;
  }
  lcd.setCursor(lcdCursor, 1);
  // ecriture sortie mcp23017
  delay(10);
  objetConnectEntree.digitalWrite(dsoIn, HIGH);
  delay(10);
  // lecture et affichage de la lecture de l'entree du mcp23017
  if (objetConnectSortie.digitalRead(dsoOut)) {
    resultat -= 1;
  }
  // affichage 1 = ok, 0 = nok
  if (resultat == 2) {
    lcd.print(F("1"));
  } else {
    lcd.print(F("0"));
  }
}

/**routine de test pour un dso avec arduino **/
void testDsoObjetArduino (byte lcdCursor,  class Adafruit_MCP23017 objetConnectSortie, int dsoIn, int dsoOut)
{
  int resultat = 2; // test du dso niveau haut et bas
  lcd.setCursor(lcdCursor, 1);
  // ecriture sortie mcp23017        delay(10);
  digitalWrite(dsoIn, LOW);
  delay(10);
  // lecture et affichage de la lecture de l'entree du mcp23017
  if (objetConnectSortie.digitalRead(dsoOut)) {
    resultat -= 1 ;
  }
  lcd.setCursor(lcdCursor, 1);
  // ecriture sortie mcp23017
  delay(10);
  digitalWrite(dsoIn, HIGH);
  delay(10);
  // lecture et affichage de la lecture de l'entree du mcp23017
  if (objetConnectSortie.digitalRead(dsoOut)) {
    resultat -= 1;
  }
  // affichage 1 = ok, 0 = nok
  if (resultat == 2) {
    lcd.print(F("1"));
  } else {
    lcd.print(F("0"));
  }
}

/** toutes les commandes switchings sur on **/
void commandesOn() {
  digitalWrite(ARDP2_B1, HIGH);
  digitalWrite(ARDP2_D1, HIGH);
  digitalWrite(ARDP2_A1, HIGH);
  digitalWrite(ARDP2_C1, HIGH);
  mcpP1.digitalWrite(MCP1_D3, HIGH);
  mcpP1.digitalWrite(MCP1_B1, HIGH);
  mcpP1.digitalWrite(MCP1_A2, HIGH);
  mcpP1.digitalWrite(MCP1_D1, LOW);
  digitalWrite(ARDP1_C2, LOW);
  digitalWrite(ARDP1_D2, LOW);
}

/** test des sorties switch et valid **/
void testSortiesValidSwitch (bool etat, int tab[]) {
  if (etat) {
    // test des sorties commandes : colonne lcd, objet Connecteur Sortie, cde Sortie, etat de la sortie
    tab[0] = testCmdObjetMcp23017(0, mcpP1, MCP1_B2, etat); //sortie sys valid
    tab[1] = testCmdObjetMcp23017(1, mcpP1, MCP1_C1, etat); //sortie cde sys valid
    tab[2] = testCmdObjetMcp23017(2, mcpP1, MCP1_B3, etat); //sortie switch nor
    tab[3] = testCmdObjetMcp23017(3, mcpP1, MCP1_D4, etat); //sortie switch alt
    tab[4] = testCmdObjetMcp23017(4, mcpP1, MCP1_C4, etat); //sortie switch ora
  } else {
    if (testCmdObjetMcp23017(0, mcpP1, MCP1_B2, etat) == 1 and  tab[0] == 1)  lcd.print(F("1")); lcd.print(F("0"));
    if (testCmdObjetMcp23017(1, mcpP1, MCP1_C1, etat) == 1 and  tab[0] == 1)  lcd.print(F("1")); lcd.print(F("0"));
    if (testCmdObjetMcp23017(2, mcpP1, MCP1_B3, etat) == 1 and  tab[0] == 1)  lcd.print(F("1")); lcd.print(F("0"));
    if (testCmdObjetMcp23017(3, mcpP1, MCP1_D4, etat) == 1 and  tab[0] == 1)  lcd.print(F("1")); lcd.print(F("0"));
    if (testCmdObjetMcp23017(4, mcpP1, MCP1_C4, etat) == 1 and  tab[0] == 1)  lcd.print(F("1")); lcd.print(F("0"));
  }
}

/** test des commandes switch et valid **/
int testCmdObjetMcp23017 (byte lcdCursor,  class Adafruit_MCP23017 objetConnectSortie, int cmdOut, byte etat) {
  lcd.setCursor(lcdCursor, 1);
  // lecture et affichage de la lecture de l'entree du mcp23017
  // affichage 1 = ok, 0 = nok
  if (objetConnectSortie.digitalRead(cmdOut) == etat) {
    return 1;
  } else {
    return 0;
  }
}

