#include <gtest/gtest.h>
#include "tmc/resampling_wheel.h"
class ResamplingWheelTest : public testing::Test {
protected:
	virtual void SetUp() 
	{
		//see http://youtu.be/FjRX_i3SsJA
		std::vector<double> w;
		n = 5;
		count.resize(n,0);
		int sum = 0;
		const std::vector<int> sample;
		int nruns = 1;
		w.push_back(0.6);
		w.push_back(1.2);
		w.push_back(2.4);
		w.push_back(0.6);
		w.push_back(1.2);
		rw = new polaris::ResamplingWheel(w);	

	}
	virtual void TearDown()
	{
		delete rw;
	}
	polaris::ResamplingWheel* rw;
	std::vector<double> count;
	int n;
};


TEST_F(ResamplingWheelTest, 1) {
		int nruns = 10000;
		int sample_size = 2;
		double sum = 0.0;
		for (int i = 0; i < nruns; i++)
		{
			rw->Resample(sample_size);
			for (auto it = rw->getSample().cbegin(); it != rw->getSample().cend(); ++it)
			{
				count[*it]+=1;
				sum += 1.0;
			}
		}
		//normalize frequencies 
		for (int i = 0; i < this->n; i++)
			count[i] = count[i] / sum;
	EXPECT_NEAR(0.1, this->count[0], 0.05);
	EXPECT_NEAR(0.2, this->count[1], 0.05);
	EXPECT_NEAR(0.4, this->count[2], 0.05);
	EXPECT_NEAR(0.1, this->count[3], 0.05);
	EXPECT_NEAR(0.2, this->count[4], 0.05);
}

TEST_F(ResamplingWheelTest, 2) {
		int nruns = 5;
		int sample_size = 10000;
		double sum = 0.0;
		for (int i = 0; i < nruns; i++)
		{
			rw->Resample(sample_size);
			for (auto it = rw->getSample().cbegin(); it != rw->getSample().cend(); ++it)
			{
				count[*it]+=1;
				sum+=1.0;
			}
		}
		//normalize frequencies 
		for (int i = 0; i < this->n; i++)
			count[i] = count[i] / sum;
	EXPECT_NEAR(0.1, this->count[0], 0.01);
	EXPECT_NEAR(0.2, this->count[1], 0.01);
	EXPECT_NEAR(0.4, this->count[2], 0.01);
	EXPECT_NEAR(0.1, this->count[3], 0.01);
	EXPECT_NEAR(0.2, this->count[4], 0.01);
}