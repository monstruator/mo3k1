
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
#define SUM_DCP_K1			1250	//max �᫮  �ᯮ��㥬�� ᫮� � ���ᨢ� ��� K1
#define SUM_DCP_K2			6250	//max �᫮  �ᯮ��㥬�� ᫮� � ���ᨢ� ��� K2



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
			 c_OI; //���-�� ��
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
	unsigned short M[4]; //������� �ࠢ����� ��.1
	short jump; //��ॡ�� ��⥭��
	short no_navi; //������⢨� ������樨
	short num_com; //��騩 �����
	unsigned int pr1_c; //����� � ��.1
	UPOS 		   U;
	obmen_MO3_MO3K_t  from_MO3;	
    obmen_MO3K_MO3_t  to_MO3;		
	unsigned short PR1[8];//����� ��ࢮ�� �ਡ��
	unsigned short toPR1[8];//����� ��� ��. 1.0
	unsigned short Dout41[60];
	simfonia41_t	simfonia41;		//������樮��� ����� ����������� 2��

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
			 c_OI; //���-�� ��
	//v 4.1-----------------------------------------------
	float			P_FACT;			//������, ࠤ
	float			beta_FACT;		//㣮� ����, ࠤ
	short			UR_sign_K1;		//�஢��� ᨣ���� �� ������ �1 (0-100) -⠡�� ᨣ���� �1
	short			UR_sign_K2;		//�஢��� ᨣ���� �� ������ �2 (0-100) -⠡�� ᨣ���� �2
	short			GL_priem;		//����稥 ᨣ���� "�ਥ�" �� ��室� ���ਨ
									//(1-���� ᨣ���, 0- ��� ᨣ����)
									//(⠡�� �����-�ਥ�)	
	short			GL_CP;			//	����稥 ᨣ���� "��" �� ��室� ���ਨ
									//(1-���� ᨣ���, 0- ��� ᨣ����)
									//(⠡�� �����-��)	
	unsigned short	cr_SEANCE;		//���浪��� ����� ���ᨢ� ������ 横�� ��।�� 
									//0-USHRT_MAX
	short			sum_DCP;		//������⢮ �ਭ���� ᫮� � ���ᨢ� ��
	//v 4.2-----------------------------------------------
	unsigned short	Mispr;		//��᪠ ��ࠢ���� ��������� �� 3.1
	//iz 4.1----------------------------------------------
	unsigned short	cr_com;	//���浪��� ����� ��᫥���� �������, �뤠���� �� ��.4.1 � ��.3.1 
							//0-USHRT_MAX
	short			num_com;//����� ��᫥���� �뤠���� �������
							//������� 1 - �����⮢�� � ᥠ��� �裡
							//������� 2 - ��砫� ᥠ�� �裡
							//������� 3 - �����襭��  ᥠ�� �裡
							//��ࠬ���� ������� 1
	short			num_KS;	//����� ������ �裡 1 - ��-�1, 2 - ��-�2 
	short			Nkey_SHAKR;	// ����� ���� ���� 
	float			P_ANT_1;	//������ ��⥭�� (� ������ ��砫� ᥠ��), ࠤ
	float			beta_1;		//㣮� ���� ��⥭�� (� ������ ��砫� ᥠ��), ࠤ
	float			Fd_1;		//ᤢ�� ����� (� ������ ��砫� ᥠ��), ��� 
	int				T_SS;		//�६� ��砫� �� ᥪ � �ଠ� ���
								//(�६� ��ࢮ�� ����祭�� ��� ��᫥ �室� �� � ��� �裡)
								//��������� ��⥭�� � ᤢ�� ����� �� ������ ⠪�
	float			P_ANT;		//������ ��⥭��, ࠤ
	float			beta;		//㣮� ���� ��⥭��, ࠤ
	float			Fd;			//ᤢ�� �����, ��� 
}LOG_MEM;