#include <iostream>
#include <vector> 
#include <cmath>
#include <bits/stdc++.h>
using namespace std;

//#define D ;


int getMovesToNextClosest(pair<int,int> globalOrigin, vector<pair<int,int>> beepers ){
   //first go through and create an array of ints representing the indices of the beepers to try 
   int beeperIndices[beepers.size()]; 
   int n = sizeof(beeperIndices) / sizeof(beeperIndices[0]);
   for(int i = 0; i< beepers.size(); i++){ beeperIndices[i] = i; } 

   //iterate through once and calculate the distance to get a running min: 
   int runningMin = -1; 

   //now that I have a running min- I want to go through and permute the indices, and do the same thing
   do { 
       #ifdef D
        cout<<"Checking the point in this order:"<<endl;
        for(int i = 0; i<n; i++){
         cout<<"Index "<<i<<": "<<beepers[beeperIndices[i]].first<<","<<beepers[beeperIndices[i]].second<<"   ";
        }
        cout<<endl;
        #endif

        int newRunningMin = 0; 
        newRunningMin+= abs(beepers[beeperIndices[0]].first - globalOrigin.first) + abs(beepers[beeperIndices[0]].second - globalOrigin.second);
        #ifdef D
        cout<<"first added: "<<newRunningMin<<" to the new running min."<<endl;
        #endif
        for(int i = 0; i < beepers.size(); i++){
            if( i == beepers.size() - 1) {
                newRunningMin += abs(beepers[beeperIndices[i]].first - globalOrigin.first) + abs(beepers[beeperIndices[i]].second - globalOrigin.second); 
                if(runningMin == -1) runningMin = newRunningMin; 
                else if(newRunningMin < runningMin) runningMin = newRunningMin;
                continue;}
            //how long does it take to get from me to the thing in front of me
           #ifdef D
            cout<<"i added: "<<abs(beepers[beeperIndices[i]].first - beepers[beeperIndices[i+1]].first) + abs(beepers[beeperIndices[i]].second - beepers[beeperIndices[i+1]].second)<<" to the new running min"<<endl;
           #endif
            newRunningMin += abs(beepers[beeperIndices[i]].first - beepers[beeperIndices[i+1]].first) + abs(beepers[beeperIndices[i]].second - beepers[beeperIndices[i+1]].second);
        }  
       #ifdef D
        cout<<"Running min: "<<runningMin<<endl; 
        cout<<"New Running Min:"<<newRunningMin<<endl;
       #endif
    } while (next_permutation(beeperIndices, beeperIndices + n));
  
  return runningMin;
}


int main(){

//First get the amount of trials:
 int trials;
 cin >> trials;

 //vector for the answers
 vector<int> answers; 

 for(int i = 0; i<trials; i++){  
    
    //the answer: 
    int moves = 0;

    //A vector of beeper tuples
    vector<pair<int,int>> beepers; 

    //Get the grid dimensions:
    int gridshapeX, gridshapeY; 
    cin >> gridshapeX >> gridshapeY; 

    //Get the origin as a tuple: 
    pair<int,int> origin; 
    cin >> origin.first >> origin.second; 

    //Get the beeper amount: 
    int beeperCount; 
    cin >> beeperCount; 

    if(beeperCount == 0){ answers.push_back(0); continue;}
    
    //Get each of the beepers as tuples: 
    for(int j = 0; j < beeperCount; j++){
        int beeperX, beeperY; 
        cin >> beeperX >> beeperY; 
        pair<int,int> beeper(beeperX, beeperY);
        beepers.push_back(beeper); 
    }

    if(beeperCount == 1){
        answers.push_back( 2 * ( abs(beepers[0].first - origin.first) + abs( beepers[0].second - origin.second))); 
        continue; 
    }

    //Recursively get moves to the next closest point: 
    moves = getMovesToNextClosest(origin, beepers); 

    //push back the answer at the end
    answers.push_back(moves); 

 }//end of trials

 for(int answer: answers){
     cout<<answer<<endl;
 }


return 0;
}