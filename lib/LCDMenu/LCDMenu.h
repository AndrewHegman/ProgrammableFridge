#ifndef LCD_MENU_H_
#define LCD_MENU_H_

#define LCD_NUMBER_OF_LINES (2)
#define LCD_NUMBER_OF_ROWS  (16)

#include "LinkedList.h"
#include <Arduino.h>

struct menuScreen_t {
    LinkedList<String> text;
};

class LCDMenu{
    public:
        LCDMenu();

        /**
         * Return the current menu screen
         */
        menuScreen_t current();

        /**
         * Advance to the next menu screen
         */
        menuScreen_t next();
        
        /**
         *  Go back to the previous menu screen
         */
        menuScreen_t previous();
        
        /**
         * The action to be taken depends on the
         * current menu screen.
         */
        menuScreen_t select();

        /**
         * \brief Register a new menu screen to be displayed.
         * Screens are displayed numerically in order.
         * This allows the select button to advance to
         * a new screen that may or may not be in 
         * numerical order.
         * 
         * \param menu The text to be displayed in this screen
         * \param index The order in which to display this screen
         * \param select Determines which screen to go to
         *               when the select button is pressed.
         *               Select button presses can be ignored 
         *               by setting this parameter to 'index'
         */
        void RegisterMenuScreen(menuScreen_t menu, uint8_t index, uint8_t select);

        /**
         * \brief Register a new menu screen to be displayed.
         * Screens will be displayed numerically in order.
         * 
         * \param menu The text to be displayed in this screen
         * \param index The order in which to display this screen
         */
        void RegisterMenuScreen(menuScreen_t menu, uint8_t index = -1);

        /**
         * \brief Attempts to remove a menu screen at given index.
         *        Returns true on success, false if index is not valid.
         * 
         * \param index Index of screen to be removed
         */
        void RemoveMenuScreen(uint8_t index);

        /**
         * \brief Return the number of currently registered
         *        screens.
         * \return uint8_t Number of registered screens
         */
        uint8_t length();

    private:
        /**
         * \brief Think of this like a vector of all the menu screens
         */
        LinkedList<menuScreen_t>* menuScreens;
        
        /**
         * This dictionary maintains what screen
         * to go to when the select button is pressed.
         * Select button presses can be ignored by
         * setting the key equal to the pair (in dictionary
         * lingo).
         */
        LinkedList<uint8_t>* selectDictionary;

        uint8_t currentScreen;

};

#endif