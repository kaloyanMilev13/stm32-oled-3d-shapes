/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2026 STMicroelectronics.
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
#include "ssd1306.h"
#include "ssd1306_fonts.h"
#include "math.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

#define NUM_OF_ITEMS 7
#define MAX_TITLE_LENGTH 20

// ' scroll', 1x64px
const unsigned char epd_bitmap__scroll[] = { 0x80, 0x80, 0x00, 0x80, 0x00, 0x80,
		0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80,
		0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80,
		0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80,
		0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80,
		0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x00, 0x00, 0x00 };
// ' selected', 118x21px
const unsigned char epd_bitmap__selected[] = { 0x3f, 0xff, 0xff, 0xff, 0xff,
		0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x40, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x30, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x18, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0c, 0x80, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0c, 0x80, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x0c, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x0c, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0c, 0x80, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0c, 0x80, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x0c, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x0c, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0c, 0x80, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0c, 0x80, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x0c, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x0c, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0c, 0x80, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0c, 0x80, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x1c, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x38, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
		0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x1f, 0xff, 0xff, 0xff, 0xff,
		0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe0 };

// ' eye', 16x16px
const unsigned char epd_bitmap__eye[] = { 0x04, 0x91, 0xc4, 0x92, 0x20, 0x04,
		0x0f, 0xf0, 0x10, 0x08, 0x20, 0x04, 0x41, 0x82, 0x83, 0xc1, 0x83, 0xc1,
		0x41, 0x81, 0x60, 0x02, 0x10, 0x0c, 0x0f, 0xf0, 0x20, 0x04, 0x45, 0x22,
		0x85, 0x22 };
// ' gun', 16x16px
const unsigned char epd_bitmap__gun[] = { 0x00, 0x40, 0x00, 0x40, 0x00, 0xf0,
		0xff, 0x8c, 0xad, 0x04, 0x91, 0x04, 0xff, 0xe4, 0x01, 0x24, 0x00, 0xf2,
		0x00, 0x32, 0x00, 0x22, 0x00, 0x42, 0x00, 0x7e, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00 };
// ' smiley', 16x16px
const unsigned char epd_bitmap__smiley[] = { 0x10, 0x20, 0x10, 0x20, 0x10, 0x20,
		0x10, 0x20, 0x10, 0x20, 0x10, 0x20, 0x00, 0x04, 0x80, 0x04, 0x80, 0x04,
		0x40, 0x08, 0x60, 0x08, 0x20, 0x10, 0x1f, 0xe0, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00 };
// ' sun', 16x16px
const unsigned char epd_bitmap__sun[] = { 0x04, 0x48, 0x44, 0x10, 0x21, 0x01,
		0x0e, 0xc2, 0x0e, 0xb2, 0xdb, 0x70, 0x1f, 0xe9, 0x3b, 0x78, 0x3f, 0xb4,
		0xbd, 0xfc, 0x1b, 0xfe, 0x1b, 0xb4, 0xcf, 0xf8, 0x0f, 0xe3, 0x23, 0x90,
		0x24, 0x08 };
// ' tree', 16x16px
const unsigned char epd_bitmap__tree[] = { 0x5f, 0xe0, 0x79, 0x20, 0xf7, 0xb0,
		0xfa, 0xf0, 0x5d, 0x20, 0x3b, 0xe0, 0x0b, 0x00, 0x0b, 0x00, 0x0f, 0x00,
		0x0b, 0x00, 0x0b, 0x00, 0x0d, 0x00, 0x0b, 0x00, 0x3f, 0xc0, 0x00, 0x00,
		0x00, 0x00 };

// ' cube', 16x16px
const unsigned char epd_bitmap__cube[] = { 0xff, 0xfc, 0xc0, 0x0c, 0xa0, 0x14,
		0x90, 0x24, 0x8f, 0xc4, 0x88, 0x44, 0x88, 0x44, 0x88, 0x44, 0x88, 0x44,
		0x8f, 0xc4, 0x90, 0x24, 0xa0, 0x14, 0xc0, 0x0c, 0xff, 0xfc, 0x00, 0x00,
		0x00, 0x00 };
