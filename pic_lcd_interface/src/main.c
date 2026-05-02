#include <xc.h>
#include <stdint.h>

// Configuración de bits del PIC16F886
#pragma config FOSC = INTRC_NOCLKOUT, WDTE = OFF, PWRTE = ON, MCLRE = ON, CP = OFF, LVP = OFF

#define _XTAL_FREQ 4000000 

// Definición de pines (Asegúrate de que la conexión física coincida)
#define RS RB0
#define EN RB1
#define D4 RB2
#define D5 RB3
#define D6 RB4
#define D7 RB5

// Función auxiliar para enviar solo 4 bits (necesaria para inicialización)
void Lcd_Port(char a) {
    D4 = (a & 1) ? 1 : 0;
    D5 = (a & 2) ? 1 : 0;
    D6 = (a & 4) ? 1 : 0;
    D7 = (a & 8) ? 1 : 0;
}

void Lcd_Cmd(char a) {
    RS = 0;             
    Lcd_Port(a >> 4);   // Enviar nibble alto
    EN = 1; __delay_us(40); EN = 0; 
    Lcd_Port(a & 0x0F); // Enviar nibble bajo
    EN = 1; __delay_us(40); EN = 0;
    __delay_ms(2);      // Esperar a que el LCD procese
}

void Lcd_Init() {
    Lcd_Port(0x00);
    __delay_ms(20);
    
    // Secuencia de inicialización modo 4 bits según datasheet
    RS = 0;
    Lcd_Port(0x03); EN = 1; __delay_ms(5); EN = 0;
    __delay_ms(5);
    Lcd_Port(0x03); EN = 1; __delay_ms(5); EN = 0;
    __delay_us(150);
    Lcd_Port(0x03); EN = 1; __delay_ms(5); EN = 0;
    
    Lcd_Port(0x02); EN = 1; __delay_ms(5); EN = 0; // Cambiar a modo 4 bits

    Lcd_Cmd(0x28); // 2 líneas, matriz 5x8
    Lcd_Cmd(0x0C); // Display ON, Cursor OFF
    Lcd_Cmd(0x06); // Incremento de cursor
    Lcd_Cmd(0x01); // Limpiar pantalla
    __delay_ms(2);
}

void Lcd_Write_Char(char a) {
    RS = 1;
    Lcd_Port(a >> 4);
    EN = 1; __delay_us(40); EN = 0;
    Lcd_Port(a & 0x0F);
    EN = 1; __delay_us(40); EN = 0;
    __delay_us(40);
}

void Lcd_Write_String(char *a) {
    for(int i=0; a[i]!='\0'; i++) Lcd_Write_Char(a[i]);
}

void Lcd_Set_Cursor(char row, char col) {
    char address = (row == 1) ? (0x80 + col - 1) : (0xC0 + col - 1);
    Lcd_Cmd(address);
}

void main(void) {
    TRISB = 0x00; 
    ANSEL = 0x00;  // Digital I/O
    ANSELH = 0x00; 
    
    Lcd_Init();
    
    while(1) {
        Lcd_Cmd(0x01); // Lcd_Clear
        __delay_ms(2);
        Lcd_Set_Cursor(1,1);
        Lcd_Write_String("PIC16F886 OK");
        Lcd_Set_Cursor(2,1);
        Lcd_Write_String("Lab. Biomedica");
        __delay_ms(2000);
    }
}