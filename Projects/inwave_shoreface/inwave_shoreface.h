/*
** svn $Id: shoreface.h 429 2009-12-20 17:30:26Z arango $
*******************************************************************************
** Copyright (c) 2002-2010 The ROMS/TOMS Group                               **
**   Licensed under a MIT/X style license                                    **
**   See License_ROMS.txt                                                    **
*******************************************************************************
**
** Options for Shore Face Planar Beach Test Case.
**
** Application flag:   SHOREFACE
** Input scripts:      ocean_shoreface.h
**                     sediment_shoreface.h
*/

#define ROMS_MODEL
#define INWAVE_MODEL

#define ACX_ADVECTION
#define ACY_ADVECTION
#define ACT_ADVECTION
#undef DOPPLER

#define EAST_AC_GRADIENT
#define WEST_AC_CLAMPED
#define NORTH_AC_GRADIENT
#define SOUTH_AC_GRADIENT
#undef THETA_AC_PERIODIC

#define WEC_VF
#define ENERGY_DISSIPATION
#define WDISS_INWAVE
#undef ROELVINK

#define UV_VIS2
#define MIX_S_UV
#undef DIAGNOSTICS_UV
#undef AVERAGES
#undef AVERAGES_WEC
#define WET_DRY
#define OUT_DOUBLE
#define UV_ADV
#define UV_C2ADVECTION
#undef  TS_MPDATA
#define TS_U3HADVECTION
#define DJ_GRADPS
#undef  SALINITY
#define SOLVE3D
#define SPLINES
#undef NS_PERIODIC
#define EASTERN_WALL

#define WEST_FSCHAPMAN
#define WEST_M2FLATHER
#define WEST_M3GRADIENT
#define WEST_M2SGRADIENT
#define WEST_M3SGRADIENT

#define NORTH_FSCHAPMAN
#define NORTH_M2FLATHER
#define NORTH_M3GRADIENT
#define NORTH_M2SGRADIENT
#define NORTH_M3SGRADIENT

#define SOUTH_FSCHAPMAN
#define SOUTH_M2FLATHER
#define SOUTH_M3GRADIENT
#define SOUTH_M2SGRADIENT
#define SOUTH_M3SGRADIENT


#define MASKING
#ifdef MASKING
# undef ANA_MASK
#endif
#undef ANA_GRID
#define ANA_INITIAL
#define ANA_FSOBC
#define ANA_M2OBC
#define ANA_SMFLUX
#define UV_QDRAG

#ifdef SOLVE3D
# undef  SSW_BBL
# ifdef SSW_BBL
#  define SSW_CALC_ZNOT
#  undef  SSW_LOGINT
# endif

# undef SEDIMENT
# ifdef SEDIMENT
#  undef  SED_MORPH
#  define SUSPLOAD
#  define BEDLOAD_MPM
#  undef  BEDLOAD_SOULSBY
#  define AVERAGES_BEDLOAD
# endif
# if defined SEDIMENT || defined SG_BBL || defined MB_BBL || defined SSW_BBL
#  define ANA_SEDIMENT
# endif

# define ANA_STFLUX
# define ANA_SSFLUX
# define ANA_BPFLUX
# define ANA_BTFLUX
# define ANA_BSFLUX
# define ANA_SPFLUX
# define ANA_SRFLUX
# undef  ANA_VMIX

# define GLS_MIXING
# if defined GLS_MIXING
#  define KANTHA_CLAYSON
#  define N2S2_HORAVG
#  undef CRAIG_BANNER
#  undef CHARNOK
#  undef ZOS_HSIG
#  undef TKE_WAVEDISS
# endif

#endif