// ' pyr', 16x16px
const unsigned char epd_bitmap__pyr[] = { 0x01, 0x80, 0x03, 0x40, 0x05, 0x40,
		0x05, 0x40, 0x05, 0x20, 0x09, 0x20, 0x09, 0x20, 0x11, 0x10, 0x11, 0x10,
		0x23, 0x10, 0x24, 0x88, 0x48, 0x48, 0x50, 0x28, 0xa0, 0x14, 0xff, 0xfc,
		0x00, 0x00 };

// Array of all bitmaps for convenience. (Total bytes used to store images in PROGMEM = 240)
const int bitmap_icons_LEN = 7;
const unsigned char *bitmap_icons[7] = { epd_bitmap__eye, epd_bitmap__gun,
		epd_bitmap__smiley, epd_bitmap__sun, epd_bitmap__tree, epd_bitmap__cube,
		epd_bitmap__pyr };

char menu_items[NUM_OF_ITEMS][MAX_TITLE_LENGTH] = {
		{ "Arrow" },
		{ "Penta 1" },
		{"Penta 2" },
		{ "Prism" },
		{ "Tetra" },
		{ "Cube" },
		{ "Pyramid" }
	};


/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_I2C1_Init(void);
/* USER CODE BEGIN PFP */
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
#define PI 3.14159265358979323846f
#define DEG2RAD (PI / 180.0f)
#define MAX_POINTS 10


#define FRAME_PERIOD_MS       33U    // 30 FPS
#define ROTATION_SPEED_DPS    60.0f  // Degrees per second
#define SCALE_SPEED_RAD_S     2.0f   // Scale oscillation speed

uint32_t lastTime = 0;
uint32_t now = 0;
uint32_t elapsed = 0;

float angle = 0.0;
float rad = 0.0f;
float z_offset = -5.0; //offset po z
float mult = 70.0; //scaler
int osc = 0; //oscilator
int shape = 0;
int n; //tochki

uint8_t currentScreen = 0;
int itemSelected = 1;
int previousItem = 0;
int nextItem = 2;

uint8_t selectPressed = 0;
uint8_t down_buttonIsPressed = 0;
uint8_t up_buttonIsPressed = 0;

const int cube_points[8][3] = { //x, y, z
		{ -1, -1, 1 }, // 0
		{ 1, -1, 1 }, // 1
		{ 1, 1, 1 }, // 2
		{ -1,1, 1 }, // 3
		{ -1, -1, -1 }, // 4
		{ 1, -1, -1 }, // 5
		{ 1, 1, -1 }, // 6
		{ -1, 1, -1 } // 7
	};

const int pyramid_points[5][3] = { // x , y , z
		{ 1, -1, -1 }, // 0
		{ -1, -1, -1 }, // 1
		{ -1, -1, 1 }, // 2
		{ 1, -1, 1 }, // 3
		{ 0, 1, 0 }, // 4
	};

const int tetrahedron_points[4][3] = { //x,y,z
		{0, -1, -1}, // 0
		{1, -1, 1}, //1
		{-1, -1, 1}, //2
		{0, 1, 0} //3
		//zashtoto osnowata ne e kwadrat i winagi she izglejda leko ploska s celi chisla
	};

const int triangular_prism [6][3] =  { //x,y,z

		{0, -1, -1}, // 0
		{1, -1, 1}, // 1
		{-1, -1, 1}, // 2

		{0, 1, -1}, // 3
		{1, 1, 1}, // 4
		{-1, 1, 1} // 5

	};

const int pentagon_one_points [10][3] = { //x,y,z

		{0, 1.5, -1},
		{2 , 0.5, -1},
		{1, -1.5, -1},
		{-1, -1.5, -1},
		{-2, 0.5, -1},

		{0, 1.5, 1},
		{2 , 0.5, 1},
		{1, -1.5, 1},
		{-1, -1.5, 1},
		{-2, 0.5, 1}

};

