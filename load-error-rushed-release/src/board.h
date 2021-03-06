#ifndef BOARD_H
#define BOARD_H

#define BLTDDAT_ADDR		0xDFF000
#define DMACONR_ADDR		0xDFF002
#define VPOSR_ADDR		0xDFF004
#define VHPOSR_ADDR		0xDFF006
#define DSKDATR_ADDR		0xDFF008
#define JOY0DAT_ADDR		0xDFF00A
#define JOY1DAT_ADDR		0xDFF00C
#define CLXDAT_ADDR		0xDFF00E
#define ADKCONR_ADDR		0xDFF010
#define POT0DAT_ADDR		0xDFF012
#define POT1DAT_ADDR		0xDFF014
#define POTINP_ADDR		0xDFF016
#define SERDATR_ADDR		0xDFF018
#define DSKBYTR_ADDR		0xDFF01A
#define INTENAR_ADDR		0xDFF01C
#define INTREQR_ADDR		0xDFF01E
#define DSKPTH_ADDR		0xDFF020
#define DSKPTL_ADDR		0xDFF022
#define DSKLEN_ADDR		0xDFF024
#define DSKDAT_ADDR		0xDFF026
#define REFPTR_ADDR		0xDFF028
#define VPOSW_ADDR		0xDFF02A
#define VHPOSW_ADDR		0xDFF02C
#define COPCON_ADDR		0xDFF02E
#define SERDAT_ADDR		0xDFF030
#define SERPER_ADDR		0xDFF032
#define POTGO_ADDR		0xDFF034
#define JOYTEST_ADDR		0xDFF036
#define STREQU_ADDR		0xDFF038
#define STRVBL_ADDR		0xDFF03A
#define STRHOR_ADDR		0xDFF03C
#define STRLONG_ADDR		0xDFF03E
#define BLTCON0_ADDR		0xDFF040
#define BLTCON1_ADDR		0xDFF042
#define BLTAFWM_ADDR		0xDFF044
#define BLTALWM_ADDR		0xDFF046
#define BLTCPTH_ADDR		0xDFF048
#define BLTCPTL_ADDR		0xDFF04A
#define BLTBPTH_ADDR		0xDFF04C
#define BLTBPTL_ADDR		0xDFF04E
#define BLTAPTH_ADDR		0xDFF050
#define BLTAPTL_ADDR		0xDFF052
#define BLTDPTH_ADDR		0xDFF054
#define BLTDPTL_ADDR		0xDFF056
#define BLTSIZE_ADDR		0xDFF058
#define BLTCON0L_ADDR		0xDFF05A
#define BLTSIZV_ADDR		0xDFF05C
#define BLTSIZH_ADDR		0xDFF05E
#define BLTCMOD_ADDR		0xDFF060
#define BLTBMOD_ADDR		0xDFF062
#define BLTAMOD_ADDR		0xDFF064
#define BLTDMOD_ADDR		0xDFF066
#define BLTCDAT_ADDR		0xDFF070
#define BLTBDAT_ADDR		0xDFF072
#define BLTADAT_ADDR		0xDFF074
#define SPRHDAT_ADDR		0xDFF078
#define BPLHDAT_ADDR		0xDFF07A
#define DENISEID_ADDR		0xDFF07C
#define DSKSYNC_ADDR		0xDFF07E
#define COP1LCH_ADDR		0xDFF080
#define COP1LCL_ADDR		0xDFF082
#define COP2LCH_ADDR		0xDFF084
#define COP2LCL_ADDR		0xDFF086
#define COPJMP1_ADDR		0xDFF088
#define COPJMP2_ADDR		0xDFF08A
#define COPINS_ADDR		0xDFF08C
#define DIWSTRT_ADDR		0xDFF08E
#define DIWSTOP_ADDR		0xDFF090
#define DDFSTRT_ADDR		0xDFF092
#define DDFSTOP_ADDR		0xDFF094
#define DMACON_ADDR		0xDFF096
#define CLXCON_ADDR		0xDFF098
#define INTENA_ADDR		0xDFF09A
#define INTREQ_ADDR		0xDFF09C
#define ADKCON_ADDR		0xDFF09E
#define AUD0LCH_ADDR		0xDFF0A0
#define AUD0LCL_ADDR		0xDFF0A2
#define AUD0LEN_ADDR		0xDFF0A4
#define AUD0PER_ADDR		0xDFF0A6
#define AUD0VOL_ADDR		0xDFF0A8
#define AUD0DAT_ADDR		0xDFF0AA
#define AUD1LCH_ADDR		0xDFF0B0
#define AUD1LCL_ADDR		0xDFF0B2
#define AUD1LEN_ADDR		0xDFF0B4
#define AUD1PER_ADDR		0xDFF0B6
#define AUD1VOL_ADDR		0xDFF0B8
#define AUD1DAT_ADDR		0xDFF0BA
#define AUD2LCH_ADDR		0xDFF0C0
#define AUD2LCL_ADDR		0xDFF0C2
#define AUD2LEN_ADDR		0xDFF0C4
#define AUD2PER_ADDR		0xDFF0C6
#define AUD2VOL_ADDR		0xDFF0C8
#define AUD2DAT_ADDR		0xDFF0CA
#define AUD3LCH_ADDR		0xDFF0D0
#define AUD3LCL_ADDR		0xDFF0D2
#define AUD3LEN_ADDR		0xDFF0D4
#define AUD3PER_ADDR		0xDFF0D6
#define AUD3VOL_ADDR		0xDFF0D8
#define AUD3DAT_ADDR		0xDFF0DA
#define BPL1PTH_ADDR		0xDFF0E0
#define BPL1PTL_ADDR		0xDFF0E2
#define BPL2PTH_ADDR		0xDFF0E4
#define BPL2PTL_ADDR		0xDFF0E6
#define BPL3PTH_ADDR		0xDFF0E8
#define BPL3PTL_ADDR		0xDFF0EA
#define BPL4PTH_ADDR		0xDFF0EC
#define BPL4PTL_ADDR		0xDFF0EE
#define BPL5PTH_ADDR		0xDFF0F0
#define BPL5PTL_ADDR		0xDFF0F2
#define BPL6PTH_ADDR		0xDFF0F4
#define BPL6PTL_ADDR		0xDFF0F6
#define BPL7PTH_ADDR		0xDFF0F8
#define BPL7PTL_ADDR		0xDFF0FA
#define BPL8PTH_ADDR		0xDFF0FC
#define BPL8PTL_ADDR		0xDFF0FE
#define BPLCON0_ADDR		0xDFF100
#define BPLCON1_ADDR		0xDFF102
#define BPLCON2_ADDR		0xDFF104
#define BPLCON3_ADDR		0xDFF106
#define BPL1MOD_ADDR		0xDFF108
#define BPL2MOD_ADDR		0xDFF10A
#define BPLCON4_ADDR		0xDFF10C
#define CLXCON2_ADDR		0xDFF10E
#define BPL1DAT_ADDR		0xDFF110
#define BPL2DAT_ADDR		0xDFF112
#define BPL3DAT_ADDR		0xDFF114
#define BPL4DAT_ADDR		0xDFF116
#define BPL5DAT_ADDR		0xDFF118
#define BPL6DAT_ADDR		0xDFF11A
#define BPL7DAT_ADDR		0xDFF11C
#define BPL8DAT_ADDR		0xDFF11E
#define SPR0PTH_ADDR		0xDFF120
#define SPR0PTL_ADDR		0xDFF122
#define SPR1PTH_ADDR		0xDFF124
#define SPR1PTL_ADDR		0xDFF126
#define SPR2PTH_ADDR		0xDFF128
#define SPR2PTL_ADDR		0xDFF12A
#define SPR3PTH_ADDR		0xDFF12C
#define SPR3PTL_ADDR		0xDFF12E
#define SPR4PTH_ADDR		0xDFF130
#define SPR4PTL_ADDR		0xDFF132
#define SPR5PTH_ADDR		0xDFF134
#define SPR5PTL_ADDR		0xDFF136
#define SPR6PTH_ADDR		0xDFF138
#define SPR6PTL_ADDR		0xDFF13A
#define SPR7PTH_ADDR		0xDFF13C
#define SPR7PTL_ADDR		0xDFF13E
#define SPR0POS_ADDR		0xDFF140
#define SPR0CTL_ADDR		0xDFF142
#define SPR0DATA_ADDR		0xDFF144
#define SPR0DATB_ADDR		0xDFF146
#define SPR1POS_ADDR		0xDFF148
#define SPR1CTL_ADDR		0xDFF14A
#define SPR1DATA_ADDR		0xDFF14C
#define SPR1DATB_ADDR		0xDFF14E
#define SPR2POS_ADDR		0xDFF150
#define SPR2CTL_ADDR		0xDFF152
#define SPR2DATA_ADDR		0xDFF154
#define SPR2DATB_ADDR		0xDFF156
#define SPR3POS_ADDR		0xDFF158
#define SPR3CTL_ADDR		0xDFF15A
#define SPR3DATA_ADDR		0xDFF15C
#define SPR3DATB_ADDR		0xDFF15E
#define SPR4POS_ADDR		0xDFF160
#define SPR4CTL_ADDR		0xDFF162
#define SPR4DATA_ADDR		0xDFF164
#define SPR4DATB_ADDR		0xDFF166
#define SPR5POS_ADDR		0xDFF168
#define SPR5CTL_ADDR		0xDFF16A
#define SPR5DATA_ADDR		0xDFF16C
#define SPR5DATB_ADDR		0xDFF16E
#define SPR6POS_ADDR		0xDFF170
#define SPR6CTL_ADDR		0xDFF172
#define SPR6DATA_ADDR		0xDFF174
#define SPR6DATB_ADDR		0xDFF176
#define SPR7POS_ADDR		0xDFF178
#define SPR7CTL_ADDR		0xDFF17A
#define SPR7DATA_ADDR		0xDFF17C
#define SPR7DATB_ADDR		0xDFF17E
#define COLOR00_ADDR		0xDFF180
#define COLOR01_ADDR		0xDFF182
#define COLOR02_ADDR		0xDFF184
#define COLOR03_ADDR		0xDFF186
#define COLOR04_ADDR		0xDFF188
#define COLOR05_ADDR		0xDFF18A
#define COLOR06_ADDR		0xDFF18C
#define COLOR07_ADDR		0xDFF18E
#define COLOR08_ADDR		0xDFF190
#define COLOR09_ADDR		0xDFF192
#define COLOR10_ADDR		0xDFF194
#define COLOR11_ADDR		0xDFF196
#define COLOR12_ADDR		0xDFF198
#define COLOR13_ADDR		0xDFF19A
#define COLOR14_ADDR		0xDFF19C
#define COLOR15_ADDR		0xDFF19E
#define COLOR16_ADDR		0xDFF1A0
#define COLOR17_ADDR		0xDFF1A2
#define COLOR18_ADDR		0xDFF1A4
#define COLOR19_ADDR		0xDFF1A6
#define COLOR20_ADDR		0xDFF1A8
#define COLOR21_ADDR		0xDFF1AA
#define COLOR22_ADDR		0xDFF1AC
#define COLOR23_ADDR		0xDFF1AE
#define COLOR24_ADDR		0xDFF1B0
#define COLOR25_ADDR		0xDFF1B2
#define COLOR26_ADDR		0xDFF1B4
#define COLOR27_ADDR		0xDFF1B6
#define COLOR28_ADDR		0xDFF1B8
#define COLOR29_ADDR		0xDFF1BA
#define COLOR30_ADDR		0xDFF1BC
#define COLOR31_ADDR		0xDFF1BE
#define HTOTAL_ADDR		0xDFF1C0
#define HSSTOP_ADDR		0xDFF1C2
#define HBSTRT_ADDR		0xDFF1C4
#define HBSTOP_ADDR		0xDFF1C6
#define VTOTAL_ADDR		0xDFF1C8
#define VSSTOP_ADDR		0xDFF1CA
#define VBSTRT_ADDR		0xDFF1CC
#define VBSTOP_ADDR		0xDFF1CE
#define SPRHSTRT_ADDR		0xDFF1D0
#define SPRHSTOP_ADDR		0xDFF1D2
#define BPLHSTRT_ADDR		0xDFF1D4
#define BPLHSTOP_ADDR		0xDFF1D6
#define HHPOSW_ADDR		0xDFF1D8
#define HHPOSR_ADDR		0xDFF1DA
#define BEAMCON0_ADDR		0xDFF1DC
#define HSSTRT_ADDR		0xDFF1DE
#define VSSTRT_ADDR		0xDFF1E0
#define HCENTER_ADDR		0xDFF1E2
#define DIWHIGH_ADDR		0xDFF1E4
#define BPLHMOD_ADDR		0xDFF1E6
#define SPRHPTH_ADDR		0xDFF1E8
#define SPRHPTL_ADDR		0xDFF1EA
#define BPLHPTH_ADDR		0xDFF1EC
#define BPLHPTL_ADDR		0xDFF1EE
#define FMODE_ADDR		0xDFF1FC

#endif // #ifndef BOARD_H
