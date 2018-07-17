#include <sys/types.h>
#include <sys/socket.h>
#include <sys/kernel.h>
#include <netinet/in.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <netdb.h>
#include <math.h>
#include "../include/shared_mem.h"
#include "../include/gloriya.h"
#include "../include/IO_DT_SOI.h"
#include "../include/func_IP.h"

#define SRC_PORT41 8206
#define DST_PORT41 8206

# define max_len_OUT    4096*8
# define max_len_IP     4096*8

static Udp_Client_t Uc41;
char bufi[1024], gloria_start=0, gloria_count=90;	
obmen_MO3_MO3K_t rec4;
unsigned int pr1_c_old=0, i;
unsigned int AK_c=0; 																//счетчик обмена после поступления команды АК
short cr_com41=0,cr_com42=0,cr_comAK=0;	
int paramAKcom=0,bytes;
short byta2,len_OUT,j;
div_t   vol;   				 														// vol.quot - количество полных томов
char          pack_buf[1500];  														// буфер задачи obm_41_31. Выходные данные в Socket
char                 numb_pack,     												// текущий номер пакета
                         numb_vol;      											// текущий номер тома в пакете

struct   
{ 
     char      out_buf[max_len_OUT];  												// данные для Socket'a
} ip_out; 

	  
main ()
{
	float 	C1,C2,C3,C4,C5,C6,C7,C8;
	int i1=0,i2=0,gss=0,i3,KAR;
	int rez;
	unsigned short buf; 															//для переворота управляющих слов пр. 1.0
	struct ispr_mo3k *ispr;

	C1=2048./pi;C2=4096.0/360.0;C3=180./pi;C4=C1*Kncu;
	C5=C2*Kncu;C6=C1*Kq;C7=C3;C8=C2*Kq;

	//------------------------------------------- инициализация канала UDP ---------------------------------------------------
	i = Udp_Client_Ini(&Uc41,"194.1.1.7",SRC_PORT41,DST_PORT41);
	printf(" Udp_Init=%d	\n", i);
			
	delay(4000);
	open_shmem(); 																	//открытие общего поля
	delay(1000);

	memset(&p->to_MO3,0,sizeof(obmen_MO3K_MO3_t));
	//p->to_MO3.to42.Mispr = 0xFFFF;
	ispr = (struct ispr_mo3k *) & p->to_MO3.to42.Mispr;
	
	while(1)
	{
		bytes = Udp_Client_Read(&Uc41,bufi,4096);
		//printf(" read=%d size1=%d size2=%d size3=%d sizeALL=%d\n",
		//bytes,sizeof(obmen_42_31_2t),sizeof(obmen_41_31_2t),sizeof(obmen_AK_MN3_MO3K_t),sizeof(obmen_MO3_MO3K_t));
		memcpy(&rec4,&bufi[4],sizeof(obmen_MO3_MO3K_t));
		//-------------------------------------- выбор управляющей команды ---------------------------------------------------
		if (rec4.from42.cr_com!=cr_com42) 
		{	
			printf(" New Command 4.2  = %d (4.2) cr_com = %d\n",	
			rec4.from42.num_com,p->from_MO3.from42.cr_com);

			p->num_com=rec4.from42.num_com;
			cr_com42=rec4.from42.cr_com;
			p->from_MO3.from42=rec4.from42;											//копируем при утверждении команды
			if ((p->num_com==12)||(p->num_com==14)) 
			{
				gloria_count=100;
				p->to_MO3.to42.status_test=1;
				p->to_MO3.to42.count_test++;
			}
			if ((p->num_com==11)||(p->num_com==13)) 
			{
				p->to_MO3.to42.status_test=1;
				p->to_MO3.to42.count_test++;
			}
			if (p->num_com==5)
			{
				p->M[0]=p->from_MO3.from42.M1;
				p->M[1]=p->from_MO3.from42.M2;
				p->M[2]=p->from_MO3.from42.M3&0xFFFE;  // ant = 1 / ekv = 0
				//------------- vremenno ----------------------------------------------------
				p->M[3]=p->from_MO3.from42.M4;
				printf("m0=%d, M1=%d, M2=%d, M3=%d\n", p->M[0], p->M[1], p->M[2], p->M[3]);
				buf=0;																//переворачиваем управляющие слова пр. 1
				for(i1=0;i1<16;i1++) {buf+=((p->M[0]>>i1)&1)<<(15-i1);} p->M[0]=buf;buf=0;
				for(i1=0;i1<16;i1++) {buf+=((p->M[1]>>i1)&1)<<(15-i1);} p->M[1]=buf;buf=0;
				for(i1=0;i1<16;i1++) {buf+=((p->M[2]>>i1)&1)<<(15-i1);} p->M[2]=buf;buf=0;
				for(i1=0;i1<16;i1++) {buf+=((p->M[3]>>i1)&1)<<(15-i1);} p->M[3]=buf;buf=0;
			}
		}
		if ((rec4.fromAK.cr_com!=cr_comAK)&&(rec4.fromAK.num_com!=0)) 
		{
			p->from_MO3.fromAK=rec4.fromAK;
			p->num_com=rec4.fromAK.num_com;
			cr_comAK=rec4.fromAK.cr_com;
			pr1_c_old=p->pr1_c;														//сохраним счетчик обмена с пр.1
			AK_c=1;
			p->to_MO3.toAK.kzv=0;
			printf(" New Command AK = %d (4.2), p[0]=%d , cr_com = %d\n",
			p->from_MO3.fromAK.num_com,p->from_MO3.fromAK.a_params[0],p->from_MO3.fromAK.cr_com);
			if ((p->num_com==301)&&(p->num_com==300)) p->M[2]=p->M[2]|0x0001; 		//РЭЛЕ АК
			paramAKcom=0;
			//if (p->num_com==292)&&(p->from_MO3.fromAK.a_params[0]==6))
			switch(p->num_com)
			{
				case 300 : 	switch(p->from_MO3.fromAK.a_params[0])
						{
							case 0: if (p->from_MO3.fromAK.a_params[1]==1) 
										{paramAKcom=1;p->M[2]=p->M[2]|0x0001;printf("AK\n");} else {printf("KA\n");paramAKcom=2;p->M[2]=p->M[2]&0xFFFE;}break;
							case 1:	if (p->from_MO3.fromAK.a_params[1]==1) 
										{paramAKcom=3;p->M[2]=p->M[2]|0x0001;printf("AK\n");} else {printf("KA\n");paramAKcom=4;p->M[2]=p->M[2]&0xFFFE;}break;
						}
				break;
				
				case 202 :  paramAKcom=5; break;
			
				case 264 :  paramAKcom=6; break;
				
				case 265 : 	switch(p->from_MO3.fromAK.a_params[0])
						{
							case 2:	case 3: paramAKcom=7;break; 
							case 4: case 5: 
							case 6:	case 7: 
							case 8: case 9: paramAKcom=8;break;
						}
				break;
				
				case 281 : 	switch(p->from_MO3.fromAK.a_params[0])
						{
							case 1:	paramAKcom=9;break; 
							case 2: paramAKcom=10;break;
						}
				break;			
			
				case 290 :	paramAKcom=11; break;									//8410			
			
				case 292 : 	switch(p->from_MO3.fromAK.a_params[0])
						{
							case 1:	case 2:
							//case 4:	case 5:
							//case 7:	paramAKcom=15;break; 
							//case 7:
							//case 8:	
							paramAKcom=14;break; 
							case 3: paramAKcom=13;break;
							case 9: paramAKcom=12;break;
						}
				break;	
			}
			switch(paramAKcom)
			{
				case 1  : p->M[3]=(p->M[3]&0x0380)|0x8400;break;
				case 2  : p->M[3]=(p->M[3]&0x0380)|0x8000;break;
				case 3  : p->M[3]=(p->M[3]&0x0380)|0xEC30;break;
				case 4  : p->M[3]=(p->M[3]&0x0380)|0xE830;break;
				case 5  : switch(p->from_MO3.fromAK.a_params[0])
						{
							case 1: KAR=4;break;
							case 2: KAR=2;break;
							case 3: KAR=6;break;
							case 4: KAR=1;break;
							case 5: KAR=5;break;
							case 6: KAR=3;break;
						}
						p->M[3]=(p->M[3]&0xF87F)|(KAR<<7);
				break;
				case 6  : case 12 : p->M[3]=0x9450;break;
				case 7  : case 13 : p->M[3]=0x8C30;break;
				case 8  : case 11 : case 14 : p->M[3]=0x8410;break;
				case 9  : p->M[3]=0x8400;break;
				case 10 : p->M[3]=0x0400;break;
				case 15 : p->M[3]=0x8210;break;
			}
			printf("M=%d\n",paramAKcom); 
		}

		if (rec4.from41.cr_com!=cr_com41) 
		{
			if (p->num_com!=rec4.from41.num_com)
			printf(" New Command 4.1 = %d\n",rec4.from41.num_com);
		
			p->from_MO3.from41=rec4.from41;
			p->num_com=p->from_MO3.from41.num_com;
			cr_com41=p->from_MO3.from41.cr_com;
			p->M[0]=0x0000;
			p->M[1]=0x000e;
			p->M[2]=p->from_MO3.from42.M3&0xFFFE;
			//p->M[3]=0x8410;		
			if (p->num_com==3) p->to_MO3.to41.pr_GSS=0;
		}
	
		memcpy(&p->to_MO3.SIMF32,&p->Dout41[0],sizeof(p->to_MO3.SIMF32)); 
		memcpy(&p->to_MO3.CEB,&p->CEB,sizeof(p->to_MO3.CEB)); 
		//printf("crcom=%x n_com=%x NKS=%x NSHKR=%x Nd_FR4=%x N_FR4=%x ZUNf=%x N_psp=%x Vr=%f Ar=%f\n",
		//p->from41.cr_com,p->from41.num_com,p->from41.num_KS,
		//p->from41.Nkey_SHAKR,p->from41.Nd_FRCH,p->from41.N_FRCH,
		//p->from41.ZUNf,p->from41.Nans_PSP,p->from41.Vr,p->from41.Ar);

		if (p->U.SUM_20==0) p->to_MO3.to41.Fd_FACT=0;
		else p->to_MO3.to41.Fd_FACT=-(float)0.24414*p->U.DPL_1;
		if (p->to_MO3.to41.UR_sign_K1>17) p->to_MO3.to41.PrM_K1=1;else p->to_MO3.to41.PrM_K1=0;

	//	printf("                                Angle_Pr1 b=%4.3f q=%4.3f   \n",p->to_MO3.to41.beta_FACT*57.3,p->to_MO3.to41.P_FACT*57.3);
	//	printf(" Angle_PR4 = %4.3f %4.3f\n",p->from_MO3.from41.beta*57.3,p->from_MO3.from41.P_ANT*57.3);
	//	printf("					 P_ANT = %4.3f", p->from_MO3.from41.P_ANT*57.3);
	//	printf(" P_FACT = %4.3f  com42=%d \n",p->to_MO3.to41.P_FACT*57.3,p->from42.num_com);

	//printf("n_com1=%d cr_com=%d\n",p->from_MO3.from41.num_com,p->from_MO3.from41.cr_com);
	//printf("Angl 2=%02f r=%f NK%d  \n",from41.P_ANT,p->to_MO3.to41.P_FACT,from41.num_com);
	
		gloria_count++;
		if (gloria_count>100)
		{
			switch (p->num_com)
			{
				case 1 : case 2 :
					if ((p->from_MO3.from41.Nkey_SHAKR<=31)&&(p->from_MO3.from41.Nkey_SHAKR>=0))
					rez=gloriya(1,p->from_MO3.from41.num_KS-1, 31); //p->from_MO3.from41.Nkey_SHAKR !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!VREMENNO!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
				break;

				case 12 :  rez = gloriya(1,0,31);									//test K1
						   printf("rez = gloriya(1,0,31);//test K1 \n\n");
				break;
				
				case 14 :  rez = gloriya(1,1,31);									//test K2
				break;
			    
				default :  rez = gloriya(1,0,31);									//test K1
						//printf("rez = gloriya(1,0,31);//test K1 \n\n");//mode_gl = (mode_gl==1) ? 0 : 1;
						//rez = gloriya(1,mode_gl,31);								//test K
					   	//printf("def: rez = gloriya(1, %d, 31) = %d;//test K%d \n\n", mode_gl,rez,(mode_gl+1));
			}

			if (rez) ispr->gl=0;//p->to_MO3.to42.Mispr=p->to_MO3.to42.Mispr&0xFEFF;
			else 	 ispr->gl=1;//p->to_MO3.to42.Mispr=p->to_MO3.to42.Mispr|0x0100;

			gloria_count=0;
		}

		if ((p->to_MO3.to41.pr_GSS==0)&&((p->num_com==1)||(p->num_com==2)))
		{
			gss++; 																	//считаем время от первой ком нач сеанса
			printf("num_com=%d gss=%d %d\n",p->num_com,p->to_MO3.to41.pr_GSS,gss);
			if (gss>30) 	
			{
				p->to_MO3.to41.pr_GSS=1;	gss=0;
			}
		}
		if (p->num_com==3) {gloria_start=p->to_MO3.to41.pr_GSS=gss=0;}

		//--------------------------------------------------------------------------------------------------
		buf=0;
		for(i1=0;i1<16;i1++) {buf+=((p->from_MO3.from42.M1>>i1)&1)<<(15-i1);} p->from_MO3.from42.M1=buf;buf=0;
		for(i1=0;i1<16;i1++) {buf+=((p->from_MO3.from42.M2>>i1)&1)<<(15-i1);} p->from_MO3.from42.M2=buf;buf=0;
		for(i1=0;i1<16;i1++) {buf+=((p->from_MO3.from42.M3>>i1)&1)<<(15-i1);} p->from_MO3.from42.M3=buf;buf=0;
		for(i1=0;i1<16;i1++) {buf+=((p->from_MO3.from42.M4>>i1)&1)<<(15-i1);} p->from_MO3.from42.M4=buf;buf=0;

		//printf("M1=%x M2=%x M3=%x M4=%x\n",p->from_MO3.from42.M1,p->from_MO3.from42.M2,p->from_MO3.from42.M3,p->from_MO3.from42.M4);

		//---------------------------------------- Для СЕРЕЖИ ---------------------------------------------
		if (p->num_com==9) p->toPR1[6]=0xFA30;	

		//printf("cr_com=%x n_com=%x NKS=%d NSHKR=%x Nd_FR4=%d N_FR4=%x ZUNf=%x N_psp=%x Vr=%f Ar=%f\n",
		//p->from41.cr_com,p->from41.num_com,p->from41.num_KS,
		//p->from41.Nkey_SHAKR,p->from41.Nd_FRCH,p->from41.N_FRCH,
		//p->from41.ZUNf,p->from41.Nans_PSP,p->from41.Vr,p->from41.Ar);
	
		//printf(" %04x   %04x   %04x   %04x ",p->from_MO3.from42.M1,p->from_MO3.from42.M2,p->from_MO3.from42.M3,p->from_MO3.from42.M4);
		//printf(" CR=%d NC=%d \n",p->from_MO3.from42.cr_com,p->from_MO3.from42.num_com);
  
		p->to_MO3.to42.USign=p->PR1[6]; 											//уровень сигнала ПРД из сост ПР1.0
		p->to_MO3.to42.sum_K1=p->U.SUM_4;
		p->to_MO3.to42.D_K1=(float)p->U.DPL_1*244.14;
		//УГЛЫ
		p->to_MO3.to42.q=(p->PR1[0]-1991)*2/RADtoGRAD; 								//АЗИМУТ
		if (p->PR1[2]&0x800) p->to_MO3.to42.beta=(360-p->PR1[2]/C2)/C3; 			//УГОЛ МЕСТА
		else p->to_MO3.to42.beta=-p->PR1[2]/C1;
		if (p->to_MO3.to42.beta>1.823) p->to_MO3.to42.beta=1.8344; 					//5729.57

		//if (p->PR1[1]&0x800) p->to_MO3.to42.alfa=(p->PR1[1]-0xFFF)/12.27/RADtoGRAD;		//КРЕН
		//else p->to_MO3.to42.alfa=p->PR1[1]/C4; 
		p->to_MO3.to42.Ms1=p->PR1[3];   											//состояние прибора 1.0
		p->to_MO3.to42.Ms2=p->PR1[4];
		p->to_MO3.to42.Ms3=p->PR1[5];	
		for(i2=0;i2<16;i2++) {buf+=((p->to_MO3.to42.Ms1>>i2)&1)<<(15-i2);} p->to_MO3.to42.Ms1=buf;buf=0;
		for(i2=0;i2<16;i2++) {buf+=((p->to_MO3.to42.Ms2>>i2)&1)<<(15-i2);} p->to_MO3.to42.Ms2=buf;buf=0;
		for(i2=0;i2<16;i2++) {buf+=((p->to_MO3.to42.Ms3>>i2)&1)<<(15-i2);} p->to_MO3.to42.Ms3=buf;buf=0;

		//---------------------------------------- UPR AK ------------------------------------------------------
		if (AK_c>0) 																//пакет с новой командой АК
		{
			if (p->pr1_c - pr1_c_old>10) 											//больше 2 ответов от Пр.1
			{
				p->to_MO3.toAK.cr_com=p->from_MO3.fromAK.cr_com;
				p->to_MO3.toAK.num_com=p->from_MO3.fromAK.num_com;
				p->to_MO3.toAK.lp2_param=p->from_MO3.fromAK.a_params[0];
				p->to_MO3.toAK.kzv=1;
				AK_c=0;
				printf("paramAKcom=%d\n",paramAKcom);
			
				switch(p->to_MO3.toAK.num_com)
				{
					case 301 : if (p->PR1[3]&0x0004) p->to_MO3.toAK.kzv=0; break;
					default  : if (p->PR1[5]<7) p->to_MO3.toAK.kzv=0;      break; 
				}
				/*switch(paramAKcom)
				{
					case 1 : case 3: case 4: 
						//if ((p->PR1[3]&0x0004)&&(p->PR1[5]&0x0004)) p->to_MO3.toAK.kzv=0;	break;
						if (p->PR1[3]&0x0004) p->to_MO3.toAK.kzv=0;	break;
				
					case 2 : 		  
						if ((p->PR1[3]&0x0004)&&(p->PR1[5]&0x0006)) p->to_MO3.toAK.kzv=0;	break;
						if (p->PR1[3]&0x0004) p->to_MO3.toAK.kzv=0;	break;
				}
				*/
				//p->M[3]=p->M[3]&0xFFFE; 											//Отключение ФК СВЧ КА
				printf("Окончание команды %d. kzv=%d pr1.3=%x pr1.5=%x\n",p->to_MO3.toAK.num_com,p->to_MO3.toAK.kzv,p->PR1[3],p->PR1[5]);
			}
			else 	if (AK_c>10)  //2
			{
				p->to_MO3.toAK.cr_com=p->from_MO3.fromAK.cr_com;
				p->to_MO3.toAK.num_com=p->from_MO3.fromAK.num_com;
				p->to_MO3.toAK.lp2_param=p->from_MO3.fromAK.a_params[0];
				p->to_MO3.toAK.kzv=1;
				AK_c=0;
				printf("Окончание команды %d. Нет ответа от пр. 1. kzv=1\n",p->to_MO3.toAK.num_com);
			}
			else AK_c++; 
		}

		//printf("pr1_c=%d pr1_c_old=%d AK_c=%d\n",p->pr1_c,pr1_c_old,AK_c);
		//printf("num_com=%d kzv=%x\n",p->from_MO3.fromAK.num_com,p->to_MO3.toAK.kzv);
		//---------------------------------------------------------------------------------------------
		len_OUT = sizeof(obmen_MO3K_MO3_t); 										//!!!!
		//printf("size=%d size41=%d size42=%d sizeAK=%d\n",sizeof(obmen_MO3K_MO3_t),sizeof(obmen_31_41_2t),sizeof(obmen_31_42_2t),sizeof(obmen_AK_MO3K_MN3_t));
		memcpy(ip_out.out_buf,&p->to_MO3,sizeof(obmen_MO3K_MO3_t)); 				///!!!!!!
		if (len_OUT <= 1400) 
		{
			pack_buf[0] = 80 ;               
			pack_buf[1] = numb_pack;
			pack_buf[2] = 1;
			pack_buf[3] = 1;
			for (j = 0; j<1400; j++) pack_buf[j+4] = ip_out.out_buf[j];
			Udp_Client_Send(&Uc41,pack_buf,(len_OUT+4));
		}
		else
		{
			pack_buf[0] = 81 ;               
			vol = div(len_OUT,1400);
			pack_buf[1] = numb_pack;
			pack_buf[2] = 1;
			pack_buf[3] = vol.quot + 1;
			for (numb_vol = 1;numb_vol <=vol.quot; numb_vol++)
			{
				for (j = 0; j<1400; j++) {pack_buf[j+4] = ip_out.out_buf[(1400*(numb_vol-1))+j]; }
				Udp_Client_Send(&Uc41,pack_buf,1404);
				if ((numb_vol==3)||(numb_vol==6)||(numb_vol==9)||(numb_vol==12))  delay(2);			//пауза между пакетами
				pack_buf[2] ++ ;
			}
                
			for (j = 0; j<vol.rem; j++) pack_buf[j+4] = ip_out.out_buf[(1400*(vol.quot))+j]; 
			Udp_Client_Send(&Uc41,pack_buf,(vol.rem + 4));
		}
		numb_pack ++;
	}   //------------------------------------------------- END WHILE --------------------------------------
}	//----------------------------------------------------- END MAIN() -------------------------------------
