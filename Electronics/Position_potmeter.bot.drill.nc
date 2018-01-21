(.../EAGLE-7.7.0/ulp/pcb-gcode.ulp)
(Copyright 2005 - 2012 by John Johnson)
(See readme.txt for licensing terms.)
(This file generated from the board:)
(.../Electronics/Position_potmeter.brd)
(Current profile is .../EAGLE-7.1.0/ulp/profiles/mach.pp  )
(This file generated 5-12-2017 16:45)
(Settings from pcb-machine.h)
(spindle on time = 1.0000)
(spindle speed = 20000.0000)
(tool change at 0.0000  0.0000  25.4000 )
(feed rate xy = F0     )
(feed rate z  = F508   )
(Z Axis Settings)
(  High     Up        Down     Drill)
(40.0000 	2.5400  	-0.0750 	-1.8000 )
(Settings from pcb-defaults.h)
(isolate min = 0.0254)
(isolate max = 0.5080)
(isolate step = 0.1270)
(Generated top outlines, top drill, bottom outlines, bottom drill, )
(Unit of measure: mm)
( Tool|       Size       |  Min Sub |  Max Sub |   Count )
( T01 | 1.016mm 0.0400in | 0.0000in | 0.0000in |       0 )
( T02 | 1.100mm 0.0433in | 0.0000in | 0.0000in |       0 )
( T03 | 3.000mm 0.1181in | 0.0000in | 0.0000in |       0 )
( T04 | 5.800mm 0.2283in | 0.0000in | 0.0000in |       0 )
(Metric Mode)
G21
(Absolute Coordinates)
G90
S20000
G00 Z40.0000 
G00 X0.0000  Y0.0000  
M03
G04 P1.000000
M05
G00 Z25.4000 
M06 T01  ; 1.0160  
G00 Z2.5400  
M03
G04 P1.000000
G82 X-9.4600 Y2.0000  Z-1.8000 F508   R2.5400  P0.500000
G82 X-12.0000 Y2.0000  
G82 X-14.5400 Y2.0000  
M05
G00 Z25.4000 
G00 X0.0000  Y0.0000  
M06 T02  ; 1.1000  
G00 Z2.5400  
M03
G04 P1.000000
G82 X-7.7500 Y11.5000 Z-1.8000 F508   R2.5400  P0.500000
G82 X-16.2500 Y11.5000 
M05
G00 Z25.4000 
G00 X0.0000  Y0.0000  
M06 T03  ; 3.0000  
G00 Z2.5400  
M03
G04 P1.000000
G82 X-4.5000 Y7.0000  Z-1.8000 F508   R2.5400  P0.500000
G82 X-12.0000 Y17.0000 
G82 X-12.0000 Y27.0000 
G82 X-19.5000 Y7.0000  
M05
G00 Z25.4000 
G00 X0.0000  Y0.0000  
M06 T04  ; 5.8000  
G00 Z2.5400  
M03
G04 P1.000000
G82 X-12.0000 Y17.0000 Z-1.8000 F508   R2.5400  P0.500000
T01 
G00 Z40.0000 
M05
M02
