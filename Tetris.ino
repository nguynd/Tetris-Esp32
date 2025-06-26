// ✅ Gộp toàn bộ code Tetris với Dual OLED + Score/Level/Next/Hold
// ✅ Có lưu Top 5 điểm cao bằng EEPROM
// ✅ Hiển thị bảng xếp hạng khi thua và nhấn nút 25 để chơi lại
// ⚙️ Sẵn sàng flash ESP32

#include <Wire.h>
#include <EEPROM.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_ADDR 0x3C
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

TwoWire I2C_MAIN = TwoWire(0);
TwoWire I2C_SIDE = TwoWire(1);
Adafruit_SSD1306 displayMain(SCREEN_WIDTH, SCREEN_HEIGHT, &I2C_MAIN, -1);
Adafruit_SSD1306 displaySide(SCREEN_WIDTH, SCREEN_HEIGHT, &I2C_SIDE, -1);

const uint8_t BOARD_W = 10, BOARD_H = 20, PIX = 5;
const uint8_t X0 = (SCREEN_HEIGHT - BOARD_W * PIX) / 2;
const uint8_t Y0 = (SCREEN_WIDTH  - BOARD_H * PIX);

uint8_t board[BOARD_H][BOARD_W] = {0};
uint8_t curPiece, curRot, nextPiece, heldPiece = 255;
int8_t  curX, curY;
bool hasHeldThisTurn = false;
uint32_t score = 0;  uint8_t level = 1;
uint32_t tickMs = 450, lastFallMs = 0;

#define HS_NUM       5
#define EEPROM_SIZE  64
uint32_t highScores[HS_NUM];

#define BTN_LEFT   32
#define BTN_RIGHT  33
#define BTN_ROTATE 25
#define BTN_DOWN   26
#define BTN_HOLD   27
struct Btn{const uint8_t pin; bool prev=false; uint32_t last=0;};
const uint16_t DEBOUNCE_MS = 120;
Btn btnLeft{BTN_LEFT}, btnRight{BTN_RIGHT}, btnRot{BTN_ROTATE}, btnDown{BTN_DOWN}, btnHold{BTN_HOLD};

const uint16_t tetromino[7][4] = {
  {0x0F00,0x2222,0x00F0,0x4444}, {0x0660,0x0660,0x0660,0x0660},
  {0x0E40,0x4C40,0x4E00,0x4640}, {0x06C0,0x8C40,0x06C0,0x8C40},
  {0x0C60,0x4C80,0x0C60,0x4C80}, {0x8E00,0x6440,0x0E20,0x44C0},
  {0x2E00,0x4460,0x0E80,0xC440}
};

void setup(){
  Serial.begin(115200);
  I2C_MAIN.begin(21,22); I2C_SIDE.begin(19,18);
  if(!displayMain.begin(SSD1306_SWITCHCAPVCC,OLED_ADDR)||!displaySide.begin(SSD1306_SWITCHCAPVCC,OLED_ADDR)) while(true);
  displayMain.setRotation(1); displaySide.setRotation(1);
  displayMain.clearDisplay(); displaySide.clearDisplay();
  displayMain.display(); displaySide.display();
  pinMode(BTN_LEFT,INPUT_PULLUP); pinMode(BTN_RIGHT,INPUT_PULLUP);
  pinMode(BTN_ROTATE,INPUT_PULLUP); pinMode(BTN_DOWN,INPUT_PULLUP); pinMode(BTN_HOLD,INPUT_PULLUP);
  EEPROM.begin(EEPROM_SIZE);  loadHighScores();
  randomSeed(esp_random());   nextPiece=random(0,7); newPiece();
}

void loop(){
  if(btnPressed(btnLeft)  && !collide(curX-1,curY,curRot)) curX--;
  if(btnPressed(btnRight) && !collide(curX+1,curY,curRot)) curX++;
  if(btnPressed(btnRot))  rotateCW();
  if(btnPressed(btnDown)  && !collide(curX,curY+1,curRot)) curY++;
  if(btnPressed(btnHold)) holdPiece();

  if(millis()-lastFallMs>tickMs){
    lastFallMs=millis();
    if(!collide(curX,curY+1,curRot)) curY++;
    else{
      mergePiece(); clearLines(); newPiece();
      if(collide(curX,curY,curRot)){
        int idx = updateHighScores(score);
        showHighScores(idx);
        while(true){
          if(!digitalRead(BTN_ROTATE)){
            delay(200); restartGame(); break;
          }
        }
      }
    }
  }
  drawMain(); drawSide();
}

bool btnPressed(Btn &b){
  bool now=!digitalRead(b.pin);
  if(now && !b.prev && (millis()-b.last>DEBOUNCE_MS)){
    b.last=millis(); b.prev=now; return true;
  }
  b.prev=now; return false;
}

