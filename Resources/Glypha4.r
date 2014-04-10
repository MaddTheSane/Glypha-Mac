#include <Carbon/Carbon.r>
#include "G4enums.h"

resource 'DLOG' (131, "About") {
	{79, 107, 285, 526},
	dBoxProc,
	invisible,
	noGoAway,
	0x2,
	131,
	"AboutWndo",
	24932
	/****** Extra bytes follow... ******/
	/* $"696F 0000 0000 0000 0000 0000 0000 0000"            /* io.............. */
	/* $"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	/* $"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	/* $"0059 002B 0067 008C 0002 0002 0100 0002"            /* .Y.+.g.å........ */
	/* $"5772 0000 0000 0002 571A 0000 0003 0000"            /* Wr......W....... */
	/* $"0001 0064 0001 000A 0000 0000 0000 0000"            /* ...d...¬........ */
	/* $"0000 0000 0000 0000 0100 0000 0000 0000"            /* ................ */
	/* $"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	/* $"0000 0000 0000 0000 0000 003C 0000 414A"            /* ...........<..AJ */
	/* $"000A 8001 8001 7FFF 7FFF 0000 002A 008D"            /* .¬Ä.Ä..ˇ.ˇ...*.ç */
	/* $"FF00 0000 0000 0001 0202 5980 0000 0000"            /* ˇ.........YÄ.... */
	/* $"0000 0000 0000 162E 0A4B 6579 626F 6172"            /* ........¬Keyboar */
	/* $"6420 3200 0000 01CC 0000 4530 0002 0003"            /* d 2....Ã..E0.... */
	/* $"0449 636F 6E65 4E56 00A1 010A 00A1 0149"            /* .IconeNV.°.¬.°.I */
	/* $"00A1 014B 00A3 0107 00A3 0108 00A3 014B"            /* .°.K.£...£...£.K */
	/* $"00A3 014C"                                          /* .£.L */
};

resource 'DLOG' (133, "Configure Game") {
	{120, 144, 228, 394},
	dBoxProc,
	visible,
	noGoAway,
	0x3,
	133,
	"ConfigureGameWndo",
	centerMainScreen
};

data 'DLOG' (130, "High Score Name") {
	$"0078 00BE 00C5 01CA 0001 0000 0000 0000"            /* .x.æ.≈. ........ */
	$"03E7 0082 0B48 6953 636F 7265 4E61 6D65"            /* .Á.Ç.HiScoreName */
};

resource 'ICON' (997) {
	$"000F F000 0070 0E00 0183 C180 030F F0C0"
	$"040E 7020 081C 3810 1018 1808 3018 180C"
	$"2018 1804 401C 3802 400C 3002 400E 7002"
	$"8007 E001 8003 C001 8001 8001 80FF FF01"
	$"8001 8001 8001 8001 8001 8001 8001 8001"
	$"4001 8002 4001 8002 4001 8002 2001 8004"
	$"3001 800C 1001 8008 0801 8010 0401 8020"
	$"0301 80C0 0181 8180 0070 0E00 000F F0"
};

resource 'ICON' (137) {
	$"0001 8000 0003 C000 0003 C000 0006 6000"
	$"0006 6000 000C 3000 000C 3000 0018 1800"
	$"0019 9800 0033 CC00 0033 CC00 0063 C600"
	$"0063 C600 00C3 C300 00C3 C300 0183 C180"
	$"0183 C180 0303 C0C0 0303 C0C0 0603 C060"
	$"0601 8060 0C01 8030 0C00 0030 1800 0018"
	$"1801 8018 3003 C00C 3003 C00C 6001 8006"
	$"6000 0006 C000 0003 FFFF FFFF 7FFF FFFE"
};

resource 'DITL' (133, "Configure Game") {
	{	/* array DITLarray: 6 elements */
		/* [1] */
		{80, 184, 100, 242},
		Button {
			enabled,
			"Okay"
		},
		/* [2] */
		{42, 8, 62, 96},
		Button {
			enabled,
			"Clear Scores"
		},
		/* [3] */
		{40, 207, 57, 227},
		EditText {
			enabled,
			""
		},
		/* [4] */
		{40, 130, 56, 200},
		StaticText {
			disabled,
			"Sound Vol:"
		},
		/* [5] */
		{55, 164, 71, 200},
		StaticText {
			disabled,
			"(0-7)"
		},
		/* [6] */
		{8, 8, 24, 127},
		StaticText {
			enabled,
			"Configure Game…"
		}
	}
};

resource 'DITL' (130, "High Score Name") {
	{	/* array DITLarray: 4 elements */
		/* [1] */
		{49, 202, 69, 260},
		Button {
			enabled,
			"Okay"
		},
		/* [2] */
		{51, 11, 67, 149},
		EditText {
			enabled,
			""
		},
		/* [3] */
		{8, 8, 40, 224},
		StaticText {
			enabled,
			"Your score #^0 of the ten best!  Enter y"
			"our name (15 chars.)."
		},
		/* [4] */
		{8, 228, 40, 260},
		Icon {
			disabled,
			997
		}
	}
};

resource 'DITL' (128, "Red Alert") {
	{	/* array DITLarray: 4 elements */
		/* [1] */
		{80, 223, 100, 281},
		Button {
			enabled,
			"Okay"
		},
		/* [2] */
		{8, 8, 72, 241},
		StaticText {
			disabled,
			"^0"
		},
		/* [3] */
		{76, 8, 92, 211},
		StaticText {
			disabled,
			"^1"
		},
		/* [4] */
		{8, 249, 40, 281},
		Icon {
			disabled,
			998
		}
	}
};

resource 'WIND' (128, "mainWindow") {
	{20, 0, 500, 640},
	noGrowDocProc,
	invisible,
	noGoAway,
	0x1,
	"Glypha IV",
	centerMainScreen
};

resource 'WIND' (129, "aboutWindow") {
	{0, 0, 318, 325},
	plainDBox,
	invisible,
	noGoAway,
	0x0,
	"New Window",
	centerMainScreen
};

resource 'MENU' (128, "\0x14") {
	128,
	textMenuProc,
	0x7FFFFFFB,
	enabled,
	apple,
	{	/* array: 3 elements */
		/* [1] */
		"About Glypha IV…", noIcon, noKey, noMark, plain,
		/* [2] */
		"Settings…", noIcon, ",", noMark, plain,
		/* [3] */
		"-", noIcon, noKey, noMark, plain
	}
};

resource 'MENU' (129, "Game") {
	129,
	textMenuProc,
	0x7FFFFFF1,
	enabled,
	"Game",
	{	/* array: 5 elements */
		/* [1] */
		"New Game", noIcon, "N", noMark, plain,
		/* [2] */
		"Pause Game", noIcon, "P", noMark, plain,
		/* [3] */
		"End Game", noIcon, "E", noMark, plain,
		/* [4] */
		"-", noIcon, noKey, noMark, plain,
		/* [5] */
		"Quit", noIcon, "Q", noMark, plain
	}
};

