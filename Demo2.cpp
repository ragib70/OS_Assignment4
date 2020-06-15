#include<iostream>
#include<fstream>
#include<pthread.h>
#include<unistd.h>
#include<semaphore.h>

using namespace std;

int MAXQ = 3;
bool service1 = false;
bool service2 = false;
bool servicedone1 = false;
bool servicedone2 = false;
int count = 0;
sem_t mutex;


void *consumer(void *argaps){

  sem_wait(&mutex);
  long thread_id=long(argaps);
  cout<<"IN CURRENT_CUSTOMER : "<<thread_id<<endl;
  //if(count > MAXQ){
  if(thread_id>=MAXQ){
	    cout<<"IN CURRENT_CUSTOMER : "<<thread_id<<" Left as Queue Full "<<endl;
    	count-=1;
    	sem_post(&mutex);
    	pthread_exit(0);
  }
  //}
  if(service1 == false){
    service1 = true;
    while(!servicedone1);
    servicedone1=false;
    service1 = false;
    count-=1;
    sem_post(&mutex);
  }
  else{
    service2 = true;
    while(!servicedone2);
    servicedone2=false;
    service2 = false;
    count-=1;
    sem_post(&mutex);
  }
  pthread_exit(0);

}

void *service_counter1(void *argaps){
  ofstream outfile("Service_Counter1.txt");
  while(1){
    sleep(2);
    if(service1==false){
      outfile<<"IN SERVICE_COUNTER1 : Sleep"<<endl;
      cout<<"IN SERVICE_COUNTER1 : Sleep"<<endl;
    }
    else{
      outfile<<"IN SERVICE_COUNTER1 : Giving Service"<<endl;
      cout<<"IN SERVICE_COUNTER1 : Giving Service"<<endl;
      sleep(4);
      outfile<<"IN SERVICE_COUNTER1 : Service Done"<<endl;
      cout<<"IN SERVICE_COUNTER1 : Service Done"<<endl;
      servicedone1 = true;
      while(servicedone1);
    }
  }
  pthread_exit(0);
}


void *service_counter2(void *argaps){
  ofstream outfile("Service_Counter2.txt");
  while(1){
    sleep(2);
    if(service2==false){
      outfile<<"IN SERVICE_COUNTER2 : Sleep"<<endl;
      cout<<"IN SERVICE_COUNTER2 : Sleep"<<endl;
    }
    else{
      outfile<<"IN SERVICE_COUNTER2 : Giving Service"<<endl;
      cout<<"IN SERVICE_COUNTER2 : Giving Service"<<endl;
      sleep(4);
      outfile<<"IN SERVICE_COUNTER2 : Service Done"<<endl;
      cout<<"IN SERVICE_COUNTER2 : Service Done"<<endl;
      servicedone2 = true;
      while(servicedone2);
    }
  }
  pthread_exit(0);
}

int main(){

  pthread_t svc1;
  pthread_t svc2;

  sem_init(&mutex, 0, 2);

  cout<<" Text in main"<<endl;
  pthread_create(&svc1,NULL,service_counter1,NULL);
  pthread_create(&svc2,NULL,service_counter2,NULL);

  int num=0;
  while(1){
    //string str;
    //cout<<"IN MAIN: Input Yes or No"<<endl;
    //cin>>str;

    //if(str=="Yes" || str=="yes"){
      cout<<"IN MAIN: Enter the number of consumer"<<endl;
      cin>>num;

      pthread_t cns[num];
      for(int i=0; i<num; i++){
        int tidd = count+i;
        pthread_create(cns+i,NULL,consumer,(void *)tidd);
      }
    //}
      count+=num;
  }

  pthread_join(svc1,NULL);
  pthread_join(svc2,NULL);
  sem_destroy(&mutex);
  return 0;
}
