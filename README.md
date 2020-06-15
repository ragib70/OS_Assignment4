# OS_Assignment4
Concurrency Control
There is a service counter which has a limited waiting queue outside it. It works as follows:

  The counter remains open till the waiting queue is not empty
  If the queue is already full, the new customer simply leaves
  If the queue becomes empty, the outlet doors will be closed (service personel sleep)
  Whenever a customer arrives at the closed outlet, he/she needs to wake the person at the counter with a wake-up call


Implement the above-described problem using semaphores or mutuexes along with threads. Also show how it works, if there are 2 service personel, and a single queue. Try to simulate all possible events that can take place, in the above scenario.
