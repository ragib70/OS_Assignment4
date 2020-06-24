#include<iostream>
#include<fstream>
#include<pthread.h>
#include<unistd.h>
#include<semaphore.h>
#include <queue>

using namespace std;

int MAXQ = 5;
bool service = false;
bool servicedone = false;
int count = 0;
sem_t mutex;

sem_t mutex_for_queue_count;
queue<int> q;

void *consumer(void *argaps){

  long thread_id=long(argaps);


  if(thread_id>=MAXQ){
      sem_wait(&mutex_for_queue_count);
      cout<<"CURRENT_CUSTOMER : "<<thread_id<<" Left as Queue Full "<<endl;
      count-=1;
      // cout << "In Queue : " << count << endl;
      sem_post(&mutex_for_queue_count);
      pthread_exit(0);
  }




  sem_wait(&mutex);

  cout<<"IN CURRENT_CUSTOMER : "<<thread_id<<endl;

  service = true;
  while(!servicedone);
  servicedone=false;
  service = false;
  count-=1;
  q.push(thread_id);
  // cout << "In Queue : " << count << endl;
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
  sem_init(&mutex_for_queue_count, 0, 1);

  for(int i=0; i<MAXQ; i++)
    q.push(i);

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
      int tidd_array[num];
      int tidd = 0;
      for(int i=0; i<num; i++){
        if (!q.empty()) {
          tidd = q.front();
          q.pop();
        }
        else{
          tidd = count+i;
        }
        tidd_array[i] = tidd;
      }
      count+=num;
      for(int i=0; i<num; i++){
        pthread_create(cns+i,NULL,consumer,(void *)tidd_array[i]);
      }

  }

  pthread_join(svc,NULL);
  sem_destroy(&mutex);
  return 0;
}
