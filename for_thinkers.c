#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include <strings.h>
#include <semaphore.h>



pthread_mutex_t* Forks;
int* Thinkers_hunger;
int N;

void get_Lfork(int name)
{
	if (name == 0)  pthread_mutex_lock(Forks + N - 1);
	else            pthread_mutex_lock(Forks + name - 1);
}
void get_Rfork(int name)
{
	pthread_mutex_lock(Forks + name);
}
void put_Lfork(int name)
{
	if (name == 0)  pthread_mutex_unlock(Forks + N - 1);
	else            pthread_mutex_unlock(Forks + name - 1);
}
void put_Rfork(int name)
{
	pthread_mutex_unlock(Forks + name);
}




int tryget_Lfork(int name)
{
	if (name == 0)  return pthread_mutex_lock(Forks + N - 1);
	else            return pthread_mutex_lock(Forks + name - 1);
}
int tryget_Rfork(int name)
{
	return pthread_mutex_lock(Forks + name);
}

void* eat0(int* pName)
{
	int name = *pName;
	while (Thinkers_hunger[name] > 0){
		get_Lfork(name);
		get_Rfork(name);
		Thinkers_hunger[name]--;
		printf("Ph %d is eating %d sec, hunger left: %d\n", name, 3, Thinkers_hunger[name]);
		sleep(3);
		put_Lfork(name);
		put_Rfork(name);
		printf("Ph %d is thinking %d sec\n", name, 3);
		sleep(3);
	}
	printf("Ph %d is fed and now will only think\n", name);
}
void* eat1(int* pName)
{
	int name = *pName;
	while (Thinkers_hunger[name] > 0){
		int t;
		if (tryget_Lfork(name) != 0){
			put_Rfork(name);
			t = rand()%5+1;
			printf("Ph %d is thinking %d sec\n", name, t);
			sleep(t);
		}
		if (tryget_Rfork(name) != 0){
			put_Lfork(name);
			t = rand()%5+1;
			printf("Ph %d is thinking %d sec\n", name, t);
			sleep(t);
		}
		Thinkers_hunger[name]--;
		printf("Ph %d is eating %d sec, hunger left: %d\n", name, 3, Thinkers_hunger[name]);
		sleep(3);
		put_Lfork(name);
		put_Rfork(name);
		printf("Ph %d is thinking %d sec\n", name, 3);
		sleep(3);
	}
	printf("Ph %d is fed and now will only think\n", name);
}

int main()
{
	int Hunger, algo, *Nums;
	pthread_t* P;
	while (1){
		printf("Please, enter count of thinkers, type of algorithm(0 or 1) and their hunger:\n");
		scanf("%d%d%d", &N, &algo, &Hunger);
		if (N == 0) break;
	
		Forks = (pthread_mutex_t*)malloc(N * sizeof(pthread_mutex_t));
		Thinkers_hunger = (int*)malloc(N * sizeof(int));
		P = (pthread_t*)malloc(N * sizeof(pthread_t));
		Nums = (int*)malloc(N * sizeof(int));
		int i;
		for (i = 0; i < N; i++){
			pthread_mutex_init(Forks + i, NULL);
			Thinkers_hunger[i] = Hunger;
			Nums[i] = i;
		}
		
		for (i = 0; i < N; i++){
			if (algo == 0) pthread_create(P + i, NULL, (void * (*)(void *))eat0, Nums + i);
			else	       pthread_create(P + i, NULL, (void * (*)(void *))eat1, Nums + i);
		}
		
		
		for (i = 0; i < N; i++){
			pthread_join(P[i], NULL);
		}
		for (i = 0; i < N; i++){
			pthread_mutex_destroy(Forks + i);
		}
		free(Forks);
		free(Thinkers_hunger);
		free(P);
		free(Nums);
		printf("Now all thinkers are fed. Thank you!\n");
	}
	exit(0);
}







