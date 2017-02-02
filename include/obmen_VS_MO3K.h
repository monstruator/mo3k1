
#include "obmenVS_AK_MO3K.h"
#include "obmen_220113.h"

#define PI 3.14159 
#define NAVtoRAD PI/(65356)
#define Kmema 12.27
#define Maxncu 0x4AA
#define Minncu 0xF55
#define Maxmema 0x7A2
#define Minmema 0x85D
#define GRADtoRAD  0.00153398
#define RADtoGRAD 651.898648
#define pi 	3.14159  
#define Kncu 	12.27
#define msec 1000000
#define Kq 	0.5
#define P 1 //  pa3peweHue 1/3anpem 0 ne4amu
#define tow 50 // napaMemp owugaHuR uMeH=5c
#define RAD 5729.57	
#define SUM_DCP_K1			1250	//max число  используемых слов в массиве ДЦП K1
#define SUM_DCP_K2			6250	//max число  используемых слов в массиве ДЦП K2



typedef struct
{
	float   RAZN_0,
			RAZN_1,
			SUM_4,
			SUM_20;
	short 	DPL_1;
unsigned int DPL_2,
			 ZI_DATA,
			 ZI_DOST,
			 OI[SUM_DCP_K1],
			 c_OI; //кол-во ОИ
} UPOS;

	struct ispr_mo3k {
		//
		unsigned int cvsA : 1;
		unsigned int cvsB : 1;
		unsigned int k1 : 1;
		unsigned int k2 : 1;

		unsigned int nkA : 1;
		unsigned int nkB : 1;
		unsigned int sevA : 1;
		unsigned int mo1k : 1;
		//  
		unsigned int gl : 1;
		unsigned int sevB : 1;
		unsigned int reserv : 6;
	};
	
		 
typedef struct 
{
	short lvlK2;
	unsigned short CEB[6];
	unsigned short M[4]; //команды управления пр.1
	short jump; //переброс антенны
	short no_navi; //отсутствие навигации
	short num_com; //общий номер
	unsigned int pr1_c; //обмен с пр.1
	UPOS 		   U;
	obmen_MO3_MO3K_t  from_MO3;	
    obmen_MO3K_MO3_t  to_MO3;		
	unsigned short PR1[8];//данные первого прибора
	unsigned short toPR1[8];//данные для пр. 1.0
	unsigned short Dout41[60];
	simfonia41_t	simfonia41;		//навигационные данные обновляются 2гц

}SHMEM_31;


typedef struct 
{
	unsigned short CEB[6];
	// UPOS-----------------------------------------------
	float   RAZN_0,
			RAZN_1,
			SUM_4,
			SUM_20;
	short 	DPL_1;
	unsigned int DPL_2,
			 ZI_DATA,
			 ZI_DOST,
			 c_OI; //кол-во ОИ
	//v 4.1-----------------------------------------------
	float			P_FACT;			//азимут, рад
	float			beta_FACT;		//угол места, рад
	short			UR_sign_K1;		//уровень сигнала по каналу К1 (0-100) -табло сигнала К1
	short			UR_sign_K2;		//уровень сигнала по каналу К2 (0-100) -табло сигнала К2
	short			GL_priem;		//наличие сигнала "прием" на выходе Глории
									//(1-есть сигнал, 0- нет сигнала)
									//(табло Глория-прием)	
	short			GL_CP;			//	наличие сигнала "ср" на выходе Глории
									//(1-есть сигнал, 0- нет сигнала)
									//(табло Глория-ср)	
	unsigned short	cr_SEANCE;		//порядковый номер массива данных цикла передачи 
									//0-USHRT_MAX
	short			sum_DCP;		//количество принятых слов с массиве дцп
	//v 4.2-----------------------------------------------
	unsigned short	Mispr;		//Маска исправности аппаратуры пр 3.1
	//iz 4.1----------------------------------------------
	unsigned short	cr_com;	//порядковый номер последней команды, выданной из пр.4.1 в пр.3.1 
							//0-USHRT_MAX
	short			num_com;//номер последней выданной команды
							//команда 1 - подготовка к сеансу связи
							//команда 2 - начало сеанса связи
							//команда 3 - завершение  сеанса связи
							//параметры команды 1
	short			num_KS;	//номер канала связи 1 - нр-к1, 2 - пр-к2 
	short			Nkey_SHAKR;	// номер ключа ШАКР 
	float			P_ANT_1;	//азимут антенны (в момент начала сеанса), рад
	float			beta_1;		//угол места антенны (в момент начала сеанса), рад
	float			Fd_1;		//сдвиг частоты (в момент начала сеанса), Кгц 
	int				T_SS;		//время начала СС сек в формате СЕВ
								//(время первого включения КПИ после входа КА в круг связи)
								//положение антенны и сдвиг частоты на каждом такте
	float			P_ANT;		//азимут антенны, рад
	float			beta;		//угол места антенны, рад
	float			Fd;			//сдвиг частоты, Кгц 
}LOG_MEM;