resource 'MENU' (130, "Options") {
	130,
	textMenuProc,
	0x7FFFFFFB,
	enabled,
	"Options",
	{	/* array: 4 elements */
		/* [1] */
		"Help", noIcon, noKey, noMark, plain,
		/* [2] */
		"High Scores", noIcon, "H", noMark, plain,
		/* [3] */
		"-", noIcon, noKey, noMark, plain,
		/* [4] */
		"Toggle Full–screen", noIcon, "F", noMark, plain
	}
};

read 'PICT' (kAboutPictID) "132.pict";
read 'PICT' (kBackgroundPictID) "130.pict";
read 'PICT' (kPlayerPictID) "135.pict";
read 'PICT' (kPlayerMaskID) "136.pict";
read 'PICT' (kNumberPictID) "137.pict";
read 'PICT' (kIdlePictID) "138.pict";
read 'PICT' (kEnemyWalkPictID) "139.pict";
read 'PICT' (kEnemyWalkMaskID) "141.pict";
read 'PICT' (kEnemyFlyPictID) "140.pict";
read 'PICT' (kEnemyFlyMaskID) "142.pict";
read 'PICT' (kFlamePictID) "143.pict";
read 'PICT' (kEggPictID) "144.pict";
read 'PICT' (kEggMaskID) "145.pict";
read 'PICT' (kPlatformPictID) "146.pict";
read 'PICT' (kHelpPictID) "131.pict";
read 'PICT' (kHandPictID) "128.pict";
read 'PICT' (kHandMaskID) "129.pict";
read 'PICT' (kEyePictID) "147.pict";
read 'PICT' (kEyeMaskID) "148.pict";
read 'PICT' (kObeliskPictID) "134.pict";
read 'PICT' (kObeliskMaskPictID) "149.pict";
read 'PICT' (kBannerPictID) "150.pict";

read 'snd ' (kBaseBufferSoundID + 0, "Ahnk", purgeable) "Ahnk.snd";
read 'snd ' (kBaseBufferSoundID + kBirdSound, "Bird", purgeable) "Bird.snd";
read 'snd ' (kBaseBufferSoundID + kBonusSound, "Bonus", purgeable) "Bonus.snd";
read 'snd ' (kBaseBufferSoundID + kBoom1Sound, "Boom 1", purgeable) "Boom 1.snd";
read 'snd ' (kBaseBufferSoundID + kBoom2Sound, "Boom 2", purgeable) "Boom 2.snd";
read 'snd ' (kBaseBufferSoundID + kSplashSound, "Drip", purgeable) "Drip.snd";
read 'snd ' (kBaseBufferSoundID + kFlapSound, "Flap", purgeable) "Flap.snd";
read 'snd ' (kBaseBufferSoundID + kFlipSound, "Flip", purgeable) "Flip.snd";
read 'snd ' (kBaseBufferSoundID + kGrateSound, "Grating", purgeable) "Grating.snd";
read 'snd ' (kBaseBufferSoundID + kLightningSound, "Lightning", purgeable) "Lightning.snd";
read 'snd ' (kBaseBufferSoundID + kMusicSound, "Music", purgeable) "Music.snd";
read 'snd ' (kBaseBufferSoundID + kRezSound, "Rez", purgeable) "Rez.snd";
read 'snd ' (kBaseBufferSoundID + kScreechSound, "Screech", purgeable) "Screech.snd";
read 'snd ' (kBaseBufferSoundID + kSpawnSound, "Spawn", purgeable) "Spawn.snd";
read 'snd ' (kBaseBufferSoundID + kWalkSound, "Walk", purgeable) "Walk.snd";
read 'snd ' (kBaseBufferSoundID + kFlap2Sound, "Flap 2", purgeable) "Flap 2.snd";
read 'snd ' (kBaseBufferSoundID + kScrape2Sound, "Scrape 2", purgeable) "Scrape 2.snd";

resource 'ICN#' (128) {
	{	/* array: 2 elements */
		/* [1] */
		$"000C 3000 0070 0E00 01E0 0780 03C3 C3C0"
		$"07C7 E3E0 0F87 E1F0 1F8F F1F8 3F8F F1FC"
		$"3F8F F1FC 7F8F F1FE 7F87 E1FE 7FC7 E3FE"
		$"FFC3 C3FF FFE1 87FF FFE0 07FF FCF0 0F3F"
		$"FC00 003F FC00 003F FC00 003F FCFC 3F3F"
		$"7FFC 3FFE 7FFC 3FFE 7FFC 3FFE 3FFC 3FFC"
		$"3FFC 3FFC 1FFC 3FF8 0FFC 3FF0 07FC 3FE0"
		$"03F8 1FC0 01F0 0F80 0070 0E",
		/* [2] */
		$"000F F000 007F FE00 01FF FF80 03FF FFC0"
		$"07FF FFE0 0FFF FFF0 1FFF FFF8 3FFF FFFC"
		$"3FFF FFFC 7FFF FFFE 7FFF FFFE 7FFF FFFE"
		$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"
		$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"
		$"7FFF FFFE 7FFF FFFE 7FFF FFFE 3FFF FFFC"
		$"3FFF FFFC 1FFF FFF8 0FFF FFF0 07FF FFE0"
		$"03FF FFC0 01FF FF80 007F FE00 000F F0"
	}
};

resource 'ICN#' (129) {
	{	/* array: 2 elements */
		/* [1] */
		$"1FF8 7000 1FE0 1800 1FC0 1400 1F87 9200"
		$"1F8F D100 1F0F D080 1F1F F040 1F1F F020"
		$"1F1F FFF0 1F1F E3F0 1F0F C3F0 1F8F C7F0"
		$"1F87 87F0 1FC3 0FF0 1FC0 0FF0 19E0 1E70"
		$"1800 0070 1800 0070 1800 0070 19F8 7E70"
		$"1FF8 7FF0 1FF8 7FF0 1FF8 7FF0 1FF8 7FF0"
		$"10F8 7FF0 1278 7FF0 1278 7FF0 1278 7FF0"
		$"10F0 3FF0 13E0 1FF0 13E0 1FF0 1FE0 1FF0",
		/* [2] */
		$"1FFF F000 1FFF F800 1FFF FC00 1FFF FE00"
		$"1FFF FF00 1FFF FF80 1FFF FFC0 1FFF FFE0"
		$"1FFF FFF0 1FFF FFF0 1FFF FFF0 1FFF FFF0"
		$"1FFF FFF0 1FFF FFF0 1FFF FFF0 1FFF FFF0"
		$"1FFF FFF0 1FFF FFF0 1FFF FFF0 1FFF FFF0"
		$"1FFF FFF0 1FFF FFF0 1FFF FFF0 1FFF FFF0"
		$"1FFF FFF0 1FFF FFF0 1FFF FFF0 1FFF FFF0"
		$"1FFF FFF0 1FFF FFF0 1FFF FFF0 1FFF FFF0"
	}
};

