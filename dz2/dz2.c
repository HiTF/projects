#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <unistd.h> 
#include <string.h> 
/* !программа использует файлы: 
 * timew.txt - содерижт информацию о времени мойки посуды
 * timep.txt  - содержит информацию о времени сушки посуды
 * w.txt - содержит информацию о грязной посуде
 * table.txt - выступает в качестве стола
 */ 
#define THREADS_COUNT 2
#define TABLE_LIMIT 10 //лимит для стола
#define POSUDA_NAME 20 //для длины имени посуды
#define QUANITY 100 //количество позиций грязной посуды/типов посуды(для времени)
int TABLE = 0; //глобальный счетчик для стола

struct sembuf mybuf;
typedef struct posuda { //структура, с помощью которой будет записана в память вся исходная грязная посуда 
	char name[POSUDA_NAME]; 
	int qua;  
} posuda_t;
typedef struct ptime { //стуктура, с помощью которой буду хранится данные о времени сушки и мойки
	char name[POSUDA_NAME]; 
	int t;  
} ptime_t;

int semid = -1;
FILE *table;
   
void SemOp(int sem_num, int n)
{
	mybuf.sem_num = sem_num;
	mybuf.sem_op = n;
	mybuf.sem_flg = 0;
}

int waiting(int j){
	mybuf.sem_num = j;
    mybuf.sem_flg = 0;
    return mybuf.sem_op;
}
/*функция мойщика: мы считываем и заносим в массив структур время для посуды каждого типа
 * считываем в массив структур данные о грязной посуде
 * мы запускаем цикл "мойки", мы проходим по массиву структур грязной посуды, определяем тип взятой посуды и уходим в спячку на время "мойки"
 * далее мы записываем строку с наименованием типа посды вфайла table, увеличиваем глобальный счетчик на 1 
 * после цикла записываем в конец "ALL"
 */ 
 void* washer(void* dummy) { 
	FILE *time_file,*w_file;
	ptime_t timew[QUANITY];
	posuda_t gruz[QUANITY];
	int i=0;
	int b = 0;
	char str[POSUDA_NAME];
	time_file = fopen("timew.txt", "r");
	while (fscanf (time_file, "%s%d", timew[i].name, &(timew[i].t)) != EOF) {
		i++;
	}
	fclose(time_file);
	i=0;
	w_file = fopen("w.txt", "r");
	while (fscanf (w_file, "%s%d", gruz[i].name, &(gruz[i].qua)) != EOF) {
		i++;
	}	
	int y = 0;
	for(y = 0; y < i; y++){
		int k = 0;
		while(strcmp(gruz[y].name,timew[k].name) != 0){
			k++;
		}
		k = timew[k].t;
		for(int q = gruz[y].qua; q > 0; q--){	
			sleep(k);
			while(1){
				if (TABLE_LIMIT > TABLE){
					if (waiting(0) != -1){
						SemOp(1, -1);
						fseek(table, 0, SEEK_SET);
						for(int e = 0; e < (b+1); e++){
							fscanf (table, "%s", str);
						}
						fputs(gruz[y].name, table);
						fputs("\n", table);
						SemOp(1, 1);
						break;
					}
				}
			}
			TABLE++;
			b++;
			printf("единица %s помыта и уложена на стол\n",(gruz[y].name));
		}
	}
	while(1){
		fseek(table, 0, SEEK_SET);
		for(int e = 0; e < (b+1); e++){
				fscanf (table, "%s", str);
		}
		fputs("ALL", table);
		fputs("\n",table);
		TABLE++;
		SemOp(1,  1);
		break;
	}
	fclose(w_file);
	return NULL;
}
/*функция сушильщика: мы считываем и заносим в массив структур время для посуды каждого типа
 * далее мы считыаем строку из файла table, уменьшаем глобальный счетчик на 1 и уходим в спячку на время "сушки"
 * так в бесконечном цикле, пока не считаем "ALL"
 */ 
void* wiping(void* dummy) {
	FILE *time_file;
	ptime_t timep[QUANITY];
	int i = 0;
	int k = 0;
	int b = 0;
	char str[POSUDA_NAME];
	time_file = fopen("timep.txt", "r");
	while (fscanf (time_file, "%s%d", timep[i].name, &(timep[i].t)) != EOF) {
		i++;
	}
	fclose(time_file);	
	while (1){
		if (TABLE < 100){
			SemOp(0, 1);
			}
		if ((TABLE > 0)&&(waiting(0) > -1)){
			SemOp(0,-1);
			fseek(table, b,SEEK_SET);
			fscanf (table, "%s", str);
			b=b+strlen(str)+1;
			SemOp(0,1);
			if (strcmp(str,"ALL")==0){
				exit(-1);
			}else{
				while(strcmp(str,timep[k].name)!=0){
					k++;
				}
			TABLE--;
			int j = timep[k].t;
			sleep(j);
			printf("Единица посуды %s протерта\n",(timep[k].name));
			k = 0;
			}
		}
	}
	return NULL;
}
 
int main()
{
	char pathname[] = "dz2.c";
	key_t key = ftok(pathname, 0);
    if((semid = semget(key, 2, 0666 | IPC_CREAT)) < 0) {
		printf("Can\'t get semid\n");
		exit(-1);
	}
	SemOp(0, 1);
	SemOp(1, 1);
	table = fopen("table.txt", "w+");
	pthread_t thread_ids[THREADS_COUNT];
	pthread_create(&thread_ids[0], (pthread_attr_t *)NULL, &washer, table);
	pthread_create(&thread_ids[1], (pthread_attr_t *)NULL, &wiping, table);
	int i;
	for (i = 0; i < THREADS_COUNT; ++i){
		pthread_join(thread_ids[i], (void **) NULL);
	}
	fclose(table);
	return 0;
}
