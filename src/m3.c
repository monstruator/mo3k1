//------------------------------------------ m3.c ---------------------------------------------//  19.06.18
//-------------------------------- Пр. 3.1 изделие 3Ц-30.0-М ------------------------------------//
//---------------------------------------- CEPBEP -----------------------------------------------//
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stddef.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/pci.h>
#include <sys/osinfo.h>
#include <process.h>
#include <conio.h>
#pragma off (check_stack)
#include <math.h>
#include <sys/psinfo.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/proxy.h>
#include <sys/kernel.h>
#include "../include/tx_drv.h"
#include "../include/Mod_B.h"
#define ver "0.9.0"															// Версия

float oldKK,deltaKK,beta1,alfa1,grad=180.0/PI,Flt=0;
short oldKOD,deltaKOD,byta2;
unsigned short toPR1[8]={0x07C7,0x0000,0x0000,0x0000,0x000e,0x0000,0x8410,0};
struct DefCMD acmd[2]= {{0x1028,toPR1},{0x1428,NULL}}; 						// KC+D[] MK npu6opa 1

void main(int argc, char *argv[])
{	int i,i2,j,k,res,owu6ka, A1;
    pid_t pid_timer;
    timer_t id_timer;
	struct itimerspec timer;
    struct sigevent event;
	short b2; 	
	unsigned int N_TIMER=0;													//кол-во срабатываний таймера
	short TIMER10=0,TIMER41=0;												//обмен с пр1.0	
	short Pmax=0,STEP_AS=0,STEP_AS1=0;
	struct ispr_mo3k *ispr;
//----------------------------------- onucaHue gaHHblx npu pa6ome c MK ------------------------------------------//
	dev_tx_t *dev;
	float 	C1,C2,C3,C4,C5,C6,C7,C8;
	int TIMESEV,setANT=0,minus_x,direction=0;
	double PSI=0,TETA=0,oldPSI,oldTETA;
	double x,y,x1,y1,C,S,ri,r1,r2,r3,
	x2=0,y2=0;																//дельты по качкам
	double prim,primq,primcos;
	float KK=0,KK1=0;  														//курс корабля
	//const AgpecHK=28,AgpecCEB=31,nogAgpecHK=0,nogAgpecCEB=0;				// agpeca OY
	const AgpecHK=28,AgpecCEB=18,nogAgpecHK=0,nogAgpecCEB=0;				// agpeca OY ???(CEB=17,18)
	const Ynp_np1=1,HK=2,CEB=0;												// No KAH MK
	const Cnp1=1,CK1=Cnp1+8,CK2=CK1+20;										//cMeweHue gaHHblx B Dout
	const Cq=16,C42np1=10;													//cMeweHue gaHHblx B Dout
	unsigned pci_index=0;
	unsigned char s;
	int SIMF[6]={0,0,0,0}; 													//наличие симфонии 0,1 - ModA :  2,3 - ModB : 4,5 - sevA
	float old_RAZN=0, old_RAZN_1=0, fUM=0, fDUM=0, old_RAZN_0=0; 								//для Авто-Сопровождения
	
	C1=2048./pi;C2=4096.0/360.0;C3=180./pi;C4=C1*Kncu;
	C5=C2*Kncu;C6=C1*Kq;C7=C3;C8=C2*Kq;

    pid_timer = qnx_proxy_attach( 0, 0, 0, -1 );
    if( pid_timer == -1 ) {printf( "Unable to attach proxy." );return;}
    //------------------------------------ Attach to the timer -------------------------------------------------/
    event.sigev_signo = -pid_timer;
    id_timer = timer_create( CLOCK_REALTIME, &event );
    if( id_timer == -1 ) {printf( "Unable to attach timer." );return;}
    timer.it_value.tv_sec     = 1L; 										//start after X sec
    timer.it_value.tv_nsec    = 0L;
    timer.it_interval.tv_sec  = 0;
    timer.it_interval.tv_nsec = 200*msec;
    timer_settime( id_timer, 0, &timer, NULL );

	printf("\nCTAPT M3\n");

	create_shmem();
	delay(1000);
	open_shmem();

	ispr = (struct ispr_mo3k *) & p->to_MO3.to42.Mispr;
	p->pr1_c=p->jump=0;
	//---------------------------------- Hacmpouka MaH4ecm.KaH. ---------------------------------------------//
	owu6ka=0;dev=OpenTx(pci_index);

	if(regim_kk(dev,Ynp_np1,true)==-1)   {owu6ka|=0x4000;}					//Heucnp-Mble.owu6ku
	if(regim_ou(dev,HK,AgpecHK,true)==-1){owu6ka|=0x2000;}
	if(regim_ou(dev,CEB,AgpecCEB,true)==-1) printf("Error OU CEB\n");
	Init_ModB();
	//p->to_MO3.to42.Mispr=0;
	p->toPR1[0]=0x07C7;
	//p->toPR1[1]=p->toPR1[2]=p->toPR1[3]=0x0000;
	p->M[0]=0;
	p->M[1]=0x000e;
	p->M[2]=0x0000;
	p->M[3]=0x8410;
//	p->M[3]=0xC430; 														//включение УМ

	//--------------------------------------- CEPBEP --------------------------------------------------------//
	for(;;)
	{
		pid=Receive(0,dev->tx_B,N_B*2);s=0;owu6ka&=0xF000;					//ucnp-Mble.owu6ku
		if(pid==dev->proxyR[Ynp_np1])s=1;
		if(pid==dev->proxyERR[Ynp_np1])s=2;
		if(pid==dev->proxyR[HK])s=3;
		if(pid==dev->proxyOUT[HK])s=4;
		if(pid==dev->proxyMODE[HK])s=5;
		if(pid==dev->proxyR[CEB])s=6;
		if(pid==dev->proxyOUT[CEB])s=7;
		if(pid==dev->proxyMODE[CEB])s=8;
		if(pid==proxy_RS)s=9;
		if(pid==pid_timer)s=12;
 

		switch(s)
		{	
			case 0:owu6ka|=1;break; 										// HEBEPEH proxy

			case 1:															// ЧТЕНИЕ ДАННЫХ ИЗ ПР1
				if(KK_end(dev,Ynp_np1,2)==-1){owu6ka|=2;break;}
				p->pr1_c++;
				for(i=0;i<8;i++) p->PR1[i]=dev->tx_B[10+i];
				//for(i=0;i<3;i++) printf(" %d=%04x",i,dev->tx_B[10+i]);printf(" from\n");
				//for(i=0;i<3;i++) printf(" %d=%x",i,toPR1[i]);printf(" to\n");
				//for(i=3;i<8;i++) p->PR1[i]=0;
				//for(i=3;i<8;i++) printf(" %x",p->PR1[i]);printf("\n");
				p->PR1[3] = 0xfff8;											// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!vremenno IVEP norm!!!!!!!!!!!!!!!
				//p->PR1[4]=p->PR1[4]|0x00ff; 								//лишние ниже списка
				p->Dout41[Cq]=p->PR1[0];  									// q
				p->Dout41[Cq+1]=p->PR1[1];									// ncu
				p->Dout41[Cq+2]=p->PR1[2];									// mema
		
				if (alfa1>0.251) p->to_MO3.to42.alfa=0.260;
				else if (alfa1<-0.251) p->to_MO3.to42.alfa=-0.260;
				else if (p->PR1[1]&0x800) p->to_MO3.to42.alfa=(p->PR1[1]-0xFFF)/12.27/RADtoGRAD;		//КРЕН
				else p->to_MO3.to42.alfa=p->PR1[1]/C4;	

				//--------------------------------- ANGLE ----------------------------------------------------------------
				//memcpy(&byta2,&p->Dout41[0],2);	 
				//Flt=byta2*pi/(1<<14); 
				//p->onia41.Kg=Flt; 
				//printf("Kypc=%8.4f \n",KK);
				p->to_MO3.to41.P_FACT=(p->Dout41[16]-1991)*2/RADtoGRAD+KK+x2;		//Азимут+курс
				
				if (p->to_MO3.to41.P_FACT<0) p->to_MO3.to41.P_FACT+=2*pi;			//Азимут+курс
				if (p->to_MO3.to41.P_FACT>2*PI) p->to_MO3.to41.P_FACT-=2*pi;		//Азимут+курс
				if (p->Dout41[18]&0x800) p->to_MO3.to41.beta_FACT=(360-p->Dout41[18]/C2)/C3; 		//УГОЛ МЕСТА
				else p->to_MO3.to41.beta_FACT=-p->Dout41[18]/C1;
				//p->to_MO3.to41.beta_FACT+=y2;
				//printf(" beta=%f  ",x2*57.32,y2*57.32);
				//printf("\n");
				TIMER10=0;//был обмен с пр1.0
			break;
			
			case 2:															// ОШИБКА ОБМЕНА С ПР1
				if(KK_end(dev,Ynp_np1,2)==-1)owu6ka|=1024;else owu6ka|=4;
			break;
	
			case 3:															// HK
				SIMF[0]++; //есть симфония
				if (SIMF[0]==60000) SIMF[0]=0;
				if(ou_read(dev,HK,nogAgpecHK)){owu6ka|=8;break;}
				//printf("Read: "); 	for(j=0;j<15;j++) printf("%x ",dev->tx_B[j]);printf("\n");
				if((dev->tx_B[3])!=15)  
				{
					owu6ka|=512;
					//printf("Error read HK words 15 != %d, %x\n",dev->tx_B[3], dev->tx_B[3]);
					break;
				}
				for(j=0;j<15;j++) p->Dout41[j]=dev->tx_B[4+j]; 				//--- npueM HK
				//printf("Dout_41: "); 	for(j=0;j<24;j++) printf("%x ",p->Dout41[j]);printf("\n");

				KK=p->Dout41[0]*pi/(1<<15);
				//printf("Kypc=%8.4f \n",KK*57.32);

				//-------------------------------------- КАЧКИ ---------------------------------------
				//if(p->Dout41[3]&0x8000) PSI=-(p->Dout41[3])*NAVtoRAD/4;
				//else PSI=(float)p->Dout41[4]*NAVtoRAD/4;
				//if(p->Dout41[1]&0x8000) TETA=-(p->Dout41[2])*NAVtoRAD/4;
				//else TETA=(float)p->Dout41[2]*NAVtoRAD/4;

				if (p->Dout41[3]&0x8000) PSI=-(0xffff-p->Dout41[3])*pi/(1<<15);
				else PSI=p->Dout41[3]*pi/(1<<15);

				if (p->Dout41[2]&0x8000) TETA=-(0xffff-p->Dout41[2])*pi/(1<<15);
				else TETA=p->Dout41[2]*pi/(1<<15);

				if (PSI>0.262)  PSI=0.262;                  if (PSI<-0.262)  PSI=-0.262;
				if (TETA>0.262) TETA=0.262;                 if (TETA<-0.262) TETA=-0.262;
				//printf(" TETA=%1.3f(%4.3f) ",TETA,TETA*57.32);
			break;
	
			case 4:owu6ka|=32;break; 										// HEBEPEH proxy HK

			case 5:															// npueM KY cuHxp HK
				ou_mode_read(dev,HK,0x8000);
			break;
	
			case 6:															// npueMHuk CEB
				SIMF[4]++; 													//есть  ModB
				if (SIMF[4]==60000) SIMF[2]=0;			
				if(ou_read(dev,CEB,nogAgpecCEB)){owu6ka|=64;break;}
				//if((dev->tx_B[3])!=6){owu6ka|=128;break;}
				for(j=0;j<6;j++) p->CEB[j]=dev->tx_B[4+j]; 					//--- npueM CEB
				//for(j=0;j<6;j++) printf(" %x",p->CEB[j]);printf("\n"); 	//--- npueM CEB
				//printf("%02x:%02x:%02x ", p->CEB[2]>>8,p->CEB[3]>>8,p->CEB[3]&0x00ff);printf("\n");
			break;

			case 7:case 8:owu6ka|=256;break; 								// HEBEPEH proxy CEB

			case 9:   														// Обмен с МодБ 
				SIMF[2]++; 													// есть  ModB
				if (SIMF[2]==60000) SIMF[2]=0;
				i=Read_ModB(); 												// читаем данные из Мод Б
				if ((i!=48) || (Din_ModB[0] != 0xba))							// если не целый пакет или битый
				{
					//printf("Error words != 48, i = %d     word(ab) = %x\n", i, Din_ModB[0]);
					ispr->nkB=1;
					break; 													// если не целый пакет - выход
				}
				//if (Din_ModB[1]==0) ispr->nkB=0; 							// всегда работает навигация в Мод Б
				if (Din_ModB[1]==1) 										// Навигация присутствует
				{
					ispr->nkB=0; 											// навигац Мод Б исправна
					if (ispr->nkA==1) 										// если нет навигации в Мод А
					for(j=0;j<15;j++) p->Dout41[j]=Din_ModB[j+2]; 			// используем из Б
					//printf("Read - B: "); for(j=0;j<17;j++) printf("%x ",Din_ModB[j]); printf("\n");			
					//printf("ISPR HK mod B: %d, read: %d\n", ispr->nkB, Din_ModB[1]);		// Печать наличия навигации на ЦВС3.1 Б
				}
				else 
				{
					ispr->nkB=1;											// Навигация на ЦВС3.1 Б - остутствует, признак на отправку
				}
		
				if (Din_ModB[17]==0) ispr->sevB=1; 							// признак наличия СЕВ в Мод Б
				else 
				{
					ispr->sevB=0; 											// сев Мод Б исправен
					for(j=0;j<6;j++) p->CEB[j]=Din_ModB[18+j]; 				// npueM CEB
				}			 		
				//printf("B- ");	for(j=2;j<17;j++) printf("%x ",Din_ModB[j]);printf("\n");	
			break;
			
			case 12:														//обработчик таймера (10 Гц) 
				N_TIMER++;													//счетчик тиков 
				TIMER41++;
				if (TIMER41>1) 												// 3/10 Hz
				{
					TIMER10++;
					if (TIMER10<10)	ispr->mo1k=0; 							//есть пр1.0
					else ispr->mo1k=1; 										//нет пр1.0 			
					if (p->num_com==3) 										//конец сеанса связи
					{
						setANT=0;
						p->jump=0;
						p->toPR1[0]=1991;
						p->toPR1[2]=0;
					}		
					if (p->num_com==1) 										//подготовка к сеансу связи
					{
						if ((ispr->nkA)&&(ispr->nkB))
						{
//							printf("нет навигации\n");
						if ((p->from_MO3.from41.D>0)&&(p->from_MO3.from41.D<6.3)) KK=p->from_MO3.from41.D;
						}
						KK1=p->from_MO3.from41.P_ANT_1-KK;
						printf("KK1=%1.4f ANT1=%1.4f KK=%1.4f ANT2=%1.4f ",KK1*57.32,p->from_MO3.from41.P_ANT_1*57.32,KK*57.32,p->from_MO3.from41.P_ANT_2*57.32);
						if (KK1> 4.71225) p->jump=-1;
						if (KK1<-4.71225) p->jump=+1;
						KK1=KK1+2*p->jump*pi;
						if (p->from_MO3.from41.P_ANT_2 > p->from_MO3.from41.P_ANT_1)
						if (abs(p->from_MO3.from41.P_ANT_2-p->from_MO3.from41.P_ANT_1)<PI) direction=1; 		//по часовой
						else direction=0;
						else
						if (abs(p->from_MO3.from41.P_ANT_2-p->from_MO3.from41.P_ANT_1)<PI) direction=0; 		//против часовой
						else direction=1;
						if ((KK1> pi/2)&& (direction)) 
						{
							p->jump--; 			//движение по часовой
							KK1=KK1+2*p->jump*pi;
						}
						else if ((KK1<-pi/2)&&(!direction)) 
						{
							p->jump++; 	//движение против часовой
							KK1=KK1+2*p->jump*pi;
						}

						printf("KK1=%f j=%d dir=%d \n",KK1*57.32,p->jump,direction);

						if (p->from_MO3.from41.beta_1>=0)	p->toPR1[2]=-p->from_MO3.from41.beta_1*C1;//Угол места
						else								p->toPR1[2]=(360+(-p->from_MO3.from41.beta_1*C3))*C2;//

						p->toPR1[0]=KK1*RADtoGRAD/2+1991;					//Азимут
						setANT=1;											//была настройка углов перед сеансом
					}
					else if (p->num_com==2) 								//сеанс связи начался
					{
						if (!setANT) 										//не проводили настройку
						{
							KK1=p->from_MO3.from41.P_ANT_1-KK;
							if (KK1> 4.71225) p->jump=-1;
							if (KK1<-4.71225) p->jump=+1;
							KK1=KK1+2*p->jump*pi;
							
							if (p->from_MO3.from41.P_ANT_2>p->from_MO3.from41.P_ANT_1)
							if (abs(p->from_MO3.from41.P_ANT_2-p->from_MO3.from41.P_ANT_1)<PI) direction=1; //по часовой
							else direction=0;
							else
							if (abs(p->from_MO3.from41.P_ANT_2-p->from_MO3.from41.P_ANT_1)<PI) direction=0; //против часовой
							else direction=1;

							//printf(" KK1d=%1.4f ",KK1);
							if ((KK1> pi/2)&& (direction)) 		p->jump--; //движение по часовой
							else if ((KK1<-pi/2)&&(!direction)) p->jump++; //движение против часовой
							setANT=1;//была настройка углов перед сеансом
							//printf("KK1=%f j=%d dir=%d \n",KK1*57.32,p->jump,direction);
							//printf("KK1=%1.4f ANT1=%1.4f KK=%1.4f ANT2=%1.4f \n",KK1,p->from_MO3.from41.P_ANT_1,KK,p->from_MO3.from41.P_ANT_2);
						}
						KK1=p->from_MO3.from41.P_ANT-KK;
						printf("ANT=%1.3f KK=%1.3f KK1=%1.3f ",p->from_MO3.from41.P_ANT,KK,KK1);
				
						if ((KK1+2*p->jump*pi)> 4.71225) {printf("KK1+J=%f JUMP--\n",KK1+2*p->jump*pi);p->jump--;}
						if ((KK1+2*p->jump*pi)<-4.71225) {printf("KK1+J=%f JUMP++\n",KK1+2*p->jump*pi);p->jump++;}
						KK1=KK1+2*p->jump*pi;
			
						printf("KK+j=%1.3f j=%d\n",KK1,p->jump);
						if (p->Dout41[1]&0x8000) PSI=-(0xffff-p->Dout41[1])*pi/(1<<14);
						else PSI=p->Dout41[1]*pi/(1<<14);

						if (p->Dout41[3]&0x8000) TETA=(0xffff-p->Dout41[3])*pi/(1<<14);
						else TETA=-p->Dout41[3]*pi/(1<<14); //бортовая
						//--------------------------------- качки ? ---------------------------------------------
						x=(double)KK1; 									//азимут от 4-1
						if (x<0) 
						{	
							x+=2*PI;	
							minus_x=1;	
						}
						else minus_x=0;
						y=(double)p->from_MO3.from41.beta;
						r1=cos(y);			r3=sin(y);
						r2=r1*cos(x);		r1=r1*sin(x);
						C=cos(-PSI);		S=sin(-PSI);
						x1=C*r2+S*r3;		r3=C*r3-S*r2;
						S=sin(-TETA)*r1+cos(-TETA)*r3;
						y1=asin(S);
						prim=x1/cos(y1);
						if (prim>1) prim=1;
						x1=acos(prim);
						if (abs(x1-x) > abs(2*PI-x1-x)) x1=2*PI-x1;
						if (minus_x==1) x1=x1-2*PI;
						x2=x-x1; 											//дельта по x
						y2=y-y1; 											//дельта по y	
						
						//---------------------------------- качки ? ----------------------------------------------
						//x1=KK1; //без качек
						//y1=p->from_MO3.from41.beta;							//без качек
						//printf("KK=%3.1f x0=%3.1f y0=%3.1f PSI=%3.1f TETA=%3.1f x1=%3.1f y1=%3.1f\n\n",
						//KK*grad,x*grad,y*grad,PSI*grad,TETA*grad,x1*grad,y1*grad);

						p->toPR1[0]=x1*RADtoGRAD/2+1991;					//Азимут
						if (y1>=0)	p->toPR1[2]=-y1*C1;						//Угол места
						else 	p->toPR1[2]=(360+(-y1*C3))*C2;
					}
					else 
					//if ((p->num_com==4)||(p->num_com==5)) 				//4-я или 5-я команда из 4.2
					if (p->num_com==4) 										//4-я или 5-я команда из 4.2
					{	//---------------------------управление пр. 1.0 из 4.2-------------------------------------
						//------------------------------------ Углы -----------------------------------------------
						//printf("Az%f Um=%f\n",p->from_MO3.from42.q,p->from_MO3.from42.beta);
						//memcpy(&b2,&p->Dout41[5],2);	 KK=b2*pi/(1<<14);	
						KK1=p->from_MO3.from42.q;//-KK;						//Азимут
				
						if (p->from_MO3.from42.Rejim_AS==1) 				//режим АС
						{
							//------------------------------ В разработке -----------------------------------------
							p->to_MO3.to42.pr_rejim_AS=1;
																			// AC от Павла
							//printf("SUM_20 = %d r0 = %f  abs=%f to41=%x",p->to_MO3.to41.UR_sign_K1,p->U.RAZN_0,fabs(p->U.RAZN_0),p->toPR1[0]);

							//if (p->U.SUM_4 > 5e11) 
							if (p->to_MO3.to41.UR_sign_K1 > 39)
							{// АС по Азимуту:
								if ((fabs(p->U.RAZN_0) > 0.3) && (old_RAZN_0 != p->U.RAZN_0))
								{	
									A1=0;
									old_RAZN_0 = p->U.RAZN_0;
									A1 = -p->U.RAZN_0 * 3;//31.48;
									p->toPR1[0] = (p->PR1[0]&0x0fff) + A1;

									// fA  = (p->PR1[0]  -1991) * 2/RADtoGRAD;  fA  -= fA  * 0.0475; 		// коррекция вывода
									// fAz = (p->toPR1[0]-1991) * 2/RADtoGRAD;  fAz -= fAz * 0.0475; 		// коррекция вывода
									// printf("\n\nКоррекция по Азимуту: Old Az= % 5.1f  dAz= % 5.1f  new Az= % 5.1f\n",
									// (fAz*180/pi), ((fAz-fA)*180/pi), (fAz*180/pi) ); 		// вывод в гр.
								}
								else A1=0;
								// АС по УМ
								if ((fabs(p->U.RAZN_1) > 0.3) && (old_RAZN_1 != p->U.RAZN_1))
								{	
									fUM=0;  fDUM=0.;
									old_RAZN_1 = p->U.RAZN_1;
									if (p->PR1[2]&0x800)	fUM =  (360 - p->PR1[2]/C2)/C3; 		//УМ код в рад.
									else					fUM = -(p->PR1[2]/C1);
	
									//printf("\n\nКоррекция по У.Места: Old UM= % 5.1f",(fUM*180/pi)); 		// вывод в гр.

									fDUM = 0.007 * p->U.RAZN_1; 		// 0.5гр.(в рад.) * U.RAZN_1
									fUM = fUM - fDUM;

									//printf("  dUM= % 5.1f  new UM= % 5.1f\n", ((-fDUM)*180/pi), (fUM*180/pi)); 		// вывод в гр.
	
									if (fUM >= 0) p->toPR1[2] = -fUM*C1;			//перевод в код
									else 		  p->toPR1[2]=(360+(-fUM*C3))*C2;		//
								}
								else fUM=0;
							}
						}
						else 												//если не АС
						{
							if (KK1==0)
							{
								//printf("PSI=%f TETA=%f  ",PSI,TETA);
								beta1=p->from_MO3.from42.beta-PSI;
								if (beta1>=0)	p->toPR1[2]=-beta1*C1;		//Угол места
								else p->toPR1[2]=(360+(-beta1*C3))*C2;

								alfa1=p->from_MO3.from42.alfa-TETA;
								if (alfa1>=0)	p->toPR1[1]=alfa1*C4;		//KPEH
								else p->toPR1[1]=0xFFF+(alfa1*RADtoGRAD)*12.27;

								p->toPR1[0]=KK1*RADtoGRAD/2+1991;			//Азимут	
								//printf("alfa1=%f beta1=%f \n",alfa1,beta1);
							}
							else 
							{
								if (p->from_MO3.from42.beta>=0)	p->toPR1[2]=-p->from_MO3.from42.beta*C1;		//Угол места
								else p->toPR1[2]=(360+(-p->from_MO3.from42.beta*C3))*C2;//
								if (p->from_MO3.from42.alfa>=0)	p->toPR1[1]=p->from_MO3.from42.alfa*C4;		//KPEH
								else p->toPR1[1]=0xFFF+(p->from_MO3.from42.alfa*RADtoGRAD)*12.27;
								p->toPR1[0]=KK1*RADtoGRAD/2+1991;			//Азимут
								//printf("			KK1=%f \n", KK1*57.32);
							}
						}
						p->to_MO3.to42.pr_rejim_AS=p->from_MO3.from42.Rejim_AS;
				
					}
					//printf("n_c=%d \n",p->num_com);
	    	
					if (p->num_com==301) //
					{
						if (p->from_MO3.fromAK.beta>=0)	p->toPR1[2]=-p->from_MO3.fromAK.beta*C1;
						else p->toPR1[2]=(360+(-p->from_MO3.fromAK.beta*C3))*C2;
						//p->toPR1[0]=p->from_MO3.fromAK.Peleng*RADtoGRAD/2+1991;			
						KK1=p->from_MO3.fromAK.Peleng-KK;
						//if (KK1>pi) KK1=-KK1;
						if (KK1>4.71225) KK1=KK1-2*PI;
						if (KK1<-4.71225) KK1=KK1+2*PI;
						printf("Peleng=%2.2f KK=%1.2f KK1=%1.2f\n", p->from_MO3.fromAK.Peleng, KK, KK1);
						p->toPR1[0]=KK1*RADtoGRAD/2+1991;	
					}
					TIMER41=0;
					//printf("SIMF0=%d SIMF1=%d\n",SIMF[0],SIMF[1]);
					if (SIMF[1]<SIMF[0]) ispr->nkA=0; 						//есть симф A
					else ispr->nkA=1;										//нет симф 
					SIMF[1]=SIMF[0];
					if (SIMF[3]<SIMF[2]) ispr->cvsB=0; 						//есть 
					else ispr->cvsB=1; 										//нет модБ,СЕВ,НК
					SIMF[3]=SIMF[2];
					if (SIMF[5]<SIMF[4]) ispr->sevA=0; 						//есть сев A
					else ispr->sevA=1; //нет 
					SIMF[5]=SIMF[4];

					if ((ispr->nkA==1)&&(ispr->nkB==1)) p->Dout41[24]=0;
					else p->Dout41[24]=1;
					//printf("ispr=%x \n",p->to_MO3.to42.Mispr);
					//printf("jmp=%d \n",p->jump);
				} //------------------------- конец 3-х Герц -----------------------------------
				//-------------------------- 10 Hz -------------------------
				p->toPR1[3]=p->M[0];		p->toPR1[4]=p->M[1];
				p->toPR1[5]=p->M[2];		p->toPR1[6]=p->M[3];		

				for(i=0;i<3;i++) p->toPR1[i]=p->toPR1[i]&0x0fff;
				//-------------------------- 1 Pr -------------------------
				for(i=0;i<8;i++) toPR1[i]=p->toPR1[i];
				//for(i=3;i<8;i++) printf("  %x",toPR1[i]);printf("   to  \n");
				if((KK_frame(dev,Ynp_np1,2,acmd))==-1){owu6ka|=16;break;}

				if (ispr->nkA==1) Write_ModB(); 							//если нет навигации запрос в Модуль Б
				else if (N_TIMER&1) Write_ModB();							//иначе частота 5 Гц 
			break; 
		}//--------------------------------- end switch --------------------------------------
	} //--------------------------------- end CEPBEP for(;;) ---------------------------------
}//----------------------------------------- end main() --------------------------------------

