#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd_display(0x27, 16, 2);

const short trig_pin = 8;
const short echo_pin = 9;
const short buzzer_pin = 6;

short cur_distance = -1;

unsigned long last_lcd_time = 0;
unsigned long last_buzzer_time = 0;
bool buzzer_state = false;

short getDistance()
{
  digitalWrite(trig_pin, LOW);
  delayMicroseconds(5);
  digitalWrite(trig_pin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig_pin, LOW);

  unsigned long duration = pulseIn(echo_pin, HIGH, 50000);

  return (duration * 0.0343) / 2;
}

void setup()
{
  lcd_display.init();
  lcd_display.backlight();

  pinMode(trig_pin, OUTPUT);
  pinMode(echo_pin, INPUT);
  pinMode(buzzer_pin, OUTPUT);
}

void loop()
{
  unsigned long cur_millis = millis();

  if (cur_millis - last_lcd_time >= 200)
  {
    last_lcd_time = cur_millis;
    cur_distance = getDistance();

    lcd_display.setCursor(0, 0);

    if (cur_distance <= 0)
    {
      lcd_display.print("Error sensor    ");
    }
    else
    {
      lcd_display.print("Distance: ");
      lcd_display.print(cur_distance);
      lcd_display.print(" cm  ");
    }
  }

  if (cur_distance >= 30 || cur_distance <= 0)
  {
    noTone(buzzer_pin);
    buzzer_state = false;
  }
  else if (cur_distance < 10)
  {
    tone(buzzer_pin, 1200);
  }
  else
  {
    short interval = 250;
    short fr = 800;

    if (cur_distance >= 10 && cur_distance < 20)
    {
      interval = 100;
      fr = 400;
    }

    if (cur_millis - last_buzzer_time >= interval)
    {
      last_buzzer_time = cur_millis;
      buzzer_state = !buzzer_state;

      if (buzzer_state)
      {
        tone(buzzer_pin, fr);
      }
      else
      {
        noTone(buzzer_pin);
      }
    }
  }
}