const int pentagon_two_points [10][3] = { //x,y,z
		{0,-1,-2},
		{2, -1, -1},
		{1, -1, 1},
		{-1, -1, 1},
		{-2, -1, -1},

		{0, 1, -2},
		{2, 1, -1},
		{1, 1, 1},
		{-1, 1, 1},
		{-2, 1, -1}
};

const int arrow_points [4][3] = {

		{0, 0, 2},
		{1, 1, 1},
		{1, -1, 1},
		{2, 0, 0}

};

static void displayCube(int points_2d[][2]) {

	//CLear Screen
	ssd1306_Fill(Black);

	//Display Cube
	ssd1306_Line(points_2d[0][0], points_2d[0][1], points_2d[1][0],points_2d[1][1], White); // 0 -1
	ssd1306_Line(points_2d[1][0], points_2d[1][1], points_2d[2][0],points_2d[2][1], White); // 1 - 2
	ssd1306_Line(points_2d[2][0], points_2d[2][1], points_2d[3][0],points_2d[3][1], White); // 2 - 3
	ssd1306_Line(points_2d[3][0], points_2d[3][1], points_2d[0][0],points_2d[0][1], White); // 3 - 0

	ssd1306_Line(points_2d[4][0], points_2d[4][1], points_2d[5][0],points_2d[5][1], White); // 4 - 5
	ssd1306_Line(points_2d[5][0], points_2d[5][1], points_2d[6][0],points_2d[6][1], White); // 5 - 6
	ssd1306_Line(points_2d[6][0], points_2d[6][1], points_2d[7][0],points_2d[7][1], White); // 6 - 7
	ssd1306_Line(points_2d[7][0], points_2d[7][1], points_2d[4][0],points_2d[4][1], White); // 7 - 4

	ssd1306_Line(points_2d[0][0], points_2d[0][1], points_2d[4][0], points_2d[4][1], White); // 0 - 4
	ssd1306_Line(points_2d[1][0], points_2d[1][1], points_2d[5][0],points_2d[5][1], White); // 1 - 5
	ssd1306_Line(points_2d[2][0], points_2d[2][1], points_2d[6][0],points_2d[6][1], White); // 2 - 6
	ssd1306_Line(points_2d[3][0], points_2d[3][1], points_2d[7][0],points_2d[7][1], White); // 3 - 7

	ssd1306_UpdateScreen();

}

static void displayPentagon(int points_2d[][2]) {

	//CLear Screen
	ssd1306_Fill(Black);

	//Display Cube
	ssd1306_Line(points_2d[0][0], points_2d[0][1], points_2d[1][0],points_2d[1][1], White); // 0 - 1
	ssd1306_Line(points_2d[1][0], points_2d[1][1], points_2d[2][0],points_2d[2][1], White); // 1 - 2
	ssd1306_Line(points_2d[2][0], points_2d[2][1], points_2d[3][0],points_2d[3][1], White); // 2 - 3
	ssd1306_Line(points_2d[3][0], points_2d[3][1], points_2d[4][0],points_2d[4][1], White); // 3 - 4
	ssd1306_Line(points_2d[4][0], points_2d[4][1], points_2d[0][0],points_2d[0][1], White); // 4 - 0

	ssd1306_Line(points_2d[5][0], points_2d[5][1], points_2d[6][0],points_2d[6][1], White); // 5 - 6
	ssd1306_Line(points_2d[6][0], points_2d[6][1], points_2d[7][0],points_2d[7][1], White); // 6 - 7
	ssd1306_Line(points_2d[7][0], points_2d[7][1], points_2d[8][0],points_2d[8][1], White); // 7 - 8
	ssd1306_Line(points_2d[8][0], points_2d[8][1], points_2d[9][0],points_2d[9][1], White); // 8 - 9
	ssd1306_Line(points_2d[9][0], points_2d[9][1], points_2d[5][0],points_2d[5][1], White); // 9 - 5

	ssd1306_Line(points_2d[0][0], points_2d[0][1], points_2d[5][0],points_2d[5][1], White); // 0 - 5
	ssd1306_Line(points_2d[1][0], points_2d[1][1], points_2d[6][0],points_2d[6][1], White); // 1 - 6
	ssd1306_Line(points_2d[2][0], points_2d[2][1], points_2d[7][0],points_2d[7][1], White); // 2 - 7
	ssd1306_Line(points_2d[3][0], points_2d[3][1], points_2d[8][0],points_2d[8][1], White); // 3 - 8
	ssd1306_Line(points_2d[4][0], points_2d[4][1], points_2d[9][0],points_2d[9][1], White); // 4 - 9

	ssd1306_UpdateScreen();

}

