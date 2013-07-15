#include <vector>
#include <algorithm>
#include <time.h>
namespace polaris {

// borrowed from here http://youtu.be/wNQVo6uOgYA
class ResamplingWheel
{
public:
	ResamplingWheel(const std::vector<double>& w)
	{
		weights_sum = 0;
		w_max = 0;
		for (auto it = w.cbegin(); it != w.cend(); ++it)
			weights_sum += *it;
		for (auto it = w.cbegin(); it != w.cend(); ++it)
			weights_.push_back(*it / weights_sum);
		n = (int)w.size();
		w_max = *std::max_element(weights_.begin(), weights_.end());
	}
	void Resample(int sample_size)
	{
		CalculateSample(sample_size);
	}
	const std::vector<int>& getSample()
	{		
		return sample_;
	}
private:
	std::vector<double> weights_;
	double weights_sum;
	double w_max;
	std::vector<int> sample_;
	int n;
	void CalculateSample(int sample_size)
	{
		sample_.clear();
		int index;
		double beta = 0;
		double rf;
		//srand(time(NULL));
		// draw a uniform index
		index = rand() % n;
		//std::cout << "Initial index: " << index << "\n";
		for (int i = 0; i < sample_size; i++)
		{
			// U{0,...,2*w_max}
			rf = (double)rand() / RAND_MAX * 2.0 *w_max;
			beta += rf;
			while (beta > weights_[index])
			{
				beta -= weights_[index];
				index = (index + 1) % n;
			}
			sample_.push_back(index);
		}
	}
};

}