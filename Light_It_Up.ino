const uint8_t ledPins[9] = {22, 23, 24, 25, 26, 27, 28, 29, 30};
const uint8_t switchPins[9] = {31, 32, 33, 34, 35, 36, 37, 38, 39};
const uint8_t nextStagePin = 45;

const uint8_t crossLights[9][5] = {
  {0, 1, 2, 3, 6},  // 開關31 -> 燈22,23,24,25,28
  {0, 1, 2, 4, 7},  // 開關32 -> 燈22,23,24,26,29
  {0, 1, 2, 5, 8},  // 開關33 -> 燈22,23,24,27,30
  {0, 3, 4, 5, 6},  // 開關34 -> 燈22,25,26,27,28
  {1, 3, 4, 5, 7},  // 開關35 -> 燈23,25,26,27,29
  {2, 3, 4, 5, 8},  // 開關36 -> 燈24,25,26,27,30
  {0, 3, 6, 7, 8},  // 開關37 -> 燈22,25,28,29,30
  {1, 4, 6, 7, 8},  // 開關38 -> 燈23,26,28,29,30
  {2, 5, 6, 7, 8}   // 開關39 -> 燈24,27,28,29,30
};

int stageState[9] = {0};
int pressState[9] = {0};
bool inRunHorseMode = false;
int lastStageState[9] = {0};
float score = 0;
bool gameOver = false;

bool lastRawPressState[9] = {0};
int stepCount = 0;
unsigned long stageStartTime = 0;

void setup() {
  for (int i = 0; i < 9; i++) {
    pinMode(ledPins[i], OUTPUT);
    pinMode(switchPins[i], INPUT_PULLUP);
  }
  pinMode(nextStagePin, INPUT_PULLUP);
  randomSeed(analogRead(A0));
  Serial.begin(9600);

  generateNewStage();
  updateLedsFromStates();
}

void loop() {
  if (gameOver) {
    gameOverBlink();
    return;
  }

  readSwitches();  // 讀取玩家按鈕

  if (!inRunHorseMode) {
    int counts[9] = {0};
    for (int i = 0; i < 9; i++) {
      if (stageState[i]) 
        for (int j = 0; j < 5; j++) counts[crossLights[i][j]]++;
      if (pressState[i]) 
        for (int j = 0; j < 5; j++) counts[crossLights[i][j]]++;
    }

    updateLedsFromCounts(counts);

    if (allLedsOn(counts)) {
      inRunHorseMode = true;

      unsigned long clearTime = millis() - stageStartTime;
      float timeInSeconds = clearTime / 1000.0;
      
      float baseScore = 10.0;
      float bonusScore = 0.0;
      bonusScore += (timeInSeconds < 10.0) ? ((10.0 - timeInSeconds) * 10) : 0;
      bonusScore -= (timeInSeconds < 15.0) ? 0 : ((timeInSeconds - 15.0) * 3);
      bonusScore += (stepCount < 3) ? ((3 - stepCount) * 10) : 0;
      bonusScore -= (stepCount > 5) ? 0 : ((stepCount - 5) * 3);

      float totalScore = baseScore + bonusScore;
      score += totalScore;

      Serial.println("~~~~~ 關卡完成 ~~~~~");
      Serial.print("總共走了 ");
      Serial.print(stepCount);
      Serial.println(" 步");

      Serial.print("花費時間：");
      Serial.print(timeInSeconds, 2);
      Serial.println(" 秒");

      Serial.print("本關得分：");
      Serial.print(totalScore, 0);
      Serial.println(" 分\n");

      if (score < 0) {
        Serial.println("!!!!! Game Over !!!!!");
        gameOver = true;
        return;
      }

      delay(1000);
    }
  } else {
    runHorseLampFast();

    if (digitalRead(nextStagePin) == LOW) {
      inRunHorseMode = false;
      generateNewStage();
      updateLedsFromStates();
      for (int i = 0; i < 9; i++) pressState[i] = false;
      stepCount = 0;
      stageStartTime = millis();
      Serial.println("======= 下一關開始 =======");
      delay(300);
    }
  }

  delay(50);
}

void readSwitches() {
  for (int i = 0; i < 9; i++) {
    bool currentRawState = (digitalRead(switchPins[i]) == LOW);
    if (!lastRawPressState[i] && currentRawState) {
      pressState[i] = !pressState[i];
      stepCount++;
      Serial.print("已按下次數：");
      Serial.println(stepCount);
    }
    lastRawPressState[i] = currentRawState;
  }
}

void updateLedsFromCounts(int counts[9]) {
  for (int i = 0; i < 9; i++) {
    digitalWrite(ledPins[i], (counts[i] % 2 == 1) ? HIGH : LOW);
  }
}

bool allLedsOn(int counts[9]) {
  for (int i = 0; i < 9; i++) {
    if (counts[i] % 2 == 0) return false;
  }
  return true;
}

void updateLedsFromStates() {
  int counts[9] = {0};
  for (int i = 0; i < 9; i++) {
    if (stageState[i]) {
      for (int j = 0; j < 5; j++) counts[crossLights[i][j]]++;
    }
  }
  updateLedsFromCounts(counts);
}

void generateNewStage() {
  int candidate[9];
  int tries = 0;
  bool valid = false;

  do {
    tries++;
    for (int i = 0; i < 9; i++) candidate[i] = 0;

    int pressCount = random(1, 5);
    for (int i = 0; i < pressCount; i++) {
      int idx = random(0, 9);
      candidate[idx] = 1;
    }

    valid = false;
    for (int i = 0; i < 9; i++) {
      if (candidate[i] != lastStageState[i]) {
        valid = true;
        break;
      }
    }

    if (tries > 50) {
      valid = true;
    }

  } while (!valid);

  for (int i = 0; i < 9; i++) {
    stageState[i] = candidate[i];
    lastStageState[i] = candidate[i];
  }
}

void runHorseLampFast() {
  static int pos = 0;
  static unsigned long lastTime = 0;
  unsigned long now = millis();
  if (now - lastTime > 60) {
    lastTime = now;
    for (int i = 0; i < 9; i++) {
      digitalWrite(ledPins[i], (i == pos) ? HIGH : LOW);
    }
    pos = (pos + 1) % 9;
  }
}

void gameOverBlink() {
  static unsigned long lastBlink = 0;
  static bool blinkState = false;
  unsigned long now = millis();

  if (now - lastBlink > 300) {
    lastBlink = now;
    blinkState = !blinkState;
    for (int i = 0; i < 9; i++) {
      digitalWrite(ledPins[i], blinkState ? HIGH : LOW);
    }
  }
}