static void displayPyramid(int points_2d[][2]) {

	//CLear Screen
	ssd1306_Fill(Black);

	ssd1306_Line(points_2d[0][0], points_2d[0][1], points_2d[1][0],points_2d[1][1], White); // 0 -1
	ssd1306_Line(points_2d[1][0], points_2d[1][1], points_2d[2][0],points_2d[2][1], White); // 1 - 2
	ssd1306_Line(points_2d[2][0], points_2d[2][1], points_2d[3][0],points_2d[3][1], White); // 2 - 3
	ssd1306_Line(points_2d[3][0], points_2d[3][1], points_2d[0][0],points_2d[0][1], White); // 3 - 0

	ssd1306_Line(points_2d[0][0], points_2d[0][1], points_2d[4][0],points_2d[4][1], White); // 4 - 5
	ssd1306_Line(points_2d[1][0], points_2d[1][1], points_2d[4][0],points_2d[4][1], White); // 5 - 6
	ssd1306_Line(points_2d[2][0], points_2d[2][1], points_2d[4][0],points_2d[4][1], White); // 6 - 7
	ssd1306_Line(points_2d[3][0], points_2d[3][1], points_2d[4][0],points_2d[4][1], White); // 7 - 4

	ssd1306_UpdateScreen();
}

static void displayTriPrism(int points_2d[][2]) {

	//CLear Screen
	ssd1306_Fill(Black);

	//Display Prism
	ssd1306_Line(points_2d[0][0], points_2d[0][1], points_2d[1][0],points_2d[1][1], White); // 0 -1
	ssd1306_Line(points_2d[1][0], points_2d[1][1], points_2d[2][0],points_2d[2][1], White); // 1 - 2
	ssd1306_Line(points_2d[2][0], points_2d[2][1], points_2d[0][0],points_2d[0][1], White); // 2 - 0

	ssd1306_Line(points_2d[3][0], points_2d[3][1], points_2d[4][0],points_2d[4][1], White); // 3 - 4
	ssd1306_Line(points_2d[4][0], points_2d[4][1], points_2d[5][0],points_2d[5][1], White); // 4 - 5
	ssd1306_Line(points_2d[5][0], points_2d[5][1], points_2d[3][0],points_2d[3][1], White); // 5 - 3


	ssd1306_Line(points_2d[0][0], points_2d[0][1], points_2d[3][0],points_2d[3][1], White); // 0 - 3
	ssd1306_Line(points_2d[1][0], points_2d[1][1], points_2d[4][0],points_2d[4][1], White); // 1 - 4
	ssd1306_Line(points_2d[2][0], points_2d[2][1], points_2d[5][0],points_2d[5][1], White); // 2 - 5

	ssd1306_UpdateScreen();

}


