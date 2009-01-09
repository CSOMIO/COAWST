/***********************************************************************
     
                              COPYRIGHT
     
     The following is a notice of limited availability of the code and 
     Government license and disclaimer which must be included in the 
     prologue of the code and in all source listings of the code.
     
     Copyright notice
       (c) 1977  University of Chicago
     
     Permission is hereby granted to use, reproduce, prepare 
     derivative works, and to redistribute to others at no charge.  If 
     you distribute a copy or copies of the Software, or you modify a 
     copy or copies of the Software or any portion of it, thus forming 
     a work based on the Software and make and/or distribute copies of 
     such work, you must meet the following conditions:
     
          a) If you make a copy of the Software (modified or verbatim) 
             it must include the copyright notice and Government       
             license and disclaimer.
     
          b) You must cause the modified Software to carry prominent   
             notices stating that you changed specified portions of    
             the Software.
     
     This software was authored by:
     
     Argonne National Laboratory
     J. Michalakes: (630) 252-6646; email: michalak@mcs.anl.gov
     Mathematics and Computer Science Division
     Argonne National Laboratory, Argonne, IL  60439
     
     ARGONNE NATIONAL LABORATORY (ANL), WITH FACILITIES IN THE STATES 
     OF ILLINOIS AND IDAHO, IS OWNED BY THE UNITED STATES GOVERNMENT, 
     AND OPERATED BY THE UNIVERSITY OF CHICAGO UNDER PROVISION OF A 
     CONTRACT WITH THE DEPARTMENT OF ENERGY.
     
                      GOVERNMENT LICENSE AND DISCLAIMER
     
     This computer code material was prepared, in part, as an account 
     of work sponsored by an agency of the United States Government.
     The Government is granted for itself and others acting on its 
     behalf a paid-up, nonexclusive, irrevocable worldwide license in 
     this data to reproduce, prepare derivative works, distribute 
     copies to the public, perform publicly and display publicly, and 
     to permit others to do so.  NEITHER THE UNITED STATES GOVERNMENT 
     NOR ANY AGENCY THEREOF, NOR THE UNIVERSITY OF CHICAGO, NOR ANY OF 
     THEIR EMPLOYEES, MAKES ANY WARRANTY, EXPRESS OR IMPLIED, OR 
     ASSUMES ANY LEGAL LIABILITY OR RESPONSIBILITY FOR THE ACCURACY, 
     COMPLETENESS, OR USEFULNESS OF ANY INFORMATION, APPARATUS, 
     PRODUCT, OR PROCESS DISCLOSED, OR REPRESENTS THAT ITS USE WOULD 
     NOT INFRINGE PRIVATELY OWNED RIGHTS.

***************************************************************************/

/*#define PADIT		/* add page at beginning and end of allocation */
#if 0
# define BASE_MALLOC   fence_malloc
# define BASE_FREE     fence_free
#else
# define BASE_MALLOC   malloc
# define BASE_FREE     free
#endif

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#ifdef T3D
#include <errno.h>
#endif
#include "rsl.h"

/*
extern int EF_ALIGNMENT;
extern int EF_PROTECT_BELOW;
extern int EF_PROTECT_FREE;
*/


static char zero_length_storage[] = "" ;

#if !(defined(vpp) || defined(vpp2) || defined(SUN))
static struct mallinfo minf ;
#endif

static char *last_f ;
static int last_l ;
static int last_s ;

#ifdef O2K
static struct mallinfo mallinfo() {} ;
#endif

void * rsl_malloc(f,l,s)
   char * f ;
   int l, s ;
{
   char mess[128] ;
   void *retval ;
   int s2, tries ;

/*
EF_PROTECT_BELOW = 0 ;
EF_PROTECT_FREE = 1 ;
*/

   if ( s == 0 )
   {
     retval = (void *) zero_length_storage ;
   }
   else 
   {
#ifdef PADIT
     s2 = s + 1024 ;
#else
     s2 = s ;
#endif
     tries = 0 ;
     while ((retval=(void *)BASE_MALLOC(s2))==(void *)NULL)
     {
       tries++ ;
       sprintf(mess,
"rsl_malloc failed allocating %d bytes, called %s, line %d, try %d\n",
       s,f,l,tries) ;
       perror(mess) ;
#if !(defined(vpp) || defined(vpp2) || defined(SUN))
       minf = mallinfo() ;
       fprintf(stderr,"mallinfo: arena %d\n",minf.arena)  ;
       fprintf(stderr,"mallinfo: ordblks %d\n",minf.ordblks)  ;
       fprintf(stderr,"mallinfo: smblks %d\n",minf.smblks)  ;
       fprintf(stderr,"mallinfo: hblks %d\n",minf.hblks)  ;
       fprintf(stderr,"mallinfo: hblkhd %d\n",minf.hblkhd)  ;
       fprintf(stderr,"mallinfo: usmblks %d\n",minf.usmblks)  ;
       fprintf(stderr,"mallinfo: fsmblks %d\n",minf.fsmblks)  ;
       fprintf(stderr,"mallinfo: uordblks %d\n",minf.uordblks)  ;
       fprintf(stderr,"mallinfo: fordblks %d\n",minf.fordblks)  ;
       fprintf(stderr,"mallinfo: keepcost %d\n",minf.keepcost)  ;
#ifdef SUNINFO
       fprintf(stderr,"mallinfo: mkfast %d\n",minf.mkfast)  ;
       fprintf(stderr,"mallinfo: nblks %d\n",minf.nblks)  ;
       fprintf(stderr,"mallinfo: grain %d\n",minf.grain)  ;
       fprintf(stderr,"mallinfo: uordbytes %d\n",minf.uordbytes)  ;
       fprintf(stderr,"mallinfo: allocated %d\n",minf.allocated)  ;
       fprintf(stderr,"mallinfo: treeoverhead %d\n",minf.treeoverhead)  ;
#endif
#endif
       if ( tries >= 2 )
       { 
	 system("lsps -a") ;
	 sleep(1) ;
       }
       if ( tries >= 3 ) 
       {
	 system("lsps -a") ;
	 RSL_FATAL(2) ;
       }
     }
   }
#if !(defined(vpp)||defined(vpp2)) || defined(sx) || defined(alphavector)
   if ( s > 0 )
     bzero( retval, s2 ) ;	/* return zero'd storage always */
#else
   if ( s > 0 )
   { int l, lb ;
     l = s2/sizeof(int) ;
     lb = l*sizeof(int) ;
     vizero_( retval, &l  ) ;
     l = s2-lb ;
     vbzero_( retval+lb, &l ) ;	/* return zero'd storage always */
   }
#endif

#ifdef PADIT
   retval = retval + 512 ;
#endif

#ifdef T3D_COMMENTOUT
   if ( rsl_myproc == 0 )
   {
   if (malloc_check(0))
   {
    if ( rsl_myproc == 0 )
    fprintf(stderr,"%s, %d ---problem from malloc_check--- %d bytes last %s %d %d \n",f,l,s,last_f, last_l, last_s) ;
    exit(3);
   }
   }
   last_s = s ;
   last_l = l ;
   last_f = f ;
#endif

   return(retval) ;
}

rsl_free( p )
   char * p ;
{
   if ( p == zero_length_storage ) return ;    /* fix from ANU */
#ifdef PADIT
   BASE_FREE ( p-512 ) ;
#else
   BASE_FREE ( p ) ;
#endif
   p = NULL ;
}


