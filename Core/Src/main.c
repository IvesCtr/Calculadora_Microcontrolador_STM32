/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

	#include "OledGrph.h"
	#include "OledDriver.h"
	#include "stdlib.h"
	#include "math.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
SPI_HandleTypeDef hspi1;

UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI1_Init(void);
static void MX_USART1_UART_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

#include <stdio.h>
#include <math.h>
#include <string.h>

// Função para apagar o último caractere de uma string
void apagar(char *str){
    int tam = strlen(str);
    str[tam-1] = '\0';
}

// Função que corrige floats em que há 0s entre o ponto e o primeiro dígito diferente de 0
void corretor(char *str, int len, int dig){

    char correto[16];
    strcpy(correto, str);
    correto[dig+1] = '\0';
    char aux[16];
    int numero = len-dig-1;

    for(int i = 0; i < numero; i++){
        aux[i] = str[dig+1+i];
    }
    aux[numero]= '\0';

    int zeros = 5 - strlen(aux);
    for(int i = 0; i < zeros; i++)
        correto[dig+1+i] = '0';

    correto[dig+1+zeros] = '\0';
    strcat(correto, aux);
    strcpy(str, correto);
}

// Função para descobrir a quantidade de dígitos presentes em um número inteiro
int contDigitos(int num){
	int digitos = 1;
	while(num/10 != 0){
		num /= 10;
		digitos++;
	}

	return digitos;
}

// Função que formata uma string para números negativos
void negative(char *str, int len){
    int i = 0;
    char strcp[16];
    strcp[0] = '-';

    for(int i = 0; i < len+1; i++){
        strcp[i+1] = str[i];
    }
    strcpy(str, strcp);
}

// Função que reverte uma string (a faz de trás para frente)
void reverse(char *str, int len)
{
	int i=0, j=len-1, temp;
	while (i<j)
	{
    	temp = str[i];
    	str[i] = str[j];
    	str[j] = temp;
    	i++; j--;
	}
}

 // Converts a given integer x to string str[].  d is the number
 // of digits required in output. If d is more than the number
 // of digits in x, then 0s are added at the beginning.

// Função que converte inteiro em string
int intToStr(int x, char str[], int d)
{
	int i = 0;
	while (x)
	{
		str[i++] = (x%10) + '0';
    	x = x/10;
	}

	reverse(str, i);
	str[i] = '\0';
	//corretor(str, strlen(str));
	return i;
}

double n1;
int dig;

// Função que converte float em string
void ftoa(double n, char *res)
{
	if(n < 0)
		n1 = n*(-1.0);
	else
		n1 = n;
	// Extract integer part
	int ipart = (int)n1;

	// Extract floating part
	double fpart = n1 - (double)ipart;

	// convert integer part to string
	int i = intToStr(ipart, res,0);

	// check for display option after point
	//if (afterpoint != 0)
	//{
	if(ipart != 0){
		if(fpart != 0)
			res[i] = '.';

		fpart = fpart * 100000.0;
		dig = contDigitos(ipart);

		intToStr((int)fpart, res + i + 1, 2);
		if((int)fpart < 10000 && (int)fpart != 0)
			corretor(res, strlen(res), dig);

        if(n < 0)
            negative(res, dig+6);
	} else{
		res[0] = '0';

		if(fpart != 0){
			res[1] = '.';
			i++;
		}

		fpart = fpart * 100000.0 ;
		dig = contDigitos(ipart);

		intToStr((int)fpart, res + i + 1, 2);
		if((int)fpart < 10000 && (int)fpart != 0)
			corretor(res, strlen(res), dig);

        if(n < 0)
            negative(res, dig+6);
	}
	//}

}

/* IN�?CIO Operações da calculadora
 * -Adição-
 * -Subtração-
 * -Multiplicação-
 * -Divisão-
 * -Potenciação-
 * -Radiciação-
 * -Fatorial-
 * -Logaritmo-
 * -Exponencial-
 * */

double soma(double a, double b){
	double res = a + b;
	return res;
}

double sub(double a, double b){
	double res = a - b;
	return res;
}

double mult(double a, double b){
	double res = a * b;
	return res;
}

double divi(double a, double b){
	float res = a / b;
	return res;
}

double pot(double a, double b){
	double res = pow(a,b);
	return res;
}

double raiz(double a, double b){
	float res = pow(a,1/b);
	return res;
}

