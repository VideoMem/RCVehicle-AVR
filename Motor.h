#define VFD_MOTOR_LOOPS 4
#define VFD_3PH_STATES 6

#define UPH 2
#define VPH 3
#define WPH 4

class Motor {
    public:
            static Motor* pointer;
            volatile unsigned long Position[VFD_MOTOR_LOOPS];
            volatile char Dir[VFD_MOTOR_LOOPS];

            Motor() {
                Motor::pointer = this;
                for(int i = 0; i < VFD_MOTOR_LOOPS; ++i) {
                    Motor::Position[i] = 0;
                    Motor::Dir[i] = 1;
                }
            }

            static void advance(unsigned char i) {
                Motor::pointer->Position[i]+=Motor::pointer->Dir[i];
                if(Motor::pointer->Position[i] > 0xFFFFFFFB && Motor::pointer->Dir[i] > 0)
                    Motor::pointer->Position[i] = 0;
                if(Motor::pointer->Position[i] > 0xFFFFFFFB && Motor::pointer->Dir[i] < 0)
                    Motor::pointer->Position[i] = 0xFFFFFFFB;

                Motor::output(0);
            }

            static void output(unsigned char i) {
                unsigned char seq = Motor::pointer->Position[i] % VFD_3PH_STATES;
                switch(seq) {
                    case 0:
                        digitalWrite(UPH, HIGH); digitalWrite(VPH, LOW);  digitalWrite(WPH, HIGH);
                    break;
                    case 1:
                        digitalWrite(UPH, HIGH); digitalWrite(VPH, LOW);  digitalWrite(WPH, LOW);
                    break;
                    case 2:
                        digitalWrite(UPH, HIGH); digitalWrite(VPH, HIGH); digitalWrite(WPH, LOW);
                    break;
                    case 3:
                        digitalWrite(UPH, LOW);  digitalWrite(VPH, HIGH); digitalWrite(WPH, LOW);
                    break;
                    case 4:
                        digitalWrite(UPH, LOW);  digitalWrite(VPH, HIGH); digitalWrite(WPH, HIGH);
                    break;
                    case 5:
                        digitalWrite(UPH, LOW);  digitalWrite(VPH, LOW);  digitalWrite(WPH, HIGH);
                    break;
                    default:
                        digitalWrite(UPH, LOW);
                        digitalWrite(VPH, LOW);
                        digitalWrite(WPH, LOW);
                    break;
                }
            }
};
