      PROGRAM mct_coupler
!
!svn $Id$
!=======================================================================
!  Copyright (c) 2002-2010 The ROMS/TOMS Group                         !
!    Licensed under a MIT/X style license                              !
!    See License_ROMS.txt                           Hernan G. Arango   !
!==================================================== John C. Warner ===
!                                                                      !
!  Master program to couple ROMS/TOMS to other models using the Model  !
!  Coupling Toolkit (MCT) library.                                     !
!                                                                      !
!  The following models are coupled to ROMS/TOMS:                      !
!                                                                      !
#ifdef WRF_COUPLING
!  WRF, Weather Research and Forecasting model:                        !
!       http://www.wrf-model.org                                       !
!                                                                      !
#endif
#ifdef SWAN_COUPLING
!  SWAN, Simulating WAves Nearshore model:                             !
!        http://vlm089.citg.tudelft.nl/swan/index.htm                  !
!                                                                      !
#endif
!=======================================================================
!
      USE mod_param
      USE mod_parallel
      USE mod_iounits
      USE mod_scalars
!
      USE m_MCTWorld, ONLY : MCTWorld_clean => clean

      USE ocean_control_mod, ONLY : ROMS_initialize
      USE ocean_control_mod, ONLY : ROMS_run
      USE ocean_control_mod, ONLY : ROMS_finalize
#if defined SWAN_COUPLING
      USE waves_control_mod, ONLY : SWAN_driver_init
      USE waves_control_mod, ONLY : SWAN_driver_run
      USE waves_control_mod, ONLY : SWAN_driver_finalize
#endif
#if defined SWAN_COUPLING || defined REFDIF_COUPLING
      USE ocean_coupler_mod, ONLY : finalize_ocn2wav_coupling
#endif
#ifdef WRF_COUPLING
      USE ocean_coupler_mod, ONLY : finalize_ocn2atm_coupling
#endif
!
      implicit none
!
!  Local variable declarations.
!
      logical, save :: first

      integer :: MyColor, MyCOMM, MyError, MyKey, Nnodes
      integer :: ng, pelast

      integer, dimension(Ngrids) :: Tstr   ! starting ROMS time-step
      integer, dimension(Ngrids) :: Tend   ! ending   ROMS time-step

      real(r4) :: CouplingTime             ! single precision
!
!-----------------------------------------------------------------------
!  Initialize distributed-memory (MPI) configuration
!-----------------------------------------------------------------------
!
!  Initialize MPI execution environment.
! 
      CALL mpi_init (MyError)
!
!  Get rank of the local process in the group associated with the
!  comminicator.
!
      CALL mpi_comm_size (MPI_COMM_WORLD, Nnodes, MyError)
      CALL mpi_comm_rank (MPI_COMM_WORLD, MyRank, MyError)
!
!  Set temporarily the ocean communicator to current handle before
!  splitting so the input coupling script name can be broadcasted to
!  all the nodes.
!
      OCN_COMM_WORLD=MPI_COMM_WORLD
!
!  Read in coupled model parameters from standard input.
!    
      CALL read_CouplePar (iNLM)
!
!  Allocate several coupling variables.
!
#ifdef REFINED_GRID
# if defined AIR_OCEAN && !defined WAVES_OCEAN
      N_mctmodels=Ngrids+1
# elif defined AIR_OCEAN && defined WAVES_OCEAN
      N_mctmodels=Ncouple*Ngrids+1
# else
      N_mctmodels=Ncouple*Ngrids
# endif
#else
      N_mctmodels=Ncouple
#endif
!
!  Assign processors to the models.
!
      peOCN_frst=0
      peOCN_last=peOCN_frst+NnodesOCN-1
      pelast=peOCN_last
#ifdef WAVES_OCEAN
      peWAV_frst=peOCN_last+1
      peWAV_last=peWAV_frst+NnodesWAV-1
      pelast=peWAV_last
#endif
#ifdef AIR_OCEAN
      peATM_frst=pelast+1
      peATM_last=peATM_frst+NnodesATM-1
      pelast=peATM_last
