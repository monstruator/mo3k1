#include <stdio.h>
#include <signal.h>
#include <time.h>
#include <sys/proxy.h>
#include <sys/kernel.h>
#include <stdlib.h>
#include "../include/shared_mem.h"

void main( void )
  {
	FILE *out_fp = NULL;
    timer_t id;
	pid_t proxy;
	unsigned int i;
	char j[2]={2,1};
    struct itimerspec timer;
    struct sigevent event;
	short cr_SEANCE=0;	
	
	open_shmem();

    /* Get a proxy for the timer to kick */
    proxy = qnx_proxy_attach( 0, 0, 0, -1 );
    if( proxy == -1 ) {
      printf( "Unable to attach proxy." );
      return;
    }
	
    /* Attach to the timer */
    event.sigev_signo = -proxy;
    id = timer_create( CLOCK_REALTIME, &event );
    if( id == -1 ) {
      printf( "Unable to attach timer." );
      return;
    }


    timer.it_value.tv_sec     = 1L; //start after X sec
    timer.it_value.tv_nsec    = 0L;
    timer.it_interval.tv_sec  = 0;
    timer.it_interval.tv_nsec = 1000000000;
    timer_settime( id, 0, &timer, NULL );

	if ( ( out_fp = fopen( "//1/home/seversk/new31/data_log", "w" ) ) == NULL ) 
			fprintf( stderr, "Couldn't create/open file %s.\n", strerror( errno ) );


    /* Wait for the proxy */
    while(1) 
	{
    	Receive( proxy, 0, 0 );
		//printf("lvl = %d data=%d Nmas=%d\n",p->to41.UR_sign_K1,p->U.c_OI,p->to41.cr_SEANCE);
		//printf("%d\n",p->from41.num_KS); 

//		if (p->from41.num_KS==1) 
		{	
			printf("SUM_4=%05e  SUM_20=%05e \nDPL_1=%x DPL_2=%x \n OI=%x c_OI=%d\n",
			//!p->U.RAZN_0,p->U.RAZN_1,
			p->U.SUM_4,p->U.SUM_20,p->U.DPL_1,p->U.DPL_2,p->U.OI[p->U.c_OI],p->U.c_OI);
			printf("ZI_DATA=%x ZI_DOST=%x\n",p->U.ZI_DATA,p->U.ZI_DOST);
			printf("\n");
		}

		//	printf("V=%f A=%f D=%f T=%d \n",p->from41.Vr,p->from41.Ar,p->from41.D,p->from41.T_SS);
//			printf("H=%d M=%d S=%d T41=%d T31=%d\n",p->Dout41[30],p->Dout41[31],p->Dout41[32],p->from41.T_SS,p->Dout41[30]*3600+p->Dout41[31]*60+p->Dout41[32]);
//			printf("  D=%d \n",p->from41.T_SS-10-(p->Dout41[30]*3600+p->Dout41[31]*60+p->Dout41[32]));
			
//		printf("cr_seance=%d\n",p->to41.cr_SEANCE);
    	//printf(" size=%d\n",sizeof(obmen_31_41_t)); ///!!!!!!

		if (cr_SEANCE!=p->to41.cr_SEANCE)
		{
			if ( out_fp != NULL ) 
			{
				if (p->from41.num_KS==1) 
				{
					for(i=0;i<p->to41.sum_DCP;i++) fwrite( &p->to41.DCP_K1[i], 4, 1, out_fp );
 					printf("Есть массив по К1 N%d. Количество слов %d\n",p->to41.cr_SEANCE,p->to41.sum_DCP);
				}
				else 
				{
					for(i=0;i<p->to41.sum_DCP;i++) fwrite( &p->to41.DCP_K2[i], 2, 1, out_fp );
 					printf("Есть массив по К2 N%d. Количество слов %d\n",p->to41.cr_SEANCE,p->to41.sum_DCP);
				}
			}
		}
		cr_SEANCE=p->to41.cr_SEANCE;		

		if (kbhit()) break;
    }

	fclose( out_fp );
  }