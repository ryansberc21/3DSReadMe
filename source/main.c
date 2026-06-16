// main.c
// A simple guide app for a modded 3DS, written in C using the 3DS homebrew libraries.
// Written for educational purposes and to help friends get started with their modded 3DS devices. Not intended for distribution or commercial use.
// By realberch, June 2024.

// Special thanks to Alex and Hank Johnson for letting me have so much fun for free and for being my guinea pigs for this app.


#include <3ds.h>
#include <stdio.h>
#include <string.h>

typedef enum {
    SCREEN_MAIN,
    SCREEN_GETTING_STARTED,
    SCREEN_INSTALLING_ROMS,
    SCREEN_INSTALLING_CIAS,
    SCREEN_INSTALLING_NDS,
    SCREEN_THEMES,
    SCREEN_THEMES_ETC
} Screen;

typedef struct {
    const char* label;
    Screen target;
} MenuItem;

static Screen currentScreen = SCREEN_MAIN;
static Screen previousScreen = SCREEN_MAIN;
static int selected = 0;

void clearScreen()
{
    printf("\x1b[2J");
    printf("\x1b[1;1H");
}

void drawHeader()
{
    printf("Ryan's Modded 3DS Guide\n");
    printf("by Ryan Bercich\n");
	printf("16 June 2024\n");
    printf("=======================\n\n");
}

void drawControls()
{
    printf("\n\n");
    printf("D-Pad: Move\n");
    printf("A: Select\n");
    printf("B: Back\n");
    printf("START: Exit\n");
}

void drawMenu(const MenuItem* items, int count)
{
    for (int i = 0; i < count; i++)
    {
        if (i == selected)
            printf("> %s\n", items[i].label);
        else
            printf("  %s\n", items[i].label);
    }
}

void drawTextPage(const char* title, const char* body)
{
    clearScreen();
    drawHeader();

    printf("%s\n", title);
    printf("-----------------------\n\n");
    printf("%s\n", body);

    drawControls();
}

void setScreen(Screen next)
{
    previousScreen = currentScreen;
    currentScreen = next;
    selected = 0;
}

void goBack()
{
    switch (currentScreen)
    {
        case SCREEN_GETTING_STARTED:
        case SCREEN_INSTALLING_ROMS:
        case SCREEN_THEMES:
            currentScreen = SCREEN_MAIN;
            break;

        case SCREEN_INSTALLING_CIAS:
        case SCREEN_INSTALLING_NDS:
            currentScreen = SCREEN_INSTALLING_ROMS;
            break;

        case SCREEN_THEMES_ETC:
            currentScreen = SCREEN_THEMES;
            break;

        default:
            currentScreen = SCREEN_MAIN;
            break;
    }

    selected = 0;
}

int getMenuItemCount(Screen screen)
{
    switch (screen)
    {
        case SCREEN_MAIN:
            return 3;

        case SCREEN_INSTALLING_ROMS:
            return 2;

        case SCREEN_THEMES:
            return 1;

        default:
            return 0;
    }
}

