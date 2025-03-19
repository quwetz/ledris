bool skull[16][10] = { 
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
    {0, 0, 1, 1, 1, 1, 0, 0, 0, 0}, 
    {0, 0, 1, 0, 0, 0, 1, 0, 0, 0}, 
    {0, 1, 0, 1, 0, 1, 0, 1, 0, 0}, 
    {0, 1, 1, 0, 1, 0, 1, 1, 0, 0},  
    {0, 1, 0, 0, 0, 0, 0, 1, 0, 0},  
    {1, 0, 0, 1, 1, 1, 0, 0, 1, 0},  
    {0, 0, 0, 0, 1, 0, 0, 0, 0, 0},  
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, 
    {0, 0, 1, 0, 0, 0, 1, 0, 0, 0},  
    {0, 1, 1, 1, 0, 1, 1, 1, 0, 1},  
    {0, 0, 1, 0, 0, 0, 1, 0, 0, 0},  
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},  
    {0, 1, 0, 0, 0, 0, 0, 1, 1, 0},
    {0, 0, 1, 1, 0, 1, 1, 0, 0, 0}
};

const int SER_R = 1;
const int RCLK_R = 2;
const int SRCLK_R = 3;
const int SRCLR_R = 4;

const int SER_C = 8;
const int RCLK_C = 9;
const int SRCLK_C = 10;
const int SRCLR_C = 11;

const int RIGHT = 5;
const int LEFT = 6;
const int ROTATE = 7;

long lastStep = 0;
long stepLength = 130;

bool currentImage[16][10];
bool fixedBlocks[16][10];

bool button_left;
bool button_right;
bool button_rotate;
bool button_rotate_before;
bool gameOver = false;


void drawScreen();
void writeImg(bool img[][10]);
void fixBrick();

class Brick {

    /*Brick types:
       0 = I
       1 = T
       2 = O
       3 = L
       4 = J
       5 = Z
       6 = S
    */

  public:
    Brick(int pos_x, int pos_y, int r, int brick_type) {
      x = pos_x;
      y = pos_y;
      rotation = r;
      type = brick_type;
    }