static void displayTetra(int points_2d[][2]) {

	//CLear Screen
	ssd1306_Fill(Black);

	ssd1306_Line(points_2d[0][0], points_2d[0][1], points_2d[1][0],points_2d[1][1], White); // 0 -1
	ssd1306_Line(points_2d[1][0], points_2d[1][1], points_2d[2][0],points_2d[2][1], White); // 1 - 2
	ssd1306_Line(points_2d[2][0], points_2d[2][1], points_2d[0][0],points_2d[0][1], White); // 2 - 0

	ssd1306_Line(points_2d[0][0], points_2d[0][1], points_2d[3][0],points_2d[3][1], White); // 0 - 3
	ssd1306_Line(points_2d[1][0], points_2d[1][1], points_2d[3][0],points_2d[3][1], White); // 1 - 3
	ssd1306_Line(points_2d[2][0], points_2d[2][1], points_2d[3][0],points_2d[3][1], White); // 2 - 3

	ssd1306_UpdateScreen();
}

static void displayArrow(int points_2d[][2]) {

	//CLear Screen
	ssd1306_Fill(Black);

	ssd1306_Line(points_2d[0][0], points_2d[0][1], points_2d[1][0],points_2d[1][1], White); // 0 - 1
	ssd1306_Line(points_2d[0][0], points_2d[0][1], points_2d[2][0],points_2d[2][1], White); // 0 - 2
	ssd1306_Line(points_2d[0][0], points_2d[0][1], points_2d[3][0],points_2d[3][1], White); // 0 - 3

	ssd1306_UpdateScreen();
}

static void rotateXYZ(const int original_points[][3], float rotated_points[][3],float rad, float z_offset, int n) {
	//rotate x, y, z
	for (int i = 0; i < n; i++) {
		//X value
		rotated_points[i][0] = original_points[i][0] * cos(rad)
				- original_points[i][2] * sin(rad);

		//Y val - ne se promenq, zashtoto wurtim po Y-axis
		rotated_points[i][1] = original_points[i][1];

		//Z val + Z_offset
		rotated_points[i][2] = original_points[i][0] * sin(rad)
				+ original_points[i][2] * cos(rad) + z_offset;
	}
}

static void pointsConvert2D(int points_2d[][2], float rotated_points[][3],
		int n) {
	//3d to 2d
	for (int i = 0; i < n; i++) {
		//X val = Xc = x / z
		points_2d[i][0] = roundf(
				64 + rotated_points[i][0] / rotated_points[i][2] * mult); // +64 za centrirane

				//Y val = Yc = y / z
		points_2d[i][1] = roundf(
				32 + rotated_points[i][1] / rotated_points[i][2] * mult); //+32 za centrirane
	}
}


