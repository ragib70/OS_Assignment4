#include<iostream>
#include<fstream>
#include<pthread.h>
#include<unistd.h>
#include<semaphore.h>

using namespace std;

int MAXQ = 3;
bool service = false;
bool servicedone = false;
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
  service = true;
  while(!servicedone);
  servicedone=false;
  service = false;
  count-=1;
  sem_post(&mutex);
  pthread_exit(0);

}

void *service_counter(void *argaps){
  ofstream outfile("Service_Counter.txt");
  while(1){
    sleep(2);
    if(service==false){
      outfile<<"IN SERVICE_COUNTER : Sleep"<<endl;
      cout<<"IN SERVICE_COUNTER : Sleep"<<endl;
    }
    else{
      outfile<<"IN SERVICE_COUNTER : Giving Service"<<endl;
      cout<<"IN SERVICE_COUNTER : Giving Service"<<endl;
      sleep(4);
      outfile<<"IN SERVICE_COUNTER : Service Done"<<endl;
      cout<<"IN SERVICE_COUNTER : Service Done"<<endl;
      servicedone = true;
      while(servicedone);
    }
  }
  pthread_exit(0);
}

int main(){

  pthread_t svc;

  sem_init(&mutex, 0, 1);

  cout<<" Text in main"<<endl;
  pthread_create(&svc,NULL,service_counter,NULL);

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

  pthread_join(svc,NULL);
  sem_destroy(&mutex);
  return 0;
}
