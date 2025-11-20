#include <Arduino.h>
#include <GEM_u8g2.h>
#include <input.h>

U8G2_ST7565_ERC12864_F_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 18, /* mosi=*/ 4, /* cs=*/ 5, /* dc=*/ 2, /* reset=*/ 15);

// Create variables that will be editable through the menu and assign them initial values
int number = -512;
bool enablePrint = false;

// Define spinner boundaries
// Use brace-initialization for aggregate or provide a constructor in the type.
GEMSpinnerBoundariesInt spinnerBoundaries{10, -1000, 1000}; // min: -1000, max: 1000, step: 10
GEMSpinner spinner(spinnerBoundaries);

// Create two menu item objects of class GEMItem, linked to number and enablePrint variables 
GEMItem menuItemInt("Number:", number);
GEMItem menuItemBool("Enable print:", enablePrint);

// Create menu button that will trigger printData() function. It will print value of our number variable
// to Serial monitor if enablePrint is true. We will write (define) this function later. However, we should
// forward-declare it in order to pass to GEMItem constructor
void printData(); // Forward declaration
GEMItem menuItemButton("Print", printData);

// Create menu page object of class GEMPage. Menu page holds menu items (GEMItem) and represents menu level.
// Menu can have multiple menu pages (linked to each other) with multiple menu items each
GEMPage menuPageMain("Main Menu");

// Create menu object of class GEM_u8g2. Supply its constructor with reference to u8g2 object we created earlier
GEM_u8g2 menu(u8g2, GEM_POINTER_ROW, GEM_ITEMS_COUNT_AUTO);
// Which is equivalent to the following call (you can adjust parameters to better fit your screen if necessary):
// GEM_u8g2 menu(u8g2, /* menuPointerType= */ GEM_POINTER_ROW, /* menuItemsPerScreen= */ GEM_ITEMS_COUNT_AUTO, /* menuItemHeight= */ 10, /* menuPageScreenTopOffset= */ 10, /* menuValuesLeftOffset= */ 86);

void setupMenu() {
  // Add menu items to menu page
  menuPageMain.addMenuItem(menuItemInt);
  menuPageMain.addMenuItem(menuItemBool);
  menuPageMain.addMenuItem(menuItemButton);

  // Add menu page to menu and set it as current
  menu.setMenuPageCurrent(menuPageMain);
}

void setup() {
  Serial.begin(115200);
  // activate pull-up resistors for all buttons; TODO: remove when UART
  for(uint8_t i = 0; i < BUTTONS_COUNT; i++){
    pinMode(buttonsPins[i], INPUT_PULLUP);
    Serial.println(buttonsPins[i]);
  }

  // U8g2 library init. Pass pin numbers the buttons are connected to.
  // The push-buttons should be wired with pullup resistors (so the LOW means that the button is pressed)
  u8g2.begin();
  
  u8g2.setContrast(0);

  // Menu init, setup and draw
  menu.init();
  setupMenu();
  menu.drawMenu();
}

void loop() {
  readInput(&menu);
}

void printData() {
  // If enablePrint flag is set to true (checkbox on screen is checked)...
  if (enablePrint) {
    // ...print the number to Serial
    Serial.print("Number is: ");
    Serial.println(number);
  } else {
    Serial.println("Printing is disabled, sorry:(");
  }
}