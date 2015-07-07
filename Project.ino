#include<Servo.h>
#define servo_pin 9
#define pin1 2
#define pin2 3
#define pump 4
#define ut1 5
#define ut2 6
Servo myservo; //using servo on any pin blocks the pwm function on pins 9 and 10 so i chose pin 9 to connect the servo
int servo_pos = 90,flag1 = 0 , flag2 = 0;

void setup() 
{
  
  // put your setup code here, to run once:
  // Initialize one 2 pins for motor driver as output.
  // lets say pin 1 == 12 & pin 2 == 11.
  // forward condition -- 1 - HIGH & 2 - LOW.
  // Backward condition -- 2 - HIGH & 1 - LOW.
  pinMode( pin1 , OUTPUT ); // Output for Motor Propelor.
  pinMode( pin2 , OUTPUT ); // Output for Motor Propelor.
  pinMode( ut1 , OUTPUT); // Output for Ultrasonic sensor.
  pinMode( ut2 , INPUT);  // Input for Ultrasonic sensor.
  pinMode( pump , OUTPUT );
  servo_init(); // Initializing Servo.
  Serial.begin(9600);
}

int servo_init()
{
  myservo.attach(servo_pin); //attach servo on pwm pin 9
  myservo.write(servo_pos);//set initial position of servo at 90 degrees (so that it stays in between and then can turned both ways)
}

void turn(char t)
{
  if( t == 'd' && flag2 == 1 )//d as in right to turn right => turn(d);
  {
    servo_pos += 5; //increasing angle by 5 deg. i.e. towards the 180 degree side
    Serial.println("Turning Right ");
  }
  else if( t == 'a' && flag2 == 1 )//a as in left to turn left => turn(a);
  {
    servo_pos -= 5; //decrease angle by 5 deg. i.e. towards the 0 degree side
    Serial.println("Turning Left ");
  }
  myservo.write(servo_pos); //set servo final position
  delay(500);//delay for 0.5 secs so that user doesn't turn fully very quickly
  //please suggest a way to get this delay out if possible as it will make the robo control while turning a bit slow
  if( flag2 == 0 )
  {
    Serial.println( "Can't turn RUV as it is not in motion. ");
  }
}

void updown( char up ) // Check for reverse mode.
{
  // take max depth.
  // apply some relation between pump on duration and volume of water filled. FOR NOW TAKE IT AS FOR 1SEC PUMP ON WE FILL 100ML.
  digitalWrite( pin1 , LOW );
  digitalWrite( pin2 , LOW );
  flag2 = 0;
  Serial.println( "Stopping propeller motion" );
  if( up == ' ' )
  {
    digitalWrite( pump , HIGH );
    Serial.println( "Filling the MBT " );
    flag1 = 1;
  }
  if( up == 'b' && flag1 == 0 )
  {
    Serial.println( "Pump is already in rest state ");
  }
  if( up == 'b' && flag1 == 1 )  
  {
    digitalWrite( pump , LOW );
    Serial.println( "Stopping Pump " );
    flag1 = 0;
  }
}

long maxdepth()
{
  long duration, distance;
  digitalWrite( ut1 , LOW);  // Added this line
  delay(2); // Added this line
  digitalWrite( ut1 , HIGH);
  //  delayMicroseconds(1000); - Removed this line
  delay(10); // Added this line
  digitalWrite( ut1 , LOW);
  duration = pulseIn( ut2 , HIGH);
  distance = (duration/2) / 29.1;
  Serial.print("Maximum Depth for rover is --> ");
  Serial.println(distance);
  if ( distance <= 10 )
  {
    Serial.println(" Too Close to bottom");
  }
}

void motion( char m ) 
{
  // put your main code here, to run repeatedly:
  char a = 'w';
  if( m == 'w' )
  {
    // Loop for forward Motion.
    digitalWrite( pin1 , HIGH );
    digitalWrite( pin2 , LOW );
    Serial.println("Forward Motion ");
    flag2 = 1;
  }
  if( m == 's' )
  {
    // Loop for Backward motion.
    m = Serial.read();
    digitalWrite( pin1 , LOW );
    digitalWrite( pin2 , HIGH );
    Serial.println("Backward Motion ");
    flag2 = 1;
  }
}

void closeall()
{
  digitalWrite( pin1 , LOW );
  digitalWrite( pin2 , LOW );
  digitalWrite( ut1 , LOW );
  digitalWrite( ut2 , LOW );
  digitalWrite( pump , LOW );
  flag1 = 0;
  flag2 = 0;
}

void loop()
{
  int ch;
  ch = Serial.read();
  delay(500);
  switch(ch)
  { 
   case ' ' :    updown(' ');
                 break;

   case 'b' :    updown('b');
                 break;
                 
   case 'w' :    motion('w');
                 break;
   
   case 's' :    motion('s');
                 break;
   
   case 'a' :    turn('a');
                 break;
   
   case 'd' :    turn('d');
                 break;

   case 'k' :    maxdepth();
                 break;
                 
   case 'x' :    closeall();
                 myservo.write(90);
                 Serial.println( " Clossing All Output " ); 
  }
}
