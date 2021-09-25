/*
 * main.c
 *
 *  Created on: Sep 23, 2021
 *      Author: ahmet
 */

#include <avr/io.h>
#include <stdlib.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "uart.h"

#ifndef F_CPU
#error "F_CPU undefined, please define CPU frequency in Hz in Makefile"
#endif

// Harici UART kutuphanesi icin gerekli on ayarlar
#define UART_BAUD_RATE      9600
#define TEMP_BUFFER_SIZE	64


volatile uint8_t ADC_index = 0;
volatile uint16_t ADC_Output[2];
char buffer[2][10]; // ADC 10 bit cozunurluge sahip oldugu icin 


// Fonksiyon Prototipleri
void adc_setup();
void start_conversion();



ISR(ADC_vect) // ADC Conversion Complete interrupt
{
	if(ADC_index == 0)  // Sira ADC0 okumasinda ise
	{
		ADMUX &= ~(15 << 0); // ADMUX bitlerini temizle, yani ADC0 secili
	}


	if(ADC_index == 1) // Sira ADC1 okumasinda ise
	{
		ADMUX &= ~(15 << 0); // ADMUX bitlerini temizle
		ADMUX |= (1 << MUX0); // Ve ADC1'i sec
	}

	start_conversion(); // ADC Conversion islemi

	if(ADC_index == 0) // ADC0 conversion islemi tamamlandi ise
	{
		ADC_Output[0] = ADCW; // ADCWord register'indan veriyi al
    	itoa(ADC_Output[0], buffer[0], sizeof(buffer[0])); // int veriyi string'e cevir ve buffer'a at
    	// Eger UART ile ilgili veriyi yazdirma islemimiz yoksa buna gerek yok
	}

	if(ADC_index == 1) // ADC1 conversion islemi tamamlandi ise
	{
		 ADC_Output[1] = ADCW;
		 itoa(ADC_Output[1], buffer[1], sizeof(buffer[1]));   // ayni seyler
	}

	ADC_index ++;
	if(ADC_index > 1)
	{
		ADC_index = 0;
	}
}

int main(void)
{
	sei(); //interrupt'lari aktive et
   	uart_init( UART_BAUD_SELECT(UART_BAUD_RATE,F_CPU) ); // UART lib icin on tanim ayarlari
   	adc_setup();


	while(1)
	{
		// Verileri UART ile yolla
		uart_puts("Analog0 = ");
		uart_puts(buffer[0]);
		uart_putc('\n');
		uart_puts("Analog1 = ");
		uart_puts(buffer[1]);
		uart_putc('\n');
		_delay_ms(300);
	}

}

void adc_setup()
{
	ADCSRA |= (1 << ADEN); // ADC Enable bit
	ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); // ADC Prescaler 128
	ADCSRA |= (1 << ADIE); // ADC Complate interrupt aktive et
	ADMUX  |= (1 << REFS0); // Referans voltaj olarak AVcc'i sectik, kanal secimi ISR icinde idi
	DIDR0 |= (1 << ADC0D) | (1 << ADC1D); // Digital input buffer'ini devre disi biraktik ki gereksiz
										  // yere calismasin
	start_conversion();
}

void start_conversion()
{
	ADCSRA |= (1 << ADSC); // Conversion islemine basla, islem tamamlandigi zaman bu bit geri sifir oluyor.
}