void newPiece(){curPiece=nextPiece; nextPiece=random(0,7); curRot=0; curX=3; curY=0; hasHeldThisTurn=false;}
bool collide(int8_t nx,int8_t ny,uint8_t nRot){
  uint16_t s=tetromino[curPiece][nRot];
  for(int8_t y=0;y<4;y++) for(int8_t x=0;x<4;x++)
    if(s & (0x8000>>(y*4+x))){
      int8_t bx=nx+x, by=ny+y;
      if(bx<0||bx>=BOARD_W||by>=BOARD_H) return true;
      if(by>=0 && board[by][bx])        return true;
    }
  return false;
}
void mergePiece(){
  uint16_t s=tetromino[curPiece][curRot];
  for(int8_t y=0;y<4;y++) for(int8_t x=0;x<4;x++)
    if(s & (0x8000>>(y*4+x))){
      int8_t bx=curX+x, by=curY+y;
      if(by>=0 && by<BOARD_H && bx>=0 && bx<BOARD_W) board[by][bx]=curPiece+1;
    }
}
void clearLines(){
  uint8_t lines=0;
  for(int8_t y=BOARD_H-1;y>=0;y--){
    bool full=true; for(int8_t x=0;x<BOARD_W;x++) if(!board[y][x]){full=false;break;}
    if(full){ for(int8_t yy=y;yy>0;yy--) memcpy(board[yy],board[yy-1],BOARD_W);
              memset(board[0],0,BOARD_W); y++; lines++; }
  }
  switch(lines){case 1:score+=100;break;case 2:score+=250;break;case 3:score+=500;break;case 4:score+=1000;break;}
  level=score/2000+1; tickMs=450-(level-1)*30; if(tickMs<100) tickMs=100;
}
void rotateCW(){uint8_t n=(curRot+1)&3; if(!collide(curX,curY,n)) curRot=n;}
void holdPiece(){
  if(hasHeldThisTurn) return;
  if(heldPiece==255){ heldPiece=curPiece; newPiece(); }
  else{ uint8_t t=curPiece; curPiece=heldPiece; heldPiece=t; curRot=0; curX=3; curY=0; }
  hasHeldThisTurn=true;
}

void drawMain(){
  displayMain.clearDisplay();
  for(int8_t y=0;y<BOARD_H;y++) for(int8_t x=0;x<BOARD_W;x++)
    if(board[y][x]) displayMain.fillRect(X0+x*PIX, Y0+y*PIX, PIX,PIX, SSD1306_WHITE);
  uint16_t s=tetromino[curPiece][curRot];
  for(int8_t y=0;y<4;y++) for(int8_t x=0;x<4;x++)
    if(s & (0x8000>>(y*4+x))){int bx=curX+x, by=curY+y; if(by>=0)
      displayMain.fillRect(X0+bx*PIX, Y0+by*PIX, PIX,PIX, SSD1306_WHITE);}
  displayMain.drawRect(X0-1,Y0-1,BOARD_W*PIX+2,BOARD_H*PIX+2,SSD1306_WHITE);
  displayMain.display();
}

void drawSide(){
  displaySide.clearDisplay(); displaySide.setRotation(1);
  displaySide.setTextColor(SSD1306_WHITE);
  displaySide.setTextSize(1); displaySide.setCursor(0,0); displaySide.print("SCORE");
  displaySide.setTextSize(2); displaySide.setCursor(0,8); displaySide.println(score);
  displaySide.setTextSize(1); displaySide.setCursor(0,26); displaySide.print("LEVEL");
  displaySide.setTextSize(2); displaySide.setCursor(0,34); displaySide.println(level);
  displaySide.setTextSize(1); displaySide.setCursor(0,55); displaySide.print("NEXT");
  uint16_t ns=tetromino[nextPiece][0];
  for(int8_t y=0;y<4;y++) for(int8_t x=0;x<4;x++)
    if(ns & (0x8000>>(y*4+x))) displaySide.fillRect(40+x*5,55+y*5,5,5,SSD1306_WHITE);
  displaySide.setTextSize(1); displaySide.setCursor(0,85); displaySide.print("HOLD");
  if(heldPiece!=255){
    uint16_t hs=tetromino[heldPiece][0];
    for(int8_t y=0;y<4;y++) for(int8_t x=0;x<4;x++)
      if(hs & (0x8000>>(y*4+x))) displaySide.fillRect(40+x*5,85+y*5,5,5,SSD1306_WHITE);
  }
  displaySide.display();
}

void loadHighScores(){
  for(int i=0;i<HS_NUM;i++){
    uint32_t v=0;
    for(int b=0;b<4;b++)
      v |= ((uint32_t)EEPROM.read(i*4+b))<<(8*b);
    highScores[i]=v;
  }
  // Nếu EEPROM chứa toàn 0xFFFFFFFF hoặc 0x00000000 thì reset
  bool invalid = true;
  for (int i = 0; i < HS_NUM; i++) {
    if (highScores[i] != 0xFFFFFFFF && highScores[i] != 0) {
      invalid = false;
      break;
    }
  }
  if (invalid) {
    for (int i = 0; i < HS_NUM; i++) highScores[i] = 0;
    saveHighScores();
  }
}
void saveHighScores(){
  for(int i=0;i<HS_NUM;i++) for(int b=0;b<4;b++) EEPROM.write(i*4+b,(highScores[i]>>(8*b))&0xFF);
  EEPROM.commit();
}
int updateHighScores(uint32_t s){
  int idx=-1; for(int i=0;i<HS_NUM;i++) if(s>highScores[i]){idx=i;break;}
  if(idx==-1) return -1;
  for(int j=HS_NUM-1;j>idx;j--) highScores[j]=highScores[j-1]; highScores[idx]=s;
  saveHighScores(); return idx;
}
void showHighScores(int highlight){
  displayMain.clearDisplay();
  displayMain.setTextSize(1);
  displayMain.setTextColor(SSD1306_WHITE);
  displayMain.setCursor(12,0); displayMain.print("GAME OVER!");
  displayMain.setCursor(18,12); displayMain.print("TOP 5");
  for(int i=0;i<HS_NUM;i++){
    int y=24+i*8;
    if(i==highlight){ displayMain.setCursor(0,y); displayMain.print((char)0x10); }
    displayMain.setCursor(6,y);
    displayMain.print(String(i+1));
    displayMain.print(". ");
    displayMain.print(highScores[i]);
  }
  displayMain.display();
}
void restartGame(){
  memset(board,0,sizeof(board)); score=0; level=1; tickMs=450; heldPiece=255;
  hasHeldThisTurn=false; nextPiece=random(0,7); newPiece();
  displayMain.clearDisplay(); displayMain.display();
}
