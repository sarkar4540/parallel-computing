#include <random>
#include <iostream>
#include <chrono>

using namespace std::chrono;

void example_random_number_generator()
{
   double lbound = 0;
   double ubound = 10;
   std::uniform_real_distribution<double> urd(lbound, ubound);
   std::default_random_engine re;
   for (int i = 0; i < 10; i++)
     std::cout << urd(re) << std::endl;

}

void example_timer_usage()
{
  auto start = high_resolution_clock::now();
  auto stop = high_resolution_clock::now();
  auto duration = duration_cast<milliseconds>(stop - start);
  std::cout << duration.count() << std::endl;
}
