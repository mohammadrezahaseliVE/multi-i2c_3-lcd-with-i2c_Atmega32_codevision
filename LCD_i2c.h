/*******************************************************
Project : I2C library for run LCD(LM016L)
Version : 1.1.5
Date    : 05/13/2022
Author  : Mohammad Reza Haseli 

*******************************************************/


//-----Proto-Type-------//
void lcd_init();			//----LCD Initializing Function
void toggle();				//----Latching function of LCD
void lcd_send_cmd (char cmd);		//----Function to send Command to LCD
void lcd_send_hf_cmd(char cmd);      //----Function to send half Command to LCD
void lcd_send_data (char data);		//----Function to send data to LCD
void lcd_clear (void);			//----Function to clear LCD
void lcd_put_cur(int row, int col);		//----Function to change Cursor of Lcd	
void lcd_send_string(char *str);		//----Function to Send String to LCD
void delay(int ms);
//----------------------//


void toggle ()
{
	TWDR |= 0x02;					//---PIN En de la LCD en = 1;  -----Latching data in to LCD data register using High to Low signal
	TWCR = (1<<TWINT) | (1<<TWEN);	//---Enable I2C and clear interrupt- Esta linea y la siguiente simepre van despues de querer mandar un coamndo por TDWR
	while  (!(TWCR &(1<<TWINT)));	//---Simepre poner despues de la linea anterior al mandar datos por TWDR
	delay(1);
	TWDR &= ~0x02;					//---PIN del Enable de la LCD en = 0;
	TWCR = (1<<TWINT) | (1<<TWEN);	//---Enable I2C and clear interrupt
	while  (!(TWCR &(1<<TWINT)));
}

void lcd_send_cmd (char cmd)
{
    TWDR&=~0x01;                    //rs = 0; ----Selecting register as command register
    TWCR = (1<<TWINT) | (1<<TWEN);    //Enable I2C and clear interrupt
    while  (!(TWCR &(1<<TWINT)));
    TWDR &= 0x0F;                   //----clearing the Higher 4 bits
    TWCR = (1<<TWINT) | (1<<TWEN);    //Enable I2C and clear interrupt
    while  (!(TWCR &(1<<TWINT)));
    TWDR |= (cmd & 0xF0);            //----Masking higher 4 bits and sending to LCD
    TWCR = (1<<TWINT) | (1<<TWEN);    //Enable I2C and clear interrupt
    while  (!(TWCR &(1<<TWINT)));
    toggle();
    
    TWDR &= 0x0F;                    //----clearing the Higher 4 bits
    TWCR = (1<<TWINT) | (1<<TWEN);    //Enable I2C and clear interrupt
    while  (!(TWCR &(1<<TWINT)));
    TWDR |= ((cmd & 0x0F)<<4);        //----Masking lower 4 bits and sending to LCD
    TWCR = (1<<TWINT) | (1<<TWEN);    //Enable I2C and clear interrupt
    while  (!(TWCR &(1<<TWINT)));
    toggle();
}

void lcd_send_hf_cmd(char cmd)
{
	TWDR &=~0x01;					//PIN RS de la LCD rs = 0; ----Selecting register as Command register
	TWCR = (1<<TWINT) | (1<<TWEN);	//Enable I2C and clear interrupt
	while  (!(TWCR &(1<<TWINT)));
	TWDR &= 0x0F;					//----clearing the Higher 4 bits
	TWCR = (1<<TWINT) | (1<<TWEN);	//Enable I2C and clear interrupt
	while  (!(TWCR &(1<<TWINT)));
	TWDR |= (cmd & 0xF0);			//----Masking higher 4 bits and sending to LCD
	TWCR = (1<<TWINT) | (1<<TWEN);	//Enable I2C and clear interrupt
	while  (!(TWCR &(1<<TWINT)));
	toggle();
}


void lcd_send_data (char data)
{
    TWDR|=0x01;                        //rs = 1; ----Selecting register as command register
    TWCR = (1<<TWINT) | (1<<TWEN);    //Enable I2C and clear interrupt
    while  (!(TWCR &(1<<TWINT)));
    TWDR &= 0x0F;                    //----clearing the Higher 4 bits
    TWCR = (1<<TWINT) | (1<<TWEN);    //Enable I2C and clear interrupt
    while  (!(TWCR &(1<<TWINT)));
    TWDR |= (data & 0xF0);            //----Masking higher 4 bits and sending to LCD
    TWCR = (1<<TWINT) | (1<<TWEN);    //Enable I2C and clear interrupt
    while  (!(TWCR &(1<<TWINT)));
    toggle();
    
    TWDR &= 0x0F;                    //----clearing the Higher 4 bits
    TWCR = (1<<TWINT) | (1<<TWEN);    //Enable I2C and clear interrupt
    while  (!(TWCR &(1<<TWINT)));
    TWDR |= ((data & 0x0F)<<4);        //----Masking lower 4 bits and sending to LCD
    TWCR = (1<<TWINT) | (1<<TWEN);    //Enable I2C and clear interrupt
    while  (!(TWCR &(1<<TWINT)));
    toggle();
}

void lcd_init ()
{
    lcd_send_hf_cmd(0x30);       //-----Sequence for initializing LCD
    lcd_send_hf_cmd(0x30);       //-----     "            "              "               "
    lcd_send_hf_cmd(0x20);       //-----     "            "              "               "
    lcd_send_cmd(0x28);          //-----Selecting 16 x 2 LCD in 4Bit mode
    lcd_send_cmd(0x0C);          //-----Display ON Cursor OFF
    lcd_send_cmd(0x01);          //-----Clear display
    lcd_send_cmd(0x06);          //-----Cursor Auto Increment
    lcd_send_cmd(0x80);          //-----1st line 1st location of LCD 
}

void delay (int ms)
{
    int i,j;
    for(i=0;i<=ms;i++)
    for(j=0;j<=120;j++);
}

void lcd_send_string (char *str)
{
    while (*str) lcd_send_data (*str++);
}

void lcd_put_cur(int row, int col)
{
    switch (row)
    {
        case 0:
            col |= 0x80;
            break;
        case 1:
            col |= 0xC0;
            break;
    }

    lcd_send_cmd (col);
}

void lcd_clear (void)
{
    int i;
	lcd_send_cmd (0x80);
	for (i=0; i<70; i++)
	{
		lcd_send_data (' ');
	}
}