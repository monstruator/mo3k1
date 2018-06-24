if (p->from_MO3.from42.Rejim_AS==1) //режим АС
					if(p->count_as>4)
					{
						for(i=0;i<5;i++) p->mass_lvl[STEP_AS]+=p->lvl_as[i];
						p->mass_lvl[STEP_AS]=p->mass_lvl[STEP_AS]/5;//считаем среднее
						p->count_as=0;
						switch(STEP_AS)
						{
						case 0:  printf("STEP0->STEP1 lvl0=%d\n",p->mass_lvl[STEP_AS]);	
								 STEP_AS=1; break;
						case 1: 
							if (p->mass_lvl[STEP_AS]>p->mass_lvl[STEP_AS-1]) {printf("STEP1->STEP2 lvl1=%d\n",p->mass_lvl[STEP_AS]); STEP_AS=2;} //если уровень увеличился, то едем дальше. 
							else {printf("STEP1->STEP6 lvl1=%d\n",p->mass_lvl[STEP_AS]); STEP_AS=6;	}	//иначе едем назад
							break;
						case 2:	if (p->mass_lvl[STEP_AS]>p->mass_lvl[STEP_AS-1]) { printf("STEP2->STEP3 lvl2=%d\n",p->mass_lvl[STEP_AS]); STEP_AS=3;} //если уровень увеличился, то едем дальше. 
							else { printf("STEP2->STEP7 lvl2=%d\n",p->mass_lvl[STEP_AS]); STEP_AS=7; }	//иначе стоим и ждем изменения уроовня
							break;
						case 3:	if (p->mass_lvl[STEP_AS]>p->mass_lvl[STEP_AS-1]) { printf("STEP3->STEP4 lvl3=%d\n",p->mass_lvl[STEP_AS]); STEP_AS=4;} //если уровень увеличился, то едем дальше. 
							else { printf("STEP3->STEP7 lvl3=%d\n",p->mass_lvl[STEP_AS]); STEP_AS=7; }	//иначе стоим и ждем изменения уроовня
							break;
						case 4:	if (p->mass_lvl[STEP_AS]>p->mass_lvl[STEP_AS-1]) { printf("STEP4->STEP5 lvl4=%d\n",p->mass_lvl[STEP_AS]); STEP_AS=4;} //если уровень увеличился, то едем дальше. 
							else { printf("STEP4->STEP7 lvl4=%d\n",p->mass_lvl[STEP_AS]); STEP_AS=7; }	//иначе стоим и ждем изменения уроовня
							break;
						case 5:	if (p->mass_lvl[STEP_AS]>p->mass_lvl[STEP_AS-1]) { printf("STEP5->STEP7 lvl5=%d\n",p->mass_lvl[STEP_AS]); STEP_AS=7;} //если уровень увеличился, то едем дальше. 
							else { printf("STEP5->STEP7 lvl5=%d\n",p->mass_lvl[STEP_AS]); STEP_AS=7; }	//иначе стоим и ждем изменения уроовня
							break;							
						case 7:
							
							break;
							
						}
					
					/*printf("lvl = %d r0 = %f",p->to_MO3.to41.UR_sign_K1,p->U.RAZN_0);
					printf("Pr1=%d A1=%d newPr1+%d\n",p->PR1[0]&0x0fff,A1,p->toPR1[0]);*/
					}
					else
					{
						if(STEP_AS<6)	p->toPR1[0]=KK1*RADtoGRAD/2+1991+STEP_AS*15;//Азимут
						else 			p->toPR1[0]=KK1*RADtoGRAD/2+1991+(5-STEP_AS)*15;
					}