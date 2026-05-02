#include <xc.h>
#include <stdint.h>

// Configuración de bits
#pragma config FOSC = INTRC_NOCLKOUT, WDTE = OFF, PWRTE = ON, MCLRE = ON, CP = OFF, LVP = OFF

#define _XTAL_FREQ 4000000 

// --- Definición de Pines de Control (Puerto C) ---
#define RS RC0
#define EN RC1

// --- Definición de Puerto de Datos (Puerto B completo) ---
#define LCD_DATA PORTB

// Prototipos
void Lcd_Cmd(unsigned char a);
void Lcd_Init();
void Lcd_Write_Char(char a);
void Lcd_Write_String(char *a);
void Lcd_Set_Cursor(char row, char col);

void main(void) {
    // Configuración de Puertos
    TRISB = 0x00;  // Todo el Puerto B como salida (Datos D0-D7)
    TRISC = 0xFC;  // RC0 y RC1 como salidas (Control RS/EN)
    
    // Desactivar funciones analógicas (MUY IMPORTANTE)
    ANSEL = 0x00;  
    ANSELH = 0x00; // El Puerto B tiene entradas analógicas por defecto, hay que limpiarlas
    
    Lcd_Init();
    
    while(1) {
        Lcd_Cmd(0x01); // Borrar pantalla
        __delay_ms(2);
        Lcd_Set_Cursor(1,1);
        Lcd_Write_String("8-Bit PortB Mode");
        Lcd_Set_Cursor(2,1);
        Lcd_Write_String("Lab. Biomedica");
        __delay_ms(2000);
    }
}

// Envía comando al LCD
void Lcd_Cmd(unsigned char a) {
    RS = 0;             
    LCD_DATA = a;       // El byte fluye directo a RB0-RB7
    EN = 1; 
    __delay_us(40); 
    EN = 0;
    __delay_ms(2);      
}

void Lcd_Init() {
    __delay_ms(20);     // Espera a que el LCD despierte
    
    // Secuencia para 8 bits
    Lcd_Cmd(0x38);      // 8-bit, 2 líneas, 5x8 puntos
    Lcd_Cmd(0x0C);      // Display ON, Cursor OFF
    Lcd_Cmd(0x06);      // Auto-incremento
    Lcd_Cmd(0x01);      // Limpiar
    __delay_ms(2);
}

void Lcd_Write_Char(char a) {
    RS = 1;
    LCD_DATA = a;       // Envía el carácter
    EN = 1; 
    __delay_us(40); 
    EN = 0;
    __delay_us(40);
}

void Lcd_Write_String(char *a) {
    for(int i=0; a[i]!='\0'; i++) Lcd_Write_Char(a[i]);
}

void Lcd_Set_Cursor(char row, char col) {
    char address = (row == 1) ? (0x80 + col - 1) : (0xC0 + col - 1);
    Lcd_Cmd(address);
}