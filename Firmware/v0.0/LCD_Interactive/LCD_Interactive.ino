// ***************************************************************************************************************************
// ***************************************************************************************************************************
// **																														**
// **										Arduino LCD Interactive	Library													**
// **																														**
// **	- Firmware v0.0																										**																											**
// **																														**
// **	- Compilado en Arduino IDE v1.0.6																					**
// **		http://www.arduino.cc/en/Main/OldSoftwareReleases																**
// **	- Compilado para Arduino Mega 2560 R3																				**
// **		http://www.arduino.cc/en/Main/ArduinoBoardMega2560																**
// **	- Simulacion en Proteus v7.7 SP2																					**
// **	- Simulacion en Proteus de Arduino - Microcontrolandos																**
// **		http://microcontrolandos.blogspot.mx/2012/12/arduino-componentes-para-o-proteus.html							**
// **																														**
// **	Autor:																												**
// **																														**
// **	Daniel Roberto Becerril Angeles																						**
// **	daniel3514@gmail.com																								**																										**
// **	https://github.com/daniel3514/Arduino-LCD-Interactive-Library														**
// **																														**
// **	Licenciamiento:																										**
// **																														**
// **	GNU General Pubic Licence Version 3																					**
// **		https://www.gnu.org/copyleft/gpl.html																			**
// **																														**
// ***************************************************************************************************************************
// ***************************************************************************************************************************

// Librerias
	#include <LiquidCrystal.h>	// libreria para LCD
	#include <string.h>
	
