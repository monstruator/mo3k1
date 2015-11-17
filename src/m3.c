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
//-----------//
//----- onucaHue nepeMeHHblx np.1.0 -----//
short K2Init=0;//момент настройки К2 при команде "Начало СС"
float oldKK,deltaKK,beta1,alfa1;
float grad=180.0/PI;
short oldKOD,deltaKOD; 
short byta2;
float Flt=0;
     //-----------//
int Seans=0; //признак начала сеанса по К2
unsigned short toPR1[8]=
//{0x07C7,0x0000,0x0000,0x0000,0x7000,0x0000,0x0841,0};
{0x07C7,0x0000,0x0000,0x0000,0x000e,0x0000,0x8410,0};

struct DefCMD acmd[2]=
{{0x1028,toPR1},{0x1428,NULL}}; // KC+D[] MK npu6opa 1
      //----- nyck -----//

void main(int argc, char *argv[])
{int i,j,k,res;
	int cnt_K2=0;
    pid_t pid_timer;
    timer_t id_timer;
	struct itimerspec timer;
    struct sigevent event;
//	obmen_41_31_t from41;
	short b2,num_KS=0;	//номер канала связи 1 - нр-к1, 2 - пр-к2 	
	unsigned int N_TIMER=0;//кол-во срабатываний таймера
	short TIMER10=0;//обмен с пр1.0	
	short TIMER41=0;	
	float A; //угол поправки по разности
	struct ispr_mo3k *ispr;

//----- onucaHue gaHHblx npu pa6ome c MK -----//
int short owu6ka,i1;
pid_t pid,pid_K1,pid_41,pid_42;
dev_tx_t *dev;
unsigned short Kypc,Ckopocmb,KpeH,DuHT;
unsigned short q=1,ncu=2,mema=3;
int StateK1=1,StateK2=0;//0 - stop, 1-start
int TestK2=0;//доп настройки К2
float 	C1,C2,C3,C4,C5,C6,C7,C8;
int A1;
int TIMESEV,setANT=0,minus_x;
	double PSI=0,TETA=0,oldPSI,oldTETA;
	double x,y,x1,y1,C,S,ri,r1,r2,r3,
	x2=0,y2=0;//дельты по качкам
	double prim,primq,primcos;
float KK=0,KK1=0;  //курс корабля
//const AgpecHK=28,AgpecCEB=31,nogAgpecHK=0,nogAgpecCEB=0;// agpeca OY
const AgpecHK=18,AgpecCEB=18,nogAgpecHK=0,nogAgpecCEB=0;// agpeca OY ???(CEB=17,18)
const Ynp_np1=1,HK=2,CEB=0;// No KAH MK
const Cnp1=1,CK1=Cnp1+8,CK2=CK1+20;//cMeweHue gaHHblx B Dout
const Cq=16,C42np1=10;//cMeweHue gaHHblx B Dout
unsigned pci_index=0;
unsigned char s,pewuM_K1;
int		 SIMF[6]={0,0,0,0}; //наличие симфонии 0,1 - ModA :  2,3 - ModB : 4,5 - sevA
int		 MODB[2]={0,0};
//----- onucaHue daHHblx npu pa6ome c np.4-1,4-2 -----//
#define V 1400
unsigned short DCEB[6];//Dout42[V];

 C1=2048./pi;C2=4096.0/360.0;C3=180./pi;C4=C1*Kncu;
 C5=C2*Kncu;C6=C1*Kq;C7=C3;C8=C2*Kq;

    pid_timer = qnx_proxy_attach( 0, 0, 0, -1 );
    if( pid_timer == -1 ) {printf( "Unable to attach proxy." );return;}
    /* Attach to the timer */
    event.sigev_signo = -pid_timer;
    id_timer = timer_create( CLOCK_REALTIME, &event );
    if( id_timer == -1 ) {printf( "Unable to attach timer." );return;}
    timer.it_value.tv_sec     = 1L; //start after X sec
    timer.it_value.tv_nsec    = 0L;
    timer.it_interval.tv_sec  = 0;
    timer.it_interval.tv_nsec = 100*msec;
    timer_settime( id_timer, 0, &timer, NULL );


	printf("\nCTAPT M3\n");

	create_shmem();
	delay(1000);
	open_shmem();

	ispr = (struct ispr_mo3k *) & p->to_MO3.to42.Mispr;
	p->pr1_c=0;
	//----- Hacmpouka MaH4ecm.KaH. -----//
	owu6ka=0;dev=OpenTx(pci_index);

	if(regim_kk(dev,Ynp_np1,true)==-1){owu6ka|=0x4000;}//Heucnp-Mble.owu6ku
	if(regim_ou(dev,HK,AgpecHK,true)==-1){owu6ka|=0x2000;}
	if(regim_ou(dev,CEB,AgpecCEB,true)==-1) printf("Error OU CEB\n");
	Init_ModB();
	p->to_MO3.to42.Mispr=0;
	p->toPR1[0]=0x07C7;
	//p->toPR1[1]=p->toPR1[2]=p->toPR1[3]=0x0000;
	p->M[0]=0;
	p->M[1]=0x000e;
	p->M[2]=0x0000;
	p->M[3]=0x8410;
//	p->M[3]=0xC430; //включение УМ

for(;;)//----- CEPBEP -----//
{
 pid=Receive(0,dev->tx_B,N_B*2);s=0;owu6ka&=0xF000;//ucnp-Mble.owu6ku
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
 {	case 0:owu6ka|=1;break; // HEBEPEH proxy

	case 1://--- ЧТЕНИЕ ДАННЫХ ИЗ ПР1
		if(KK_end(dev,Ynp_np1,2)==-1){owu6ka|=2;break;}
		p->pr1_c++;
		for(i=0;i<8;i++) p->PR1[i]=dev->tx_B[10+i];
		//for(i=3;i<7;i++) printf(" %d=%04x",i,dev->tx_B[10+i]);printf(" from\n");
		//for(i=3;i<7;i++) printf(" %d=%x",i,toPR1[i]);printf(" to\n");
		//for(i=0;i<3;i++) printf(" %d",p->PR1[i]);
	
		if (!ispr->mo1k) 
		{	
			p->PR1[4]=p->PR1[4]|0x00ff; //новые Васины исправности 
			
			p->PR1[4]=p->PR1[4]|0x0f00; //ОС комп
			if (p->PR1[4]&0x8000) p->PR1[4]=p->PR1[4]|0xf000; //TVK
			//p->PR1[3]=p->PR1[3]|0xf800; //ОС комп
		}
		p->to_MO3.to42.Ms1=p->PR1[3];   //состояние прибора 1.0
		p->to_MO3.to42.Ms2=p->PR1[4];
		p->to_MO3.to42.Ms3=p->PR1[5];

		p->Dout41[Cq]=p->PR1[0];  // q
		p->Dout41[Cq+1]=p->PR1[1];// ncu
		p->Dout41[Cq+2]=p->PR1[2];// mema

		//ANGLE
	    memcpy(&byta2,&p->Dout41[0],2);	 Flt=byta2*pi/(1<<14); p->simfonia41.Kg=Flt;// printf("Kypc=%8.4f \n",Flt);
		p->to_MO3.to41.P_FACT=(p->Dout41[16]-1991)*2/RADtoGRAD+KK+x2;//Азимут+курс

		if (p->to_MO3.to41.P_FACT<0) p->to_MO3.to41.P_FACT+=2*pi;//Азимут+курс
		if (p->to_MO3.to41.P_FACT>2*PI) p->to_MO3.to41.P_FACT-=2*pi;//Азимут+курс

		if (p->Dout41[18]&0x800) p->to_MO3.to41.beta_FACT=(360-p->Dout41[18]/C2)/C3; //УГОЛ МЕСТА
			else p->to_MO3.to41.beta_FACT=-p->Dout41[18]/C1;
		p->to_MO3.to41.beta_FACT+=y2;
		//printf(" x2=%f y2=%f ",x2*57.32,y2*57.32);
		//printf("\n");
		TIMER10=0;//был обмен с пр1.0
		break;
	case 2://--- ОШИБКА ОБМЕНА С ПР1
		if(KK_end(dev,Ynp_np1,2)==-1)owu6ka|=1024;else owu6ka|=4;
		break;
	case 3://--- HK
		SIMF[0]++; //есть симфония
		if (SIMF[0]==60000) SIMF[0]=0;
		if(ou_read(dev,HK,nogAgpecHK)){owu6ka|=8;break;}
   		if((dev->tx_B[3])!=32)  {owu6ka|=512;printf("error=%d\n",dev->tx_B[3]);break;}
   		if((dev->tx_B[1])!=0x12)  break;
		//printf("%d\n",dev->tx_B[3]);
    	for(j=0;j<15;j++) p->Dout41[j]=dev->tx_B[4+j]; //--- npueM HK
		//printf("N=%d",SIMF[0]);
//		printf("ModA simf- "); 	for(j=0;j<15;j++) printf("%x ",p->Dout41[j]);printf("\n");
//		printf("ModA simf- "); 	for(j=0;j<15;j++) printf("%x ",dev->tx_B[j]);printf("\n");
		//printf("%x\n",p->Dout41[4]);

	    //memcpy(&b2,&p->Dout41[5],2);	 KK=b2*pi/(1<<14);
	    KK=p->Dout41[5]*pi/(1<<14);
		KK=KK+pi; if (KK>2*pi) KK=KK-2*pi; //переворот с кормы в нос
		
	//	printf("KK=%f ",KK);

		//КАЧКИ
//		if (p->Dout41[3]&0x8000) PSI=-(p->Dout41[3])*NAVtoRAD/4;
//		    else PSI=(float)p->Dout41[4]*NAVtoRAD/4;
//		if (p->Dout41[1]&0x8000) TETA=-(p->Dout41[2])*NAVtoRAD/4;
//		    else TETA=(float)p->Dout41[2]*NAVtoRAD/4;

		if (p->Dout41[1]&0x8000) PSI=-(0xffff-p->Dout41[1])*pi/(1<<14);
		    else PSI=p->Dout41[1]*pi/(1<<14);

		if (p->Dout41[3]&0x8000) TETA=-(0xffff-p->Dout41[3])*pi/(1<<14);
		    else TETA=p->Dout41[3]*pi/(1<<14);

//			if (dev->tx_B[6]==0x8000) PSI=0;
//			if (dev->tx_B[7]==0x8000) TETA=0;

	//	if (abs(PSI)>1/4)  PSI=oldPSI;
	//	if (abs(TETA)>1/4) TETA=oldTETA;
	//	printf(" TETA=%f(%f) PSI=%f(%f)\n",PSI,PSI*57.32,TETA,TETA*57.32);
	//	printf(" A_simf "); 	for(j=0;j<9;j++) printf("%04x ",p->Dout41[j]);printf("\n");
		
  		break;
	case 4:owu6ka|=32;break; // HEBEPEH proxy HK
	case 5://--- npueM KY cuHxp HK ---//
		ou_mode_read(dev,HK,0x8000);
		break;
	case 6://--- npueMHuk CEB ---//
		SIMF[4]++; //есть  ModB
		if (SIMF[4]==60000) SIMF[2]=0;			
		if(ou_read(dev,CEB,nogAgpecCEB)){owu6ka|=64;break;}
		//if((dev->tx_B[3])!=6){owu6ka|=128;break;}
		for(j=0;j<6;j++) p->CEB[j]=dev->tx_B[4+j]; //--- npueM CEB

		//for(j=0;j<6;j++) printf(" %x",p->CEB[j]);printf("\n"); //--- npueM CEB
		//printf("%02x:%02x:%02x ", p->CEB[2]>>8,p->CEB[3]>>8,p->CEB[3]&0x00ff);printf("\n");
		break;//--- end npueMHuk CEB ---//
	case 7:case 8:owu6ka|=256;break; // HEBEPEH proxy CEB
	case 9:   //Обмен с МодБ 
		SIMF[2]++; //есть  ModB
		if (SIMF[2]==60000) SIMF[2]=0;		
		i=Read_ModB(); //читаем данные из Мод Б
		//if (i!=48) break; //если не целый пакет - выход
		//printf("nk=%x sev=%x\n",Din_ModB[1],Din_ModB[2]);
		if (Din_ModB[1]==0) ispr->nkB=1; //признак наличия навигации в Мод Б
		else {
				ispr->nkB=0; //навигац Мод Б исправна
			    if (ispr->nkA==1) //если нет навигации в Мод А
					for(j=0;j<15;j++) p->Dout41[j]=Din_ModB[j+2]; //используем из Б
			 }		
		if (Din_ModB[2]==0) ispr->sevB=1; //признак наличия СЕВ в Мод Б
		else {
				ispr->sevB=0; //навигац Мод Б исправна
			    p->Dout41[30]=(Din_ModB[20]>>8)&0x000F;
				p->Dout41[30]+=(Din_ModB[20]>>12)*10;     //hours
				p->Dout41[31]=(Din_ModB[21]>>8)&0x000F;
				p->Dout41[31]+=(Din_ModB[21]>>12)*10;	//minutes
				p->Dout41[32]=Din_ModB[21]&0x000F;
				p->Dout41[32]+=((Din_ModB[21]>>4)&0x000f)*10; //seconds
			 }			 
		
		//printf("ModB - ");	for(j=0;j<3;j++) printf("%x ",Din_ModB[j]);printf("\n");
		//ispr->nkB=ispr->sevB=0;//временно
		break;
	case 12://обработчик таймера (10 Гц) 
		N_TIMER++;//счетчик тиков 

		TIMER41++;
		if (TIMER41>1) //  3/10 Hz
		{
			TIMER10++;
			//printf("TIMER10=%d\n",TIMER10);
			if (TIMER10<10) 
			{
				ispr->mo1k=0; //есть пр1.0
				//p->to_MO3.to42.Ms1=0;   //состояние прибора 1.0
				//p->to_MO3.to42.Ms2=0;
				//p->to_MO3.to42.Ms3=0;
			}
			else ispr->mo1k=1; //нет пр1.0 

			
			//printf("1=%x 2=%x 3=%x\n",p->to_MO3.to42.Ms1,p->to_MO3.to42.Ms2,p->to_MO3.to42.Ms3);			
			//printf("H=%d M=%d S=%d T41=%d T31=%d \n",p->Dout41[30],p->Dout41[31],p->Dout41[32],p->from_MO3.from41.T_SS,p->Dout41[30]*3600+p->Dout41[31]*60+p->Dout41[32]);			
			//printf("navi=%d jump=%d \n",p->no_navi,p->jump);
			
			//КАЧКИ
			oldPSI=PSI;
			oldTETA=TETA;		
	
			if (p->num_com==1) //подготовка к сеансу связи
			{
				//=0;
			    //memcpy(&b2,&p->Dout41[5],2);	 KK=b2*pi/(1<<14);
				//if (p->no_navi==1) KK=1.218; //printf("navi=%d\n",p->no_navi);
				KK1=p->from_MO3.from41.P_ANT_1-KK;
				if ((KK1>pi/2)&&(p->from_MO3.from41.P_ANT_2>p->from_MO3.from41.P_ANT_1)) p->jump=-1;
					else if ((KK1<-pi/2)&&(p->from_MO3.from41.P_ANT_2<p->from_MO3.from41.P_ANT_1)) p->jump=1;

				if (KK1>4.71225)  p->jump=-1;	
				if (KK1<-4.71225) p->jump=1;

				KK1=KK1+2*p->jump*pi;
				oldKK=KK1; //сохраним установленный азимут
	    		if (p->from_MO3.from41.beta_1>=0)	p->toPR1[2]=-p->from_MO3.from41.beta_1*C1;//Угол места
			    else p->toPR1[2]=(360+(-p->from_MO3.from41.beta_1*C3))*C2;//
				p->toPR1[0]=KK1*RADtoGRAD/2+1991;//Азимут
				oldKOD=p->toPR1[0];//сохранение кода угла	
				setANT=1;//была настройка углов перед сеансом
			}
			else
			if (p->num_com==2) //сеанс связи начался
			{
			    //memcpy(&b2,&p->Dout41[5],2);	 KK=b2*pi/(1<<14);	
				//if (p->no_navi==1) KK=1.218; //printf("navi=%d\n",p->no_navi);
				KK1=p->from_MO3.from41.P_ANT-KK;
				KK1=KK1+2*p->jump*pi;
				
				if (KK1>2*PI) KK1=KK1-2*PI;
				if (KK1<-2*PI) KK1=KK1+2*PI;
//!!!
				x=(double)KK1;	
				if (x<0) {x+=2*PI;minus_x=1;} else minus_x=0;
				y=(double)p->from_MO3.from41.beta;
		//		PSI=(double)i*rad;		
		//		TETA=(double)i*rad;		// град
				r1=cos(y);			r3=sin(y);
				r2=r1*cos(x);		r1=r1*sin(x);
				C=cos(-PSI);S=sin(-PSI);
				x1=C*r2+S*r3;		r3=C*r3-S*r2;
				S=sin(-TETA)*r1+cos(-TETA)*r3;
				y1=asin(S);
				prim=x1/cos(y1);	if (prim>1) prim=1;
				x1=acos(prim);
				if (abs(x1-x)>abs(2*PI-x1-x)) x1=2*PI-x1;
				if (minus_x==1) x1=x1-2*PI;
				x2=x-x1; //дельта по x
				y2=y-y1; //дельта по y
				//printf(" x0=%3.1f y0=%3.1f PSI=%3.1f TETA=%3.1f x1=%3.1f y1=%3.1f\n",
				//KK*grad,y*grad,PSI*grad,TETA*grad,x1*grad,y1*grad);
				
				KK1=x1;

				oldKOD=p->PR1[0];
				oldKK=(oldKOD-1991)/325.94915;//Азимут установленный	

				if (KK1!=oldKK)
				{
					deltaKK=KK1-oldKK;

					//if (deltaKK>0) deltaKK-=0.01;else deltaKK+=0.01;  
					
					if (deltaKK>5) {deltaKK-=2*pi;}//printf("-\n");}  
					if (deltaKK<-5) {deltaKK+=2*pi;}//printf("+\n");}
					if (deltaKK>5) {deltaKK-=2*pi;}//printf("-\n");}  
					if (deltaKK<-5) {deltaKK+=2*pi;}//printf("+\n");}

					//deltaKK=deltaKK*RADtoGRAD/2;
					deltaKOD=(short)(deltaKK*RADtoGRAD/2);
					//deltaKOD=(short)deltaKK;
					//printf("P_ANT=%3.2f  KK=%3.2f  oldKK=%3.2f dKK=%3.2f dKOD=%d\n",
					//p->from_MO3.from41.P_ANT*57.32,KK*57.32,oldKK*57.32,deltaKK,deltaKOD);
						
					if (deltaKOD!=0)
					{
						p->toPR1[0]=oldKOD+deltaKOD;
					//	oldKOD=p->toPR1[0];
					//	oldKK=(oldKOD-1991)/325.94915;//Азимут установленный	
					//	printf("	toPR1=%d  deltaKOD=%d realKK=%f\n",p->toPR1[0],deltaKOD,oldKK);
					}
				}

				if (y1>=0)	p->toPR1[2]=-y1*C1;//Угол места
		    		else p->toPR1[2]=(360+(-y1*C3))*C2;//
			}
			else 
	    	//if ((p->num_com==4)||(p->num_com==5)) //4-я или 5-я команда из 4.2
	    	if (p->num_com==4) //4-я или 5-я команда из 4.2
			{	//управление пр. 1.0 из 4.2
				//Углы
				//printf("Az%f Um=%f\n",p->from_MO3.from42.q,p->from_MO3.from42.beta);
			    //memcpy(&b2,&p->Dout41[5],2);	 KK=b2*pi/(1<<14);	
				KK1=p->from_MO3.from42.q;//-KK;//Азимут
				
				if (p->from_MO3.from42.Rejim_AS==1) //режим АС
				{
					//printf("lvl = %f r0 = %f ",p->U.SUM_20,p->U.RAZN_0);
					p->to_MO3.to42.pr_rejim_AS=1;
					if ((p->U.SUM_20>30)&&(abs(p->U.RAZN_0<1.1)))	
						A1=-p->U.RAZN_0*31.48;
					else	A1=0;
					p->toPR1[0]=(p->PR1[0]&0x0fff)+A1;
					//printf("Pr1=%d A1=%d newPr1+%d\n",p->PR1[0]&0x0fff,A1,p->toPR1[0]);
				}
				else //если не АС
				{
					if (KK1==0)
					{
//						//printf("PSI=%f TETA=%f\n",PSI,TETA);

						beta1=p->from_MO3.from42.beta-TETA;
						if (beta1>=0)	p->toPR1[2]=-beta1*C1;//Угол места
						else p->toPR1[2]=(360+(-beta1*C3))*C2;//

						alfa1=p->from_MO3.from42.alfa-PSI;
						if (alfa1>=0)	p->toPR1[1]=alfa1*C4;//KPEH
						else p->toPR1[1]=0xFFF+(alfa1*RADtoGRAD)*12.27;

						p->toPR1[0]=KK1*RADtoGRAD/2+1991;//Азимут

					}
					else 
					{
						if (p->from_MO3.from42.beta>=0)	p->toPR1[2]=-p->from_MO3.from42.beta*C1;//Угол места
						else p->toPR1[2]=(360+(-p->from_MO3.from42.beta*C3))*C2;//
						if (p->from_MO3.from42.alfa>=0)	p->toPR1[1]=p->from_MO3.from42.alfa*C4;//KPEH
						else p->toPR1[1]=0xFFF+(p->from_MO3.from42.alfa*RADtoGRAD)*12.27;
						p->toPR1[0]=KK1*RADtoGRAD/2+1991;//Азимут
						//printf("			KK1=%f \n", KK1*57.32);
					}
				}
				p->to_MO3.to42.pr_rejim_AS=p->from_MO3.from42.Rejim_AS;
				
			}
			//printf("n_c=%d \n",p->num_com);
	    	
			if (p->num_com==301) //
			{	//
    			if (p->from_MO3.fromAK.beta>=0)	p->toPR1[2]=-p->from_MO3.fromAK.beta*C1;//╙уюы ьхёЄр
		    	else p->toPR1[2]=(360+(-p->from_MO3.fromAK.beta*C3))*C2;//
				p->toPR1[0]=p->from_MO3.fromAK.Peleng*RADtoGRAD/2+1991;//└чшьєЄ			
			}
				//-------------------------------------------------------------
			TIMER41=0;
			if (SIMF[1]<SIMF[0]) ispr->nkA=0; //есть симф A
			else ispr->nkA=1; //нет симф 
			SIMF[1]=SIMF[0];
 			if (SIMF[3]<SIMF[2]) ispr->cvsB=0; //есть 
			else ispr->cvsB=1; //нет модБ,СЕВ,НК
			SIMF[3]=SIMF[2];
			if (SIMF[5]<SIMF[4]) ispr->sevA=0; //есть сев A
			else ispr->sevA=1; //нет 
			SIMF[5]=SIMF[4];

			if ((ispr->nkA==1)&&(ispr->nkB==1)) p->Dout41[24]=0;
			else p->Dout41[24]=1;
  			//printf("ispr=%x \n",p->to_MO3.to42.Mispr);
			//printf("n_k=%d \n",p->num_com);
			//printf("jmp=%d \n",p->jump);
			//printf("ISPR=%x sevB=%x\n",p->to_MO3.to42.Mispr,p->Dout41[24]);

		} //конец 3-х Герц
		//-------------------------- 10 Hz -------------------------
		p->toPR1[3]=p->M[0];		p->toPR1[4]=p->M[1];
		p->toPR1[5]=p->M[2];		p->toPR1[6]=p->M[3];	

		for(i=0;i<3;i++) p->toPR1[i]=p->toPR1[i]&0x0fff;
		//-------------------------- 1 Pr -------------------------
		for(i=0;i<8;i++) toPR1[i]=p->toPR1[i];
		//for(i=3;i<8;i++) printf("  %x",toPR1[i]);printf("\n");
		//printf("toPR1=%x from42=%f\n",toPR1[2],p->from_MO3.from41.beta);
	 	if((KK_frame(dev,Ynp_np1,2,acmd))==-1){owu6ka|=16;break;}

		if (ispr->nkA==1) Write_ModB(); //если нет навигации запрос в Модуль Б
		else if (N_TIMER&1) Write_ModB();//иначе частота 5 Гц 
		break; 
 }// ----- end switch
} // ----- end CEPBEP for(;;)
}//----- end main()

