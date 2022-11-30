#include "project.h"

/* Needed for Bargraph */
extern uint8 const CYCODE LCD_customFonts[];
uint16 curPos = 160u;
uint16 maxPos = 0u;
int8 i = 0;

int IsInfected = 0;
int Is = 5000;
int n = 0;

void CustomLCD_DrawHorizontalBG(uint8 row, uint8 column, uint8 maxCharacters, uint8 value);
void CustomLCD_WriteData(uint8 dByte);
void CustomLCD_WrDatNib(uint8 nibble);

void CapSense_DisplayState(void)
   {
    /* Display BUTTON1 state */
    if (CapSense_CheckIsWidgetActive(CapSense_BUTTON1__BTN))
    {
        Pin_2_Write(0u);
        IsInfected = 1;
        curPos = 0u;
        LCD_Position(1u, 11u);
        LCD_PrintString("     ");
    }
    else
    {
        IsInfected = 0;
    }
    /* Find Slider Position */
    if(IsInfected == 0)
    {
        curPos--;//CapSense_GetCentroidPos(CapSense_LINEARSLIDER0__LS);//curPos + 1;//
        CustomLCD_DrawHorizontalBG(0u, 0u, 16u, curPos >> 1);
        CyDelay(100);
        if(curPos == 0u)
        {
            while(i < 32)
            {
                LCD_Position(0u,7u);
                LCD_PrintString("Sick");
                LCD_Position(1u, 11u);
                LCD_PrintString("Alarm");
                Pin_2_Write(1u);
                CyDelay(500u);
                Pin_2_Write(0u);
                LCD_ClearDisplay();
                CyDelay(500u);
                i++;
            }
        }
    }
    
    
}
void CustomLCD_WriteData(uint8 dByte) 
{
    uint8 nibble;

    LCD_IsReady();
    
    nibble = dByte >> LCD_NIBBLE_SHIFT;

    /* Write high nibble */
    CustomLCD_WrDatNib(nibble);
    nibble = dByte & LCD_NIBBLE_MASK;
    /* Write low nibble */
    CustomLCD_WrDatNib(nibble);
}
void CustomLCD_WrDatNib(uint8 nibble) 
{
    LCD_PORT_DR_REG &= ((uint8)(~LCD_E));
    LCD_PORT_DR_REG |= (nibble | LCD_E);
    LCD_PORT_DR_REG |= LCD_RS;
}
void CustomLCD_DrawHorizontalBG(uint8 row, uint8 column, uint8 maxCharacters, uint8 value) \
                                                            
    {
        /* 8-bit Counter */
        uint8 count8;
        uint8 fullChars;
        uint8 remainingPixels;

        /* Number of full characters to draw */
        fullChars = value / LCD_CHARACTER_WIDTH;

        /* Number of remaining pixels to draw */
        remainingPixels = value % LCD_CHARACTER_WIDTH;

        /* Ensure that the maximum character limit is followed. */
        if(fullChars >= maxCharacters)
        {
            fullChars = maxCharacters;
        }

        /* Put Cursor at start position */
        LCD_Position(row, column);

        /* Write full characters */
        for(count8 = fullChars; count8 >  0u; count8--)
        {
            CustomLCD_WriteData(LCD_CUSTOM_0);
        }

        if(fullChars < maxCharacters)
        {
            /* Write remaining pixels */
            LCD_WriteData(remainingPixels);

            if(fullChars < (maxCharacters - 1u))
            {
                /* Fill with whitespace to end of bar graph */
                for(count8 = (maxCharacters - fullChars - 1u); count8 > 0u; count8--)
                {
                    CustomLCD_WriteData(LCD_CUSTOM_5);
                }
            }
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
    LCD_PrintString("Korpan B14 COVID");

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