//--- Ynoc K1 ---// 21.06.11
#include <unistd.h>
#include <sys/osinfo.h>
#include <time.h>
#include <process.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/dev.h>
#include "../include/shared_mem.h"
#include "../include/my_pcs.h"
#define P 1
#define TIMEOUT 500
#define RAZN0 0
#define RAZN1 1
#define SUM4  2
#define SUM20 3
#define ZI    4
#define DPL1  7
#define YP    8
#define DPL2 10
pid_t proxy_DRV2=0,
	  proxy_RS; // Mod_B 
int rez=0, err=0 ;
unsigned char massiv[9];
char nastroika[]={0xB8, 0x00, 0x00, 0x00, 0xB9, 0x4B};
pid_t proxy_DRV1=0; 
int msg=25,rmsg,cntl_code=0,ii=1;
int chan1=8, chan2=2, chan_rd, chan_wr; // chan2 - связь со стендом 			// chan1 - связь с устройством (УПОС(8) или ПОСТ-3Ц(3))
//-----мои данные-----
unsigned int massivK1[100],CK1=0; //данные из К1 и кол-во
unsigned int massiv_U[5]; 
unsigned short massiv_D[2];
unsigned int massiv_R[5]; 
char POROGS[]={0xD6, 0x42, 0x00, 0x2D, 0x62, 0x58, 0x04};//e+7
//char POROGS[]={0xD6, 0x42, 0x79, 0x05, 0x54, 0x00, 0x05};//e+10
//char POROGD[]={0xD6, 0x44, 0x00, 0x2D, 0x62, 0x58, 0x04};//e+7
char POROGD[]={0xD6, 0x44, 0x79, 0x05, 0x54, 0x00, 0x05};//e+10
float porog_f;
int	porog_i; 
float f_itog_urov=0,f_itog_razn=0,f_dop=0,koef=2.7e-9;

int f=1;
//int q=0;
float floatSUM_4;
float porog_sf = 0;
float SREDN;
float porog_df = 0;

