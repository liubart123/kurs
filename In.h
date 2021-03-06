﻿#pragma once
#define IN_MAX_LEN_TEXT 1024*1024
#define IN_CODE_ENDL '\n'
#define IN_CODE_TABLE {\
}
namespace In
{
	struct IN
	{
		enum
		{
			T = 1024,
			F = 2048,
			I = 4096
		};
		int size;
		int lines;
		int ignor;
		unsigned char* text;
		int code[256];
	};
	const int TABLE[] = {
		/*  (0)*/IN::F, /* (1)*/IN::F, /* (2)*/IN::F, /* (3)*/IN::F, /* (4)*/IN::F, /* (5)*/IN::F, /* (6)*/IN::F, /* (7)*/IN::F, 
		/* (8)*/ IN::F, /*  (9)*/IN::T, /*  (10)*/IN::T, /*  (11)*/IN::F, /*  (12)*/IN::F, /*(13)*/IN::F, /* (14)*/IN::F, /* (15)*/IN::F, /*
		/* (16)*/IN::F, /* (17)*/IN::F, /* (18)*/IN::F, /* (19)*/IN::F, /* (20)*/IN::T, /* (21)*/IN::F, /* (22)*/IN::T, /* (23)*/IN::F, 
		/* (24)*/IN::F, /* (25)*/IN::F, /*26*/ IN::F, /* (27)*/IN::T, /* (28)*/IN::F, /* (29)*/IN::F, /* (30)*/IN::F,
		/* (31)*/IN::F, /*  (32)*/IN::T, /*! (33)*/IN::T, /*" (34)*/IN::T, /*# (35)*/IN::F, /*$ (36)*/IN::F, /*% (37)*/IN::T, /*& (38)*/IN::T, 
		/*' (39)*/IN::T, /*( (40)*/IN::T, /*) (41)*/IN::T, /** (42)*/IN::T, /*+ (43)*/IN::T, /*, (44)*/IN::T, /*- (45)*/IN::T,
		/*. (46)*/IN::F, /*/ (47)*/IN::T, /*0 (48)*/IN::T, /*1 (49)*/IN::T, /*2 (50)*/IN::T, /*3 (51)*/IN::T, /*4 (52)*/IN::T, /*5 (53)*/IN::T, 
		/*6 (54)*/IN::T, /*7 (55)*/IN::T, /*8 (56)*/IN::T, /*9 (57)*/IN::T, /*: (58)*/IN::T, /*; (59)*/IN::T, /*< (60)*/IN::T,
		/*= (61)*/IN::T, /*> (62)*/IN::T, /*? (63)*/IN::T, /*@ (64)*/IN::F, /*A (65)*/IN::T, /*B (66)*/IN::T, /*C (67)*/IN::T, /*D (68)*/IN::T, 
		/*E (69)*/IN::T, /*T (70)*/IN::T, /*G (71)*/IN::T, /*H (72)*/IN::T, /*I (73)*/IN::T, /*J (74)*/IN::T, /*K (75)*/IN::T,
		/*L (76)*/IN::T, /*M (77)*/IN::T, /*N (78)*/IN::T, /*O (79)*/IN::T, /*P (80)*/IN::T, /*Q (81)*/IN::T, /*R (82)*/IN::T, /*S (83)*/IN::T, 
		/*T (84)*/IN::T, /*U (85)*/IN::T, /*V (86)*/IN::T, /*W (87)*/IN::T, /*X (88)*/IN::T, /*Y (89)*/IN::T, /*Z (90)*/IN::T,
		/*[ (91)*/IN::T, /*\ (92)*/IN::T, /*] (93)*/IN::T, /*^ (94)*/IN::T, /*_ (95)*/IN::T, /*` (96)*/IN::T, /*a (97)*/IN::T, /*b (98)*/IN::T, 
		/*c (99)*/IN::T, /*d (100)*/IN::T, /*e (101)*/IN::T, /*f (102)*/IN::T, /*g (103)*/IN::T, /*h (104)*/IN::T, /*i (105)*/IN::T,
		/*j (106)*/IN::T, /*k (107)*/IN::T, /*l (108)*/IN::T, /*m (109)*/IN::T, /*n (110)*/IN::T, /*o (111)*/IN::T, /*p (112)*/IN::T, /*q (113)*/IN::T, 
		/*r (114)*/IN::T, /*s (115)*/IN::T, /*t (116)*/IN::T, /*u (117)*/IN::T, /*v (118)*/IN::T, /*w (119)*/IN::T, /*x (120)*/IN::T,
		/*y (121)*/IN::T, /*z (122)*/IN::T, /*{ (123)*/IN::T, /*| (124)*/IN::T, /*} (125)*/IN::T, /*~ (126)*/IN::F, /* (127)*/IN::F, 
		/*? (128)*/IN::F, /*? (129)*/IN::F, /*' (130)*/IN::T, /*? (131)*/IN::F, /*" (132)*/IN::T, /*: (133)*/IN::F, /*┼ (134)*/IN::F, /*╪ (135)*/IN::F,
		/*? (136)*/IN::F, /*% (137)*/IN::T, /*? (138)*/IN::F, /*< (139)*/IN::T, /*? (140)*/IN::F, /*? (141)*/IN::F, /*? (142)*/IN::F, /*? (143)*/IN::F, /*? (144)*/IN::F, /*' (145)*/IN::F, /*' (146)*/IN::F, /*" (147)*/IN::F, /*" (148)*/IN::F, /* (149)*/IN::F, /*- (150)*/IN::F,
		/*- (151)*/IN::F, /*? (152)*/IN::F, /*F (153)*/IN::F, /*? (154)*/IN::F, /*> (155)*/IN::F, /*? (156)*/IN::F, /*? (157)*/IN::F, /*? (158)*/IN::F, /*? (159)*/IN::F, /*  (160)*/IN::F, /*Ў (161)*/IN::F, /*ў (162)*/IN::F, /*? (163)*/IN::F, /*¤ (164)*/IN::F, /*? (165)*/IN::F,
		/*│ (166)*/IN::F, /* (167)*/IN::F, /*Ё (168)*/IN::F, /*c (169)*/IN::F, /*Є (170)*/IN::F, /*< (171)*/IN::F, /*┐ (172)*/IN::F, /*- (173)*/IN::F, /*R (174)*/IN::F, /*Ї (175)*/IN::F, /*° (176)*/IN::F, /*+ (177)*/IN::F, /*? (178)*/IN::F, /*? (179)*/IN::F, /*? (180)*/IN::F,
		/*ч (181)*/IN::F, /* (182)*/IN::F, /*· (183)*/IN::F, /*ё (184)*/IN::F, /*№ (185)*/IN::F, /*є (186)*/IN::F, /*> (187)*/IN::F, /*? (188)*/IN::F, /*? (189)*/IN::F, /*? (190)*/IN::F, /*ї (191)*/IN::F, /*А (192)*/IN::F, /*Б (193)*/IN::F, /*В (194)*/IN::F, /*Г (195)*/IN::F,
		/*Д (196)*/IN::F, /*Е (197)*/IN::F, /*Ж (198)*/IN::F, /*З (199)*/IN::F, /*И (200)*/IN::F, /*Й (201)*/IN::F, /*К (202)*/IN::F, /*Л (203)*/IN::F, /*М (204)*/IN::F, /*Н (205)*/IN::F, /*О (206)*/IN::F, /*П (207)*/IN::F, /*Р (208)*/IN::F, /*С (209)*/IN::F, /*Т (210)*/IN::F,
		/*У (211)*/IN::F, /*Ф (212)*/IN::F, /*Х (213)*/IN::F, /*Ц (214)*/IN::F, /*Ч (215)*/IN::F, /*Ш (216)*/IN::F, /*Щ (217)*/IN::F, /*Ъ (218)*/IN::F, /*Ы (219)*/IN::F, /*Ь (220)*/IN::F, /*Э (221)*/IN::F, /*Ю (222)*/IN::F, /*Я (223)*/IN::F, /*а (224)*/IN::F, /*б (225)*/IN::F,
		/*в (226)*/IN::F, /*г (227)*/IN::F, /*д (228)*/IN::F, /*е (229)*/IN::F, /*ж (230)*/IN::F, /*з (231)*/IN::F, /*и (232)*/IN::F, /*й (233)*/IN::F, /*к (234)*/IN::F, /*л (235)*/IN::F, /*м (236)*/IN::F, /*н (237)*/IN::F, /*о (238)*/IN::F, /*п (239)*/IN::F, /*р (240)*/IN::F,
		/*с (241)*/IN::F, /*т (242)*/IN::F, /*у (243)*/IN::F, /*ф (244)*/IN::F, /*х (245)*/IN::F, /*ц (246)*/IN::F, /*ч (247)*/IN::F, /*ш (248)*/IN::F, /*щ (249)*/IN::F, /*ъ (250)*/IN::F, /*ы (251)*/IN::F, /*ь (252)*/IN::F, /*э (253)*/IN::F, /*ю (254)*/IN::F, /*я (255)*/IN::F,

	};
	IN getin(wchar_t infile[]);
}