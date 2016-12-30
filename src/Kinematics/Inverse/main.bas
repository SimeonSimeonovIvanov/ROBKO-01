CLS

REM INPUT:
X0 = 100
Y0 = 100
Z0 = 100
P0 = 0
R1 = 0

REM CONST:
H = 150
L1 = 190
LG = 90

C = 1

IF Z0 < 0 THEN
    Z0 = 0
END IF

IF Z0 < 300 AND X0 < 0 THEN
    X0 = 100
END IF


RR = SQR(X0 * X0 + Y0 * Y0)
LF = 2 * L1 + LG

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

PRINT T1
PRINT T2
PRINT T3
PRINT T4
PRINT T5

