#include <iostream>
#include <queue>
#include <vector> 
#include <cmath>
#include <algorithm>
using namespace std;

//#define D ;

//#define DC ;

#define PI 3.14159265


double angleBetween(pair<int,int> point1, pair<int,int> point2){
//Returns the angle between point 1 and point 2
 double deltaX = point2.first - point1.first; 
 double deltaY = point2.second - point1.second;
 double theta;
 theta = (atan2(deltaY, deltaX))*180.0/(M_PI); 
 #ifdef D
 cout<<"Theta between:"<<point1.first<<","<<point1.second<<" and "<<point2.first<<","<<point2.second<<" is: "<<theta<<endl;
 #endif 
 return theta;
}

double getMagnitude(pair<int,int> point1, pair<int,int> point2){
  //returns the distance between 
  double magnitude = ( (point2.first - point1.first) * (point2.first-point1.first) + (point2.second - point1.second) * (point2.second-point1.second) ); 
  #ifdef D
  cout<<"Magnitude between:"<<point1.first<<","<<point1.second<<" and "<<point2.first<<","<<point2.second<<" is: "<<magnitude<<endl;
  #endif
  return magnitude; 
}

int getDeterminant(pair<int,int> point1, pair<int,int> point2, pair<int,int> point3){
//Returns the cross product for the vectors made with these 3 points
int determinant = (1*( (point2.first * point3.second) - (point2.second * point3.first))) +
                 ((-1*point1.first) *( (1 * point3.second) - (point2.second * 1))) +
                 (point1.second*( (1 * point3.first) - (point2.first * 1)));
#ifdef D
cout<<"Determinant beween: "<<point1.first<<","<<point1.second<<" ; "<<point2.first<<","<<point2.second<<" ; "<<point3.first<<","<<point3.second<<" is: "<<determinant<<endl;
#endif
return determinant;
}

int checkCCW(pair<int,int> point1, pair<int,int> point2, pair<int,int> point3){
//Returns the cross product for the vectors made with these 3 points
int determinant = getDeterminant(point1,point2,point3);
#ifdef D
cout<<"Determinant beween: "<<point1.first<<","<<point1.second<<" ; "<<point2.first<<","<<point2.second<<" ; "<<point3.first<<","<<point3.second<<" is: "<<determinant<<endl;
#endif
//if zero, that means that they are on the same line so we're going to have to find the one furthest out in our check
if(determinant == 0) return 0; 
//if negative that means we did a counter clockwise turn
if(determinant > 0) return 1; 
//else return a -1 for a clockwise turn 
return -1; 
}

bool comparePoints(pair<int,int> firstPoint, pair<int,int> secondPoint){
  return (firstPoint.first == secondPoint.first) && (firstPoint.second == secondPoint.second); 
}

bool sortPoints ( pair<pair<double,double>,pair<int,int>> firstPoint, pair<pair<double,double>, pair<int,int>> secondPoint){
   double angle1 = firstPoint.first.first; 
   double angle2 = secondPoint.first.first; 
   double mag1 = firstPoint.first.second; 
   double mag2 = secondPoint.first.second;
   
   //we want the smallest angle, if angles are the same we want the smaller magnitude
   return( angle1 < angle2 || ( angle1 == angle2 && mag1 < mag2)); 
}

