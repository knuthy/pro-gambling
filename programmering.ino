/* 
  Firedimensionelt String array, med 15 elementer:

    - Våben-navn
    - Skin-navn
    - Pris
    - Farvekode

*/
String caseone[ 15 ][ 4 ] = { 
  { 
    "XM1014", 
    "Quicksilver",
    "0.8", 
    "BLUE" 
  }, 
  { 
    "SCAR-20",
    "Grotto",
    "10.1", 
    "BLUE" 
  }, 
  { 
    "M249", 
    "System Lock",
    "1.98", 
    "BLUE" 
  }, 
  { 
    "Glock-18", 
    "Catacombs",
    "1.62", 
    "BLUE" 
  }, 
  { 
    "MP9", 
    "Deadly Poison",
    "4.88", 
    "BLUE" 
  }, 
  { 
    "Dual Berettas",
    "Urban Shock",
    "4.24", 
    "PURPLE" 
  }, 
  { 
    "MAC-10",
    "Malachite",
    "4.61", 
    "PURPLE" 
  }, 
  { 
    "Sawed-Off",
    "Serenity",
    "4.86", 
    "PURPLE" 
  }, 
  { 
    "Desert Eagle", 
    "Naga",
    "16.31", 
    "PURPLE" 
  }, 
  { 
    "P250", 
    "Muertos",
    "16.89", 
    "PINK" 
  }, 
  { 
    "AK-47", 
    "Cartel",
    "39.42", 
    "PINK" 
  }, 
  { 
    "M4A4", 
    "Dragon King",
    "59.52", 
    "PINK" 
  }, 
  { 
    "AWP", 
    "Man-o'-war",
    "44.44", 
    "RED" 
  }, 
  { 
    "Galil AR", 
    "Chatterbox",
    "187.82", 
    "RED" 
  }, 
  { 
    "Karambit", 
    "Doppler",
    "420.69", 
    "GOLD" 
  } 
};

// Biblioteker
#include <TouchScreen.h> //touch library
#include <LCDWIKI_GUI.h> //Core graphics library
#include <LCDWIKI_KBV.h> //Hardware-specific library

// Definerer skærmen med arduino-pins
LCDWIKI_KBV screen(ILI9341,A3,A2,A1,A0,A4);

// Definerer LCD pins
#define YP A3
#define XM A2
#define YM 9 
#define XP 8 

// Laver en ny instans af touchskærm klassen, med pins og modstand
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

// Minimum og maximum værdier til mapping-funktion, så skærmkoordinatsættet fra skærmen er det samme på LCD-pladen.
#define TS_MINX 911
#define TS_MAXX 117
#define TS_MINY 87 
#define TS_MAXY 906

// Definerer farvekoder -http://www.rinkydinkelectronics.com/calc_rgb565.php
#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF
#define GREY    0xC618
#define SKIN_BLUE 0x4B5F
#define SKIN_PURPLE 0x8A3F
#define SKIN_PINK 0xD17C
#define SKIN_RED 0xEA49
#define SKIN_GOLD 0xFEA0

// Definerer min og max trykkekraft for et acceptabelt tryk på skærmen
#define MINPRESSURE 10
#define MAXPRESSURE 1000

// Variable til spillet
bool at_menu = false;
bool openingCase = false;
float balance = 100;
int openCounts = 0;
float case_price = 10;

// Funktion til at vise string
void show_string(String str, int16_t x, int16_t y, uint8_t csize, uint16_t fc, uint16_t bc, boolean mode) {
    screen.Set_Text_Mode(mode);
    screen.Set_Text_Size(csize);
    screen.Set_Text_colour(fc);
    screen.Set_Text_Back_colour(bc);
    screen.Print_String(str, x, y);
}

// Funktion til at render loading-skærm
void render_welcome() {
  // Gør hele skærmen sort
  screen.Fill_Screen(BLACK);
  
  // Lav text
  show_string("Case Opening", CENTER, 125, 2, WHITE, BLACK, 1);
  show_string("By Rask & Knuth", CENTER, 175, 2, RED, BLACK, 1);
  
}

// Funktion til at render hjem-skærm
void render_home() {
  // Nu er vi ved menuen
  at_menu = true;
  
  // Gør hele skærmen sort
  screen.Fill_Screen(BLACK);
  
  // Lav kasser og tekst
  screen.Fill_Round_Rectangle(5, 5, 235, 50, 5);
  show_string("Balance: $"+String(balance), CENTER, 21, 2, WHITE, BLACK, 1);
  
  show_string("Chroma Case", CENTER, 155, 2, SKIN_GOLD, WHITE, 1);
  
  screen.Fill_Round_Rectangle(5, 260, 235, 310, 5);
  // String interpolation med pris (String)
  show_string("Open Case: $"+String(case_price), CENTER, 278, 2, WHITE, BLACK, 1);

}

// Funktion til at render spillerens balance
void render_balance() {
  // Overskriv firkant
  screen.Fill_Round_Rectangle(5, 5, 235, 50, 5);
  // Overskriv balance med string interpolation (String)
  show_string("Balance: $"+String(balance), CENTER, 21, 2, WHITE, BLACK, 1);
}