#endif
      IF (pelast.ne.Nnodes-1) THEN
        IF (MyRank.eq.0) THEN
          WRITE (stdout,10) pelast, Nnodes
 10       FORMAT (/,' mct_coupler - Number assigned processors: '       &
     &            ,i3.3,/,15x,'not equal to spawned MPI nodes: ',i3.3)
        END IF
        STOP
      ELSE
        IF (MyRank.eq.0) THEN
          WRITE (stdout,20) peOCN_frst, peOCN_last
 20       FORMAT (/,' Model Coupling: ',/,                              &
     &            /,7x,'Ocean Model MPI nodes: ',i3.3,' - ', i3.3)
#ifdef WAVES_OCEAN
          WRITE (stdout,21) peWAV_frst, peWAV_last
 21       FORMAT (/,7x,'Waves Model MPI nodes: ',i3.3,' - ', i3.3)
#endif
#ifdef AIR_OCEAN
          WRITE (stdout,22) peATM_frst, peATM_last
 22       FORMAT (/,7x,'ATM Model MPI nodes: ',i3.3,' - ', i3.3)
#endif
        END IF
      END IF
!
!  Split the communicator into SWAN, WRF, and ROMS subgroups based 
!  on color and key.
!
      MyKey=0
      IF ((peOCN_frst.le.MyRank).and.(MyRank.le.peOCN_last)) THEN
        MyColor=OCNid
      END IF
#ifdef WAVES_OCEAN
      IF ((peWAV_frst.le.MyRank).and.(MyRank.le.peWAV_last)) THEN
        MyColor=WAVid
      END IF
#endif
#ifdef AIR_OCEAN
      IF ((peATM_frst.le.MyRank).and.(MyRank.le.peATM_last)) THEN
        MyColor=ATMid
      END IF
#endif
      CALL mpi_comm_split (MPI_COMM_WORLD, MyColor, MyKey, MyCOMM,      &
     &                     MyError)
!
!-----------------------------------------------------------------------
!  Run coupled models according to the processor rank.
!-----------------------------------------------------------------------
!
#if defined SWAN_COUPLING
      IF (MyColor.eq.WAVid) THEN
        CALL SWAN_driver_init (MyCOMM, REAL(TI_WAV_OCN))
        CALL SWAN_driver_run (REAL(TI_WAV_OCN))
        CALL SWAN_driver_finalize
      END IF
#elif defined REFDIF_COUPLING
      IF (MyColor.eq.WAVid) THEN
        CouplingTime=REAL(TimeInterval(Iocean,Iwaves))
        CALL refdif_initialize (MyCOMM)
        CALL refdif_run (CouplingTime, INPname(Iwaves))
        CALL refdif_finalize
      END IF
#endif
#ifdef WRF_COUPLING
      IF (MyColor.eq.ATMid) THEN
!!      CALL module_wrf_top_mp_wrf_init (MyCOMM)
!!      CALL module_wrf_top_mp_wrf_run (REAL(TI_ATM_OCN))
!!      CALL module_wrf_top_mp_wrf_finalize
        CALL module_wrf_top_wrf_init (MyCOMM)
        CALL module_wrf_top_wrf_run (REAL(TI_ATM_OCN))
        CALL module_wrf_top_wrf_finalize
      END IF
#endif
      IF (MyColor.eq.OCNid) THEN
        first=.TRUE.
        Nrun=1
        IF (exit_flag.eq.NoError) THEN
          CALL ROMS_initialize (first, MyCOMM)
        END IF
        DO ng=1,Ngrids
          Tstr(ng)=ntstart(ng)
          Tend(ng)=ntend(ng)+1
        END DO
        IF (exit_flag.eq.NoError) THEN
          CALL ROMS_run (Tstr, Tend)
        END IF
        CALL ROMS_finalize
#if defined SWAN_COUPLING || defined REFDIF_COUPLING
        CALL finalize_ocn2wav_coupling
#endif
#ifdef WRF_COUPLING
        CALL finalize_ocn2atm_coupling
#endif
      END IF
!
!-----------------------------------------------------------------------
!  Terminates all the mpi-processing and coupling.
!-----------------------------------------------------------------------
!
      CALL mpi_barrier (MPI_COMM_WORLD)
      CALL MCTWorld_clean ()
      CALL mpi_finalize (MyError)

      STOP

      END PROGRAM mct_coupler
