// main.c
// A simple guide app for a modded 3DS, written in C using the 3DS homebrew libraries.
// Written for educational purposes and to help friends get started with their modded 3DS devices. Not intended for distribution or commercial use.
// By realberch, June 2024.

// Special thanks to Alex and Hank Johnson for letting me have so much fun for free and for being my guinea pigs for this app.

#include <3ds.h>
#include <stdio.h>
#include <string.h>

#define TOP_SCREEN_COLUMNS 50
#define TOP_SCREEN_ROWS    30

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

void printAt(int row, int col, const char* text)
{
    if (row < 1) row = 1;
    if (col < 1) col = 1;
    printf("\x1b[%d;%dH%s", row, col, text);
}

void printCentered(int row, const char* text)
{
    int len = strlen(text);
    int col = ((TOP_SCREEN_COLUMNS - len) / 2) + 1;

    if (col < 1)
        col = 1;

    printAt(row, col, text);
}

void drawHeader()
{
    printCentered(2, "Ryan's Modded 3DS Guide");
    printCentered(3, "by Ryan Bercich");
    printCentered(4, "16 June 2026");
    printCentered(6, "==============================");
}

void drawControls()
{
    printCentered(27, "D-Pad: Move   A: Select   B: Back");
    printCentered(28, "START: Exit");
}

void drawPageControls()
{
    printCentered(27, "B: Back");
    printCentered(28, "START: Exit");
}

void drawSectionTitle(const char* title)
{
    printCentered(8, title);
    printCentered(9, "------------------------------");
}

void drawMenu(const MenuItem* items, int count)
{
    int startRow = 12;
    int startCol = 13;

    for (int i = 0; i < count; i++)
    {
        printf("\x1b[%d;%dH", startRow + (i * 2), startCol);

        if (i == selected)
            printf("> %-26s", items[i].label);
        else
            printf("  %-26s", items[i].label);
    }
}

void drawBodyText(const char* body)
{
    int row = 11;
    int col = 3;
    const char* p = body;

    printf("\x1b[%d;%dH", row, col);

    while (*p != '\0' && row < 26)
    {
        if (*p == '\n')
        {
            row++;
            printf("\x1b[%d;%dH", row, col);
        }
        else
        {
            putchar(*p);
        }

        p++;
    }
}

void drawTextPage(const char* title, const char* body)
{
    clearScreen();
    drawHeader();
    drawSectionTitle(title);
    drawBodyText(body);
    drawPageControls();
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
        drawSectionTitle("Installing new ROMs");

        MenuItem items[] = {
            {"Installing CIAs", SCREEN_INSTALLING_CIAS},
            {"Installing NDS", SCREEN_INSTALLING_NDS}
        };

        drawMenu(items, 2);
        drawControls();
    }
    else if (currentScreen == SCREEN_THEMES)
    {
        drawSectionTitle("Themes");

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
            "/roms/nds/     DS games\n"
            "/cias/         CIA installers\n"
            "/3ds/          Homebrew apps\n"
            "/_nds/         DS launcher files\n"
            "/luma/         Custom firmware files"
        );
    }
    else if (currentScreen == SCREEN_INSTALLING_CIAS)
    {
        drawTextPage(
            "Installing CIAs",
            "CIA files are installed using FBI.\n"
            "FBI is located in the System folder.\n\n"
            "Steps:\n"
            "1. Power off the 3DS.\n"
            "2. Put the SD card in a computer.\n"
            "3. Copy the .cia file to /cias/.\n"
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
            "3. Select the .nds game.\n"
            "4. For ROM hacks, use a custom name.\n"
            "5. Give it a unique title.\n"
            "6. Check HOME Menu for a wrapped gift.\n\n"
            "If the icon does not appear, reboot."
        );
    }
    else if (currentScreen == SCREEN_THEMES_ETC)
    {
        drawTextPage(
            "Theme Setup",
            "Themes are managed with Anemone3DS.\n\n"
            "To install themes:\n"
            "1. Go to themeplaza.art.\n"
            "2. Scan the QR code for a theme.\n"
            "3. Select the theme in Anemone3DS.\n"
            "4. Press A, then UP for full install.\n\n"
            "You can switch themes at any time.\n"
            "You can also shuffle themes so each\n"
            "startup can use a different one.\n\n"
            "Reboot if things do not work right away."
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
