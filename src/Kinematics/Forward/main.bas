CLS

REM INPUT (RAD):
T1 = .7853982
T2 = -.6095284
T3 = 2.152298
T4 = 51.42136
T5 = -51.42136

REM CONST:
H = 150
L1 = 190
L2 = 170
LG = 90

C = 1

PP = (T4 + T5) / 2
RR = (T4 - T5) / 2 - R1 * T1

RP = L1 * COS(T2) + L2 * COS(T3) + LG * COS(PP)
XX = RP * COS(T1)
YY = RP * SIN(T1)
ZZ = H - L1 * SIN(T2) - L2 * SIN(T3) - LG * SIN(PP)

PP = PP * C
RR = RR * C

PRINT "XX = ", XX
PRINT "YY = ", YY
PRINT "XX = ", ZZ
PRINT "PP = ", PP
PRINT "RR = ", RR

