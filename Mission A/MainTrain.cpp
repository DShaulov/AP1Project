
#include <iostream>
#include "anomaly_detection_util.h"

using namespace std;

bool wrong(float val, float expected){
	return val<expected-0.001 || val>expected+0.001;
}

// this is a simple test to put you on the right track
int main(){


	const int N=10;

	float x1[]={1,2,3,4,5,6,7,8,9,10};
	float y1[]={2.1,4.2,6.1,8.1,10.3,12.2,14.4,16.1,18.2,20.3};

	float x2[]={-10,50,-20,4,5,-6,70,87,0.67,0};
	float y2[]={-2.1,-4.2,6.1,8.1,10.3,-12.2,0,16.1,18.2,0};

	Point* ps[N];
	for(int i=0;i<N;i++)
		ps[i]=new Point(x1[i],y1[i]);

	Point* ps2[N];
	for(int i=0;i<N;i++)
		ps2[i]=new Point(x2[i],y2[i]);

	Line l=linear_reg(ps,N);
	Line l2=linear_reg(ps2,N);

	Point p(4,8);
	Point p2(0,0);

	float v1[]={var(x1,N),cov(x1,y1,N),pearson(x1,y1,N),l.a,l.b,l.f(4),dev(p,l)};
	float e1[]={8.25,16.63,0.999,2.015,0.113,8.176,0.176};

	float v2[]={var(x2,N),cov(x2,y2,N),pearson(x2,y2,N),l2.a,l2.b,l2.f(4),dev(p2,l2)};
	float e2[]={1228.2284,53.0894,0.999,2.015,0.113,8.176,0.176};


	for(int i=0;i<7;i++)
		if(wrong(v1[i],e1[i]))
			cout<<"error for check "<<i<<" (-14)"<<endl;

	for(int i=0;i<7;i++)
		if(wrong(v2[i],e2[i]))
			cout<<"error for check "<<i<<" (-14)"<<endl;

	for(int i=0;i<N;i++)
		delete ps[i];

	cout<<"done"<<endl;
	return 0;
}