resource 'vers' (1) {
	0x1,
	0x3,
	release,
	0x0,
	0,
	"Glypha III 1.0.3",
	"Glypha III 1.0.3\nScheherazade 1995-96"
};

resource 'vers' (2) {
	0x1,
	0x3,
	release,
	0x0,
	0,
	"Glypha III 1.0.3",
	"Glypha III 1.0.3"
};

data 'cicn' (997) {
	$"0000 0000 8010 0000 0000 0020 0020 0000"            /* ....Ä...... . .. */
	$"0000 0000 0000 0048 0000 0048 0000 0000"            /* .......H...H.... */
	$"0004 0001 0004 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0004 0000 0000 0020 0020"            /* ............. .  */
	$"0000 0000 0004 0000 0000 0020 0020 0000"            /* ........... . .. */
	$"0000 000F F000 007F FE00 01FF FF80 03FF"            /* .......˛..ˇˇÄ.ˇ */
	$"FFC0 07FF FFE0 0FFF FFF0 1FFF FFF8 3FFF"            /* ˇ¿.ˇˇ‡.ˇˇ.ˇˇ¯?ˇ */
	$"FFFC 3FFF FFFC 7FFF FFFE 7FFF FFFE 7FFF"            /* ˇ¸?ˇˇ¸.ˇˇ˛.ˇˇ˛.ˇ */
	$"FFFE FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ˇ˛ˇˇˇˇˇˇˇˇˇˇˇˇˇˇ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇ */
	$"FFFF 7FFF FFFE 7FFF FFFE 7FFF FFFE 3FFF"            /* ˇˇ.ˇˇ˛.ˇˇ˛.ˇˇ˛?ˇ */
	$"FFFC 3FFF FFFC 1FFF FFF8 0FFF FFF0 07FF"            /* ˇ¸?ˇˇ¸.ˇˇ¯.ˇˇ.ˇ */
	$"FFE0 03FF FFC0 01FF FF80 007F FE00 000F"            /* ˇ‡.ˇˇ¿.ˇˇÄ..˛... */
	$"F000 0000 0000 000F F000 007C 3E00 00F0"            /* .........|>.. */
	$"0F00 03F1 8FC0 07E3 C7E0 0FE7 E7F0 0FE7"            /* ...Òè¿.„«‡.ÁÁ.Á */
	$"E7F0 1FE7 E7F8 3FE3 C7FC 3FF3 CFFC 3FF1"            /* Á.ÁÁ¯?„«¸?Ûœ¸?Ò */
	$"8FFC 7FF8 1FFE 7FFC 3FFE 7FFE 7FFE 7F00"            /* è¸.¯.˛.¸?˛.˛.˛.. */
	$"00FE 7FFE 7FFE 7FFE 7FFE 7FFE 7FFE 7FFE"            /* .˛.˛.˛.˛.˛.˛.˛.˛ */
	$"7FFE 3FFE 7FFC 3FFE 7FFC 3FFE 7FFC 1FFE"            /* .˛?˛.¸?˛.¸?˛.¸.˛ */
	$"7FF8 0FFE 7FF0 0FFE 7FF0 07FE 7FE0 03FE"            /* .¯.˛..˛..˛.‡.˛ */
	$"7FC0 00FE 7F00 007E 7E00 000F F000 0000"            /* .¿.˛...~~...... */
	$"0000 0000 0000 0000 000C 0000 FFFF FFFF"            /* ............ˇˇˇˇ */
	$"FFFF 0001 FFFF FFFF CCCC 0002 CCCC CCCC"            /* ˇˇ..ˇˇˇˇÃÃ..ÃÃÃÃ */
	$"9999 0003 9999 9999 6666 0004 0000 0000"            /* ôô..ôôôôff...... */
	$"6666 0005 0000 0000 3333 0006 0000 0000"            /* ff......33...... */
	$"8888 0007 0000 0000 7777 0008 0000 0000"            /* àà......ww...... */
	$"5555 0009 0000 0000 4444 000A 0000 0000"            /* UU.∆....DD.¬.... */
	$"2222 000B 0000 0000 1111 000F 0000 0000"            /* "".............. */
	$"0000 0000 0000 0000 6612 2266 0000 0000"            /* ........f."f.... */
	$"0000 0000 0000 0666 1222 2223 6660 0000"            /* .......f.""#f`.. */
	$"0000 0000 0006 6661 2233 3322 3666 6000"            /* ......fa"33"6f`. */
	$"0000 0000 0066 6612 23BB BB32 2366 6600"            /* .....ff.#ªª2#ff. */
	$"0000 0000 0777 7712 3BBB BBB2 23F7 7770"            /* .....ww.;ªª≤#˜wp */
	$"0000 0000 7777 7122 3BBB 7772 223B 7777"            /* ....wwq";ªwr";ww */
	$"0000 0007 7777 7123 BBB7 7777 123B 7777"            /* ....wwq#ª∑ww.;ww */
	$"7000 0077 7777 7123 BBB7 7777 123B B777"            /* p..wwwq#ª∑ww.;∑w */
	$"7700 0044 4444 4123 BB44 4444 123B B444"            /* w..DDDA#ªDDD.;¥D */
	$"4400 0444 4444 4123 BB44 4444 123B B444"            /* D..DDDA#ªDDD.;¥D */
	$"4440 0444 4444 4122 3B44 4441 223B B444"            /* D@.DDDA";DDA";¥D */
	$"4440 0444 4444 4422 3B44 4441 23BB B444"            /* D@.DDDD";DDA#ª¥D */
	$"4440 8888 8888 8822 23F8 8812 23BB B888"            /* D@ààààà"#¯à.#ª∏à */
	$"8888 8888 8888 8882 2238 8122 3BBB 8888"            /* àààààààÇ"8Å";ªàà */
	$"8888 8888 8888 8882 2222 2222 3BBB 8888"            /* àààààààÇ"""";ªàà */
	$"8888 8888 8811 8888 2222 2223 BBB8 1388"            /* ààààà.àà"""#ª∏.à */
	$"8888 9999 9912 1111 1222 2221 1111 2399"            /* ààôôô....""!..#ô */
	$"9999 9999 9912 2222 2222 2222 2222 23BB"            /* ôôôôô.""""""""#ª */
	$"9999 9999 9923 3333 3322 2333 3333 23BB"            /* ôôôôô#333"#333#ª */
	$"9999 9999 9933 BBBB BB12 23BB BBBB 33BB"            /* ôôôôô3ªªª.#ªªª3ª */
	$"9999 0555 5555 BBBB B512 23BB BBBB BBBB"            /* ôô.UUUªªµ.#ªªªªª */
	$"AAA0 0555 5555 BB55 5512 23BB AAAA AABB"            /* ™†.UUUªUU.#ª™™™ª */
	$"AAA0 0555 5555 BB55 5512 23BB AAAA AAAA"            /* ™†.UUUªUU.#ª™™™™ */
	$"AAA0 0055 5555 5555 5512 23BB 5555 5555"            /* ™†.UUUUUU.#ªUUUU */
	$"5500 00AA AAAA AAAA AA12 23BB AAAA AAAA"            /* U..™™™™™™.#ª™™™™ */
	$"AA00 000A AAAA AAAA AA12 23BB AAAA AAAA"            /* ™..¬™™™™™.#ª™™™™ */
	$"A000 0000 AAAA AAAA AA12 23BB AAAA AAAA"            /* †...™™™™™.#ª™™™™ */
	$"0000 0000 0AAA AAAA AA12 23BB AAAA AAA0"            /* ....¬™™™™.#ª™™™† */
	$"0000 0000 00BB BBBB B122 223B BBBB BB00"            /* .....ªªª±"";ªªª. */
	$"0000 0000 000B BBBB 1222 2223 BBBB B000"            /* ......ªª.""#ªª∞. */
	$"0000 0000 0000 0BBB 1222 2223 BBB0 0000"            /* .......ª.""#ª∞.. */
	$"0000 0000 0000 0000 2333 3333 0000 0000"            /* ........#333.... */
	$"0000"                                               /* .. */
};

