#include<iostream>
#include<fstream>
#include<pthread.h>
#include<unistd.h>
#include<semaphore.h>
#include<queue>

using namespace std;

int MAXQ = 4;
bool service1 = false;
bool service2 = false;
bool servicedone1 = false;
bool servicedone2 = false;
int count = 0;
sem_t mutex;

sem_t mutex_for_queue_count;
queue<int> q;
void *consumer(void *argaps){

  long thread_id=long(argaps);
  //if(count > MAXQ){
  if(thread_id>=MAXQ){
    sem_wait(&mutex_for_queue_count);
    time_t now = time(0);
    tm *ltm = localtime(&now);
    cout<<ltm->tm_hour << ":"<<ltm->tm_min << ":"<<ltm->tm_sec <<" CURRENT_CUSTOMER : "<<thread_id<<" Left as Queue Full "<<endl;
    count-=1;
    // cout << "In Queue : " << count << endl;
    sem_post(&mutex_for_queue_count);
    pthread_exit(0);
  }
  //}

  sem_wait(&mutex);

  //cout<<"IN CURRENT_CUSTOMER : "<<thread_id<<endl;

  if(service1 == false){
    service1 = true;
    while(!servicedone1);
    servicedone1=false;
    service1 = false;
  }
  else{
    service2 = true;
    while(!servicedone2);
    servicedone2=false;
    service2 = false;
  }
  sem_wait(&mutex_for_queue_count);
  count-=1;
  sem_post(&mutex_for_queue_count);
  q.push(thread_id);
  sem_post(&mutex);
  pthread_exit(0);

}

void *service_counter1(void *argaps){
  int customer = 0;
  ofstream outfile("Service_Counter1.txt");
  while(1){
    sleep(2);
    if(service1==false){
      customer = 0;
      time_t now = time(0);
      tm *ltm = localtime(&now);
      outfile<<ltm->tm_hour << ":"<<ltm->tm_min << ":"<<ltm->tm_sec <<" IN SERVICE_COUNTER1 : Sleep"<<endl;
      cout<<ltm->tm_hour << ":"<<ltm->tm_min << ":"<<ltm->tm_sec <<" IN SERVICE_COUNTER1 : Sleep"<<endl;
    }
    else{
      time_t now = time(0);
      tm *ltm = localtime(&now);
      outfile<<ltm->tm_hour << ":"<<ltm->tm_min << ":"<<ltm->tm_sec<<" IN SERVICE_COUNTER1 : Giving Service to Customer "<<customer<<endl;
      cout<<ltm->tm_hour << ":"<<ltm->tm_min << ":"<<ltm->tm_sec<<" IN SERVICE_COUNTER1 : Giving Service to Customer "<<customer<<endl;
      sleep(4);
      time_t exit = time(0);
      tm *ltt = localtime(&exit);
      outfile<<ltt->tm_hour << ":"<<ltt->tm_min << ":"<<ltt->tm_sec<<" IN SERVICE_COUNTER1 : Service Done for Customer "<<customer<<endl;
      cout<<ltt->tm_hour << ":"<<ltt->tm_min << ":"<<ltt->tm_sec<<" IN SERVICE_COUNTER1 : Service Done for Customer "<<customer<<endl;
      customer+=2;
      servicedone1 = true;
      while(servicedone1);
    }
  }
  pthread_exit(0);
}


void *service_counter2(void *argaps){
  int customer = 1;
  ofstream outfile("Service_Counter2.txt");
  while(1){
    sleep(2);
    if(service2==false){
      customer = 1;
      time_t now = time(0);
      tm *ltm = localtime(&now);
      outfile<<ltm->tm_hour << ":"<<ltm->tm_min << ":"<<ltm->tm_sec <<" IN SERVICE_COUNTER2 : Sleep"<<endl;
      cout<<ltm->tm_hour << ":"<<ltm->tm_min << ":"<<ltm->tm_sec <<" IN SERVICE_COUNTER2 : Sleep"<<endl;
    }
    else{
      time_t now = time(0);
      tm *ltm = localtime(&now);
      outfile<<ltm->tm_hour << ":"<<ltm->tm_min << ":"<<ltm->tm_sec<<" IN SERVICE_COUNTER2 : Giving Service to Customer "<<customer<<endl;
      cout<<ltm->tm_hour << ":"<<ltm->tm_min << ":"<<ltm->tm_sec<<" IN SERVICE_COUNTER2 : Giving Service to Customer "<<customer<<endl;
      sleep(4);
      time_t exit = time(0);
      tm *ltt = localtime(&exit);
      outfile<<ltt->tm_hour << ":"<<ltt->tm_min << ":"<<ltt->tm_sec<<" IN SERVICE_COUNTER2 : Service Done for Customer "<<customer<<endl;
      cout<<ltt->tm_hour << ":"<<ltt->tm_min << ":"<<ltt->tm_sec<<" IN SERVICE_COUNTER2 : Service Done for Customer "<<customer<<endl;
      customer+=2;
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
  sem_init(&mutex_for_queue_count, 0, 1);

  q.push(0);
  q.push(1);
  q.push(2);

  cout<<"Text in main"<<endl;
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
      sem_wait(&mutex_for_queue_count);
      count+=num;
      sem_post(&mutex_for_queue_count);
      for(int i=0; i<num; i++){
        pthread_create(cns+i,NULL,consumer,(void *)tidd_array[i]);
      }
  }

  pthread_join(svc1,NULL);
  pthread_join(svc2,NULL);
  sem_destroy(&mutex);
  return 0;
}
