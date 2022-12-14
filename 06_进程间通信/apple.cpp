#include<iostream>
using namespace std;

void f(int a[]) {
  cout<<a[0]<<endl;
  int s1 = sizeof(a);
  cout<<"a sizeof:"<<s1<<endl;
  int s2 = sizeof(a[0]);
  cout<<"a[0] sizeof:"<<s2<<endl;
}

void f2(int *a) {
  int s1 = sizeof(a);
  cout<<"a sizeof:"<<s1<<endl;
}

int main(){

  int a[10]={1,1,2};
  f(a);

  f2(a);

  return 0;
}