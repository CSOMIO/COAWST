/*
** svn $Id: isabel.h 996 2007-08-10 00:57:22Z jcwarner $
*******************************************************************************
** Copyright (c) 2002-2007 The ROMS/TOMS Group                               **
**   Licensed under a MIT/X style license                                    **
**   See License_ROMS.txt                                                    **
*******************************************************************************
**
** Options for Isabel Application.
**
*/

!# define INLINE_2DIO
!# define PERFECT_RESTART
!# define UV_SADVECTION
!# define TS_FIXED
!# define AVERAGES


#define AKLIMIT
#define SWAN_COUPLING
#undef MCT_INTERP_OC2WV
#undef MCT_INTERP_OC2AT
#undef MCT_INTERP_WV2AT
#undef WRF_COUPLING
#if defined SWAN_COUPLING || defined WRF_COUPLING
# define MCT_LIB
#endif
#undef  WET_DRY
#define ATM_PRESS
#undef  NEARSHORE_MELLOR
#define REFINED_GRID
#define REFINED_GRID_BC

#define UV_ADV
#define DJ_GRADPS
#define UV_COR
#undef  UV_QDRAG
#undef  UV_LOGDRAG
#define UV_VIS2
#define MIX_S_UV
#undef  TS_U3HADVECTION
#undef  TS_SVADVECTION
#define TS_MPDATA
#define TS_DIF2
#define MIX_GEO_TS

#define CURVGRID
#define MASKING
#undef  ANA_INITIAL
#define SOLVE3D
#ifdef SOLVE3D
# define SALINITY
# define NONLIN_EOS
# define SPLINES            

/* define River flows */
!# define  UV_PSOURCE
!# define  TS_PSOURCE

/* surface forcing */
# define BULK_FLUXES
# ifdef BULK_FLUXES
!# define LONGWAVE_OUT
!# define LONGWAVE
!# define ANA_SSFLUX
#  undef  COARE_TAYLOR_YELLAND
#  define EMINUSP
#  define SOLAR_SOURCE
# else
#  define ANA_SSFLUX
#  define ANA_SMFLUX
#  define ANA_STFLUX
# endif

# define ANA_BTFLUX
# define ANA_BSFLUX
# define ANA_SPFLUX
# define ANA_BPFLUX

/* Select turbulence mixing scheme */

# undef ANA_VMIX
# define GLS_MIXING
# ifdef GLS_MIXING
#  define KANTHA_CLAYSON
#  define N2S2_HORAVG
#  undef  CRAIG_BANNER
#  undef  CHARNOK
#  undef  ZOS_HSIG
#  undef  TKE_WAVEDISS
# endif

/* define water mass relaxation */
# define TCLIMATOLOGY
# define TCLM_NUDGING

# define SEDIMENT
# ifdef SEDIMENT
#  define SUSPLOAD
#  define BEDLOAD_MPM
#  undef  ANA_SEDIMENT
#  undef  UV_LOGDRAG
# endif
# define SSW_BBL
# ifdef SSW_BBL
#  define SSW_CALC_ZNOT
#  undef  ANA_SEDIMENT
#  undef  SSW_CALC_UB
#  undef  SSW_LOGINT
#  undef  SSW_Z0RIP
# endif

#else

# define ANA_SMFLUX

#endif


/*define boundary conditon */
#define NORTHERN_WALL
#define WESTERN_WALL

#define SSH_TIDES
#define ADD_FSOBC
#define UV_TIDES
#define ADD_M2OBC
#define RAMP_TIDES

#define ANA_FSOBC
#define ANA_M2OBC
#define RADIATION_2D

#define EAST_FSCHAPMAN
#define EAST_M2FLATHER
#define EAST_M3RADIATION
#define EAST_TRADIATION
#define EAST_TNUDGING
#define EAST_M3NUDGING

#define SOUTH_FSCHAPMAN
#define SOUTH_M2FLATHER
#define SOUTH_M3RADIATION
#define SOUTH_TRADIATION
#define SOUTH_TNUDGING
#define SOUTH_M3NUDGING



