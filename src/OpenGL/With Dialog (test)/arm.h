typedef struct {
	bool bOn, busi;
	float spAngle, Angle, kSpeed;
} SERVO, *LP_SERVO;

void armInitServo(void);
void cmd01(LP_SERVO lpServo, int n);
void servoOn(LP_SERVO lpServo, bool isOn);
void servoSetAngle(LP_SERVO lpServo, float angle);

void DrawUnitCylinder(int numSegs);
static void DrawUnitSphere(int numSegs);
static void DrawUnitCone(int numSegs);
static void DrawGroundPlane(int numSegs);
static void DrawJoint(int numSegs);
static void DrawBase(int numSegs);
static void DrawArmSegment(int numSegs);
static void DrawWrist(int numSegs);
static void DrawFingerBase(int numSegs);
static void DrawFingerTip(int numSegs);
static void DrawRobotArm(int numSegs);
void myDisplay();
void myReshape(int w, int h);
void myIdle();
void MotionFunc(int x, int y);
void KeyboardFunc(unsigned char key, int x, int y);
void MouseFunc(int button, int state, int x, int y);

void computePos(float deltaMove);
void computeDir(float deltaAngle);
void pressKey(int key, int xx, int yy);
void releaseKey(int key, int x, int y);
