#define SCALE	2.5f

/* Arm dimensions( mm ) */
#define BASE_HGT		190		//base hight
#define HUMERUS			178		//shoulder-to-elbow "bone"
#define ULNA			178		//elbow-to-wrist "bone"
#define GRIPPER			92		//gripper (incl.heavy duty wrist rotate mechanism) length

#define GL_BASE_HGT		( SCALE * BASE_HGT	/ 1000.0f )
#define GL_HUMERUS		( SCALE * HUMERUS	/ 1000.0f )
#define GL_ULNA			( SCALE * ULNA		/ 1000.0f )
#define GL_GRIPPER		( SCALE * GRIPPER	/ 1000.0f )

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
