CLS

CONST PI = 3.14159265#
CONST Rad = (180 / PI)

CONST H = 190
CONST L1 = 178
CONST L2 = 178
CONST LG = 92
CONST C = 1

REM Motor Consts [ Steps / Rad ]:
CONST T1const = 1222.7
CONST T2const = 1161.4
CONST T3const = 680.1
CONST T4const = 244.7
CONST T5const = 244.7
CONST T6const = 27

REM Input R1:
REM 1 or 0
CONST R1 = 0

PRINT "----------------------- Inverse Kinematics ---------------------------"
REM Input:
X0 = 300
Y0 = 1
Z0 = 1
P0 = 1
R0 = 1

PRINT ""
PRINT "X0 = ", X0, "mm."
PRINT "Y0 = ", Y0, "mm."
PRINT "Z0 = ", Z0, "mm."
PRINT "P0 = ", P0, "deg."
PRINT "R0 = ", R0, "deg."
PRINT ""

IF Z0 < 0 THEN
    Z0 = 0
END IF

IF Z0 < 300 AND X0 < 0 THEN
    X0 = 100
END IF

RR = SQR(X0 * X0 + Y0 * Y0)
LF = L1 + L2 + LG

IF Z0 = H THEN
    RM = LF
ELSE
    IF Z0 = 0 THEN
        RM = SQR(LF * LF - H * H)
    ELSE
        RM = SQR(LF * LF - (H - Z0) * (H - Z0))
    END IF
END IF

IF RR > RM THEN
    RR = RM
END IF

P0 = P0 / C
R0 = R0 / C

R0 = RR - LG * COS(P0)
Z0 = H - Z0 - LG * SIN(P0)

IF R0 = 0 THEN
    GA = SGN(Z0) * PI / 2
ELSE
    GA = ATN(Z0 / R0)
END IF

AL = SQR(R0 * R0 + Z0 * Z0) / 2
AL = ATN(SQR(L1 * L1 - AL * AL) / AL)

IF X0 = 0 THEN
    T1 = SGN(Y0) * PI / 2
ELSE
    T1 = ATN(Y0 / X0)
END IF

T2 = GA - AL
T3 = GA + AL
T4 = P0 + R0 + R1 * T1
T5 = P0 - R0 - R1 * T1

T1deg = T1 * Rad
T2deg = T2 * Rad
T3deg = T3 * Rad
T4deg = T4 * Rad
T5deg = T5 * Rad

T1step = T1 * T1const
T2step = T2 * T2const
T3step = T3 * T3const
T4step = T4 * T4const
T5step = T5 * T5const

PRINT "T1 = "; T1, "rad."; " = "; T1deg; "deg."; " = "; T1step; "step"
PRINT "T2 = "; T2, "rad."; " = "; -T2deg; "deg."; " = "; -T2step; "step"
PRINT "T3 = "; T3, "rad."; " = "; -T3deg; "deg."; " = "; -T3step; "step"
PRINT "T4 = "; T4, "rad."; " = "; T4deg; "deg."; " = "; T4step; "step"
PRINT "T5 = "; T5, "rad."; " = "; T5deg; "deg."; " = "; T5step; "step"
PRINT ""

PRINT "----------------------- Forward Kinematics ---------------------------"
REM Input: T1, T2, T3, T4, T5 [rad.]

PP = (T5 + T4) / 2
RP = (T5 - T4) / 2 - R1 * T1

RR = L1 * COS(T2) + L2 * COS(T3) + LG * COS(PP)

XX = RR * COS(T1)
YY = RR * SIN(T1)
ZZ = H - L1 * SIN(T2) - L2 * SIN(T3) - LG * SIN(PP)

PP = PP * C
RP = RP * C

PRINT ""
PRINT "XX = ", XX, "mm."
PRINT "YY = ", YY, "mm."
PRINT "ZZ = ", ZZ, "mm."
PRINT "PP = ", PP, "deg."
PRINT "RP = ", RP, "deg."