void Init_K1(NASTR)
{
	int i;
	printf("							УПОС СТАРТ K%d\n",NASTR+1);
// Проверка запуска драйвера модуля ПЦС ЦВС-3-1
	pid_drv=get_pid_process("PCS",0);
	if (!pid_drv) {
		printf("!!! Драйвер не запущен\n"); err++; rez=2;
		exit(rez);
	} 

// Cброс канала chan1
	start_pcs_s.type=7;
	start_pcs_s.cnl=chan1;
	Send(pid_drv,&start_pcs_s,&start_pcs_r,sizeof(start_pcs_s),sizeof(start_pcs_r));
	printf("Сброс канала 1. тип=%d статус=%d \n",start_pcs_r.type,start_pcs_r.status);

// Инициализация канала ПЦС для связи с каналом chan1 (УПОС или ПОСТ-3Ц)
	init_pcs_s.type=1;
	init_pcs_s.cnl=chan1;			// номер канала ПЦС
	init_pcs_s.speed=19200;			// скорость передачи данных
	init_pcs_s.b_info=8;			// кол-во бит в информационном байте
	init_pcs_s.b_StrStp=1;			// кол-во стоп-бит
	if(chan1==8)
	  init_pcs_s.b_prt='E';			// наличие и тип паритета (четность) УПОС
	if(chan1==3)
	  init_pcs_s.b_prt=0;			// наличие и тип паритета (нет) ПОСТ-3Ц
	init_pcs_s.dev=0; 				// тип у-ва (0 - обычный RS канал) 
	init_pcs_s.lvl_inp=10;			// уровень заполнения FIFO до прерывания

	i=Send(pid_drv,&init_pcs_s,&init_pcs_r,sizeof(init_pcs_s),sizeof(init_pcs_r));
	if (i==-1) printf("Нет общения с драйвером\n");
	if (init_pcs_r.type==1) 
		if (init_pcs_r.status!=0) {
			printf("!!! Инициализация канала %d не прошла\n", chan1);
			exit(2);
		}

	//printf("Инициализация канала %d прошла\n", chan1);

// Старт канала chan1
	start_pcs_s.type=2;
	start_pcs_s.cnl=chan1;
	start_pcs_s.ID_P=getpid();

	printf("my_id= %d\n",start_pcs_s.ID_P);
	Send(pid_drv,&start_pcs_s,&start_pcs_r,sizeof(start_pcs_s),sizeof(start_pcs_r));
	if (start_pcs_r.type==2) {
		if (start_pcs_r.status==-1) {
			printf("!!! Старт канала %d не выполнен\n", chan1);
			exit(2);
		} 
		else { proxy_DRV1=start_pcs_r.Proxy;
		  //printf("Старт канала %d выполнен, proxy=%d\n", chan1, proxy_DRV1);
		}
	}       

// Очистка канала chan1
	rd_cpcs_s.type=4;
	rd_cpcs_s.cnl=chan1;
	Send(pid_drv,&rd_cpcs_s,&rd_cpcs_r,sizeof(rd_cpcs_s),sizeof(rd_cpcs_r));
// Настройка канала K1 или K2
  // Передача кода настройки в канал 8 ПЦС (УПОС)
	wr_cpcs_s.type=5;
	wr_cpcs_s.cnl=chan1;
	wr_cpcs_s.cnt=6;
//	nastroika[3]=0; // 0-УПОС, 1-ПОСТ-3Ц
	nastroika[3]=NASTR; // 0-УПОС, 1-ПОСТ-3Ц


	delay(50);
	for(i=0;i<6;i++) wr_cpcs_s.uom.dt[i]=nastroika[i];// зп массива 6 байт
	Send(pid_drv,&wr_cpcs_s,&wr_cpcs_r,sizeof(wr_cpcs_s),sizeof(wr_cpcs_r));
	//printf("Настройка канала =");
	//for(i=0;i<6;i++) printf(" %02x",wr_cpcs_s.uom.dt[i]); printf("\n");
// начало теста
  // Прием подтверждения настройки
	delay(50);
	rd_cpcs_s.type=4;
	rd_cpcs_s.cnl=chan1;
	Send(pid_drv,&rd_cpcs_s,&rd_cpcs_r,sizeof(rd_cpcs_s),sizeof(rd_cpcs_r));
	printf("Ответ =");
	for(i=0;i<rd_cpcs_r.cnt;i++) printf(" %02x",rd_cpcs_r.uim.dt[i]); printf("\n");
}

Write_K1(n_com)
{
	wr_cpcs_s.type=5;
	wr_cpcs_s.cnl=chan1;
	wr_cpcs_s.cnt=2;
	wr_cpcs_s.uom.dt[0]=0x9f;
	wr_cpcs_s.uom.dt[1]=n_com;
	Send(pid_drv,&wr_cpcs_s,&wr_cpcs_r,sizeof(wr_cpcs_s),sizeof(wr_cpcs_r));
}

