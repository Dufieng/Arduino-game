#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define PADDLE_HEIGHT 10
#define PADDLE_WIDTH 2
#define BALL_SIZE 2

#define BUTTON_UP 2
#define BUTTON_DOWN 3

int paddle1_y = (SCREEN_HEIGHT - PADDLE_HEIGHT) / 2;
int paddle2_y = (SCREEN_HEIGHT - PADDLE_HEIGHT) / 2;
int ball_x = SCREEN_WIDTH / 2;
int ball_y = SCREEN_HEIGHT / 2;
int ball_speed_x = 1;
int ball_speed_y = 1;
int score1 = 0;
int score2 = 0;

void setup() {
  pinMode(BUTTON_UP, INPUT_PULLUP);
  pinMode(BUTTON_DOWN, INPUT_PULLUP);

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  display.display();
  delay(2000);
  display.clearDisplay();
}

void loop() {
  updatePaddle1();
  updatePaddle2();
  updateBall();
  checkCollision();
  drawGame();
  delay(10);
}

void updatePaddle1() {
  if(digitalRead(BUTTON_UP) == LOW) {
    paddle1_y -= 2;
    if(paddle1_y < 0) paddle1_y = 0;
  }
  if(digitalRead(BUTTON_DOWN) == LOW) {
    paddle1_y += 2;
    if(paddle1_y > SCREEN_HEIGHT - PADDLE_HEIGHT) paddle1_y = SCREEN_HEIGHT - PADDLE_HEIGHT;
  }
}

void updatePaddle2() {
  if(ball_y < paddle2_y + PADDLE_HEIGHT / 2) {
    paddle2_y -= 1;
  } else {
    paddle2_y += 1;
  }
  if(paddle2_y < 0) paddle2_y = 0;
  if(paddle2_y > SCREEN_HEIGHT - PADDLE_HEIGHT) paddle2_y = SCREEN_HEIGHT - PADDLE_HEIGHT;
}

void updateBall() {
  ball_x += ball_speed_x;
  ball_y += ball_speed_y;

  if(ball_y <= 0 || ball_y >= SCREEN_HEIGHT - BALL_SIZE) {
    ball_speed_y = -ball_speed_y;
  }
  if(ball_x <= 0) {
    score2++;
    resetBall();
  }
  if(ball_x >= SCREEN_WIDTH - BALL_SIZE) {
    score1++;
    resetBall();
  }
}

void checkCollision() {
  if(ball_x <= PADDLE_WIDTH && ball_y >= paddle1_y && ball_y <= paddle1_y + PADDLE_HEIGHT) {
    ball_speed_x = -ball_speed_x;
    ball_x = PADDLE_WIDTH;
  }
  if(ball_x >= SCREEN_WIDTH - PADDLE_WIDTH - BALL_SIZE && ball_y >= paddle2_y && ball_y <= paddle2_y + PADDLE_HEIGHT) {
    ball_speed_x = -ball_speed_x;
    ball_x = SCREEN_WIDTH - PADDLE_WIDTH - BALL_SIZE;
  }
}

void resetBall() {
  ball_x = SCREEN_WIDTH / 2;
  ball_y = SCREEN_HEIGHT / 2;
  ball_speed_x = -ball_speed_x;
  ball_speed_y = random(1, 3) * (random(0, 2) == 0 ? 1 : -1);
}

void drawGame() {
  display.clearDisplay();
  display.drawRect(0, paddle1_y, PADDLE_WIDTH, PADDLE_HEIGHT, SSD1306_WHITE);
  display.drawRect(SCREEN_WIDTH - PADDLE_WIDTH, paddle2_y, PADDLE_WIDTH, PADDLE_HEIGHT, SSD1306_WHITE);
  display.fillRect(ball_x, ball_y, BALL_SIZE, BALL_SIZE, SSD1306_WHITE);
  display.setCursor(10, 0);
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.print("Score: ");
  display.print(score1);
  display.print(" - ");
  display.print(score2);
  display.display();
}