data 'cicn' (137) {
	$"0000 0000 8010 0000 0000 0020 0020 0000"            /* ....Ä...... . .. */
	$"0000 0000 0000 0048 0000 0048 0000 0000"            /* .......H...H.... */
	$"0004 0001 0004 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0004 0000 0000 0020 0020"            /* ............. .  */
	$"0000 0000 0004 0000 0000 0020 0020 0000"            /* ........... . .. */
	$"0000 0001 8000 0003 C000 0003 C000 0007"            /* ....Ä...¿...¿... */
	$"E000 0007 E000 000F F000 000F F000 001F"            /* ‡...‡......... */
	$"F800 001F F800 003F FC00 003F FC00 007F"            /* ¯...¯..?¸..?¸... */
	$"FE00 007F FE00 00FF FF00 00FF FF00 01FF"            /* ˛...˛..ˇˇ..ˇˇ..ˇ */
	$"FF80 01FF FF80 03FF FFC0 03FF FFC0 07FF"            /* ˇÄ.ˇˇÄ.ˇˇ¿.ˇˇ¿.ˇ */
	$"FFE0 07FF FFE0 0FFF FFF0 0FFF FFF0 1FFF"            /* ˇ‡.ˇˇ‡.ˇˇ.ˇˇ.ˇ */
	$"FFF8 1FFF FFF8 3FFF FFFC 3FFF FFFC 7FFF"            /* ˇ¯.ˇˇ¯?ˇˇ¸?ˇˇ¸.ˇ */
	$"FFFE 7FFF FFFE FFFF FFFF FFFF FFFF 7FFF"            /* ˇ˛.ˇˇ˛ˇˇˇˇˇˇˇˇ.ˇ */
	$"FFFE 0001 8000 0003 C000 0006 6000 0006"            /* ˇ˛..Ä...¿...`... */
	$"6000 000C 3000 000C 3000 0018 1800 0018"            /* `...0...0....... */
	$"1800 0031 8C00 0033 CC00 0063 C600 0063"            /* ...1å..3Ã..c∆..c */
	$"C600 00C3 C300 00C3 C300 0183 C180 0183"            /* ∆..√√..√√..É¡Ä.É */
	$"C180 0303 C0C0 0303 C0C0 0603 C060 0603"            /* ¡Ä..¿¿..¿¿..¿`.. */
	$"C060 0C01 8030 0C01 8030 1800 0018 1800"            /* ¿`..Ä0..Ä0...... */
	$"0018 3001 800C 3003 C00C 6003 C006 6001"            /* ..0.Ä.0.¿.`.¿.`. */
	$"8006 C000 0003 C000 0003 FFFF FFFF 7FFF"            /* Ä.¿...¿...ˇˇˇˇ.ˇ */
	$"FFFE 0000 0000 0000 0006 0000 FFFF FFFF"            /* ˇ˛..........ˇˇˇˇ */
	$"FFFF 0001 FC00 F37D 052F 0002 DD6B 08C2"            /* ˇˇ..¸.Û}./..›k.¬ */
	$"06A2 0003 4000 4000 4000 0004 8000 8000"            /* .¢..@.@.@...Ä.Ä. */
	$"8000 0005 FFFF 648A 028C 000F 0000 0000"            /* Ä...ˇˇdä.å...... */
	$"0000 0000 0000 0000 0002 2000 0000 0000"            /* .......... ..... */
	$"0000 0000 0000 0000 0052 2500 0000 0000"            /* .........R%..... */
	$"0000 0000 0000 0000 0025 5200 0000 0000"            /* .........%R..... */
	$"0000 0000 0000 0000 0521 1250 0000 0000"            /* .........!.P.... */
	$"0000 0000 0000 0000 0251 1520 0000 0000"            /* .........Q. .... */
	$"0000 0000 0000 0000 5211 1125 0000 0000"            /* ........R..%.... */
	$"0000 0000 0000 0000 2511 1152 0000 0000"            /* ........%..R.... */
	$"0000 0000 0000 0005 2111 1112 5000 0000"            /* ........!...P... */
	$"0000 0000 0000 0002 514F F415 2000 0000"            /* ........QOÙ. ... */
	$"0000 0000 0000 0052 11FF FF11 2500 0000"            /* .......R.ˇˇ.%... */
	$"0000 0000 0000 0025 11FF FF11 5200 0000"            /* .......%.ˇˇ.R... */
	$"0000 0000 0000 0521 11FF FF11 1250 0000"            /* .......!.ˇˇ..P.. */
	$"0000 0000 0000 0251 11FF FF11 1520 0000"            /* .......Q.ˇˇ.. .. */
	$"0000 0000 0000 5211 11FF FF11 1125 0000"            /* ......R..ˇˇ..%.. */
	$"0000 0000 0000 2511 11FF FF11 1152 0000"            /* ......%..ˇˇ..R.. */
	$"0000 0000 0005 2111 11FF FF11 1112 5000"            /* ......!..ˇˇ...P. */
	$"0000 0000 0002 5111 11FF FF11 1115 2000"            /* ......Q..ˇˇ... . */
	$"0000 0000 0052 1111 11FF FF11 1111 2500"            /* .....R...ˇˇ...%. */
	$"0000 0000 0025 1111 113F F311 1111 5200"            /* .....%...?Û...R. */
	$"0000 0000 0521 1111 113F F311 1111 1250"            /* .....!...?Û....P */
	$"0000 0000 0251 1111 114F F411 1111 1520"            /* .....Q...OÙ....  */
	$"0000 0000 5211 1111 1113 3111 1111 1125"            /* ....R.....1....% */
	$"0000 0000 2511 1111 1111 1111 1111 1152"            /* ....%..........R */
	$"0000 0005 2111 1111 1111 1111 1111 1112"            /* ....!........... */
	$"5000 0002 5111 1111 114F F411 1111 1115"            /* P...Q....OÙ..... */
	$"2000 0052 1111 1111 11FF FF11 1111 1111"            /*  ..R.....ˇˇ..... */
	$"2500 0025 1111 1111 11FF FF11 1111 1111"            /* %..%.....ˇˇ..... */
	$"5200 0521 1111 1111 114F F411 1111 1111"            /* R..!.....OÙ..... */
	$"1250 0251 1111 1111 1111 1111 1111 1111"            /* .P.Q............ */
	$"1520 5251 1111 1111 1111 1111 1111 1111"            /* . RQ............ */
	$"1125 2222 2222 2222 2222 2222 2222 2222"            /* .%"""""""""""""" */
	$"2222 0555 5555 5555 5555 5555 5555 5555"            /* "".UUUUUUUUUUUUU */
	$"5550"                                               /* UP */
};

