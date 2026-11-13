#include<fstream.h>

long calc(long a, long q)
 {
 long r,m;
 r = 0;
 m = a/q;
 while(m)
  {
  r += m;
  m /= q;
  }
 return r;
 }

int primes[2000];
int pc = 0;

void presolve()
 {
 primes[0] = 2;
 pc = 1;
 for(int p = 3; p < 5000; p += 2)
  {
  for(int i = 1; i < pc; i++)
   if((p%primes[i])==0)
    break;
  if(i == pc)
   primes[pc++] = p;
  }
 }

int quant[2000];

long solve(long n, int k)
 {
 int i;
 for(i = 0; i < pc && k > 1; i++)
  while((k%primes[i])==0)
   {
   k /= primes[i];
   quant[i]++;
   }
 long r,br;
 br = -1;
 for(i = 0; i < pc; i++)
  {
  if(quant[i])
   {
   r = calc(n,primes[i])/quant[i];
   if(br == -1 || r < br)
    br = r;
   }
  }
 return br;
 }

int main()
 {
 presolve();
 ifstream input("zeroes.in");
 ofstream output("zeroes.out");
 long n;
 int k;
 k = 10;
 input >> n;
 output << solve(n,k) << "\n";
 return 0;
 }
