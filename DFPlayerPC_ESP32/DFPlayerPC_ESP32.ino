#include "DFRobotDFPlayerMini.h"

DFRobotDFPlayerMini myDFPlayer;

#define DFPLAYER_RX 26  // ESP32 receives from DFPlayer TX
#define DFPLAYER_TX 27  // ESP32 sends to DFPlayer RX

String line;
char command;
int pauseState = 0;
int repeatState = 0;

void setup() {
  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1, DFPLAYER_RX, DFPLAYER_TX);

  delay(1000);

  Serial.println();
  Serial.println(F("DFRobot DFPlayer Mini"));
  Serial.println(F("Initializing DFPlayer module ... Wait!"));

  if (!myDFPlayer.begin(Serial2)) {
    Serial.println(F("Not initialized:"));
    Serial.println(F("1. Check DFPlayer Mini connections"));
    Serial.println(F("2. Insert SD card"));
    while (true);
  }

  Serial.println(F("DFPlayer Mini module initialized!"));

  myDFPlayer.setTimeOut(500);
  myDFPlayer.volume(5);
  myDFPlayer.EQ(DFPLAYER_EQ_NORMAL);

  menu_options();
}

void loop() {
  while (Serial.available() > 0) {
    line = Serial.readStringUntil('\n');
    line.trim();

    if (line.length() == 0) return;

    command = line.charAt(0);

    if (command >= '1' && command <= '9') {
      int track = command - '0';
      Serial.print("Playing track ");
      Serial.println(track);
      myDFPlayer.play(track);
      menu_options();
    }

    else if (command == 'f') {
      int indexF = line.indexOf('f');
      int indexS = line.indexOf('s');

      if (indexF != -1 && indexS != -1 && indexF < indexS) {
        int folder = line.substring(indexF + 1, indexS).toInt();
        int song = line.substring(indexS + 1).toInt();

        Serial.print("From folder: ");
        Serial.print(folder);
        Serial.print(", playing song: ");
        Serial.println(song);

        myDFPlayer.playFolder(folder, song);
      } else {
        Serial.println("Use format: fXsY, example f1s3");
      }

      menu_options();
    }

    else if (command == 's') {
      myDFPlayer.stop();
      Serial.println("Music stopped!");
      menu_options();
    }

    else if (command == 'p') {
      pauseState = !pauseState;

      if (pauseState == 1) {
        myDFPlayer.pause();
        Serial.println("Music paused!");
      } else {
        myDFPlayer.start();
        Serial.println("Continue...");
      }

      menu_options();
    }

    else if (command == 'r') {
      repeatState = !repeatState;

      if (repeatState == 1) {
        myDFPlayer.enableLoop();
        Serial.println("Repeat mode enabled.");
      } else {
        myDFPlayer.disableLoop();
        Serial.println("Repeat mode disabled.");
      }

      menu_options();
    }

    else if (command == 'v') {
      int myVolume = line.substring(1).toInt();

      if (myVolume >= 0 && myVolume <= 30) {
        myDFPlayer.volume(myVolume);
        Serial.print("Current volume: ");
        Serial.println(myVolume);
      } else {
        Serial.println("Invalid volume. Choose 0-30.");
      }

      menu_options();
    }

    else if (command == '+') {
      myDFPlayer.volumeUp();
      Serial.println("Volume up");
      menu_options();
    }

    else if (command == '-') {
      myDFPlayer.volumeDown();
      Serial.println("Volume down");
      menu_options();
    }

    else if (command == '<') {
      myDFPlayer.previous();
      Serial.println("Previous track");
      menu_options();
    }

    else if (command == '>') {
      myDFPlayer.next();
      Serial.println("Next track");
      menu_options();
    }
  }
}

void menu_options() {
  Serial.println();
  Serial.println(F("================================================"));
  Serial.println(F("Commands:"));
  Serial.println(F(" [1-9] play MP3 file"));
  Serial.println(F(" [fXsY] play song Y from folder X"));
  Serial.println(F(" [s] stop"));
  Serial.println(F(" [p] pause / continue"));
  Serial.println(F(" [r] toggle repeat"));
  Serial.println(F(" [vX] set volume 0-30"));
  Serial.println(F(" [+ / -] volume up / down"));
  Serial.println(F(" [< / >] previous / next"));
  Serial.println(F("================================================"));
}