resource 'icl8' (128) {
	$"0000 0000 0000 0000 0000 0000 EFEF 012C"
	$"2C2C EFEF 0000 0000 0000 0000 0000 0000"
	$"0000 0000 0000 0000 00EF EFEF 012C 2C2C"
	$"2C2C 2C57 EFEF EF00 0000 0000 0000 0000"
	$"0000 0000 0000 00EF EFEF EF01 2C2C 5757"
	$"5757 2C2C 57EF EFEF EF00 0000 0000 0000"
	$"0000 0000 0000 EFEF EFEF 012C 2C57 F4F4"
	$"F4F4 572C 2C57 EFEF EFEF 0000 0000 0000"
	$"0000 0000 00F0 F0F0 F0F0 012C 57F4 F4F4"
	$"F4F4 F42C 2C57 FFF0 F0F0 F000 0000 0000"
	$"0000 0000 F0F0 F0F0 F001 2C2C 57F4 F4F4"
	$"F0F0 F02C 2C2C 57F4 F0F0 F0F0 0000 0000"
	$"0000 00F0 F0F0 F0F0 F001 2C57 F4F4 F4F0"
	$"F0F0 F0F0 012C 57F4 F0F0 F0F0 F000 0000"
	$"0000 F0F0 F0F0 F0F0 F001 2C57 F4F4 F4F0"
	$"F0F0 F0F0 012C 57F4 F4F0 F0F0 F0F0 0000"
	$"0000 D5D5 D5D5 D5D5 D501 2C57 F4F4 D5D5"
	$"D5D5 D5D5 012C 57F4 F4D5 D5D5 D5D5 0000"
	$"00D5 D5D5 D5D5 D5D5 D501 2C57 F4F4 D5D5"
	$"D5D5 D5D5 012C 57F4 F4D5 D5D5 D5D5 D500"
	$"00D5 D5D5 D5D5 D5D5 D501 2C2C 57F4 D5D5"
	$"D5D5 D501 2C2C 57F4 F4D5 D5D5 D5D5 D500"
	$"00D5 D5D5 D5D5 D5D5 D5D5 2C2C 57F4 D5D5"
	$"D5D5 D501 2C57 F4F4 F4D5 D5D5 D5D5 D500"
	$"F1F1 F1F1 F1F1 F1F1 F1F1 2C2C 2C57 FFF1"
	$"F1F1 012C 2C57 F4F4 F4F1 F1F1 F1F1 F1F1"
	$"F1F1 F1F1 F1F1 F1F1 F1F1 F12C 2C2C 57F1"
	$"F101 2C2C 57F4 F4F4 F1F1 F1F1 F1F1 F1F1"
	$"F1F1 F1F1 F1F1 F1F1 F1F1 F12C 2C2C 2C2C"
	$"2C2C 2C2C 57F4 F4F4 F1F1 F1F1 F1F1 F1F1"
	$"F1F1 F1F1 F1F1 0101 F1F1 F1F1 2C2C 2C2C"
	$"2C2C 2C57 F4F4 F4F1 0157 F1F1 F1F1 F1F1"
	$"F2F2 F2F2 F2F2 012C 0101 0101 012C 2C2C"
	$"2C2C 2C01 0101 0101 2C57 F2F2 F2F2 F2F2"
	$"F2F2 F2F2 F2F2 012C 2C2C 2C2C 2C2C 2C2C"
	$"2C2C 2C2C 2C2C 2C2C 2C57 F4F4 F2F2 F2F2"
	$"F2F2 F2F2 F2F2 2C57 5757 5757 5757 2C2C"
	$"2C57 5757 5757 5757 2C57 F4F4 F2F2 F2F2"
	$"F2F2 F2F2 F2F2 5757 F4F4 F4F4 F4F4 012C"
	$"2C57 F4F4 F4F4 F4F4 5757 F4F4 F2F2 F2F2"
	$"00D6 D6D6 D6D6 D6D6 F4F4 F4F4 F4D6 012C"
	$"2C57 F4F4 F4F4 F4F4 F4F4 F4F4 F3F3 F300"
	$"00D6 D6D6 D6D6 D6D6 F4F4 D6D6 D6D6 012C"
	$"2C57 F4F4 F3F3 F3F3 F3F3 F4F4 F3F3 F300"
	$"00D6 D6D6 D6D6 D6D6 F4F4 D6D6 D6D6 012C"
	$"2C57 F4F4 F3F3 F3F3 F3F3 F3F3 F3F3 F300"
	$"0000 D6D6 D6D6 D6D6 D6D6 D6D6 D6D6 012C"
	$"2C57 F4F4 D6D6 D6D6 D6D6 D6D6 D6D6 0000"
	$"0000 F3F3 F3F3 F3F3 F3F3 F3F3 F3F3 012C"
	$"2C57 F4F4 F3F3 F3F3 F3F3 F3F3 F3F3 0000"
	$"0000 00F3 F3F3 F3F3 F3F3 F3F3 F3F3 012C"
	$"2C57 F4F4 F3F3 F3F3 F3F3 F3F3 F300 0000"
	$"0000 0000 F3F3 F3F3 F3F3 F3F3 F3F3 012C"
	$"2C57 F4F4 F3F3 F3F3 F3F3 F3F3 0000 0000"
	$"0000 0000 00F3 F3F3 F3F3 F3F3 F3F3 012C"
	$"2C57 F4F4 F3F3 F3F3 F3F3 F300 0000 0000"
	$"0000 0000 0000 F4F4 F4F4 F4F4 F401 2C2C"
	$"2C2C 57F4 F4F4 F4F4 F4F4 0000 0000 0000"
	$"0000 0000 0000 00F4 F4F4 F4F4 012C 2C2C"
	$"2C2C 2C57 F4F4 F4F4 F400 0000 0000 0000"
	$"0000 0000 0000 0000 00F4 F4F4 012C 2C2C"
	$"2C2C 2C57 F4F4 F400 0000 0000 0000 0000"
	$"0000 0000 0000 0000 0000 0000 2C57 5757"
	$"5757 5757"
};

