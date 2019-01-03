#ifndef LCD_MENU_H_
#define LCD_MENU_H_

#define NUMBER_OF_LINES (2)
#define NUMBER_OF_ROWS  (16)

#include "LinkedList.h"

typedef struct menu_screen { char[NUMBER_OF_LINES][NUMBER_OF_ROWS]; };

class LCDMenu{
    public:
        LCDMenu();

        uint8_t currentScreen;

        /**
         * Advance to the next menu screen
         */
        menu_screen next();
        
        /**
         *  Go back to the previous menu screen
         */
        menu_screen previous();
        
        /**
         * The action to be taken depends on the
         * current menu screen.
         */
        menu_screen select();

        /**
         * \brief Register a new menu screen to be displayed.
         *        Screens are displayed numerically in order.
         * 
         * \param menu The text to be displayed in this screen
         * \param index The order in which to display this screen
         * \param select Determines which screen to go to
         *               when the select button is pressed.
         *               Select button presses can be ignored 
         *               by setting this parameter to 'index'
         */
        void RegisterMenuScreen(menu_screen menu, uint8_t index, uint8_t select);

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
        LinkedList<menu_screen>* menuScreens;
        
        /**
         * This dictionary maintains what screen
         * to go to when the select button is pressed.
         * Select button presses can be ignored by
         * setting the key equal to the pair (in dictionary
         * lingo).
         */
        LinkedList<uint8_t>* selectDictionary;

};

#endif