int Read_K1()
{
	unsigned int massiv[6]={0,0,0,0,0,0},i,b;
	short inDP[2];
	int ii=0;
	float RAZN;
	rd_cpcs_s.type=4;
	rd_cpcs_s.cnl=chan1;
    // выдача команды и прием данных
	i=Send(pid_drv,&rd_cpcs_s,&rd_cpcs_r,sizeof(rd_cpcs_s),sizeof(rd_cpcs_r));
	//printf("Ответ =");
	//for(i=0;i<rd_cpcs_r.cnt;i++) printf(" %02x",rd_cpcs_r.uim.dt[i]);
    //printf("\n");
    // переформатирование
    for (i=0;i<rd_cpcs_r.cnt;i++)  massivK1[i+CK1]=rd_cpcs_r.uim.dt[i];//дополним массив
	CK1=CK1+rd_cpcs_r.cnt;//увеличим кол-во байт в буфере
	b=rd_cpcs_r.cnt;

  while(CK1)
  {
	//printf(" %x count=%d \n",rd_cpcs_r.uim.dt[0],rd_cpcs_r.cnt);
	for(i=0;i<6;i++) massiv[i]=0;
	switch (massivK1[0])
	{
		case 0xd0 : case 0xd1 : case 0xd2  : case 0xd3 : 
					 //printf("\n  RAZN_0 \n");
					if (CK1<6) return b;
					for(i=1;i<6;i++) 
					{
						massiv[i]=massivK1[i];
						massiv[i]=massiv[i]<<((i-1)*7);
						massiv[0]+=massiv[i];
					}					
				    if (massivK1[0]==0xd0) 
					{memcpy(&RAZN,&massiv[0],4);if(RAZN!=0) p->U.RAZN_0=RAZN;}
				    if (massivK1[0]==0xd1) 
					{memcpy(&RAZN,&massiv[0],4);if(RAZN!=0) p->U.RAZN_1=RAZN;}					
				    if (massivK1[0]==0xd2) memcpy(&p->U.SUM_4,&massiv[0],4);
				    if (massivK1[0]==0xd3) memcpy(&p->U.SUM_20,&massiv[0],4);
					for(i=6;i<CK1;i++) massivK1[i-6]=massivK1[i]; 		
					CK1-=6;break;					
		case 0xe4 : p->U.ZI_DATA=p->U.ZI_DOST=0;
					if (CK1<7) return b;					
					for (i=1;i<4;i++) 
					{
						massiv[i]=massivK1[i];        
				    	massiv[i]=massiv[i]<<((i-1)*7);
						p->U.ZI_DATA+=massiv[i]; 	
					}	
					p->U.ZI_DATA=p->U.ZI_DATA&0xfffff; 	
				
					for (i=4;i<7;i++) 
					{
						massiv[i-3]=massivK1[i];        
				    	massiv[i-3]=massiv[i-3]<<((i-4)*7);
						p->U.ZI_DOST+=massiv[i-3]; 	
					}
					for(i=7;i<CK1;i++) massivK1[i-7]=massivK1[i]; 		
					CK1-=7;					
					break;
		case 0xa7 : 
					if (CK1<3) return b;	
					inDP[0]=massivK1[2];
					//printf("DP_0=%x DP_1=%x",massivK1[1],massivK1[2]);

					inDP[0]=(inDP[0]<<7)+massivK1[1];

					if (inDP[0]>0x1fff) // отрицательный доплер
						p->U.DPL_1 = - ((~inDP[0]+1) & 0x1fff);  
					else 				//положительный доплер
						p->U.DPL_1 = inDP[0];

					//printf("  inDP=%x   kodDP=%d\n\n",inDP[0],p->U.DPL_1);
					for(i=3;i<CK1;i++) massivK1[i-3]=massivK1[i]; 		
					CK1-=3;					
					break;
		
		case 0xba :	if (CK1<4) return b;
					for (i=1;i<4;i++) 
					{
						massiv[i]=massivK1[i];
				    	massiv[i]=massiv[i]<<((i-1)*7);
						p->U.DPL_2+=massiv[i]; 	
					}					
					for(i=4;i<CK1;i++) massivK1[i-4]=massivK1[i]; 		
					CK1-=4;					
					break;
		case 0xb5 :	
					if (CK1<4) return b;					
					p->U.OI[p->U.c_OI]=0;
					for (i=1;i<4;i++) 
					{
						massiv[i]=massivK1[i];
				    	massiv[i]=massiv[i]<<((i-1)*7);
						p->U.OI[p->U.c_OI]+=massiv[i]; 	
					}
					//if (p->U.SUM_20>10) 
					//ii++;
					//printf("%x %d\n",p->U.OI[p->U.c_OI],ii);
					p->U.c_OI++;					
					for(i=4;i<CK1;i++) massivK1[i-4]=massivK1[i]; 		
					CK1-=4;					
					break;
		case 0xb8 : 
					if (CK1<4) return b;					
					p->to_MO3.to41.GL_CP = massivK1[3] >>4 & 0x01;
					p->to_MO3.to41.GL_priem = massivK1[3] >>5 & 0x01;
					//printf("		read			SR = %d PR = %d", p->to41.GL_CP, p->to41.GL_priem);						
					for(i=4;i<CK1;i++) massivK1[i-4]=massivK1[i]; 		
					CK1-=4;break;					
		default :   
					for(i=1;i<CK1;i++) massivK1[i-1]=massivK1[i]; 							
					CK1-=1;
					/*
					for(i=0;i<CK1;i++) 
					{
						printf(" %02x",massivK1[i]);
					    printf("   ERROR_K1\n");
						massivK1[i]=0;
					}
					CK1=0;
					return b;
					*/
	}
  }
  return b;
}