int fat(int a){
	if(a == 0)
		return 1;
	else{
		return a*fat(a-1);
	}
}

double logarit(double a){
	float res = log(a);
	return res;
}

double expon(double a){
	float res = exp(a);
	return res;
}

/*
 * FIM Operações da calculadora
 * */

// Função responsável por concatenar os valores inteiros digitados no teclado matricial
int conc(int vet[], int k){
	double res = 0;
	int i = 0;
	while(k!=0){
		res += vet[i]*pow(10,k-1);
		k--;
		i++;
	}
	return res;
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

	  char pressionado = 'a';	// Char que armazena o caractere digitado no teclado matricial
	  char pressOp;				// Char que armazena o caractere operador
	  int vet[16];				// Vetor de inteiros que armazena os valores das teclas digitadas no teclado matricial
	  int k = 0;				// Variável de controle na conversão de vetor de inteiro para vetor de char
	  char txt[16];				// String (vetor de char) que aparece no Oled
	  char linha[4] = "\r\n"; 	// String auxiliar para formatar no computador
	  char *txt2 = &txt;		// Ponteiro que aponta para o endereço da string que aparece no oled
	  char operUart[30];		// String para o histórico (aparece na UART)
	  char valUart[16];			// String para auxiliar operUart, gravando o valor2
	  double valor = 0;			// Primeiro valor digitado (antes da escolha da operação)
	  double valor2 = 0;		// Segundo valor digitado (depois da escolha da operação)
	  int operacao;				// Variável para definir a operação no switch case
	  double op;				// variável que armazena o resultado da operação entre os valores 1 e 2
	  int animacao=0;			// Variável relacionada a aniamações de iniciar, reiniciar e desligar
	  int cont1 = 0; 			// Tirar o 0 padrão de inicialização
	  int cont; 				// Separa as etapas: colocar número ou colocar operação
	  int format=0; 			// Impedir que se coloque um segundo valor após as operações ! log e

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_SPI1_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */

  OledInit(); // Inicializa o oLed
  OledClear(); //- Apaga todos os dados  do oLed
  OledSetCursor(0,0); //Coloca o cursor na posição desejada da tela. Cada caractere ocupa uma coluna.
  //HAL_UART_Transmit(&huart1, "Historico\r", 10, 100);
  //OledPutString("comp"); //Escreve uma string na tela.
  //OledPutChar('?'); //Escreve um char na tela.

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

  while (1)
  {
  	OledPutChar('0');
    cont=0;
	  while(cont < 100){
		  inicio:

		  // Quando cont for par, o usuário entra com os dígitos do número desejado.
		  if(cont%2==0){
			  for (int i = 0; i < 4; i++) {

				  	  	  	  // Linhas do teclado matricial - Inicialmente, todas resetadas
			  		          HAL_GPIO_WritePin(GPIOF, GPIO_PIN_0, GPIO_PIN_RESET);
			  		          HAL_GPIO_WritePin(GPIOF, GPIO_PIN_1, GPIO_PIN_RESET);
			  		          HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_RESET);
			  		          HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_RESET);

			  		          // Aqui, fica setando em 1 cada uma das 4 linhas, enquanto as outras 3 permanecem resetadas
			  		          // Lembrar que o while acontece extremamente rápido!
			  		          if (i == 0) {
			  		            HAL_GPIO_WritePin(GPIOF, GPIO_PIN_0, GPIO_PIN_SET);
			  		          } else if (i == 1) {
			  		            HAL_GPIO_WritePin(GPIOF, GPIO_PIN_1, GPIO_PIN_SET);
			  		          } else if (i == 2) {
			  		            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_SET);
			  		          } else if (i == 3) {
			  		            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_SET);
			  		          }

			  		          /*
			  		           * Essa parte do código reconhece quando uma tecla é pressionada,
			  		           * pois o curto fará com que o pino da porta da coluna seja setado
			  		           * em 1. Lembrar que o pull-down faz esse pino ser 0 quando não pressionado.
			  		           * Os comandos que ocorrem quando uma tecla é pressionada também estão aqui*/
			  		          if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_4) == 1) {

			  		            if (i == 0) {
			  		              if(cont1 == 0 && cont != 2){
			  		            	  OledClear();
			  		            	  OledSetCursor(0,0);
			  		            	  cont1++;
			  		              }
			  		              pressionado = '1';
			  		              HAL_Delay(350);
			  		              if(format != 1){
			  		            	  OledPutChar(pressionado);
			  		            	  vet[k]=1;
			  		            	  k++;
			  		              }
			  		            } else if (i == 1) {
				  		          if(cont1 == 0 && cont != 2){
				  		        	  OledClear();
				  		           	  OledSetCursor(0,0);
				  		           	  cont1++;
				  		          }
			  		              pressionado = '4';
			  		              HAL_Delay(350);
			  		              if(format != 1){
			  		            	  OledPutChar(pressionado);
			  		            	  vet[k]=4;
			  		            	  k++;
			  		              }
			  		            } else if (i == 2) {
				  		          if(cont1 == 0 && cont != 2){
				  		              OledClear();
				  		              OledSetCursor(0,0);
				  		              cont1++;
				  		          }
			  		              pressionado = '7';
			  		              HAL_Delay(350);
			  		              if(format != 1){
			  		            	OledPutChar(pressionado);
			  		              	  vet[k]=7;
			  		              	  k++;
			  		              }
			  		            } else if (i == 3) { //* para igualdade ( = )
			  		              HAL_Delay(350);
			  		              if(cont == 2){
									  OledClear();
									  OledSetCursor(0,0);
									  format = 0;
									  valor2 = conc(vet, k);
									  intToStr(valor2, valUart, k);
									  switch(operacao){
										  case 1:
											  op = soma(valor, valor2);
											  break;
										  case 2:
											  op = sub(valor, valor2);
											  break;
										  case 3:
											  op = mult(valor, valor2);
											  break;
										  case 4:
											  if(valor2 == 0){
												  OledPutString("Error");
												  HAL_Delay(3000);
												  OledClear();
												  OledSetCursor(0,0);
												  cont1 = 0;
												  OledPutChar('0');
												  cont = 0;
												  goto inicio;
											  }

											  op = divi(valor, valor2);
											  break;
										  case 5:
											  op = pot(valor, valor2);
											  break;
										  case 6:
											  if(valor < 0){
												  OledPutString("Error");
												  HAL_Delay(3000);
												  OledClear();
												  OledSetCursor(0,0);
												  cont1 = 0;
												  OledPutChar('0');
												  cont = 0;
												  goto inicio;
											  }

											  op = raiz(valor, valor2);
											  break;
										  case 7:
											  if(valor < 0){
												  OledPutString("Error");
												  HAL_Delay(3000);
												  OledClear();
												  OledSetCursor(0,0);
												  cont1 = 0;
												  OledPutChar('0');
												  cont = 0;
												  goto inicio;
											  }
											  if(valor != (int)valor){
												  OledPutString("Indefinido");
												  HAL_Delay(3000);
												  OledClear();
												  OledSetCursor(0,0);
												  cont1 = 0;
												  OledPutChar('0');
												  cont = 0;
												  goto inicio;
											  }
											  op = fat(valor);
											  break;
										  case 8:
											  if(valor <= 0){
												  OledPutString("Error");
												  HAL_Delay(3000);
												  OledClear();
												  OledSetCursor(0,0);
												  cont1 = 0;
												  OledPutChar('0');
												  cont = 0;
												  goto inicio;
											  }
											  op = logarit(valor);
											  break;
										  case 9:
											  op = expon(valor);
											  break;
									  }
									  ftoa(op, txt2);
									  OledPutString(txt);
									  cont = 1;
									  // INICIO - Formatação para o histórico
									  k = strlen(operUart); // reaproveitamento da variável k
									  operUart[k] = pressOp;
									  operUart[k+1] = '\0';
									  strcat(operUart, valUart);
									  k = strlen(operUart); // reaproveitamento da variável k
									  operUart[k] = '=';
									  operUart[k+1] = '\0';
									  k = 0;
									  strcat(operUart, txt);
									  HAL_UART_Transmit(&huart1, strcat(operUart, linha), strlen(strcat(operUart, linha)), 100);
									  valor = op;
									  ftoa(valor, operUart);
									  // FIM - Formatação para o histórico
									  goto inicio;
			  		              } else{
			  		            	  // * Código para apagar o último caractere
			  		            	  OledClear();
			  		            	  OledSetCursor(0,0);
			  		            	  if(k==0){
			  		            		  OledPutChar('0');
			  		            		  cont1 = 0;
			  		            	  }
			  		            	  else{
				  		            	  valor = conc(vet, k);
				  		            	  k--;
				  		            	  if(k==0){
				  		            		  OledPutChar('0');
				  		            		  cont1 = 0;
				  		            	  }
				  		            	  ftoa(valor, txt2);
				  		            	  apagar(txt2);
				  		            	  OledPutString(txt2);
			  		            	  }
			  		              }
			  		            }
			  		          }else if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1) == 1) {
			  		            if (i == 0) {
					  		      if(cont1 == 0 && cont != 2){
					  		          OledClear();
					  	              OledSetCursor(0,0);
					  	              cont1++;
					  	          }
			  		              pressionado = '2';
			  		              HAL_Delay(350);
			  		              if(format != 1){
			  		            	  OledPutChar(pressionado);
			  		              	  vet[k]=2;
			  		              	  k++;
			  		              }
			  		            } else if (i == 1) {
					  		      if(cont1 == 0 && cont != 2){
					  		          OledClear();
					  	              OledSetCursor(0,0);
					  	              cont1++;
					              }
			  		              pressionado = '5';
			  		              HAL_Delay(350);
			  		              if(format != 1){
			  		            	  OledPutChar(pressionado);
			  		              	  vet[k]=5;
			  		              	  k++;
			  		              }
			  		            } else if (i == 2) {
					  		      if(cont1 == 0 && cont != 2){
					  		          OledClear();
					  	              OledSetCursor(0,0);
					  	              cont1++;
					  	          }
			  		              pressionado = '8';
			  		              HAL_Delay(350);
			  		              if(format != 1){
			  		            	  OledPutChar(pressionado);
			  		              	  vet[k]=8;
			  		              	  k++;
			  		              }
			  		            } else if (i == 3) {
			  		              pressionado = '0';
			  		              HAL_Delay(350);
			  		              if(k != 0 || cont == 2){
									  if(format != 1){
										  OledPutChar(pressionado);
									  	  vet[k]=0;
									  	  k++;
									  }
			  		              }
			  		            }

			  		          }else if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_6) == 1) {
			  		            if (i == 0) {
					  		      if(cont1 == 0 && cont != 2){
					  		          OledClear();
					  	              OledSetCursor(0,0);
					  	              cont1++;
					              }
			  		              pressionado = '3';
			  		              HAL_Delay(350);
			  		              if(format != 1){
			  		            	  OledPutChar(pressionado);
			  		              	  vet[k]=3;
			  		              	  k++;
			  		              }
			  		            } else if (i == 1) {
					  		      if(cont1 == 0 && cont != 2){
					  		          OledClear();
					  	              OledSetCursor(0,0);
					  	              cont1++;
					              }
			  		              pressionado = '6';
			  		              HAL_Delay(350);
			  		              if(format != 1){
			  		            	  OledPutChar(pressionado);
			  		              	  vet[k]=6;
			  		              	  k++;
			  		              }
			  		            } else if (i == 2) {
					  		      if(cont1 == 0 && cont != 2){
					  		          OledClear();
					  	              OledSetCursor(0,0);
					  	              cont1++;
					  	          }
			  		              pressionado = '9';
			  		              HAL_Delay(350);
			  		              if(format != 1){
			  		            	  OledPutChar(pressionado);
			  		              	  vet[k]=9;
			  		              	  k++;
			  		              }
			  		            } else if (i == 3) {	// #
			  		              HAL_Delay(350);
			  		              // Escolher operador
			  		              if(cont == 0){
				  		              valor = conc(vet, k);
					  		          intToStr(valor, operUart, k);
				  		              k = 0;
				  		              cont++;
			  		              }
			  		              // Apagar último caractere
			  		              else{
			  		            	  OledClear();
			  		            	  OledSetCursor(0,0);
			  		            	  ftoa(valor, txt2);
			  		            	  OledPutString(txt);
			  		            	  OledPutChar(pressOp);
			  		            	  if(k!=0){
										  valor2 = conc(vet, k);
										  k--;
										  ftoa(valor2, txt2);
										  apagar(txt2);
										  OledPutString(txt2);
			  		            	  }
			  		              }
			  		            }
			  		          }

			  		        }
		  }
		  // ABA OPERAÇÕES - Quando cont for ímpar, significa que o usuário escolhe a operação.
		  else if(cont%2!=0){
			  for (int i = 0; i < 4; i++) {

			  			  		          HAL_GPIO_WritePin(GPIOF, GPIO_PIN_0, GPIO_PIN_RESET);
			  			  		          HAL_GPIO_WritePin(GPIOF, GPIO_PIN_1, GPIO_PIN_RESET);
			  			  		          HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_RESET);
			  			  		          HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_RESET);

			  			  		          if (i == 0) {
			  			  		            HAL_GPIO_WritePin(GPIOF, GPIO_PIN_0, GPIO_PIN_SET);
			  			  		          } else if (i == 1) {
			  			  		            HAL_GPIO_WritePin(GPIOF, GPIO_PIN_1, GPIO_PIN_SET);
			  			  		          } else if (i == 2) {
			  			  		            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_SET);
			  			  		          } else if (i == 3) {
			  			  		            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_SET);
			  			  		          }

			  			  		          if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_4) == 1) {

			  			  		            if (i == 0) {
			  			  		              pressOp = '+';
			  			  		              HAL_Delay(350);
			  			  		              OledPutChar(pressOp);
			  			  		              operacao = 1;
			  			  		              cont++;
			  			  		            } else if (i == 1) {
			  			  		              pressOp = '/';
			  			  		              HAL_Delay(350);
			  			  		              OledPutChar(pressOp);
			  			  		              operacao = 4;
			  			  		              cont++;
			  			  		            } else if (i == 2) {
			  			  		              pressOp = '!';
			  			  		              format = 1;
			  			  		              HAL_Delay(350);
			  			  		              OledPutChar(pressOp);
			  			  		              operacao = 7;
			  			  		              cont++;
			  			  		            } else if (i == 3) {	// *
			  			  		            	//Não faz nada
			  			  		            }
			  			  		          }else if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1) == 1) {
			  			  		            if (i == 0) {
			  			  		              pressOp = '-';
			  			  		              HAL_Delay(350);
			  			  		              OledPutChar(pressOp);
			  			  		              operacao = 2;
			  			  		              cont++;
			  			  		            } else if (i == 1) {
			  			  		              pressOp = '^';
			  			  		              HAL_Delay(350);
			  			  		              OledPutChar(pressOp);
			  			  		              operacao = 5;
			  			  		              cont++;
			  			  		            } else if (i == 2) {
			  			  		              pressOp = 'L';
			  			  		              format = 1;
			  			  		              //OledPutString("ln");
			  			  		              HAL_Delay(350);
			  			  		              OledPutChar(pressOp);
			  			  		              operacao = 8;
			  			  		              cont++;
			  			  		            } else if (i == 3) {
			  			  		              //0 de reiniciar
			  			  		              HAL_Delay(350);
			  			  		              OledClear();
			  			  		              OledSetCursor(0,0);
			  			  		              OledPutChar('0');
			  			  		              cont1 = 0;
			  			  		              cont++;
			  			  		              cont = 0;
			  			  		              goto inicio;
			  			  		            }

			  			  		          }else if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_6) == 1) {
			  			  		            if (i == 0) {
			  			  		              pressOp = '*';
			  			  		              HAL_Delay(350);
			  			  		              operacao = 3;
			  			  		              OledPutChar(pressOp);
			  			  		              cont++;
			  			  		            } else if (i == 1) {
			  			  		              pressOp = 'v';
			  			  		              HAL_Delay(350);
			  			  		              operacao = 6;
			  			  		              OledPutChar(pressOp);
			  			  		              cont++;
			  			  		            } else if (i == 2) {
			  			  		              pressOp = 'e';
			  			  		              format = 1;
			  			  		              HAL_Delay(350);
			  			  		              operacao = 9;
			  			  		              OledPutChar(pressOp);
			  			  		              cont++;
			  			  		            }
			  			  		          }
			  			  		        }
		  }


	  }
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK1;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_1LINE;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_HIGH;
  hspi1.Init.CLKPhase = SPI_PHASE_2EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 7;
  hspi1.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
  hspi1.Init.NSSPMode = SPI_NSS_PULSE_DISABLE;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 9600;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_HalfDuplex_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOF, GPIO_PIN_0|GPIO_PIN_1, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3
                          |GPIO_PIN_10, GPIO_PIN_RESET);

  /*Configure GPIO pins : PF0 PF1 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

  /*Configure GPIO pins : PA0 PA1 PA2 PA3
                           PA10 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3
                          |GPIO_PIN_10;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PA4 PA6 */
  GPIO_InitStruct.Pin = GPIO_PIN_4|GPIO_PIN_6;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : PB1 */
  GPIO_InitStruct.Pin = GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