    void rotate(void) {
      rotation = (rotation + 1) % 4;
      if (detectCollision()) {
        rotation = (rotation + 3) % 4;
      }
    }
    void moveLeft(void) {
      x++;
      if (detectCollision()) {
        x--;
      }
    }
    void moveRight(void) {
      x--;
      if (detectCollision()) {
        x++;
      }
    }
    void moveDown(void) {
      y--;
      if (detectCollision()) {
        if (y >= 14) {
          gameOver = true;
        }
        y++;
        fixBrick();
      }
    }
    void setPos(int new_x, int new_y) {
      x = new_x;
      y = new_y;
    }
    void setType(int new_type) {
      type = new_type;
    }
    void setRot(int newRotation) {
      rotation = newRotation;
    }
    int getY() {
      return y;
    }
    void draw(bool surface[][10]) {
      for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
          if (x + i >= 0 && x + i < 10 && y + j >= 0 && y + j < 16) {
            surface[y + j][x + i] = surface[y + j][x + i] || img[type][rotation][j][i];
          }
        }
      }
    }

  private:
    int x;
    int y;
    int rotation;
    int type;

    bool img[7][4][4][4] =
    {
      //T
      {
        { {0, 0, 0, 0},
          {0, 0, 0, 0},
          {0, 1, 1, 1},
          {0, 0, 1, 0}
        },

        { {0, 0, 0, 0},
          {0, 0, 1, 0},
          {0, 1, 1, 0},
          {0, 0, 1, 0}
        },

        { {0, 0, 0, 0},
          {0, 0, 1, 0},
          {0, 1, 1, 1},
          {0, 0, 0, 0}
        },

        { {0, 0, 0, 0},
          {0, 0, 1, 0},
          {0, 0, 1, 1},
          {0, 0, 1, 0}
        }
      },

      //I
      {
        { {0, 0, 1, 0},
          {0, 0, 1, 0},
          {0, 0, 1, 0},
          {0, 0, 1, 0}
        },

        { {0, 0, 0, 0},
          {0, 0, 0, 0},
          {1, 1, 1, 1},
          {0, 0, 0, 0}
        },

        { {0, 0, 1, 0},
          {0, 0, 1, 0},
          {0, 0, 1, 0},
          {0, 0, 1, 0}
        },

        { {0, 0, 0, 0},
          {0, 0, 0, 0},
          {1, 1, 1, 1},
          {0, 0, 0, 0}
        }
      },

      //O
      {
        { {0, 0, 0, 0},
          {0, 0, 0, 0},
          {0, 0, 1, 1},
          {0, 0, 1, 1}
        },

        { {0, 0, 0, 0},
          {0, 0, 0, 0},
          {0, 0, 1, 1},
          {0, 0, 1, 1}
        },

        { {0, 0, 0, 0},
          {0, 0, 0, 0},
          {0, 0, 1, 1},
          {0, 0, 1, 1}
        },

        { {0, 0, 0, 0},
          {0, 0, 0, 0},
          {0, 0, 1, 1},
          {0, 0, 1, 1}
        }
      },

      //Z
      {
        { {0, 0, 0, 0},
          {0, 1, 1, 0},
          {0, 0, 1, 1},
          {0, 0, 0, 0}
        },

        { {0, 0, 0, 0},
          {0, 0, 0, 1},
          {0, 0, 1, 1},
          {0, 0, 1, 0}
        },

        { {0, 0, 0, 0},
          {0, 1, 1, 0},
          {0, 0, 1, 1},
          {0, 0, 0, 0}
        },

        { {0, 0, 0, 0},
          {0, 0, 0, 1},
          {0, 0, 1, 1},
          {0, 0, 1, 0}
        }
      },

      //S
      {
        { {0, 0, 0, 0},
          {0, 0, 1, 1},
          {0, 1, 1, 0},
          {0, 0, 0, 0}
        },

        { {0, 0, 0, 0},
          {0, 1, 0, 0},
          {0, 1, 1, 0},
          {0, 0, 1, 0}
        },

        { {0, 0, 0, 0},
          {0, 0, 1, 1},
          {0, 1, 1, 0},
          {0, 0, 0, 0}
        },

        { {0, 0, 0, 0},
          {0, 1, 0, 0},
          {0, 1, 1, 0},
          {0, 0, 1, 0}
        }
      },

      //L
      {
        { {0, 0, 0, 0},
          {0, 0, 1, 0},
          {0, 0, 1, 0},
          {0, 0, 1, 1}
        },

        { {0, 0, 0, 0},
          {0, 0, 0, 0},
          {0, 1, 1, 1},
          {0, 1, 0, 0}
        },

        { {0, 0, 0, 0},
          {0, 1, 1, 0},
          {0, 0, 1, 0},
          {0, 0, 1, 0}
        },

        { {0, 0, 0, 0},
          {0, 0, 0, 1},
          {0, 1, 1, 1},
          {0, 0, 0, 0}
        }
      },

      //J
      {
        { {0, 0, 0, 0},
          {0, 0, 1, 0},
          {0, 0, 1, 0},
          {0, 1, 1, 0}
        },

        { {0, 0, 0, 0},
          {0, 1, 0, 0},
          {0, 1, 1, 1},
          {0, 0, 0, 0}
        },

        { {0, 0, 0, 0},
          {0, 0, 1, 1},
          {0, 0, 1, 0},
          {0, 0, 1, 0}
        },

        { {0, 0, 0, 0},
          {0, 0, 0, 0},
          {0, 1, 1, 1},
          {0, 0, 0, 1}
        }
      },
    };

    bool detectCollision() {
      for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
          if (img[type][rotation][i][j]) {
            if ((x + j < 0 || x + j > 9) || y + i < 0) {
              return true;
            } else if (y+i < 16 && fixedBlocks[y + i][x + j]) {
              return true;
            }
          }
        }
      }
      return false;
    }
};

Brick* brick = new Brick(2, 16, 0, random(0,7));

void setup() {
  
  pinMode(SER_R, OUTPUT);
  pinMode(RCLK_R, OUTPUT);
  pinMode(SRCLK_R, OUTPUT);
  pinMode(SRCLR_R, OUTPUT);
  pinMode(SER_C, OUTPUT);
  pinMode(RCLK_C, OUTPUT);
  pinMode(SRCLK_C, OUTPUT);
  pinMode(SRCLR_C, OUTPUT);

  pinMode(LEFT, INPUT);
  pinMode(RIGHT, INPUT);
  pinMode(ROTATE, INPUT);
  digitalWrite(ROTATE, HIGH);
  digitalWrite(LEFT, HIGH);
  digitalWrite(RIGHT, HIGH);


  //clear the shifting registers
  digitalWrite(SRCLR_R, LOW);
  digitalWrite(SRCLR_C, LOW);
  digitalWrite(SRCLR_R, HIGH);
  digitalWrite(SRCLR_C, HIGH);


  for (int i = 0; i < 10; i++) {
    for (int j = 0; j < 16; j++) {
      fixedBlocks[j][i] = 0;
    }
  }
  
}