// Puertos, variables
	// Botones cursor
		int Boton_Up     		= 51; 	// puerto del boton Up
		int Boton_Down   		= 45;	// puerto del boton down	
		int Boton_Left   		= 53;	// puerto del boton left
		int Boton_Right  		= 49;	// puerto del boton right
		int Boton_Center		= 47;	// perto del boton center
		byte LCD_Col_Pos 		= 0;	// posicion en tiempo real de lcd
		byte LCD_Row_Pos 		= 0;	// posicion en tiempo real de lcd
		byte Cursor_Conf[4][20] = {{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},		// matriz con el tamaño del LCD, en este caso 20x4
								   {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
								   {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
								   {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}};		
	// Botones Numerico Array
		int  Boton_Array_1		= 36;
		int  Boton_Array_2		= 34;
		int  Boton_Array_3		= 32;
		int  Boton_Array_4		= 30;
		int  Boton_Array_A		= 44;	
		int  Boton_Array_B		= 42;
		int  Boton_Array_C   	= 40;
		int  Boton_Array_D   	= 38;
		byte Boton_Calc 		= 17;	// valor calculado	# E * F, 17 sin valor calculado
		byte Num_Col_Pos  		= 0;	// posicion en tiempo real de lcd
		byte Num_Row_Pos 		= 0;	// posicion en tiempo real de lcd
		int  Num_Val			= 0;	// valor generado al calculo
		long Boton_Delay_Teclado = 100;	// delay de lectura de boton
	// LCD
		int LCD_RS 				= 8;	// puertos de conexion de LCD
		int LCD_E  				= 9;
		int LCD_D4 				= 10;
		int LCD_D5 				= 11;
		int LCD_D6 				= 12;
		int LCD_D7				= 13;
		LiquidCrystal lcd(LCD_RS, LCD_E, LCD_D4, LCD_D5, LCD_D6, LCD_D7);  //LCD setup

void setup() 
	{
		// Botones cursor
			pinMode(Boton_Up,      		INPUT_PULLUP);
			pinMode(Boton_Down,    		INPUT_PULLUP);
			pinMode(Boton_Left,    		INPUT_PULLUP);
			pinMode(Boton_Right,   		INPUT_PULLUP);
			pinMode(Boton_Center,  		INPUT_PULLUP);
		// Botones numerico
			pinMode(Boton_Array_1, 		OUTPUT);
			pinMode(Boton_Array_2, 		OUTPUT);
			pinMode(Boton_Array_3, 		OUTPUT);
			pinMode(Boton_Array_4,		OUTPUT);
			pinMode(Boton_Array_A, 		INPUT_PULLUP);
			pinMode(Boton_Array_B, 		INPUT_PULLUP);
			pinMode(Boton_Array_C, 		INPUT_PULLUP);
			pinMode(Boton_Array_D, 		INPUT_PULLUP);
			digitalWrite(Boton_Array_1, HIGH);
			digitalWrite(Boton_Array_2, HIGH);
			digitalWrite(Boton_Array_3, HIGH);
			digitalWrite(Boton_Array_4, HIGH);
		// LCD
			pinMode(LCD_RS, 			OUTPUT);
			pinMode(LCD_E,  			OUTPUT);
			pinMode(LCD_D7, 			OUTPUT);
			pinMode(LCD_D6, 			OUTPUT);
			pinMode(LCD_D5, 			OUTPUT);
			pinMode(LCD_D4, 			OUTPUT);
			pinMode(Back_Light_PWM,		OUTPUT);
			lcd.begin(20, 4);							//tamaño de LCD				
	}    

void loop()
	{

	}
	
void Cursor_Conf_Clear()
	{
		for (byte Conteo_Col = 0; Conteo_Col <= 19; Conteo_Col ++)
			{
				for (byte Conteo_Row = 0; Conteo_Row <= 3; Conteo_Row ++)
					{
						Cursor_Conf[Conteo_Row][Conteo_Col] = 0;
					}
			}
	}
	
void GUI_Navegar(byte matrix, int banco)
	{
		long Boton_Delay_Cursor  = 300;			// delay de lectura de boton
		byte LCD_Col_Pos_Ant;					// saber el estado anterior para borrar cursor
		byte LCD_Row_Pos_Ant;					// saber el estado anterior para borrar cursor
		// guardar valor anterior de row col
			LCD_Col_Pos_Ant = LCD_Col_Pos;
			LCD_Row_Pos_Ant = LCD_Row_Pos;
		// Dibujar cursor
			lcd.setCursor (LCD_Col_Pos, LCD_Row_Pos);
			lcd.print(">"); 													
		// navegacion
			Dibujar:
				byte Dibujar_Cursor = 0;		// saber si dibujar cursor para evitar repeticiones en lcd, 0 no dibujar, 1 dibujar >, 2 dibujar +
				// LCD Back Light *
					digitalWrite(Boton_Array_1, LOW);	// lectura linea 1
						if (digitalRead(Boton_Array_D) == LOW)
							{
								delay(Boton_Delay_Teclado);
								Back_Light_En();
							}
					digitalWrite(Boton_Array_1, HIGH);	// lectura linea 1
				// Left
					if (digitalRead(Boton_Left) == LOW)
						{
							delay (Boton_Delay_Cursor);
							byte Salida_Left = 0;
							byte LCD_Col_Pos_Temp = 0;
							LCD_Col_Pos_Temp = LCD_Col_Pos;
							while (Salida_Left == 0)
								{
									if (LCD_Col_Pos_Temp == 0)
										{
											LCD_Col_Pos_Temp = 20;
										}
											LCD_Col_Pos_Temp = LCD_Col_Pos_Temp - 1;
									if (Cursor_Conf[LCD_Row_Pos][LCD_Col_Pos_Temp] == 1)
										{
											LCD_Col_Pos = LCD_Col_Pos_Temp;
											Dibujar_Cursor = 1;
											Salida_Left = 1;
										}
								}
									goto Salida;
						}
				// Right
					if (digitalRead(Boton_Right) == LOW)
						{
							delay(Boton_Delay_Cursor);
							byte Salida_Right = 0;
							byte LCD_Col_Pos_Temp = 0;
							LCD_Col_Pos_Temp = LCD_Col_Pos;
							while (Salida_Right == 0)
								{
									LCD_Col_Pos_Temp = LCD_Col_Pos_Temp + 1;
									if (LCD_Col_Pos_Temp >= 20)
										{
											LCD_Col_Pos_Temp = 0;	// regresar al cero
										}
									if (Cursor_Conf[LCD_Row_Pos][LCD_Col_Pos_Temp] == 1)
										{
											LCD_Col_Pos = LCD_Col_Pos_Temp;
											Dibujar_Cursor = 1;
											Salida_Right = 1;
										}
								}
							goto Salida;
						}
				// Down
					if (digitalRead(Boton_Down) == LOW)
						{
							delay(Boton_Delay_Cursor);
							byte Salida_Down = 0;
							byte LCD_Row_Pos_Temp = 0;
							LCD_Row_Pos_Temp = LCD_Row_Pos;
							while (Salida_Down == 0)
								{
									LCD_Row_Pos_Temp = LCD_Row_Pos_Temp + 1;
									if (LCD_Row_Pos_Temp >= 4)
										{
											LCD_Row_Pos_Temp = 0;	// regresar al cero
										}
									if (Cursor_Conf[LCD_Row_Pos_Temp][LCD_Col_Pos] == 1)
										{
											LCD_Row_Pos = LCD_Row_Pos_Temp;
											Dibujar_Cursor = 1;
											Salida_Down = 1;
										}
								}
									goto Salida;
						}
				// Up
					if (digitalRead(Boton_Up) == LOW)
						{
							delay(Boton_Delay_Cursor);
							byte Salida_Up = 0;
							byte LCD_Row_Pos_Temp;
							LCD_Row_Pos_Temp = LCD_Row_Pos;
							while (Salida_Up == 0)
								{
									if (LCD_Row_Pos_Temp <= 0)
										{
											LCD_Row_Pos_Temp = 4;
										}
									LCD_Row_Pos_Temp = LCD_Row_Pos_Temp - 1;
									if (Cursor_Conf[LCD_Row_Pos_Temp][LCD_Col_Pos] == 1)
										{
											Dibujar_Cursor = 1;
											LCD_Row_Pos = LCD_Row_Pos_Temp;
											Salida_Up = 1;
										}
								}
							goto Salida;
						}
				// Center
					if (digitalRead(Boton_Center) == LOW)
						{
							delay(Boton_Delay_Cursor);
							byte Salida_Center = 0;
							while (Salida_Center == 0)
								{
									if (Cursor_Conf[LCD_Row_Pos][LCD_Col_Pos] == 1)
										{
											Dibujar_Cursor = 2;	// dibujar +
											Salida_Center = 1;
										}
								}
						}
			Salida:
				// Dibujar Cursor
					if (Dibujar_Cursor > 0)
						{
							if (Dibujar_Cursor == 1)
								{
									// borra el anterior
										lcd.setCursor (LCD_Col_Pos_Ant, LCD_Row_Pos_Ant);
										lcd.print(" "); 
									// escribir >
										lcd.setCursor (LCD_Col_Pos, LCD_Row_Pos);
										lcd.print(">");
									// matrix print posicion
										if (matrix == 1)
											{
												// bancos
													if (LCD_Col_Pos == 12 && LCD_Row_Pos == 0)
														{
															lcd.setCursor(1, 0);
															lcd.print("---");
														}
												// Memory
													if (LCD_Col_Pos == 4 && LCD_Row_Pos == 0)
														{
															lcd.setCursor(1, 0);
															lcd.print("---");
														}
												// Unitary
													if (LCD_Col_Pos == 8 && LCD_Row_Pos == 0)
														{
															lcd.setCursor(1, 0);
															lcd.print("---");
														}
												// banco 1
													if (LCD_Col_Pos == 0 && LCD_Row_Pos == 1)
														{
															Numerico_Write (banco, 1, 0);
														}
												// banco 2
													if (LCD_Col_Pos == 4 && LCD_Row_Pos == 1)
														{
															Numerico_Write (banco + 1, 1, 0);
														}
												// banco 3
													if (LCD_Col_Pos == 8 && LCD_Row_Pos == 1)
														{
															Numerico_Write (banco + 2, 1, 0);
														}
												// banco 4
													if (LCD_Col_Pos == 12 && LCD_Row_Pos == 1)
														{
															Numerico_Write (banco + 3, 1, 0);
														}
												// banco 5
													if (LCD_Col_Pos == 16 && LCD_Row_Pos == 1)
														{
															Numerico_Write (banco + 4, 1, 0);
														}
												// banco 6
													if (LCD_Col_Pos == 0 && LCD_Row_Pos == 2)
														{
															Numerico_Write (banco + 5, 1, 0);
														}
												// banco 7
													if (LCD_Col_Pos == 4 && LCD_Row_Pos == 2)
														{
															Numerico_Write (banco + 6, 1, 0);
														}
												// banco 8
													if (LCD_Col_Pos == 8 && LCD_Row_Pos == 2)
														{
															Numerico_Write (banco + 7, 1, 0);
														}
												// banco 9
													if (LCD_Col_Pos == 12 && LCD_Row_Pos == 2)
														{
															Numerico_Write (banco + 8, 1, 0);
														}
												// banco 10
													if (LCD_Col_Pos == 16 && LCD_Row_Pos == 2)
														{
															Numerico_Write (banco + 9, 1, 0);
														}
												// banco 11
													if (LCD_Col_Pos == 0 && LCD_Row_Pos == 3)
														{
															Numerico_Write (banco + 10, 1, 0);
														}
												// banco 12
													if (LCD_Col_Pos == 4 && LCD_Row_Pos == 3)
														{
															Numerico_Write (banco + 11, 1, 0);
														}
												// banco 13
													if (LCD_Col_Pos == 8 && LCD_Row_Pos == 3)
														{
															Numerico_Write (banco + 12, 1, 0);
														}
												// banco 14
													if (LCD_Col_Pos == 12 && LCD_Row_Pos == 3)
														{
															Numerico_Write (banco + 13, 1, 0);
														}
												// banco 15
													if (LCD_Col_Pos == 16 && LCD_Row_Pos == 3)
														{
															Numerico_Write (banco + 14, 1, 0);
														}
											}
								}
							else
								{
									// escribir +
										lcd.setCursor (LCD_Col_Pos, LCD_Row_Pos);
										lcd.print("+");
								}
			
							//salida
								LCD_Col_Pos_Ant = LCD_Col_Pos;
								LCD_Row_Pos_Ant = LCD_Row_Pos;
								if (Dibujar_Cursor == 2)
									{
										goto Salir;
									}
								else	
									{
										Dibujar_Cursor = 0;
									}
						}
			goto Dibujar;
			Salir: {}
	}
	
void Numerico_Write (int valor, int col, int row)
	{
		//posicionar el valor en los campos 000
			lcd.setCursor (col, row);
			lcd.print ("000");
		if (valor < 10)
			{
				lcd.setCursor (col + 2, row);
			}
		if (valor > 9 && valor < 100)
			{
				lcd.setCursor (col + 1, row);
			}
		if (valor > 99)
			{
				lcd.setCursor (col, row);
			}
		lcd.print (valor);
	}
	
void Numerico_Calc(byte value)
	{
	// escritura del numero desde el teclado numerico si value es 1 entra opcion de A 255 y B 0
		byte 	Salida			 = 0;
		int 	Num_Val_Temp_1 	 = 0;
		int 	Num_Val_Temp_2 	 = 0;
		int 	Num_Val_Temp_3 	 = 0;
		lcd.setCursor (Num_Col_Pos, Num_Row_Pos);
		lcd.print("___");
		lcd.blink();								// mostrar cursor
		Num_Val = 0;
		Num_Col_Pos = Num_Col_Pos + 2;
		lcd.setCursor (Num_Col_Pos, Num_Row_Pos);
		// primer numero
			Numerico_Read();						// leer boton
			if (Boton_Calc <= 9)
				{
					lcd.print (Boton_Calc);			// escribir valor calculado
					lcd.setCursor (Num_Col_Pos, Num_Row_Pos);
					Num_Val = Boton_Calc;			// valor calculado
					Num_Val_Temp_1 = Boton_Calc;	// valor temporal para el acarreo
				}
			if (Boton_Calc == 14)					// enter
				{
					goto Salida;					// num val = 0		
				}
			if (value == 1)
				{
					if (Boton_Calc == 10)			// 255
						{
							Num_Val = 255;
							goto Salida;
						}
					if (Boton_Calc == 11)			// 000
						{
							Num_Val = 0;
							goto Salida;
						}
					if (Boton_Calc == 12)			// opcion
						{
							Num_Val = 612;
							goto Salida_Option;
						}
				}
		// segundo numero
			Numerico_Read();						// leer boton
			if (Boton_Calc == 14)					// enter
				{
					Num_Val = Num_Val_Temp_1;	
					goto Salida;					// num val = num val anterior
				}
			if (value == 1)
				{
					if (Boton_Calc == 10)			// 255
						{
							Num_Val = 255;
							goto Salida;
						}
					if (Boton_Calc == 11)			// 000
						{
							Num_Val = 0;
							goto Salida;
						}
					if (Boton_Calc == 12)			// opcion
						{
							Num_Val = 612;
							goto Salida_Option;
						}
				}
			if (Boton_Calc <= 9)
				{
					// recorrer numero ya impreso
						Num_Col_Pos = Num_Col_Pos - 1;
						lcd.setCursor (Num_Col_Pos, Num_Row_Pos);
						lcd.print (Num_Val_Temp_1);
					// dibujar numero nuevo
						lcd.print (Boton_Calc);
						Num_Val_Temp_2 = Boton_Calc;
						Num_Col_Pos = Num_Col_Pos + 1;
						lcd.setCursor (Num_Col_Pos, Num_Row_Pos);
				}
		// Tercer numero
			Numerico_Read();						// leer boton
			if (Boton_Calc == 14)					// enter
				{	
					Num_Val = (Num_Val_Temp_1 * 10) + Num_Val_Temp_2;
					goto Salida;
				}
			if (value == 1)
				{
					if (Boton_Calc == 10)			// 255
						{
							Num_Val = 255;
							goto Salida;
						}
					if (Boton_Calc == 11)			// 000
						{
							Num_Val = 0;
							goto Salida;
						}
					if (Boton_Calc == 12)			// opcion
						{
							Num_Val = 612;
							goto Salida_Option;
						}
				}
			if (Boton_Calc <= 9)
				{
					// recorrer numero ya impreso
						Num_Col_Pos = Num_Col_Pos - 2;
						lcd.setCursor (Num_Col_Pos, Num_Row_Pos);
						lcd.print (Num_Val_Temp_1);
						lcd.print (Num_Val_Temp_2);
					// dibujar numero nuevo
						Num_Col_Pos = Num_Col_Pos + 2;
						lcd.setCursor (Num_Col_Pos, Num_Row_Pos);
						lcd.print (Boton_Calc);
						Num_Val = Boton_Calc + (Num_Val_Temp_1 * 100) + (Num_Val_Temp_2 * 10);
				}
		Salida:
			lcd.noBlink();
			// pintar los ceros antes del numero
				Numerico_Write (Num_Val, Num_Col_Pos - 2, Num_Row_Pos);
				Num_Col_Pos = Num_Col_Pos - 4;
			// regresar el cursor a su ligar
				lcd.setCursor (Num_Col_Pos, Num_Row_Pos);
		Salida_Option:
			{
				lcd.noBlink();
				lcd.setCursor (Num_Col_Pos, Num_Row_Pos);
				delay(300); // rebote de boton
			}
	}

void Numerico_Read()
	{
		long Num_Barrido_Time = 5;			// tiempo entre barrido de keys
		Boton_Calc = 17;					// limpiar valor para lectura
		while (Boton_Calc == 17)			// valor calculado	# E * F, 17 sin valor calculado
			{	
					// Barrido
						// Linea 1
							digitalWrite(Boton_Array_1, LOW);	// lectura linea 1
							// Lectura 1A = 1
								if (digitalRead(Boton_Array_A) == LOW)
									{
										Boton_Calc = 1;
										delay(Boton_Delay_Teclado);
									}
								delay(Num_Barrido_Time);
							// Lectura 1B = 6
								if (digitalRead(Boton_Array_B) == LOW)
									{
										Boton_Calc = 4;
										delay(Boton_Delay_Teclado);
									}
								delay(Num_Barrido_Time);
							// Lectura 1C = 7
								if (digitalRead(Boton_Array_C) == LOW)
									{
										Boton_Calc = 7;
										delay(Boton_Delay_Teclado);
									}	
								delay(Num_Barrido_Time);
							// Lectura 1D = * = 15
								if (digitalRead(Boton_Array_D) == LOW)
									{
										Boton_Calc = 15;
										delay(Boton_Delay_Teclado);
									}
								delay(Num_Barrido_Time);
							digitalWrite(Boton_Array_1, HIGH);	// lectura linea 1
							
						// Linea 2
							digitalWrite(Boton_Array_2, LOW);	// lectura linea 2
							// Lectura 2A = 2
								if (digitalRead(Boton_Array_A) == LOW)
									{
										Boton_Calc = 2;
										delay(Boton_Delay_Teclado);
									}
								delay(Num_Barrido_Time);
							// Lectura 2B = 5
								if (digitalRead(Boton_Array_B) == LOW)
									{
										Boton_Calc = 5;
										delay(Boton_Delay_Teclado);
									}
								delay(Num_Barrido_Time);
							// Lectura 2C = 8
								if (digitalRead(Boton_Array_C) == LOW)
									{
										Boton_Calc = 8;
										delay(Boton_Delay_Teclado);
									}	
								delay(Num_Barrido_Time);
							// Lectura 2D = 0
								if (digitalRead(Boton_Array_D) == LOW)
									{
										Boton_Calc = 0;
										delay(Boton_Delay_Teclado);
									}
								delay(Num_Barrido_Time);
							digitalWrite(Boton_Array_2, HIGH);	// lectura linea 2
							
						// Linea 3
							digitalWrite(Boton_Array_3, LOW);	// lectura linea 3
							// Lectura 3A = 3
								if (digitalRead(Boton_Array_A) == LOW)
									{
										Boton_Calc = 3;
										delay(Boton_Delay_Teclado);
									}
								delay(Num_Barrido_Time);
							// Lectura 3B = 4
								if (digitalRead(Boton_Array_B) == LOW)
									{
										Boton_Calc = 6;
										delay(Boton_Delay_Teclado);
									}
								delay(Num_Barrido_Time);
							// Lectura 3C = 9
								if (digitalRead(Boton_Array_C) == LOW)
									{
										Boton_Calc = 9;
										delay(Boton_Delay_Teclado);
									}	
								delay(Num_Barrido_Time);
							// Lectura 3D = 14
								if (digitalRead(Boton_Array_D) == LOW)
									{
										Boton_Calc = 14;
										delay(Boton_Delay_Teclado);
									}
								delay(Num_Barrido_Time);
							digitalWrite(Boton_Array_3, HIGH);	// lectura linea 3
						
						// Linea 4
							digitalWrite(Boton_Array_4, LOW);	// lectura linea 4
							// Lectura 4A = 10
								if (digitalRead(Boton_Array_A) == LOW)
									{
										Boton_Calc = 10;
										delay(Boton_Delay_Teclado);
									}
								delay(Num_Barrido_Time);
							// Lectura 4B = 11
								if (digitalRead(Boton_Array_B) == LOW)
									{
										Boton_Calc = 11;
										delay(Boton_Delay_Teclado);
									}
								delay(Num_Barrido_Time);
							// Lectura 4C = 12
								if (digitalRead(Boton_Array_C) == LOW)
									{
										Boton_Calc = 12;
										delay(Boton_Delay_Teclado);
									}
								delay(Num_Barrido_Time);
							// Lectura 4D = 13
								if (digitalRead(Boton_Array_D) == LOW)
									{
										Boton_Calc = 13;
										delay(Boton_Delay_Teclado);
									}
							delay(Num_Barrido_Time);
							digitalWrite(Boton_Array_4, HIGH);	// lectura linea 4
			}
	}