resource 'icl8' (129) {
	$"0000 00FF FFFF FFFF FFFF FFFF FF01 2C2C"
	$"2CFF FFFF 0000 0000 0000 0000 0000 0000"
	$"0000 00FF EFEF EFEF EFEF EF01 2C2C 2C2C"
	$"2C2C 57F4 F400 0000 0000 0000 0000 0000"
	$"0000 00FF EFEF EFEF EFEF 012C 2C57 5757"
	$"572C 2CF4 F5F4 0000 0000 0000 0000 0000"
	$"0000 00FF EFEF EFEF EF01 2C2C 57F4 F4F4"
	$"F457 2CF4 F5F5 F400 0000 0000 0000 0000"
	$"0000 00FF F0F0 F0F0 F001 2C57 F4F4 F4F4"
	$"F4F4 2CF4 F5F5 F5F4 0000 0000 0000 0000"
	$"0000 00FF F0F0 F0F0 012C 2C57 F4F4 F4F0"
	$"F0F0 2CF4 F5F5 F5F5 F400 0000 0000 0000"
	$"0000 00FF F0F0 F0F0 012C 57F4 F4F4 F0F0"
	$"F0F0 F0F4 F5F5 F5F5 F5F4 0000 0000 0000"
	$"0000 00FF F0F0 F0F0 012C 57F4 F4F4 F0F0"
	$"F0F0 F0F4 F5F5 F5F5 F5F5 F400 0000 0000"
	$"0000 00FF D5D5 D5D5 012C 57F4 F4D5 D5D5"
	$"D5D5 D5F4 F4F4 F4F4 F4F4 F4FF 0000 0000"
	$"0000 00FF D5D5 D5D5 012C 57F4 F4D5 D5D5"
	$"D5D5 D501 2C57 F4F4 D5D5 D5FF 0000 0000"
	$"0000 00FF D5D5 D5D5 012C 2C57 F4D5 D5D5"
	$"D5D5 012C 2C57 F4F4 D5D5 D5FF 0000 0000"
	$"0000 00FF D5D5 D5D5 D52C 2C57 F4D5 D5D5"
	$"D5D5 012C 57F4 F4F4 D5D5 D5FF 0000 0000"
	$"0000 00FF F1F1 F1F1 F12C 2C2C 57FF F1F1"
	$"F101 2C2C 57F4 F4F4 F1F1 F1FF 0000 0000"
	$"0000 00FF F1F1 F1F1 F1F1 2C2C 2C57 F1F1"
	$"012C 2C57 F4F4 F4F1 F1F1 F1FF 0000 0000"
	$"0000 00FF F1F1 F1F1 F1F1 2C2C 2C2C 2C2C"
	$"2C2C 2C57 F4F4 F4F1 F1F1 F1FF 0000 0000"
	$"0000 00FF F101 01F1 F1F1 F12C 2C2C 2C2C"
	$"2C2C 57F4 F4F4 F101 57F1 F1FF 0000 0000"
	$"0000 00FF F201 2C01 0101 0101 2C2C 2C2C"
	$"2C2C 0101 0101 012C 57F2 F2FF 0000 0000"
	$"0000 00FF F201 2C2C 2C2C 2C2C 2C2C 2C2C"
	$"2C2C 2C2C 2C2C 2C2C 57F4 F4FF 0000 0000"
	$"0000 00FF F22C 5757 5757 5757 572C 2C2C"
	$"5757 5757 5757 572C 57F4 F4FF 0000 0000"
	$"0000 00FF F257 57F4 F4F4 F4F4 F401 2C2C"
	$"57F4 F4F4 F4F4 F457 57F4 F4FF 0000 0000"
	$"0000 00FF D6D6 D6F4 F4F4 F4F4 D601 2C2C"
	$"57F4 F4F4 F4F4 F4F4 F4F4 F4FF 0000 0000"
	$"0000 00FF D6D6 D6F4 F4D6 D6D6 D601 2C2C"
	$"57F4 F4F3 F3F3 F3F3 F3F4 F4FF 0000 0000"
	$"0000 00FF D6D6 D6F4 F4D6 D6D6 D601 2C2C"
	$"57F4 F4F3 F3F3 F3F3 F3F3 F3FF 0000 0000"
	$"0000 00FF D6D6 D6D6 D6D6 D6D6 D601 2C2C"
	$"57F4 F4D6 D6D6 D6D6 D6D6 D6FF 0000 0000"
	$"0000 00FF 0101 0101 F3F3 F3F3 F301 2C2C"
	$"57F4 F4F3 F3F3 F3F3 F3F3 F3FF 0000 0000"
	$"0000 00FF 0101 F301 01F3 F3F3 F301 2C2C"
	$"57F4 F4F3 F3F3 F3F3 F3F3 F3FF 0000 0000"
	$"0000 00FF 0101 F301 01F3 F3F3 F301 2C2C"
	$"57F4 F4F3 F3F3 F3F3 F3F3 F3FF 0000 0000"
	$"0000 00FF 0101 F301 01F3 F3F3 F301 2C2C"
	$"57F4 F4F3 F3F3 F3F3 F3F3 F4FF 0000 0000"
	$"0000 00FF 0101 0101 F4F4 F4F4 012C 2C2C"
	$"2C57 F4F4 F4F4 F4F4 F4F4 F4FF 0000 0000"
	$"0000 00FF 0101 F4F4 F4F4 F401 2C2C 2C2C"
	$"2C2C 57F4 F4F4 F4F4 F4F4 F4FF 0000 0000"
	$"0000 00FF 0101 F4F4 F4F4 F401 2C2C 2C2C"
	$"2C2C 57F4 F4F4 F4F4 F4F4 F4FF 0000 0000"
	$"0000 00FF FFFF FFFF FFFF FF2C 5757 5757"
	$"5757 57FF FFFF FFFF FFFF FFFF"
};

