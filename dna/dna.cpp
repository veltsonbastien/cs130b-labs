#include <iostream>
#include <vector> 
#include <bits/stdc++.h>  
#include <set>
#include <string.h>

using namespace std; 

vector<int> memo;  

int powerFunc(int n){
    if(n == 0) return 1; 
    return 2 * powerFunc(n-1);
}

set<string> makeSubsets(set<string>& result, string s, int size, int originalLength){
 if(size == 0) return result;
 int tempIndex = 0; 
 while( (tempIndex + size) <= originalLength+1 ){
    string temps = s.substr(tempIndex, size);
    int slength = temps.length();
    if(slength > 0){
        result.insert(temps); //push back the string
        reverse(temps.begin(), temps.end()); 
        result.insert(temps); //push back a reversed version
    }
    tempIndex++;
 }
 return makeSubsets(result, s, size-1, originalLength); //recurse and shorten
}

pair<int,int> firstAndLastIndices(string s, char f){
    //s is the string you're looking through, and f is what you're trying to find
    if(s == "0") return make_pair(0, 1); //what should happen at first
    int first = -1, last = -1; 
    int sLength = s.length(); 
    for(int i = 0; i<sLength; i++){
        if(f != s[i]) continue; 
        if(first == -1)first = i; 
        last = i;
    }
    if(first == last){
         return make_pair(sLength-1-last,1);
    }
    return make_pair(sLength-1-last, ((sLength-1-first)-(sLength - 1 - last))+1);  //should return starting index and length for future substr()
} 

string convertDecToBin(int n){
    //convert int n from decimal to binary, and get the first and last 
     string s = bitset<64> (n).to_string(); 
    //Finding the first occurance of "1" to strip off the leading zeroes. 
    const auto loc1 = s.find('1');
    return s.substr(loc1);
}

pair<int,int> getIndices(int n, char f){
    if(n > 0) return firstAndLastIndices(convertDecToBin(n), f); 
    return firstAndLastIndices("0", f);
}

int amountOfCopies(set<string> subsetS, string partialT, string T, int memoIndex){
    //BASE CASE: 
    if(memo[memoIndex] != -1) {
        return memo[memoIndex];
    } 
    string tempPartialT; 
    tempPartialT.resize(partialT.length(), ' ');
    int runningMin = 999999;
    bool found = false;
    //CREATE the subset of sets from partialT:
    set<string> subsetPartialT;
    if(partialT.length() > 0){
      subsetPartialT = makeSubsets(subsetPartialT, partialT, partialT.length(), partialT.length()); 
    }
    pair<int,int> rangeToCheck = getIndices(memoIndex, '1');
    string substrToCheck = T.substr(rangeToCheck.first, rangeToCheck.second);
    cout<<"I'm trying to find: "<<substrToCheck<<endl; 
    cout<<"rangeToCheck.first: "<<rangeToCheck.first<<endl; 
    cout<<"rangeToCheck.second: "<<rangeToCheck.second<<endl;
    cout<<"S's subsets that match length: "<<endl;
    for(string s: subsetS){
      if(s.length() == rangeToCheck.second){
          cout<<s<<endl;
          if(s.compare(substrToCheck) == 0){
              partialT.replace(rangeToCheck.first, rangeToCheck.second, s); //replace in our partial T what we already have 
              found = true; 
          }
      }
    }
    if(!found){ //if still not found, do the same by going through subsets we made of the passed in partial T
    cout<<"Partial T's subsets that match length: "<<endl;
    for(string s: subsetPartialT){
            if(s.length() == rangeToCheck.second){
                cout<<s<<endl;
                if(s.compare(substrToCheck) == 0){
                    partialT.replace(rangeToCheck.first, rangeToCheck.second, s); //replace in our partial T what we already have
                    found = true; 
                }
            }
        }
    }

    //if it's not found, then return a junk value: 
    if(!found) {
        cout<<"It wasn't found."<<endl; 
        return 999999;
    }

    vector<int> movesWhenRecursingFromHere;
    // //now we iterate through and use the find indices again to get substrs of what needs to be filled after putting this in
     for(int i = 0; i < partialT.length(); i++){
         int memoIndexToCheck = memoIndex;
         if(partialT[i] != ' ') continue; //this means it's not a space here so move on
         memoIndexToCheck += powerFunc(i);
         movesWhenRecursingFromHere.push_back(amountOfCopies(subsetS, partialT, T, memoIndexToCheck)); 
         //now keep going while there is a space 
        int lengthOfSpace = 0;
         for(int j = i+1; j < partialT.length(); j++){
            if(partialT[j] != ' ') break; 
            int jMemoIndex = powerFunc(j); 
            movesWhenRecursingFromHere.push_back(amountOfCopies(subsetS, partialT, T, jMemoIndex + memoIndex)); //check with the current j
            memoIndexToCheck+= jMemoIndex; //add the current one 
            movesWhenRecursingFromHere.push_back(amountOfCopies(subsetS, partialT, T, memoIndexToCheck)); //and check on the new memo index to check
            lengthOfSpace++; //update the length of the space 
         }
        i+= lengthOfSpace; //further push i to not waste time
     }

    // //now that we finished getting evreything, we do a linear scan throgh our movesWhenRecursingFromHere and get the minimum, and set that 
    for(int i: movesWhenRecursingFromHere){
        if(i < runningMin) runningMin = i;
    }
    // //now that we have a running min, we set that in the memo table 
    memo[memoIndex] = runningMin;   
    return runningMin;
}

int turnSToT(string S, string T){
    //helper function, have to pass in S into a function that creates valid subsets of it 
    int copies = 0; 
    //time shortening checks: 
    if(S.compare(T) == 0) return 1; 
    string tempT = T; 
    reverse(tempT.begin(), tempT.end()); 
    if(tempT.compare(S) == 0) return 1;
    //end of time shortening checks
    set<string> subsetsOfS; 
    string partialT = "";
    partialT.resize(T.length(), ' ');
    //make the valid subsets of S 
    subsetsOfS = makeSubsets(subsetsOfS, S, S.length(), S.length());
    //pass in to a function that will seek to minimize the amount of copies 
    copies = amountOfCopies(subsetsOfS, partialT, T, 0);    
        cout<<"printing out the memoization table: "<<endl; 
        for(int k : memo){
            cout<<k<<",";
        }
        cout<<endl;
    return copies; 
}

int main(){
//First get the amount of trials:
 int trials;
 cin >> trials;
 //vector for the answers
 vector<int> answers; 
for(int i = 0; i < trials; i++){
    string S, T;
    cin >> S >> T;
    int tableSize = powerFunc(T.length()); //4 , 2 ^ 4 = 16 (different subsets)
    memo.resize(tableSize);
    for(int i=0; i<memo.size(); i++) {
     memo[i] = -1; 
    }
    memo[memo.size()-1] = 0;
    int answer = turnSToT(S,T); 
    if(answer >= 999999) cout<<"impossible"<<endl;
    else cout<<answer<<endl;
}
return 0;
}