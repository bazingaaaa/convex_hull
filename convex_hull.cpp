#include <iostream>
#include <sstream>
#include <math.h>
#include <vector>

using namespace std;
#include "common.h"
#include "linearList.h"
#include "doublyCircularList.h"

#define MAX_X 1e10
#define MIN_X 1e-10
#define EPSILON 1e-6

struct point2d
{
	float x;
	float y;
	float polar;//与目标点构成极角
	float dist;//与目标点的距离
	point2d(){x = 0, y = 0;}
	point2d(float _x, float _y) : x(_x), y(_y){}
	friend ostream& operator<<(ostream& os, const point2d& pt);
	bool operator==(const point2d& pt){
		return x == pt.x && y == pt.y;
	}
};

template<typename T>
T RandT(T _min, T _max)
{
	T temp;
	if (_min > _max)
	{
		temp = _max;
		_max = _min;
		_min = temp;
	}
	return rand() / (double)RAND_MAX *(_max - _min) + _min;
}

ostream& operator<<(ostream& os, const point2d& pt)
{
	os << " [" << pt.x << ", " << pt.y << "]" << endl;
	return os;
}

//比较函数
bool myCompare(point2d p1, point2d p2)
{
	if(p1.polar == p2.polar)
	{
		return p1.dist < p2.dist;
	} 
	return p1.polar < p2.polar;
}

//计算两个点构成极角
inline float calculatePolarBetweenTwoPoints(const point2d& basePt, const point2d& thePoint)
{
	return atan2(basePt.x - thePoint.x, thePoint.y - basePt.y) + M_PI;
}

//判断三点逆时针夹角是否大于180
bool isCounterwiseAngleLessThan180(const point2d& pt1, const point2d& pt2, const point2d& pt3)
{
	return (((pt2.x - pt1.x) * (pt3.y - pt1.y) - (pt3.x - pt1.x) * (pt2.y - pt1.y))) <= 0;
}

//从三角形内部取出一个点（随便取两条边连线的中点）
point2d drawPointFromTriangle(const point2d& p1, const point2d& p2, const point2d& p3)
{
	point2d pt((p1.x + p2.x / 2 + p3.x / 2) / 2, (p1.y + p2.y / 2 + p3.y / 2) / 2);
	return pt;
}

//判断三点是否共线
inline bool threePointsCollinear(const point2d& p1, const point2d& p2, const point2d& p3)
{
	return fabs((p3.y - p1.y) * (p2.x - p1.x) - (p2.y - p1.y) * (p3.x - p1.x)) < EPSILON;
}

//判断点集合是否共线
bool pointsCollinear(const vector<point2d>& pts)
{
	//点个数小于2时，固定共线
	if(pts.size() <= 2) return true;

	//点个数大于等于3
	for(int i = 2; i < pts.size(); i ++)
	{
		if(!threePointsCollinear(pts[i], pts[i-1], pts[i - 2]))
			return false;
	}

	return true;
}

//输出点集
void outputPts(const vector<point2d>& pts)
{
	cout << "convex hull perfoots:" << endl;
	for(int i = 0; i < pts.size(); i++)
	{
		cout << " [" << pts[i].x << ", " << pts[i].y << "] ";
	}
	cout << endl;
}

//挑选不共线点集S中的点
point2d findInsidePt(const vector<point2d>& S)
{
	for(int i = 2; i < S.size(); i++)
	{
		if(!threePointsCollinear(S[i], S[i-1], S[i - 2]))
		{
			return drawPointFromTriangle(S[i], S[i-1], S[i - 2]);
		}
	}
	return point2d(0, 0);//S点集不共线，程序不会执行到这一步
}

//计算极角，距离，并排序
void sortPtsByPolar(vector<point2d>& pts, const point2d& basePt)
{
	//计算极角和距离
	for(int i=0; i < pts.size(); i++)
	{
		pts[i].polar = calculatePolarBetweenTwoPoints(basePt, pts[i]);
		pts[i].dist = sqrt((basePt.x - pts[i].x) * (basePt.x - pts[i].x) + (basePt.y - pts[i].y) * (basePt.y - pts[i].y));
	}

	//排序
	sort(pts.begin(), pts.end(), myCompare);
}

//极点筛选（利用双向链表）,输入为按极角和距离远近排好序的点集
vector<point2d> findPerfoots(const vector<point2d>& sortedPts) 
{
	vector<point2d> perfoots;
	doublyLinkedList<point2d> dList(100);
	
	//建立双向链表
	for(int i = 0; i < sortedPts.size(); i++)
		dList.insert(i, sortedPts[i]);

	//找到y坐标最小的点
	doublyLinkedList<point2d>::iterator p;
	float yMin = MAX_X;
	for(doublyLinkedList<point2d>::iterator it = dList.begin(); it != dList.end(); ++it)
	{
		if((*it).y < yMin)
		{
			p = it;
			yMin = (*it).y;
		}
	}
	
	//筛选极点
	int count = 0;//走完一圈发现没有可删除节点时，认为链表内的点均为有效点，与原算法有所区别
	for(doublyLinkedList<point2d>::iterator x = p, rx = dList.right(x); count < dList.size();)
	{
		doublyLinkedList<point2d>::iterator rrx = dList.right(rx);
		if(isCounterwiseAngleLessThan180(*x, *rx, *rrx))
		{
			dList.erase(rx);
			rx = x, x = dList.left(rx);
			count = 0;
		}
		else
		{
			x = rx;
			rx = rrx;
			count++;
		}
	}

	//输出链表中剩余的点
	for(doublyLinkedList<point2d>::iterator it = dList.begin(); it != dList.end(); ++it)
		perfoots.push_back(*it);

	return perfoots;
}

//凸包求解
vector<point2d> solveConvexHull(vector<point2d> S)
{
	vector<point2d> perfoots;
	
	//step 1，处理退化情况
	if(S.size() < 3)
		return S;
	if(pointsCollinear(S))//S中所有点共线
	{
		float minX = MAX_X, maxX = MIN_X;
		int minPtIdx = 0, maxPtIdx = 0;

		for(int i = 0; i < S.size(); i++)//找到最大和最小的点
		{
			if(S[i].x < minX)
			{
				minX = S[i].x;
				minPtIdx = i;
			}
			if(S[i].x > maxX)
			{
				maxX = S[i].x;
				maxPtIdx = i;
			}
		}
		perfoots.push_back(S[minPtIdx]);
		perfoots.push_back(S[maxPtIdx]);
		return perfoots;
	}

	//step 2，按极角排序
	sortPtsByPolar(S, findInsidePt(S));

	//step 3，删除非极点的点
	return findPerfoots(S);
}



//求解凸包问题，算法无法处理坐标相同或十分接近的点的情形
int main()
{
	vector<point2d> perfoots;

	//输入模块
	vector<point2d> S;
	S.push_back(point2d(1000, 1000));
	S.push_back(point2d(2000, 1000));
	S.push_back(point2d(2000, 2000));
	S.push_back(point2d(1000, 2000));
	for(int i = 0; i < 1000000; i++)
		S.push_back(point2d(RandT(1001, 1009), RandT(1001, 1009)));

	//找出位于凸包上的点
	perfoots = solveConvexHull(S);
	
	//输出模块
	outputPts(perfoots);

	return 0;
}