vector<pair<int,int>> createConvexHull(vector<pair<int,int>> points){
  if(points.size() == 1) return points;
  
  //resulting vector for convexhull
  vector<pair<int,int>> convexhull; 

  //first we find the lowest y (and leftmost)
  int yMin = points[0].second; 
  int yMinIndex = 0; 
  for(int i = 1; i<points.size(); i++){
    if(points[i].second < yMin || (points[i].second == yMin && points[i].first < points[yMinIndex].first)){
      yMin = points[i].second; 
      yMinIndex = i;
    }
  }

  //get the one with the lowest y and set it to lowest most
  pair<int,int> lowestMost = points[yMinIndex];  

  //next, make a vector of pairs of points and their determinants with respect to the loweset leftmost
  vector<pair<pair<double,double>,pair<int,int>>> pointsToSort; 
  for(int i = 0; i<points.size(); i++){
    pointsToSort.push_back( make_pair( make_pair( angleBetween(lowestMost, points[i]), getMagnitude(lowestMost, points[i]) ), points[i])); 
  }

  //sort pointstosort so that it has everything sorted well using comparator
  sort(pointsToSort.begin(), pointsToSort.end(), sortPoints);

  //now that that is done, I want to get a cleaned vector of sorted points
  vector<pair<int,int>> sortedPoints; 
  for(pair<pair<double,double>,pair<int,int>> p: pointsToSort){
    sortedPoints.push_back(p.second); 
  }

#ifdef D
  cout<<"Iterating in this order: "<<endl;
  for(pair<int,int> point: sortedPoints){
    cout<<point.first<<","<<point.second<<endl;
  }
#endif

  //now that this is all done, I can push in the lowest point and the first thing of sorted points in with confidence 
  convexhull.push_back(lowestMost); 
  int indexToStart = 0;
  if(!(comparePoints(lowestMost, sortedPoints[0]))){
    convexhull.push_back(sortedPoints[0]);
    indexToStart = 1;
  }

  //now iterate through and check
  for(int j = indexToStart; j < sortedPoints.size(); j++){
   if(!(comparePoints(sortedPoints[j], lowestMost))){
   #ifdef D
    cout<<"CONVEX HULL AS OF ITERATION "<<j<<":"<<endl; 
    for(pair<int,int> p: convexhull){
      cout<<p.first<<","<<p.second<<endl;
    }
   #endif
   //now iterate through each point and pass it through the checkCCW function
   //if the point we try to add causes a clock wise turn or is colinear, we can get rid of the last point in the convex hull
   int clockWiseCheck;  
   //need the if else to check back with last point at the end
   if(convexhull.size() > 1){
    //if( j < sortedPoints.size()){ 
     clockWiseCheck = checkCCW(convexhull[convexhull.size() - 2],convexhull[convexhull.size() - 1], sortedPoints[j]); //}
    //else{ clockWiseCheck = checkCCW(convexhull[convexhull.size() - 2],convexhull[convexhull.size() - 1], convexhull[0]); }
    while(clockWiseCheck <= 0 && convexhull.size()>1){
      //get rid of the last thing in the convex hull
      #ifdef D
      cout<<"They are either collinear or it was a clockwise turn, so we have to pop back"<<endl;
      #endif
      convexhull.pop_back();
      //do the check again
      clockWiseCheck = checkCCW(convexhull[convexhull.size() - 2],convexhull[convexhull.size() - 1], sortedPoints[j]);
    }
   }
   //in every other case we can just push what's currently there 
   //as long as it's not equal to the last thing in the convex hull (to check against duplicates)
   if(!(comparePoints(sortedPoints[j], convexhull[convexhull.size()-1]))){
     convexhull.push_back(sortedPoints[j]); 
   }
   }
  }

  return convexhull;
}


int main(){
//Answers vector: 
vector< vector<pair<int,int>> > answers;  
int n; 
cin >> n; 
while(n != 0){
 //points for this vector
 vector<pair<int,int>> points;
 //push in all the points
 for(int i = 0; i < n; i++){
    int pointX, pointY; 
    cin >> pointX >> pointY;
    pair<int,int> currentPoint(pointX, pointY);  
    points.push_back(currentPoint); 
 }
 //at this point you should have vector of points. 
 //pass it into a create convex hull function 
 answers.push_back(createConvexHull(points));
 //cin n again to see what is in it
 cin >> n;
}
//at the end print everything out 
for( vector<pair<int,int>> answer: answers){
    cout<<answer.size()<<endl; 
    for(pair<int,int> answer_point: answer){
       cout<<answer_point.first<<" "<<answer_point.second<<endl; 
    }
}
return 0;
}