void loop() {

  drawScreen();
  button_left = button_left | !digitalRead(LEFT);
  button_right = button_right | !digitalRead(RIGHT);
  button_rotate = button_rotate | !digitalRead(ROTATE);

  if (millis() - lastStep > stepLength && !gameOver) {
    //////////////////////////////////
    //This is the actual game-loop
    if (button_rotate && !button_rotate_before) {
      randomSeed(millis()*analogRead(0));
      brick->rotate();
      button_rotate_before = true;
    }
    if (!button_rotate) {
      button_rotate_before = false;
    }
    if (button_right) {
      randomSeed(millis()*analogRead(0));
      brick->moveRight();
    }
    if (button_left) {
      randomSeed(millis()*analogRead(0));
      brick->moveLeft();
    }


    brick->moveDown();
   
    //end of gameloop
    //////////////////////////////////
    lastStep = millis();
  } else if (gameOver && (button_left || button_right || button_rotate)){
    for (int i = 0; i < 10; i++) {
      for (int j = 0; j < 16; j++) {
       fixedBlocks[j][i] = 0;
      }
    }
    gameOver = false;
  }
   button_left = LOW;
    button_right = LOW;
    button_rotate = LOW;
}

void drawScreen() {
  if (!gameOver) {
    for (int i = 0; i < 10; i++) {
      for (int j = 0; j < 16; j++) {
        currentImage[j][i] = fixedBlocks[j][i];
      }
    }
    brick->draw(currentImage);
    writeImg(currentImage);
  } else writeImg(skull);
}

void writeImg(bool img[][10]) {
  digitalWrite(SRCLR_C, LOW);
  digitalWrite(SRCLR_C, HIGH);

  //fill the c_register with 1s (LOW activates a column, HIGH deactivates it, because they are controlled by an PNP-transistor)
  for (int i = 0; i < 10 - 1; i++) {
    digitalWrite(SRCLK_C, LOW);
    digitalWrite(SER_C, HIGH);
    digitalWrite(SRCLK_C, HIGH);
  }

  //shift in 0
  digitalWrite(SRCLK_C, LOW);
  digitalWrite(SER_C, LOW);
  digitalWrite(SRCLK_C, HIGH);


  //fill the r_register
  for (int i = 0; i < 10; i++) {
    delay(1);

    for (int j = 0; j < 16; j++) {
      digitalWrite(SRCLK_R, LOW);
      digitalWrite(SER_R, img[j][i]);
      digitalWrite(SRCLK_R, HIGH);
    }

    //write to storage register
    digitalWrite(RCLK_C, LOW);
    digitalWrite(RCLK_R, LOW);
    digitalWrite(RCLK_C, HIGH);
    digitalWrite(RCLK_R, HIGH);


    //shift a 1 into the register for the next column
    digitalWrite(SRCLK_C, LOW);
    digitalWrite(SER_C, HIGH);
    digitalWrite(SRCLK_C, HIGH);
  }
}

//Diese Funktion muss aufgerufen werden, wenn ein Stein unten angekommen ist.
//Folgende Dinge müssen hier ausgeführt werden:
//  1. fixieren der Pixel des Bricks
//  2. überprüfen ob vollständige Zeilen vorhanden sind, gegebenenfalls diese entfernen
//  3. neuen Brick erzeugen
void fixBrick() {
  brick->draw(fixedBlocks);
  bool lineFull = true;
  int n_lines = brick->getY() + 4;
  for (int i = brick->getY(); i < n_lines; i++) {
    for (int j = 0; j < 10; j++) {
      lineFull = lineFull && fixedBlocks[i][j];
    }
    if (lineFull) {
      for (int j = i; j < 15; j++) {
        for (int k = 0; k < 10; k++) {
          fixedBlocks[j][k] = fixedBlocks[j + 1][k];
        }
      }
      for (int j = 0; j < 10; j++) {
        fixedBlocks[15][j] = 0;
      }
      i--;
      n_lines--;
    }
    lineFull = true;
  }

  brick->setPos(2, 16);
  brick->setRot(0);
  brick->setType(random(0,7));
}
