/*
 *
 */

#include "gtglfw.h"

void onExitClicked( MenuItem *pMenuItem )
{
   if( pMenuItem && pMenuItem->pMenu )
   {
      gtExitApp( pMenuItem->pMenu->pApp );
   }
}

static void lastMenuItemClicked( MenuItem *pMenuItem )
{
   if( pMenuItem )
   {
      gtPrintMenuItemStruct( pMenuItem );
   }
}

int main()
{
   App *app = gtCreateWindow( 830, 450, "Sample Menu" );

   Menu *menu = gtMenuNew( app );

   MenuBar *fileMenu = gtMenuBarNew( menu, "File" );
      gtMenuBarAddItem( fileMenu, "New",              GLFW_KEY_N, GLFW_MOD_CONTROL, lastMenuItemClicked );
      gtMenuBarAddItem( fileMenu, "Open",             GLFW_KEY_O, GLFW_MOD_CONTROL, lastMenuItemClicked );
      gtMenuBarAddItem( fileMenu, "Save",             GLFW_KEY_S, GLFW_MOD_CONTROL, lastMenuItemClicked );
      gtMenuBarAddItem( fileMenu, "Save as...",       GLFW_KEY_S, GLFW_MOD_CONTROL | GLFW_MOD_SHIFT, lastMenuItemClicked );
      gtMenuBarAddItem( fileMenu, "Close",            0, 0,                         lastMenuItemClicked );
      gtMenuBarAddItem( fileMenu, "Print",            GLFW_KEY_P, GLFW_MOD_CONTROL, lastMenuItemClicked );
      gtMenuBarAddItem( fileMenu, "Printer setup...", 0, 0,                         lastMenuItemClicked );
      gtMenuBarAddItem( fileMenu, "DOS Shell",        0, 0,                         lastMenuItemClicked );
      gtMenuBarAddItem( fileMenu, "Exit",             GLFW_KEY_Q, GLFW_MOD_CONTROL, lastMenuItemClicked );

   MenuBar *editMenu = gtMenuBarNew( menu, "Edit" );
      gtMenuBarAddItem( editMenu, "Undo",           GLFW_KEY_Z, GLFW_MOD_CONTROL, lastMenuItemClicked );
      gtMenuBarAddItem( editMenu, "Cut",            GLFW_KEY_X, GLFW_MOD_CONTROL, lastMenuItemClicked );
      gtMenuBarAddItem( editMenu, "Copy",           GLFW_KEY_C, GLFW_MOD_CONTROL, lastMenuItemClicked );
      gtMenuBarAddItem( editMenu, "Paste",          GLFW_KEY_V, GLFW_MOD_CONTROL, lastMenuItemClicked );
      gtMenuBarAddItem( editMenu, "Clear",          0, 0,                         lastMenuItemClicked );
      gtMenuBarAddItem( editMenu, "Delete",         GLFW_KEY_DELETE, 0,           lastMenuItemClicked );
      gtMenuBarAddItem( editMenu, "Paragraph",      0, 0,                         lastMenuItemClicked );
      gtMenuBarAddItem( editMenu, "Upcase Block",   0, 0,                         lastMenuItemClicked );
      gtMenuBarAddItem( editMenu, "Downcase Block", 0, 0,                         lastMenuItemClicked );
      gtMenuBarAddItem( editMenu, "Stats of Block", 0, 0,                         lastMenuItemClicked );

   MenuBar *searchMenu = gtMenuBarNew( menu, "Search" );
      gtMenuBarAddItem( searchMenu, "Find",          GLFW_KEY_F, GLFW_MOD_CONTROL, lastMenuItemClicked );
      gtMenuBarAddItem( searchMenu, "Find Next",     GLFW_KEY_G, GLFW_MOD_CONTROL, lastMenuItemClicked );
      gtMenuBarAddItem( searchMenu, "Find Previous", GLFW_KEY_G, GLFW_MOD_CONTROL | GLFW_MOD_SHIFT, lastMenuItemClicked );
      gtMenuBarAddItem( searchMenu, "Replace",       GLFW_KEY_H, GLFW_MOD_CONTROL, lastMenuItemClicked );
      gtMenuBarAddItem( searchMenu, "Go To...",      GLFW_KEY_G, GLFW_MOD_CONTROL | GLFW_MOD_ALT, lastMenuItemClicked );

   MenuBar *utilitiesMenu = gtMenuBarNew( menu, "Utilities" );
      gtMenuBarAddItem( utilitiesMenu, "Calculator",     0, 0, lastMenuItemClicked );
      gtMenuBarAddItem( utilitiesMenu, "Notepad",        0, 0, lastMenuItemClicked );
      gtMenuBarAddItem( utilitiesMenu, "Command Prompt", 0, 0, lastMenuItemClicked );
      gtMenuBarAddItem( utilitiesMenu, "System Info",    0, 0, lastMenuItemClicked );

   MenuBar *optionsMenu = gtMenuBarNew( menu, "Options" );
      gtMenuBarAddItem( optionsMenu, "Preferences", GLFW_KEY_P, GLFW_MOD_CONTROL | GLFW_MOD_ALT, lastMenuItemClicked );
      gtMenuBarAddItem( optionsMenu, "Color Theme", 0, 0, lastMenuItemClicked );
      gtMenuBarAddItem( optionsMenu, "Font Size",   0, 0, lastMenuItemClicked );
      gtMenuBarAddItem( optionsMenu, "Language",    0, 0, lastMenuItemClicked );

   MenuBar *helpMenu = gtMenuBarNew( menu, "Help" );
      gtMenuBarAddItem( helpMenu, "View Help",         GLFW_KEY_F1, 0, lastMenuItemClicked );
      gtMenuBarAddItem( helpMenu, "About",             0, 0, lastMenuItemClicked );
      gtMenuBarAddItem( helpMenu, "Check for Updates", 0, 0, lastMenuItemClicked );
      gtMenuBarAddItem( helpMenu, "Send Feedback",     0, 0, lastMenuItemClicked );

   gtBackground( app, 0x264B82 );

   while( ! gtMainLoop( app ) )
   {
      BEGINDRAWING( app );

         gtDrawMenu( menu );

      ENDDRAWING( app );
      gtWaitEvents();
   }

   gtFreeMenu( menu );
   gtCloseAll( app );
}