resource 'icl4' (128) {
	$"0000 0000 0000 550C CC55 0000 0000 0000"
	$"0000 0000 0555 0CCC CCCD 5550 0000 0000"
	$"0000 0005 5550 CCDD DDCC D555 5000 0000"
	$"0000 0055 550C CD55 55DC CD55 5500 0000"
	$"0000 0555 550C D555 555C CD55 5550 0000"
	$"0000 5555 50CC D555 555C CCD5 5555 0000"
	$"0005 5555 50CD 5555 5555 0CD5 5555 5000"
	$"0055 5555 50CD 5555 5555 0CD5 5555 5500"
	$"0055 5555 50CD 5555 5555 0CD5 5555 5500"
	$"0555 5555 50CD 5555 5555 0CD5 5555 5550"
	$"0555 5555 50CC D555 5550 CCD5 5555 5550"
	$"0555 5555 55CC D555 5550 CD55 5555 5550"
	$"5555 5555 55CC CD55 550C CD55 5555 5555"
	$"5555 5555 555C CCD5 50CC D555 5555 5555"
	$"5555 5555 555C CCCC CCCC D555 5555 5555"
	$"5555 5500 5555 CCCC CCCD 5555 0D55 5555"
	$"5555 550C 0000 0CCC CCC0 0000 CD55 5555"
	$"5555 550C CCCC CCCC CCCC CCCC CD55 5555"
	$"5555 55CD DDDD DDCC CDDD DDDD CD55 5555"
	$"5555 55DD 5555 550C CD55 5555 DD55 5555"
	$"0555 5555 5555 550C CD55 5555 5555 5550"
	$"0555 5555 5555 550C CD55 5555 5555 5550"
	$"0555 5555 5555 550C CD55 5555 5555 5550"
	$"0055 5555 5555 550C CD55 5555 5555 5500"
	$"0055 5555 5555 550C CD55 5555 5555 5500"
	$"0005 5555 5555 550C CD55 5555 5555 5000"
	$"0000 5555 5555 550C CD55 5555 5555 0000"
	$"0000 0555 5555 550C CD55 5555 5550 0000"
	$"0000 0055 5555 50CC CCD5 5555 5500 0000"
	$"0000 0005 5555 0CCC CCCD 5555 5000 0000"
	$"0000 0000 0555 0CCC CCCD 5550 0000 0000"
	$"0000 0000 0000 CDDD DDDD"
};

resource 'icl4' (129) {
	$"000F FFFF FFFF F0CC CFFF 0000 0000 0000"
	$"000F FFFF FFF0 CCCC CCDF F000 0000 0000"
	$"000F FFFF FF0C CDDD DCCF 0F00 0000 0000"
	$"000F FFFF F0CC DFFF FDCF 00F0 0000 0000"
	$"000F FFFF F0CD FFFF FFCF 000F 0000 0000"
	$"000F FFFF 0CCD FFFF FFCF 0000 F000 0000"
	$"000F FFFF 0CDF FFFF FFFF 0000 0F00 0000"
	$"000F FFFF 0CDF FFFF FFFF 0000 00F0 0000"
	$"000F FFFF 0CDF FFFF FFFF FFFF FFFF 0000"
	$"000F FFFF 0CDF FFFF FFF0 CDFF FFFF 0000"
	$"000F FFFF 0CCD FFFF FF0C CDFF FFFF 0000"
	$"000F FFFF FCCD FFFF FF0C DFFF FFFF 0000"
	$"000F FFFF FCCC DFFF F0CC DFFF FFFF 0000"
	$"000F FFFF FFCC CDFF 0CCD FFFF FFFF 0000"
	$"000F FFFF FFCC CCCC CCCD FFFF FFFF 0000"
	$"000F F00F FFFC CCCC CCDF FFF0 DFFF 0000"
	$"000F F0C0 0000 CCCC CC00 000C DFFF 0000"
	$"000F F0CC CCCC CCCC CCCC CCCC DFFF 0000"
	$"000F FCDD DDDD DCCC DDDD DDDC DFFF 0000"
	$"000F FDDF FFFF F0CC DFFF FFFD DFFF 0000"
	$"000F FFFF FFFF F0CC DFFF FFFF FFFF 0000"
	$"000F FFFF FFFF F0CC DFFF FFFF FFFF 0000"
	$"000F FFFF FFFF F0CC DFFF FFFF FFFF 0000"
	$"000F FFFF FFFF F0CC DFFF FFFF FFFF 0000"
	$"000F 0000 FFFF F0CC DFFF FFFF FFFF 0000"
	$"000F 00F0 0FFF F0CC DFFF FFFF FFFF 0000"
	$"000F 00F0 0FFF F0CC DFFF FFFF FFFF 0000"
	$"000F 00F0 0FFF F0CC DFFF FFFF FFFF 0000"
	$"000F 0000 FFFF 0CCC CDFF FFFF FFFF 0000"
	$"000F 00FF FFF0 CCCC CCDF FFFF FFFF 0000"
	$"000F 00FF FFF0 CCCC CCDF FFFF FFFF 0000"
	$"000F FFFF FFFC DDDD DDDF FFFF FFFF"
};

data 'ALRT' (128, "Red Alert") {
	$"004D 005F 00B9 0180 0080 5555"                      /* .M._.π.Ä.ÄUU */
};

resource 'STR#' (128) {
	{	/* array StringArray: 4 elements */
		/* [1] */
		"Sorry, Glypha IV needs a color Mac.  Try"
		" Glypha 3.0 (b&w).  Exit to Finder.",
		/* [2] */
		"Sorry, Glypha IV requires a monitor at l"
		"east 640 x 480.  Exit to Finder. ",
		/* [3] */
		"Glypha IV requires the monitor set to 16"
		" colors (4 bit).  Relaunch Glypha when y"
		"ou have switched to 16 colors.",
		/* [4] */
		"Yow!  Error initializing (nil handle).  "
		"We may need more memory or Glypha is dam"
		"aged.  Exit to Finder."
	}
};

resource 'STR#' (129) {
	{	/* array StringArray: 2 elements */
		/* [1] */
		"You are about to completely erase all th"
		"e high scores.  Continue?",
		/* [2] */
		"You are about to quit in the middle of a"
		" game.  Quit Glypha?"
	}
};

resource 'STR#' (160, "Prefs") {
	{	/* array StringArray: 1 elements */
		/* [1] */
		"Preferences"
	}
};

data 'zade' (0, "Owner resource") {
	$"1153 6368 6568 6572 617A 6164 6520 3139"            /* .Scheherazade 19 */
	$"3935"                                               /* 95 */
};

resource 'wctb' (128, "mainWindow") {
	{	/* array ColorSpec: 5 elements */
		/* [1] */
		wContentColor, 0, 0, 0,
		/* [2] */
		wFrameColor, 0, 0, 0,
		/* [3] */
		wTextColor, 0, 0, 0,
		/* [4] */
		wHiliteColor, 0, 0, 0,
		/* [5] */
		wTitleBarColor, 65535, 65535, 65535
	}
};