static void spinShape(const int original_points[][3], float rotated_points[][3], int points_2d[][2],float rad, float z_offset, int n, int shape){

	//rotate XYZ
	rotateXYZ(original_points, rotated_points, rad, z_offset, n);

	//3D to 2D
	pointsConvert2D(points_2d, rotated_points, n);

	switch(shape){
	case 0: displayArrow(points_2d); break;
	case 1: displayPentagon(points_2d); break;
	case 2: displayPentagon(points_2d); break;
	case 3: displayTriPrism(points_2d);break;
	case 4: displayTetra(points_2d); break;
	case 5: displayCube(points_2d); break;
	case 6:	displayPyramid(points_2d); break;
	}

}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

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
  MX_USART2_UART_Init();
  MX_I2C1_Init();
  /* USER CODE BEGIN 2 */

	const int (*original_points)[3];
	int points_2d[MAX_POINTS][2]; //x,y - 2d
	float rotated_points[MAX_POINTS][3]; //x,y,z zawurtqni w 3d
	float scalePhase = 0.0f;

	ssd1306_Init();

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	while (1) {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

		//choose screen
		if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_9) == 0 && selectPressed == 0) {
			selectPressed = 1;
			if (currentScreen == 0)
				currentScreen = 1;
			else
				currentScreen = 0;
		}

		if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_9) == 1 && selectPressed == 1)
			selectPressed = 0;

		if (currentScreen == 0) {

			if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_10) == 0
					&& down_buttonIsPressed == 0) {
				down_buttonIsPressed = 1;

				previousItem = itemSelected;
				itemSelected = nextItem;

				if (nextItem >= NUM_OF_ITEMS - 1)
					nextItem = 0;
				else
					nextItem++;
			}

			if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_10) == 1
					&& down_buttonIsPressed == 1)
				down_buttonIsPressed = 0;

			if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_5) == 0
					&& up_buttonIsPressed == 0) {
				up_buttonIsPressed = 1;

				nextItem = itemSelected;
				itemSelected = previousItem;

				if (previousItem <= 0)
					previousItem = NUM_OF_ITEMS - 1;
				else
					previousItem--;
			}

			if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_5) == 1
					&& up_buttonIsPressed == 1)
				up_buttonIsPressed = 0;

			ssd1306_Fill(Black);

			ssd1306_DrawBitmap(1, 22, epd_bitmap__selected, 118, 21, White);
			ssd1306_DrawBitmap(126, 1, epd_bitmap__scroll, 1, 64, White);

			ssd1306_DrawBitmap(4, 2, bitmap_icons[previousItem], 16, 16, White); //previous item - icon
			ssd1306_SetCursor(30, 0);
			ssd1306_WriteString(menu_items[previousItem], Font_11x18, White); //previous item - label

			ssd1306_DrawBitmap(4, 24, bitmap_icons[itemSelected], 16, 16,White); //current item - icon
			ssd1306_SetCursor(30, 23);
			ssd1306_WriteString(menu_items[itemSelected], Font_11x18, White); //current item - label

			ssd1306_DrawBitmap(4, 46, bitmap_icons[nextItem], 16, 16, White); //next item - icon
			ssd1306_SetCursor(30, 46);
			ssd1306_WriteString(menu_items[nextItem], Font_11x18, White); //next item - label

			ssd1306_UpdateScreen();
		} else if (currentScreen == 1) {

			switch(itemSelected){
				case 0: 	n = 4;
							original_points = arrow_points;
							break;

				case 1: 	n = 10;
							original_points = pentagon_one_points;
							break;

				case 2: 	n = 10;
							original_points = pentagon_two_points;
							break;

				case 3: 	n = 6;
							original_points = triangular_prism;
							break;

				case 4:		n = 4;
							original_points = tetrahedron_points;
							break;


				case 5: 	n = 8;
							original_points = cube_points;
							break;



				case 6: 	n = 5;
							original_points = pyramid_points;
							break;


				default: original_points = NULL; n = 0; break;
			}


			now = HAL_GetTick();
			elapsed = now - lastTime;
			if(now - lastTime >= FRAME_PERIOD_MS){
				lastTime = now;

			    float deltaTime = (float)elapsed / 1000.0f;
			    angle += ROTATION_SPEED_DPS * deltaTime;

			    while (angle >= 360.0f) {
			        angle -= 360.0f;
			    }


			    rad = angle * DEG2RAD;

			    scalePhase += SCALE_SPEED_RAD_S * deltaTime;

			    while (scalePhase >= 2.0f * PI) {
			        scalePhase -= 2.0f * PI;
			    }

			    mult = 70.0f + sinf(scalePhase) * 15.0f;

			    if(original_points != NULL)
			    	spinShape(original_points, rotated_points, points_2d, rad, z_offset, n, itemSelected);

			}

		}
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

  /** Configure the main internal regulator output voltage
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLLMUL_4;
  RCC_OscInitStruct.PLL.PLLDIV = RCC_PLLDIV_2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART2|RCC_PERIPHCLK_I2C1;
  PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
  PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_PCLK1;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.Timing = 0x0040040C;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
  {
    Error_Handler();
  }

  /** I2C Fast mode Plus enable
  */
  HAL_I2CEx_EnableFastModePlus(I2C_FASTMODEPLUS_I2C1);
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : LD2_Pin */
  GPIO_InitStruct.Pin = LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LD2_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : PA9 PA10 */
  GPIO_InitStruct.Pin = GPIO_PIN_9|GPIO_PIN_10;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : PB5 */
  GPIO_InitStruct.Pin = GPIO_PIN_5;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
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
	while (1) {
	}
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
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
