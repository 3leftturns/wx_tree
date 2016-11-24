#include "application.h"
#include "neopixel/neopixel.h" // use for Build IDE
// #include "neopixel.h" // use for local build

SYSTEM_MODE(AUTOMATIC);

// IMPORTANT: Set pixel COUNT, PIN and TYPE
#define PIXEL_PIN D3
#define PIXEL_COUNT 100
#define PIXEL_TYPE WS2812B

Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXEL_COUNT, PIXEL_PIN, PIXEL_TYPE);

int chancePrecip = 0;
int highTemp = 100;
int lowTemp = 0;


uint32_t red = strip.Color(0,255,0);
uint32_t orange = strip.Color(127,0,255);
uint32_t yellow = strip.Color(255,255,0);
uint32_t green = strip.Color(0,255,0);
uint32_t skyblue = strip.Color(255,0,191);
uint32_t blue = strip.Color(0,0,255);
uint32_t violet = strip.Color(127,0,255);
uint32_t pink = strip.Color(255,0,127);
uint32_t softwhite = strip.Color(224,224,224);

uint32_t pallette[] = {red, orange, yellow, green, skyblue, blue, violet, pink, softwhite};
uint32_t tempColor[] = {
     0x00ffff //0 or below
    ,0x0000ff //10
    ,0x5500ff //20
    ,0xff00ff //30
    ,0xff0090 //40
    ,0xff0000 //50
    ,0xffaa00 //60
    ,0xffff00 //70
    ,0xaaff00 //80
    ,0x55ff00 //90
    ,0x00ff00 //100+
};

String windDir = "NORTH";

void setup() {
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'

  Particle.function("windDir", setWindDirection);
  Particle.function("setHiTemp", setHighTemp);
  Particle.function("setLoTemp", setLowTemp);
  Particle.function("night", night);
  Serial.begin(9600);
}
void loop() {


}

void setChancePrecip(const char *event, const char *data) {
  String precip = data;
  chancePrecip = precip.toInt();
}

int setHighTemp(String data) {
  Particle.publish("info" "High Temperature: " + data);
  String hi = data;
  highTemp = hi.toInt();

  showTemperature();

  return highTemp;
}

int setLowTemp(String data) {
  Particle.publish("info" "Low Temperature: " + data);
  String lo = data;
  lowTemp = lo.toInt();

  showTemperature();

  return lowTemp;
}

void showTemperature() {
    if (lowTemp < 0) {
        lowTemp = 0;
    }

    if (highTemp > 100) {
        highTemp = 100;
    }

    int loIndex = lowTemp / 10;
    int hiIndex = highTemp / 10;

    int colorSteps = hiIndex - loIndex;

    int i = 0;
    for (int j= 0; j < strip.numPixels(); j++) {
        if (j % (100 / colorSteps) == 0) {
            i++;
        }

        strip.setPixelColor(j, tempColor[loIndex + i]);
    }
    strip.show();
}

int night(String data) {
    Particle.publish("info", "Entering Night Mode: " + data);
    dark();
    return 1;
}
void dark() {
    for (int j= 0; j < strip.numPixels(); j++) {
        strip.setPixelColor(j, 0, 0, 0);
    }
    strip.show();
}



//Direction collection array indices
//N  - 0
//NE - 1
//E  - 2
//SE - 3
//S  - 4
//SW - 5
//W  - 6
//NW - 7
int north[] =
  {
      3,4,5, 21,22,23, 41,42,43, 58,59,60, 74, 81,86, 93,94,94
  };
int northWest[] =
  {
      5,6,7, 23,24,25, 44,45,46, 61,62, 75, 81,87, 94,97,97,97
  };

int west[] =
  {
      7,8,9, 24,25,26, 46,47,48, 62,63, 75,76, 82,87,88, 97,97
  };
int southWest[] =
  {
      9,10,11, 27,28, 48,49,50, 64,65,66, 77,78, 82,88,89, 97,97
  };
int south[] =
  {
      12,13,14, 29,30,31, 50,51, 66,67,68,70, 78,79, 82,83,89,90
  };
int southEast[] =
  {
      14,15,16, 31,32,33,34,35, 51,52,53, 68,69,70, 79, 83,90,95
  };
int east[] =
  {
      1,16,17,18, 34,36,37, 54,55, 71,72, 79,80, 84,91,96,96,96
  };
int northEast[] =
  {
      1,2,3, 19,20,21, 39,40,41, 56,57, 72,73, 80, 85,92,97,97
  };


void showWindDirection(int myArr[]) {
    dark();
    showTemperature();
      for (int j = 0; j < 18; j++) {
          strip.setPixelColor(myArr[j], softwhite);
      }
      strip.show();
  }

int setWindDirection(String dir) {
    windDir = dir;
    String direction = dir.toUpperCase();
    Particle.publish("info", direction);
    if (direction == "NORTH") {
        showWindDirection(north);
    } else if (direction.equals("NORTHEAST")) {
        showWindDirection(northEast);
    } else if (direction.equals("EAST")) {
        showWindDirection(east);
    } else if (direction.equals("SOUTHEAST")) {
        showWindDirection(southEast);
    } else if (direction.equals("SOUTH")) {
        showWindDirection(south);
    } else if (direction.equals("SOUTHWEST")) {
        showWindDirection(southWest);
    } else if (direction.equals("WEST")) {
        showWindDirection(west);
    } else if (direction.equals("NORTHWEST")) {
        showWindDirection(northWest);
    } else {
        Particle.publish("windErr", direction);
    }

    return 1;
}

// int setWeatherFromIFTTT(String weather) {
//     //expected to come in format
//     //windDir|windSpeed
//     int index_1 = weather.indexOf('|');
//     String windDir = weather.substring(0, index_1);
//     // String windStr = weather.substring(index_1+1, weather.length())
//     int windSpeed = weather.substring(index_1+1, weather.length()).toInt();

//     return 1;
// }