int DDRead_K1()
{
	//int f = 0;
	unsigned int massiv[6]={0,0,0,0,0,0},i,b;
	short inDP[2];
	int ii=0;
	float RAZN;

	rd_cpcs_s.type=4;
	rd_cpcs_s.cnl=chan1;
	//string perenos[] = "\r\n";
    // выдача команды и прием данных
	i=Send(pid_drv,&rd_cpcs_s,&rd_cpcs_r,sizeof(rd_cpcs_s),sizeof(rd_cpcs_r));
	//printf("Ответ =");
	for(i=0;i<rd_cpcs_r.cnt;i++) printf(" %02x",rd_cpcs_r.uim.dt[i]);    printf("\n");
    // переформатирование
    for (i=0;i<rd_cpcs_r.cnt;i++)  massivK1[i+CK1]=rd_cpcs_r.uim.dt[i];//дополним массив
	CK1=CK1+rd_cpcs_r.cnt;//увеличим кол-во байт в буфере
	b=rd_cpcs_r.cnt;
  while(CK1)
  {
	//printf(" %x count=%d \n",rd_cpcs_r.uim.dt[0],rd_cpcs_r.cnt);
	for(i=0;i<6;i++) massiv[i]=0;
	switch (massivK1[0])
	{
		case 0xd2 : 
					if (CK1<6) return b;
					for(i=1;i<6;i++) 
					{
						//printf("[%d]" , massivK1[i]);
						massiv[i]=massivK1[i];
						massiv[i]=massiv[i]<<((i-1)*7);
						massiv[0]+=massiv[i];
					}					
				    if (massivK1[0]==0xd0) 
					{memcpy(&RAZN,&massiv[0],4);if(RAZN!=0) p->U.RAZN_0=RAZN;}
				    if (massivK1[0]==0xd1) 
					{memcpy(&RAZN,&massiv[0],4);if(RAZN!=0) p->U.RAZN_1=RAZN;}					
				    if (massivK1[0]==0xd2) memcpy(&p->U.SUM_4,&massiv[0],4);
				    if (massivK1[0]==0xd3) memcpy(&p->U.SUM_20,&massiv[0],4);
					for(i=6;i<CK1;i++) massivK1[i-6]=massivK1[i];
					if ((p->U.SUM_4 <= 1e6) | (p->U.SUM_4 > 1e14)){
						//q+=1;
						//printf("\n q = %d  -=MINUS=- = %e",q, p->U.SUM_4);
						//floatSUM_4 += 0;						
					}
					else
					{
						f+=1;
						floatSUM_4 += p->U.SUM_4;
					}									
					CK1-=6;break;	
		case 0xd3 : 
					 //printf("\n  RAZN_0 \n");
					if (CK1<6) return b;
					for(i=1;i<6;i++) 
					{
						massiv[i]=massivK1[i];
						massiv[i]=massiv[i]<<((i-1)*7);
						massiv[0]+=massiv[i];
					}					
				    if (massivK1[0]==0xd0) 
					{memcpy(&RAZN,&massiv[0],4);if(RAZN!=0) p->U.RAZN_0=RAZN;}
				    if (massivK1[0]==0xd1) 
					{memcpy(&RAZN,&massiv[0],4);if(RAZN!=0) p->U.RAZN_1=RAZN;}					
				    if (massivK1[0]==0xd2) memcpy(&p->U.SUM_4,&massiv[0],4);
				    if (massivK1[0]==0xd3) memcpy(&p->U.SUM_20,&massiv[0],4);
					for(i=6;i<CK1;i++) massivK1[i-6]=massivK1[i]; 						
				    printf("d3 %x %d\n",massiv,massiv);
				    //fwrite( &massiv, 1, 6, out_fp );	///p->U.OI///??????	
					
				    CK1-=6;break;							
		default :   
					for(i=1;i<CK1;i++) massivK1[i-1]=massivK1[i]; 							
					CK1-=1;
	}
	
  }
  return b;
}

