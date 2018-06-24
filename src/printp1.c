#include <stdio.h>
#include <signal.h>
#include <time.h>
#include <sys/proxy.h>
#include <sys/kernel.h>
#include <stdlib.h>
#include "../include/shared_mem.h"

void write_shot();

	LOG_MEM log_shot;

void main( void )
  {
 	FILE *out_fp_data = NULL;
	FILE *out_fp_log = NULL;
    timer_t id;
	pid_t proxy;
	unsigned int i,i2=0;
	char j[2]={2,1};
    struct itimerspec timer;
    struct sigevent event;
	short cr_SEANCE=0;	
 	char log_f[30] = "//1/log/logs_000000_0000";
	char data_f[30] ="//1/log/data_000000_0000";
	struct ispr_mo3k *ispr;

	open_shmem();

	ispr = (struct ispr_mo3k *) & p->to_MO3.to42.Mispr;
	
	//printf("p->to_MO3.to42.q=(p->PR1[0]-1991)*2/RADtoGRAD = %f \n",p->to_MO3.to42.q=(p->PR1[0]-1991)*2/RADtoGRAD);
	while(1)
	{
		if (ispr->nkA || ispr->nkB) printf("----simf %d %d\n",ispr->nkA,ispr->nkB);
		//for (i=0;i<15;i++) printf("%04x ",p->to_MO3.SIMF32[i]);printf(" ----simf %d %d\n",ispr->nkA,ispr->nkB);
		//printf("key=%d\n",p->from_MO3.from41.Nkey_SHAKR);
		delay(200);
		
	}
    
}