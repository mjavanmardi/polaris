#pragma once 

#include <vector>
#include <limits>
#include <cmath>
#include <iostream>
namespace polaris 
{
/// k-means clustering class, for details see http://youtu.be/myqnyxkdQpc
class KMClustering
{

public:
	typedef int ClusterID;
	typedef double Coordinate;
	typedef std::vector<Coordinate> Point;
	typedef std::vector<Point> PointTable;
	typedef std::vector<std::vector<Point*> > ClusterTable;
	KMClustering() {}
	void Run(const PointTable& pts, int k)
	{
		points_ = pts;
		cluster2point_.resize(k);
		n_ = (int)points_.size();
		if (n_ < k || k == 0)
			return;
		dim_ = (int) points_.at(0).size();
		point2cluster_.resize(n_);
		//assign first k points as cluster centers
		for (int i = 0; i < k; i++)
		{
			centers_.push_back(points_[i]);
		}
		bool flag = true;
		while(flag)
		{
			flag = false;
			// find the closes cluster to the point
			
			//clear previously assignments to clusters
			for (auto it  = cluster2point_.begin(); it!=cluster2point_.end(); ++it)
				it->clear();
			for (int i = 0; i < n_; i++)
			{
				Coordinate distance =   std::numeric_limits<Coordinate>::max();
				int cluster_id;
				for (int j = 0; j < k; j++)
				{
					Coordinate new_dist = dist(points_[i], centers_[j]);
					if (new_dist < distance)
					{
						distance = new_dist;
						cluster_id = j;
					}
				}
				cluster2point_[cluster_id].push_back(&(points_[i]));
				if (cluster_id != point2cluster_[i])
				{
					flag = true;
					point2cluster_[i] = cluster_id;
				}
				
			}
			if (flag) //move centers
			{
				centers_.clear();
				for (int j = 0; j < k; j++)
					centers_.push_back(mean(j));
			}


		}	

	}
	int cluster_id(int point_id) {return point2cluster_.at(point_id);}
	const PointTable& centers() const {return centers_;}

private:
	PointTable points_;
	PointTable centers_;
	// point -> cluster
	std::vector<int> point2cluster_;
	// cluster_id -> (*p1, *p2,...)
	ClusterTable cluster2point_;
	int n_, dim_;
protected:	
	//calculates the L-2 norm of x-y (euclidean distance) 
	Coordinate dist(Point x, Point y)
	{
		Coordinate result = 0.0;
		for (int i = 0; i < dim_; i++)
		{
			result += (x[i] - y[i]) * (x[i] - y[i]);
		}
		return sqrt(result);
	}

	//recalculate the location of the cluster mean
	Point mean(ClusterID cid) const
	{
		//initialize point of the proper dimensionality
		Point result(dim_, 0);
		int count = 0;
		Coordinate delta;
		std::vector<Point* > cp = cluster2point_.at(cid);
		Point *pt;
		for (auto i = cp.cbegin(); i!=cp.end(); ++i)
		{
			pt = *i;
			count++;
			for (int j = 0; j < dim_; j++)
			{
				delta = pt->at(j) - result[j];
				result[j] += delta/count;
			}

		}
		return result;
	}

};
/// This algorithms requires union-find data structure and be finished later
//template <class PointType>
//class SLClustering
//{
//public:
//	SLClustering () {}
//	Run(std::vector<PointType> pts, int n_clusters)
//	{
//		int closest_i, closest_j;
//		n = pts.size;
//		k = n;
//		cluster = new int[n];
//		count = 0;
//		for (int i = 0; i < n; i++)
//		{
//			cluster[i] = i;
//		} 
//		while (k > n_clusters)
//		{
//			Coordinate dist = numeric_limits<Coordinate>::max();
//			for (int i = 0; i < n; i++)
//			{
//				for (int j = 0; j < n; j++)
//				{
//					if cluster[i] == cluster[j]:
//						continue
//					if pts[i].Dist(pts[j]) < dist
//					{
//						closest_i = i;
//						closest_j = j;
//					}
//
//				}
//				cluster[
//			}
//		}
//	}
//
//private:
//	int cluster[];
//	int n, k;
//
//
//}
}