void writePorogs(float porog_si, float porog_d) {
int i;
printf("Установлены пороги СИ=%e & ИНФ=%e\n", porog_si, porog_d);

memcpy(&porog_i,&porog_si,4);
POROGS[2]=porog_i & 0x7f;porog_i=porog_i>>7;
POROGS[3]=porog_i & 0x7f;porog_i=porog_i>>7;
POROGS[4]=porog_i & 0x7f;porog_i=porog_i>>7;
POROGS[5]=porog_i & 0x7f;porog_i=porog_i>>7;
POROGS[6]=porog_i & 0x7f;

//порог Д
memcpy(&porog_i,&porog_d,4);
POROGD[2]=porog_i & 0x7f;porog_i=porog_i>>7;
POROGD[3]=porog_i & 0x7f;porog_i=porog_i>>7;
POROGD[4]=porog_i & 0x7f;porog_i=porog_i>>7;
POROGD[5]=porog_i & 0x7f;porog_i=porog_i>>7;
POROGD[6]=porog_i & 0x7f;
	// Посылка порога в DSP порога для синхроимпульса
	wr_cpcs_s.type=5;
	wr_cpcs_s.cnl=chan1;
	wr_cpcs_s.cnt=7;
	for(i=0;i<7;i++) wr_cpcs_s.uom.dt[i]=POROGS[i];// зп массива 7 байт
	Send(pid_drv,&wr_cpcs_s,&wr_cpcs_r,sizeof(wr_cpcs_s),sizeof(wr_cpcs_r));
	delay(50);
	
	// Посылка порога в DSP порога для инф сигнала
	wr_cpcs_s.type=5;
	wr_cpcs_s.cnl=chan1;
	wr_cpcs_s.cnt=7;
	for(i=0;i<7;i++) wr_cpcs_s.uom.dt[i]=POROGD[i];// зп массива 7 байт
	Send(pid_drv,&wr_cpcs_s,&wr_cpcs_r,sizeof(wr_cpcs_s),sizeof(wr_cpcs_r));
	delay(50);
}

void writeDopler(float dpl) 
{
int i;
printf("Установлено значение Доплера %e \n", dpl);

memcpy(&porog_i,&dpl,4);
POROGS[2]=porog_i & 0x7f;porog_i=porog_i>>7;
POROGS[3]=porog_i & 0x7f;porog_i=porog_i>>7;
POROGS[4]=porog_i & 0x7f;porog_i=porog_i>>7;
POROGS[5]=porog_i & 0x7f;porog_i=porog_i>>7;
POROGS[6]=porog_i & 0x7f;

	// Посылка порога в DSP порога для синхроимпульса
	wr_cpcs_s.type=5;
	wr_cpcs_s.cnl=chan1;
	wr_cpcs_s.cnt=7;
	for(i=2;i<7;i++) wr_cpcs_s.uom.dt[i]=POROGS[i];// зп массива 7 байт
	wr_cpcs_s.uom.dt[0]=0xD6;
	wr_cpcs_s.uom.dt[1]=0x48;

	Send(pid_drv,&wr_cpcs_s,&wr_cpcs_r,sizeof(wr_cpcs_s),sizeof(wr_cpcs_r));
	delay(50);	
}

/////////////////////////////////////////////////////////////////////////////////////////
