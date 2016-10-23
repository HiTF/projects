#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>
#include<string.h>

#define M 100000000

const int max = 1000000;
float  d = 0;
float  s = 0;

typedef struct {
  float *A;
  float *B;
  float *a;
} series_t;

typedef struct {
  int thread;
  int nthread;
 
  series_t* cpseries;
} order_t;

void *secondary(void *arg){
	//pthread_t mythid;
	//mythid = pthread_self();
	order_t* order = (order_t*) arg;
	unsigned int i;
	for(i = (((order->thread))*M/(order->nthread)); i < (((order->thread)+1)*M/(order->nthread)); i += 1) {
		*(order->cpseries->B) += *(order->cpseries->A+i);
		*(order->cpseries->A+i) += (*(order->cpseries->A+i))*(*(order->cpseries->A+i));
		*(order->cpseries->a) += *(order->cpseries->A+i);
		//printf("!%d   -  %d ",(((order->thread)-1)*M/(order->nthread)),(((order->thread))*M/(order->nthread)));
		//printf("%f \n",*(order->cpseries->A+i));
		}
		//printf("%d ",((((order->thread))*M/(order->nthread))));
		//printf("%d ",((((order->thread)+1)*M/(order->nthread))));
		*(order->cpseries->B) = (*(order->cpseries->B)/M*(order->nthread));
		*(order->cpseries->a) = (*(order->cpseries->a)/M*(order->nthread));
		d += *(order->cpseries->B);
		//printf("%f  ",*(order->cpseries->b));
		s += *(order->cpseries->a);
		//printf("%u ",(mythid));
		;
	
    return NULL;
}

int main(/*int argc, char *argv[]*/){
	srand ( time(NULL) );    
	series_t ff;
	unsigned int i = 0;
	int nthread;
	nthread = sysconf(_SC_NPROCESSORS_ONLN);
	ff.A = (float*) malloc(sizeof(float) * M);
	ff.B = (float*) malloc(sizeof(float) * nthread);
	for(i = 0; i < M; i++){
		*(ff.A + i) = (float) rand() / RAND_MAX *(max);
	}	
	printf("ряд сгенерирован\n");
	pthread_t *tid = (pthread_t*) malloc(sizeof(pthread_t) * nthread);
	for(int i = 0; i < nthread; ++i) {

		order_t* order = (order_t*) malloc(sizeof(order_t));
		if(order == NULL) {
			perror("Can't memory");
			exit(EXIT_FAILURE);
		}
		series_t* copymain = malloc(sizeof(series_t));
		copymain->A = (float*) malloc(sizeof(float) * M);
		copymain->B = (float*) malloc(sizeof(float) * nthread);
		memcpy(copymain->A, ff.A, sizeof(float)*M);
		memcpy(copymain->B, ff.B, sizeof(float)*nthread);
		copymain->a = (float*) malloc(sizeof(float)*nthread);
		memcpy(copymain->a, ff.a, sizeof(float)*nthread);
		order->cpseries = copymain;
		order->thread = i;
		order->nthread = nthread;

		int status = pthread_create(&tid[i], NULL, secondary, order);
		if(status != 0) {
			perror("failed");
			exit(EXIT_FAILURE);
		}
		
  }
	for(int i = 0; i < nthread; ++i) {
    pthread_join(tid[i], NULL);
  }
	
	d = d/nthread;
	printf("%f ",(d));
	s = s/nthread;
	printf("%f ",(s-d));
	return 0;
}
