// CM_Test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "SDKDLL.h"
#include <windows.h>

//Im showing my true colors as an old-school c dev here, but hey, it works.  This whole app is 'lazy but it works' style.
static bool bExit = false;

BOOL WINAPI consoleHandler(DWORD signal)
{

	if (signal == CTRL_C_EVENT)
	{
		printf("Ctrl-C handled\n"); // do cleanup
		bExit = true;
	}
	else
	{
		printf("Got other signal %d.\n", signal);
	}

	return TRUE;
}

int main()
{
	//Setup the control signal handler.  This will handle the Ctrl-C cleanly and return control back to the keyboard on exit.
	if (!SetConsoleCtrlHandler(consoleHandler, TRUE))
	{
		printf("\nERROR: Could not set control handler");
		return 1;
	}

	//Verify we have a keybaord to work with at all.
	if (!IsDevicePlug())
	{
		printf("Plug in the keyboard ya tool!\n");
		return 0;
	}
	else
	{
		printf("Initiating program...\n");
	}

	//Lots of the rest of the function calls have return values you can check as needed, but I found for my purposes
	// I didnt really care that much so I left that out.

	//Grab control of the board's LEDs.  Must remove control cleanly before exiting or the keyboard needs to be reset.
	if (EnableLedControl(true))
	{
		//Set large layout.  Modify as needed for your keyboard.
		SetControlDevice(DEV_MKeys_L);

		//Need to initiate automatic refresh of LED colors or this would have to be called every time any value
		// is ever changed.  In auto mode it updates immediately upon change.  There would be some cases where
		// a manual refresh might be desired, in which case you'd call this function with false as the parameter every time.
		RefreshLed(true);

		//Transition to the main meat of the app.  Run until ctrl-c.
		while (!bExit)
		{
			//Do 'stuff' in this loop.  Whatever it is you want to do with the keyboard.

			//Set a nice purple color to all keys as an example.
			SetFullLedColor(255,0,255);

			//Sleep for a bit until next refresh.  This is lazy, a timer would be better, but... im being lazy.
			//For my audio visualizer I set this as low as 5 (ms) and found it still used ~1% of my processor
			// to run so it doest have to be set so high here, but since this code doesnt actually DO anything
			// I left it rather high.
			Sleep(100);
		}
		//Only reach here after a ctrl-c event.
		//Disable control of the keyboard's LEDs, returning control back to the on-board processor.
		EnableLedControl(false);
	}
	else
	{
		printf("Failed to take control of LEDs.\n");
	}
	
    return 0;
}

