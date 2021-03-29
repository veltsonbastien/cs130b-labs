#include <string>
#include <vector> 
#include <iostream> 
using namespace std;



string getBestArgument(int m, int n,vector<string> beliefs){
 string argument = ""; 
 //Iterate through all the arguments 
 for(int i = 0; i<m; i++){
     //keep track of the zeroes and ones 
     int zeros = 0; 
     int ones = 0; 
     //now iterate through each of them and keep track 
     for(int j = 0; j<n; j++){
         if(beliefs[j][i] == '0' ) zeros++; //increment zeroes 
         else ones++; //increment ones 
     }
   //if there were more zeros than ones, that's the compromise, else it's the other. In the case of a tie it shouldn't matter 
   if(zeros >= ones) argument.insert(argument.length(),"0"); 
   else argument.insert(argument.length(),"1"); 
 }
 return argument; 
}


int main(){
//auto start = high_resolution_clock::now(); 
 //INPUT Variables 
 //Big Vector 
 vector<pair<pair<int,int>,vector<string>>> mega_input; 

//First Get the t input:  
 int t;
 cin >> t;  
 mega_input.resize(t); 

for(int i = 0; i<t; i++){
    int n, m; 
    cin >> n >> m; 

    mega_input[i].second.resize(n); 
    for(int j = 0; j < n; j++){
        //now get inputs for all values line by line 
        string inputtedBelief; 
        cin >> inputtedBelief; 
        mega_input[i].second[j] = inputtedBelief; 
    }
    //add in the int m to where it needs to go: 
    mega_input[i].first.first = m; 
    mega_input[i].first.second = n;
}

for (int i = 0; i<t; i++){
 //Iterate through the t's, and print out the argument 
 string bestArgumentString = getBestArgument(mega_input[i].first.first, mega_input[i].first.second, mega_input[i].second); 
 cout<<bestArgumentString<<endl;
 }

return 0;

}