resource 'wctb' (129) {
	{	/* array ColorSpec: 5 elements */
		/* [1] */
		wContentColor, 0, 0, 0,
		/* [2] */
		wFrameColor, 0, 0, 0,
		/* [3] */
		wTextColor, 0, 0, 0,
		/* [4] */
		wHiliteColor, 0, 0, 0,
		/* [5] */
		wTitleBarColor, 65535, 65535, 65535
	}
};

resource 'mctb' (130, "Options menu") {
	{	/* array MCTBArray: 3 elements */
		/* [1] */
		130, 2,
		{	/* array: 4 elements */
			/* [1] */
			56797, 0, 0,
			/* [2] */
			0, 13107, 13107,
			/* [3] */
			0, 13107, 26214,
			/* [4] */
			65535, 65535, 65535
		},
		/* [2] */
		130, 1,
		{	/* array: 4 elements */
			/* [1] */
			56797, 0, 0,
			/* [2] */
			0, 13107, 13107,
			/* [3] */
			0, 0, 0,
			/* [4] */
			65535, 65535, 65535
		},
		/* [3] */
		130, 4,
		{	/* array: 4 elements */
			/* [1] */
			56797, 0, 0,
			/* [2] */
			0, 13107, 13107,
			/* [3] */
			0, 13107, 26214,
			/* [4] */
			65535, 65535, 65535
		}
	}
};

resource 'mctb' (129, "Game menu") {
	{	/* array MCTBArray: 5 elements */
		/* [1] */
		129, 1,
		{	/* array: 4 elements */
			/* [1] */
			0, 0, 0,
			/* [2] */
			0, 13107, 13107,
			/* [3] */
			0, 13107, 26214,
			/* [4] */
			65535, 65535, 65535
		},
		/* [2] */
		129, 2,
		{	/* array: 4 elements */
			/* [1] */
			0, 0, 0,
			/* [2] */
			0, 13107, 13107,
			/* [3] */
			0, 13107, 26214,
			/* [4] */
			65535, 65535, 65535
		},
		/* [3] */
		129, 3,
		{	/* array: 4 elements */
			/* [1] */
			0, 0, 0,
			/* [2] */
			0, 13107, 13107,
			/* [3] */
			0, 13107, 26214,
			/* [4] */
			65535, 65535, 65535
		},
		/* [4] */
		129, 5,
		{	/* array: 4 elements */
			/* [1] */
			0, 0, 0,
			/* [2] */
			0, 13107, 13107,
			/* [3] */
			0, 13107, 26214,
			/* [4] */
			65535, 65535, 65535
		},
		/* [5] */
		129, 4,
		{	/* array: 4 elements */
			/* [1] */
			0, 0, 0,
			/* [2] */
			0, 13107, 13107,
			/* [3] */
			0, 0, 0,
			/* [4] */
			65535, 65535, 65535
		}
	}
};

resource 'mctb' (128, "\0x14 menu") {
	{	/* array MCTBArray: 2 elements */
		/* [1] */
		128, 1,
		{	/* array: 4 elements */
			/* [1] */
			0, 0, 0,
			/* [2] */
			0, 13107, 13107,
			/* [3] */
			0, 0, 0,
			/* [4] */
			65535, 65535, 65535
		},
		/* [2] */
		128, 3,
		{	/* array: 4 elements */
			/* [1] */
			0, 0, 0,
			/* [2] */
			0, 13107, 13107,
			/* [3] */
			0, 0, 0,
			/* [4] */
			65535, 65535, 65535
		}
	}
};

resource 'ics8' (128) {
	$"0000 0000 0000 2C2C 2C2C 0000 0000 0000"
	$"0000 0000 012C 5757 572C 2C57 0000 0000"
	$"0000 0000 2C57 0000 0000 2C2C 0000 0000"
	$"0000 002C 5700 0000 0000 002C 5700 0000"
	$"0000 002C 5700 0000 0000 002C 5700 0000"
	$"0000 002C 5700 0000 0000 002C 5700 0000"
	$"0000 002C 2C00 0000 0000 002C 5700 0000"
	$"0000 0000 2C2C 0000 0000 2C57 0000 0000"
	$"0000 0000 2C2C 2C00 0001 2C57 0000 0000"
	$"0000 0000 002C 2C2C 2C2C 5700 0000 0000"
	$"0000 002C 0000 2C2C 5757 0000 2C00 0000"
	$"0000 002C 2C2C 2C2C 572C 2C2C 2C00 0000"
	$"0000 002C 5757 572C 5757 5757 2C00 0000"
	$"0000 002C 0000 002C 5700 0000 2C00 0000"
	$"0000 0000 0000 012C 2C57 0000 0000 0000"
	$"0000 0000 0000 2C2C 2C57"
};

resource 'ics#' (128) {
	{	/* array: 2 elements */
		/* [1] */
		$"03C0 0FF0 0C30 1818 1818 1818 1818 0C30"
		$"0E70 07E0 13C8 1FF8 1FF8 1188 03C0 03C0",
		/* [2] */
		$"03C0 0FF0 0FF0 1FF8 1FF8 1FF8 1FF8 0FF0"
		$"0FF0 07E0 13C8 1FF8 1FF8 1188 03C0 03C0"
	}
};

resource 'ics4' (128) {
	$"0000 0011 1100 0000 0000 11BB B11B 0000"
	$"0000 1B00 0011 0000 0001 B000 0001 B000"
	$"0001 B000 0001 B000 0001 B000 0001 B000"
	$"0001 1000 0001 B000 0000 1100 001B 0000"
	$"0000 1110 011B 0000 0000 0111 11B0 0000"
	$"0001 0011 BB00 1000 0001 1111 B111 1000"
	$"0001 BBB1 BBBB 1000 0001 0001 B000 1000"
	$"0000 0001 1B00 0000 0000 0011 1B"
};

data 'isap' (128) {
	$"0000 0001 0000 0000 0000 0000 0000 0000"            /* ................ */
};

data 'TMPL' (128, "(Resourcer only) isap", purgeable) {
	$"0852 6573 6572 7665 644C 4233 300C 5573"            /* .ReservedLB30.Us */
	$"6573 2049 5370 496E 6974 4C42 4954 1255"            /* es ISpInitLBIT.U */
	$"7365 7320 496E 7075 7453 7072 6F63 6B65"            /* ses InputSprocke */
	$"744C 4249 5400 464C 4E47 0046 4C4E 4700"            /* tLBIT.FLNG.FLNG. */
	$"464C 4E47"                                          /* FLNG */
};

data 'TMPL' (129, "is\0x00\0x00", purgeable) {
	$"0546 6C61 6773 484C 4E47 0046 4C4E 4700"            /* .FlagsHLNG.FLNG. */
	$"464C 4E47 0046 4C4E 47"                             /* FLNG.FLNG */
};

