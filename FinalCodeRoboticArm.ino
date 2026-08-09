#define CUSTOM_SETTINGS
#define INCLUDE_GAMEPAD_MODULE

#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <DabbleESP32.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

// PCA9685 channels
#define SERVO_BASE       4
#define SERVO_VERTICAL   5
#define SERVO_HORIZONTAL 6
#define SERVO_GRIPPER    7

// Pulse limits for PCA9685
#define SERVO_MIN 150
#define SERVO_MAX 600

int degreesToPulse(int deg) {
  return map(deg, 0, 180, SERVO_MIN, SERVO_MAX);
}

// Starting angles
int startBase = 90;
int startVertical = 90;
int startHorizontal = 90;
int startGripper = -10;

// Movement step size
const int stepSize = 2;

// Optional: slow down repeated movement a little
const int loopDelay = 30;

void writeAllServos() {
  pwm.setPWM(SERVO_BASE,       0, degreesToPulse(startBase));
  pwm.setPWM(SERVO_VERTICAL,   0, degreesToPulse(startVertical));
  pwm.setPWM(SERVO_HORIZONTAL, 0, degreesToPulse(startHorizontal));
  pwm.setPWM(SERVO_GRIPPER,    0, degreesToPulse(startGripper));
}

void setup() {
  Serial.begin(115200);

  Wire.begin();
  pwm.begin();
  pwm.setPWMFreq(50);

  // Bluetooth name shown in Dabble
  Dabble.begin("ESP32_RobotArm");
  
  // Read current physical position (approximate) and sweep slowly to start
  int initBase       = 90;  // assume arm powers on near these angles
  int initVertical   = 90;
  int initHorizontal = 90;
  int initGripper    = 90;

  // Sweep from assumed current position to your desired start over ~2 seconds
  int steps = 50;
  for (int i = 0; i <= steps; i++) {
    float t = (float)i / steps;  // 0.0 to 1.0
    int b = initBase       + t * (startBase       - initBase);
    int v = initVertical   + t * (startVertical   - initVertical);
    int h = initHorizontal + t * (startHorizontal - initHorizontal);
    int g = initGripper    + t * (startGripper    - initGripper);

    pwm.setPWM(SERVO_BASE,       0, degreesToPulse(b));
    pwm.setPWM(SERVO_VERTICAL,   0, degreesToPulse(v));
    pwm.setPWM(SERVO_HORIZONTAL, 0, degreesToPulse(h));
    pwm.setPWM(SERVO_GRIPPER,    0, degreesToPulse(g));
    delay(40);  // 50 steps x 40ms = ~2 second sweep
  }
  
  Serial.println("Robot arm ready.");
  Serial.println("Connect using Dabble Gamepad.");
}

void loop() {
  Dabble.processInput();

  bool moved = false;

  // D-pad controls
  if (GamePad.isLeftPressed()) {
    startBase -= stepSize;
    moved = true;
  }

  if (GamePad.isRightPressed()) {
    startBase += stepSize;
    moved = true;
  }

  if (GamePad.isUpPressed()) {
    startVertical -= stepSize;
    moved = true;
  }

  if (GamePad.isDownPressed()) {
    startVertical += stepSize;
    moved = true;
  }

  // Face buttons
  if (GamePad.isTrianglePressed()) {
    startHorizontal -= stepSize;
    moved = true;
  }

  if (GamePad.isCrossPressed()) {
    startHorizontal += stepSize;
    moved = true;
  }

  if (GamePad.isCirclePressed()) {
    startGripper -= stepSize;   // close claw
    moved = true;
  }

  if (GamePad.isSquarePressed()) {
    startGripper += stepSize;   // open claw
    moved = true;
  }

  if (GamePad.isStartPressed()) {
    int steps = 50;
    for (int i = 0; i <= steps; i++) {
        float t = (float)i / steps;
        int b = startBase       + t * (90 - startBase);
        int v = startVertical   + t * (90 - startVertical);
        int h = startHorizontal + t * (90 - startHorizontal);
        int g = startGripper    + t * (90 - startGripper);

        pwm.setPWM(SERVO_BASE,       0, degreesToPulse(b));
        pwm.setPWM(SERVO_VERTICAL,   0, degreesToPulse(v));
        pwm.setPWM(SERVO_HORIZONTAL, 0, degreesToPulse(h));
        pwm.setPWM(SERVO_GRIPPER,    0, degreesToPulse(g));
        delay(30);
    }
    // Update the position variables to reflect new position
    startBase       = 90;
    startVertical   = 90;
    startHorizontal = 90;
    startGripper    = 90;
  }

  // Keep within safe ranges
  startBase       = constrain(startBase, 0, 180);
  startVertical   = constrain(startVertical, 60, 160);
  startHorizontal = constrain(startHorizontal, 0, 120);
  startGripper    = constrain(startGripper, 0, 180);

  if (moved) {
    writeAllServos();

    Serial.print("Base: ");
    Serial.print(startBase);
    Serial.print(" | Vertical: ");
    Serial.print(startVertical);
    Serial.print(" | Horizontal: ");
    Serial.print(startHorizontal);
    Serial.print(" | Gripper: ");
    Serial.println(startGripper);
  }

  delay(loopDelay);
}
