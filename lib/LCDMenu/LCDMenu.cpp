#include "LCDMenu.h"

LCDMenu::LCDMenu(){
    LinkedList<menu_screen>* menuScreens = new LinkedList<menu_screen>;
    LinkedList<uint8_t>* selectDictionary = new LinkedList<uint8_t>;

    currentScreen = 0;
}

/**
 * Advance to the next menu screen
 */
menu_screen LCDMenu::next(){
    currentScreen++;
    return menuScreens->get(currentScreen); 
}

/**
 *  Go back to the previous menu screen
 */
menu_screen LCDMenu::previous(){
    currentScreen--;
    return menuScreens->get(currentScreen);
}

/**
 * The action to be taken depends on the
 * current menu screen.
 */
menu_screen LCDMenu::select(){
    return menuScreens->get(selectDictionary->get(currentScreen));
}

/**
 * Register a new menu screen to be displayed.
 * Screens are displayed numerically in order.
 */
void LCDMenu::RegisterMenuScreen(menu_screen menu, uint8_t index, uint8_t select){
    menuScreens->add(menu, index);
    selectDictionary->add(index, select);
}

uint8_t LCDMenu::length(){
    return menuScreens->size();
}