void drawCurrentScreen()
{
    clearScreen();
    drawHeader();

    if (currentScreen == SCREEN_MAIN)
    {
        MenuItem items[] = {
            {"Getting Started", SCREEN_GETTING_STARTED},
            {"Installing new ROMs", SCREEN_INSTALLING_ROMS},
            {"Themes", SCREEN_THEMES}
        };

        drawMenu(items, 3);
        drawControls();
    }
    else if (currentScreen == SCREEN_INSTALLING_ROMS)
    {
        printf("Installing new ROMs\n");
        printf("-------------------\n\n");

        MenuItem items[] = {
            {"Installing CIAs", SCREEN_INSTALLING_CIAS},
            {"Installing NDS", SCREEN_INSTALLING_NDS}
        };

        drawMenu(items, 2);
        drawControls();
    }
    else if (currentScreen == SCREEN_THEMES)
    {
        printf("Themes\n");
        printf("-------------------\n\n");

        MenuItem items[] = {
            {"Theme setup", SCREEN_THEMES_ETC}
        };

        drawMenu(items, 1);
        drawControls();
    }
    else if (currentScreen == SCREEN_GETTING_STARTED)
    {
        drawTextPage(
            "Getting Started",
            "This 3DS has custom firmware installed.\n\n"
            "Do not format the SD card.\n"
            "Do not delete random files.\n\n"
            "Important apps:\n"
            "- FBI installs .cia files.\n"
            "- Universal-Updater updates homebrew.\n"
            "- TWiLight Menu++ launches DS games.\n"
            "- Anemone3DS manages themes.\n\n"
            "Important folders:\n"
            "/roms/nds/   DS games\n"
            "/cias/       CIA installers\n"
            "/3ds/        Homebrew apps\n"
            "/_nds/       DS launcher files\n"
            "/luma/       Custom firmware files"
        );
    }
    else if (currentScreen == SCREEN_INSTALLING_CIAS)
    {
        drawTextPage(
            "Installing CIAs",
            "CIA files are installed using FBI.\n"
			"The FBI App is located in the \'System\' folder.\n\n"
            "Steps:\n"
            "1. Power off the 3DS.\n"
            "2. Put the SD card in a computer.\n"
            "3. Copy the .cia file to /cias/ Folder.\n"
            "4. Safely eject the SD card.\n"
            "5. Put the SD card back in the 3DS.\n"
            "6. Open FBI.\n"
            "7. Go to SD -> cias.\n"
            "8. Select the .cia file.\n"
            "9. Choose Install and delete CIA.\n\n"
            "Only install CIA files from trusted sources."
        );
    }
    else if (currentScreen == SCREEN_INSTALLING_NDS)
    {
        drawTextPage(
            "Installing NDS",
            "Nintendo DS games use .nds files.\n\n"
            "To add DS games:\n"
            "1. Copy .nds files to /roms/nds/.\n"
            "2. Open TWiLight Menu++.\n"
            "3. Launch the game from the DS menu.\n\n"
            "To make a HOME Menu forwarder:\n"
            "1. Open Homebrew Launcher.\n"
            "2. Open NDSForwarder.\n"
            "3. Select the .nds game in the directory you placed it in.\n"
            "4. Forward the selected game (*NOTE: If you are forwarding a ROMhack,\n"
			"check the box to create a custom name. Give it a unique name).\n"
            "5. Check HOME Menu for a wrapped gift.\n\n"
            "If the icon does not appear, reboot."
        );
    }
    else if (currentScreen == SCREEN_THEMES_ETC)
    {
        drawTextPage(
            "Theme Setup",
            "Themes are managed with Anemone3DS.\n\n"
            "To install themes:\n"
            "1. Go to \'themeplaza.art\' and scan the QR code of the theme you want.\n"
            "2. Select the new theme and Press A, then UP for full download.\n"
            "3. Enjoy!.\n"
            "You can switch themes at any time, as well as shuffle them \n"
			"so each time you power the console on, it will be different.\n"
            "Reboot if things do not work right away.\n\n"
            "Do not delete system theme data unless you know what it is."
        );
    }
}

void selectCurrentItem()
{
    if (currentScreen == SCREEN_MAIN)
    {
        MenuItem items[] = {
            {"Getting Started", SCREEN_GETTING_STARTED},
            {"Installing new ROMs", SCREEN_INSTALLING_ROMS},
            {"Themes", SCREEN_THEMES}
        };

        setScreen(items[selected].target);
    }
    else if (currentScreen == SCREEN_INSTALLING_ROMS)
    {
        MenuItem items[] = {
            {"Installing CIAs", SCREEN_INSTALLING_CIAS},
            {"Installing NDS", SCREEN_INSTALLING_NDS}
        };

        setScreen(items[selected].target);
    }
    else if (currentScreen == SCREEN_THEMES)
    {
        MenuItem items[] = {
            {"Theme setup", SCREEN_THEMES_ETC}
        };

        setScreen(items[selected].target);
    }
}

int main(int argc, char **argv)
{
    gfxInitDefault();
    consoleInit(GFX_TOP, NULL);

    drawCurrentScreen();

    while (aptMainLoop())
    {
        hidScanInput();

        u32 kDown = hidKeysDown();

        if (kDown & KEY_START)
            break;

        int itemCount = getMenuItemCount(currentScreen);

        if (itemCount > 0)
        {
            if (kDown & KEY_DOWN)
            {
                selected++;
                if (selected >= itemCount)
                    selected = 0;

                drawCurrentScreen();
            }

            if (kDown & KEY_UP)
            {
                selected--;
                if (selected < 0)
                    selected = itemCount - 1;

                drawCurrentScreen();
            }

            if (kDown & KEY_A)
            {
                selectCurrentItem();
                drawCurrentScreen();
            }
        }

        if (kDown & KEY_B)
        {
            goBack();
            drawCurrentScreen();
        }

        gfxFlushBuffers();
        gfxSwapBuffers();
        gspWaitForVBlank();
    }

    gfxExit();
    return 0;
}