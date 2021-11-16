
#include "SimpleAnomalyDetector.h"

SimpleAnomalyDetector::SimpleAnomalyDetector()
{
	vector<correlatedFeatures> vec;
	this->cf = vec;
}

SimpleAnomalyDetector::~SimpleAnomalyDetector()
{
	// TODO Auto-generated destructor stub
}

//not finished yet. 
// MUST REMEMBER: CHECK MEMORY ALLOCATION, WHAT SHOULD BE DELEETED ETC.
void SimpleAnomalyDetector::learnNormal(const TimeSeries &ts)
{
	// TODO Auto-generated destructor stub
	TimeSeries *ts1 = new TimeSeries(ts);
	int length = ts1->getNumOfRows();
	vector<string> features = ts1->getFeatures();
	/*cout<<ts1->getNumOfCols()<<endl;
        cout<<ts1->getNumOfRows()<<endl;
        vector<float> times;
        times = ts1->getFeatureData("Time (seconds)");
        float* times1 = times.data();
        cout<<times1[3]<<endl;*/
	//	cout << "Output of begin and end: \n";

	//for (auto k = features.begin(); k != features.end(); ++k)
		// cout <<"k is " <<*k << " \n";
		//cout << "Output of begin and end: \n";
		for (int k =0; k < features.size(); k++)
		{
			//auto m = k;
			//m++;
			//for (m; m != features.crend(); m++)
			{
				 cout <<"  m is " << " " <<" *  "<<"k is " <<features.at(k) << " \n";
			}
		}


	float *x;
	float *y;

	for (auto i = features.begin(); i != features.end(); ++i)
	{
		float m = 0;

		int c = -1;
		string feature;
		vector<float> vec = ts1->getFeatureData(*i);
		float *iFeature = vec.data();
		auto j = i;
		++j;
		for (j; j != features.end(); ++j)
		{
			int l = 0;
			vector<float> vec = ts1->getFeatureData(*j);
			float *jFeature = vec.data();
			float p = pearson(iFeature, jFeature, length);
			p = abs(p);
			if (p > m)
			{
				m = p;
				c = l;
				feature = *j;
				x = iFeature;
				y = jFeature;
				//delete[] jFeature;
			}
			l++;
		}
		if (c != -1)
		{
			struct correlatedFeatures cf1;
			cf1.feature1 = *i;
			cf1.feature2 = feature;
			cf1.corrlation = m;
			cf1.lin_reg = linear_reg(x, y, length);
			float far1 = farestPoint(x,y,length, cf1.lin_reg);
			float far = 1.1*far1;
			cf1.threshold = far;
			cout << "corrlated  " << *i << "   " << feature << endl;
			this->cf.push_back(cf1);
			//delete cf1;
		}
		//delete[] iFeature;
	}
	//delete[] x;
	//delete[] y;
}

vector<AnomalyReport> SimpleAnomalyDetector::detect(const TimeSeries &ts)
{
	// TODO Auto-generated destructor stub
}

vector<correlatedFeatures> SimpleAnomalyDetector::getNormalModel()
{
	return this->cf;
}
