#include "LCDMenu.h"

LCDMenu::LCDMenu(){
    menuScreens = new LinkedList<menuScreen_t>;
    selectDictionary = new LinkedList<uint8_t>;

    currentScreen = 0;
}

menuScreen_t LCDMenu::current(){
    return menuScreens->get(currentScreen);
}

/**
 * Advance to the next menu screen
 */
menuScreen_t LCDMenu::next(){
    currentScreen++;
    return menuScreens->get(currentScreen); 
}

/**
 *  Go back to the previous menu screen
 */
menuScreen_t LCDMenu::previous(){
    currentScreen--;
    return menuScreens->get(currentScreen);
}

/**
 * The action to be taken depends on the
 * current menu screen.
 */
menuScreen_t LCDMenu::select(){
    return menuScreens->get(selectDictionary->get(currentScreen));
}


void LCDMenu::RegisterMenuScreen(menuScreen_t menu, uint8_t index, uint8_t select){
    menuScreens->add(menu, index);
    selectDictionary->add(index, select);
}

void LCDMenu::RegisterMenuScreen(menuScreen_t menu, uint8_t index){
    menuScreens->add(menu);
}

uint8_t LCDMenu::length(){
    return menuScreens->size();
}