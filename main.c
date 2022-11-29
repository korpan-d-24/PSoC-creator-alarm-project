#include "project.h"

/* Needed for Bargraph */
extern uint8 const CYCODE LCD_customFonts[];
uint16 curPos = 0u;
uint16 maxPos = 16u;
int IsInfected = 0;
int Is = 5000;
int n = 0;
void CapSense_DisplayState(void)
   {
    /* Display BUTTON0 state */

    if (CapSense_CheckIsWidgetActive (CapSense_BUTTON0__BTN))
    {
   
        Pin_1_Write(1u);
    }
    else
    {
        Pin_1_Write(0u);
    }
    /* Display BUTTON1 state */
    if (CapSense_CheckIsWidgetActive(CapSense_BUTTON1__BTN))
    {
        Pin_2_Write(0u);
        IsInfected = 1;
        curPos = 0u;
    }
    else
    {
        IsInfected = 0;
    }
    /* Find Slider Position */
    if(IsInfected == 0)
    {
        curPos++;//CapSense_GetCentroidPos(CapSense_LINEARSLIDER0__LS);//curPos + 1;//
        if(curPos == 160u)
        {
            Pin_2_Write(1u);
        }
        LCD_DrawHorizontalBG(0u, 0u, 16u, curPos >> 1);
        CyDelay(100);
    }
    
    
}

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    LCD_Start();
    CapSense_Start();
    LCD_LoadCustomFonts(LCD_customFonts);

    LCD_Position(1u, 0u);
    LCD_PrintString("Korpan");

    /* Initialize baselines */
    CapSense_InitializeAllBaselines();

    
    while(1u)
    {
        /* Check whether the scanning of all enabled widgets is completed. */
        if(0u == CapSense_IsBusy())
        {
            /* Update all baselines */
            CapSense_UpdateEnabledBaselines();

            /* Start scanning all enabled sensors */
            CapSense_ScanEnabledWidgets();
        }

        /* Display CapSense state using LED/LCD */
        CapSense_DisplayState();
    }


    for(;;)
    {
        /* Place your application code here. */
    }
}