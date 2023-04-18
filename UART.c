#include "UART.h"    // include header file "UART.h"

void USART2_Init(void){
 RCC->APB1ENR  |=  0x20000;    // Starta clockan för usart2
 RCC->AHB1ENR   |=0x01;         
 
GPIOA->MODER &=~0x00F0;        // start om GPIOA_MODER register till 0
GPIOA->MODER |= 0x00A0;        // sätter alternative function mode

GPIOA->AFR[0] &= ~0xFF00;      // Restar bits till 0
GPIOA->AFR[0] |= 0x7700;      // Sätter bits  register till USART2

USART2->BRR  =  0x0683;       // sätta baud rate till 9600 
USART2->CR1  =  0x000C;       // starta om transmitter och receiver 
USART2->CR2  =  0x000;        // Configurea  CR2 med default values 
USART2->CR3  =  0x000;        // Configurea CR3 register med default values 
USART2->CR1 |=  0x2000;      // starta USART2-CR1 med register
	
}

int USART2_write(int ch){

	while(!(USART2->SR & 0x0080)){}   // väntar tills buffren att bli tom 
	USART2->DR = (ch & 0xFF);         // skriva ch karaktart till usart2

  return ch;                          // retunera ch värdet
}

int USART2_read(void){
  while(!(USART2->SR & 0x0020)){}    // loop som vänta på att en överföring ska slutföras via USART2-gränssnittet. 
	return USART2->DR;                // retnuera USART2->DR
}

struct __FILE { int handle; };
FILE __stdin  = {0};                 //  fil pekare med handle 0
FILE __stdout = {1};                 //  fil pekare med handle 1
FILE __stderr = {2};                 //  fil pekare med handle 2

int fgetc(FILE *f) {
    int c;

    c = USART2_read();               // läsa karktar med USART2

    if (c == '\r') {                 // om karktart är lika mycket läsa ,
        USART2_write(c);             // skriva det till usart2
        c = '\n';                    // ersätta med en ny rad 
    }

    USART2_write(c);                 // skriva karaktar till USART2

    return c;                        // Returnera karktar
}

int fputc(int c, FILE *f) {    //öppnar filen
    return USART2_write(c);         
}
int n;
char str[80];
		
void test_setup(void){
	
	printf("please enter a number: ");
	scanf("%d", &n);
	printf("the number entered is: %d\r\n", n);
	printf("please type a character string: ");
	gets(str);
	printf("the character string entered is: ");
	puts(str);
	printf("\r\n");
	
}