// Funktion til at åbne kasse
void open_case() {
    
    // Gør hele skærmen sort
    screen.Fill_Screen(BLACK);
    
    // Variable til at åbne kasse
    int rollDelay = 10;
    int rollCount = 20;
    
    // Loop til en smooth rolling-animation. Vi går igennem loopet 20 gange
    for (int roll = 1; roll < rollCount + 1; roll++) {
      
      // Vælg et random tal mellem 0 og 15, da vi har 15 skins i kassen
      int randomIndex = random(15);
      // Vælg data fra array'et ud fra index'ey
      String item_name = caseone[randomIndex][0];
      String item_skin = caseone[randomIndex][1];
      String item_color = caseone[randomIndex][3];
      
      // Overskriv midten af skærmen
      screen.Fill_Rect(0, 50, 235, 200, BLACK);

      // Vis navnet på våbnet     
      show_string(item_name, CENTER, 130, 2, WHITE, BLACK, 1);

      // Vælg den korrekte farve, ud fra den angivne farvekode som string -> Vis skinnets navn og farve      
      if(item_color == "BLUE") {
        show_string(item_skin, CENTER, 155, 2, SKIN_BLUE, BLACK, 1);
      } else if(item_color == "PURPLE") {
        show_string(item_skin, CENTER, 155, 2, SKIN_PURPLE, BLACK, 1);
      } else if(item_color == "PINK") {
        show_string(item_skin, CENTER, 155, 2, SKIN_PINK, BLACK, 1);
      } else if(item_color == "RED") {
        show_string(item_skin, CENTER, 155, 2, SKIN_RED, BLACK, 1);
      } else if(item_color == "GOLD") {
        show_string(item_skin, CENTER, 155, 2, SKIN_GOLD, BLACK, 1);
      }
      
      // Er vi ved den 20. Genstand, altså sidste i loopet, som også er den endelige genstand?
      if(roll == rollCount) { // Ja
        // Vælg prisen fra array'et
        String item_price = caseone[randomIndex][2];
        // Vis prisen under skinnets navn, string interpolation
        show_string("$"+item_price, CENTER, 180, 2, WHITE, BLACK, 1);
        // Opbevar en temp variabel med skinnets pris som float, så vi kan tilføjet det til brugerens balance
        float new_price = item_price.toFloat();
        // Tilføj til balance
        balance += new_price;
        // Lav delay, så det ikke forsvinder med det samme
        delay(1500);
        // Render hjem-skærmen forfra
        render_home();
        // Sæt et delay, så der ikke registreres et kort tryk på "åbn" knappen med det samme. BUG FIX
        delay(200);
        // Nu åbner vi ikke længere en kasse
        openingCase = false;
      } else { // Nej
        // Sæt delay på presat værdi (10) gange loopets iteration. Det går altså langsommere og lamgsommere
        delay(rollDelay * roll);
      }
  
    }
}

// Setup funktion
void setup(void)  {
  // Baud rate
  Serial.begin(9600);

  // Initialisér LCD'et
  screen.Init_LCD();

  // Læs og print ID'et fra skærmen
  Serial.println(screen.Read_ID(), HEX);

  // Pinmodes
  pinMode(13, OUTPUT);

  // Render preloader
  render_welcome();
  // Vent 2000 ms
  delay(2000);
  // Render hjem-skærmen
  render_home();
}

// Loop funltion
void loop() {

  // Send strøm til LCD'et
  digitalWrite(13, HIGH);
  // Aflæs værdier på LCD'et
  TSPoint point = ts.getPoint();

  // Stop med at sende strøm til LCD'et
  digitalWrite(13, LOW);
  // Ændrer pinmodes
  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);

  // Er trykket mellem vores grænser?
  if(point.z > MINPRESSURE && point.z < MAXPRESSURE) {

    // Map trykket til at passe til koordinaterne på skærmen, og IKKE touchscreenen.
    point.x = map(point.x, TS_MINX, TS_MAXX, 0, screen.Get_Display_Width());
    point.y = map(point.y, TS_MINY, TS_MAXY, 0, screen.Get_Display_Height());
    
    // Er vi på menuskærmen?
    if(at_menu) {
      // Er en case allerede ved at blive åbnet?
      if(!openingCase) {
        // Bliver der trykket under 260px på skærmen, ALTSÅ i bunden?
        if(point.y >= 260) {
          // BUG FIX - Hvis ikke der tjekkes for åbning af kasse her igen, åbnes den to gange, da skærmen opdaterer hurtigere end loopet.
          if(!openingCase) {
            // Lag fix fra overnævnte, vi sætter allerede kassen til åbnet, så vi kan tjekke værdier i mens der ventes på skærmen
            openingCase = true;
            // Har brugeren nok penge?
            if(balance >= case_price) { // Ja
              // Træk penge fra brugerens balance
              balance-= case_price;
              // I gang sæt åbning af kassen
              open_case();
              // Læg en til totale antal åbnede kasser
              openCounts++;
              // Sæt prisen op med 5
              case_price = case_price + 5;
            } else { // Nej
              // Vi åbner IKKE en kasse ALLIGEVEL
              openingCase = false;
              // Vis error
              show_string("Du har ikke penge nok", CENTER, 180, 2, RED, BLACK, 1);
            }
          }
  
        }
      }
    }
  }
}

// Memory-fix funktion
void(* resetFunc) (void) = 0; 