#include <iostream>
#include <vector> 

using namespace std; 

//#define ENABLE_DEBUG 

struct Soldier{
 int id; 
 bool addedToGroup = false;
 int groupImIn = 0;
 vector<Soldier*> enemyList; 
 int enemiesInGroup = 0; 
 Soldier(); 
 Soldier(int idnum){
  id = idnum;
  enemyList = vector<Soldier*>(); 
 }
};


void printGroups(vector<vector<Soldier*>> optimalGroup){
    for(vector<Soldier*> group:  optimalGroup){
        for(int i = 0; i<group.size(); i++){
            if(i == 0) cout<<group.at(i)->id;
            else cout<<" "<<group.at(i)->id; 
        }
        cout<<endl;
    }
}

vector<vector<Soldier*>> createGroups(vector<Soldier*>& soldierGroup){ 
    //set up my main data structure with 1 group initially
    vector<vector<Soldier*>> optimalPairings(1); 
 
   //boolean to see if verified or not 
   bool verified = false;

   while(verified == false){

    //set all the added to groups to false 
    for(Soldier* s: soldierGroup){
        s->addedToGroup = false;
    }

    //iterate through each soldier and 
    for(int i = 0; i<soldierGroup.size(); i++){
       if(soldierGroup.at(i)->enemiesInGroup > 1){ //if enemies here is greater than one 
        //still change the size of the optimal pairings vector for size tracking purposes
        if(optimalPairings.size() == 1){ optimalPairings.resize(2); }
        soldierGroup.at(i)->enemiesInGroup = 0; 
        soldierGroup.at(i)->addedToGroup = true;
        //decrease the enemy totals (if they weren't already added to a group this round) of all since you're moving it 
        for(Soldier* enemy: soldierGroup.at(i)->enemyList){
            if(enemy->addedToGroup == false)  enemy->enemiesInGroup --; 
        }
        // flip my group association
        if(soldierGroup.at(i)->groupImIn == 0) soldierGroup.at(i)->groupImIn = 1; 
        else soldierGroup.at(i)->groupImIn = 0; 
       }
     } //end of larger for

     //clean out enemy amounts 
     for(Soldier* s: soldierGroup){
         s->enemiesInGroup = 0;
     }

     //iterate through each one by one and reupdate it's enemy counts
     for(int j = 0; j<soldierGroup.size(); j++){
         //iterate through enemies and update groups
      for(int k = 0; k<soldierGroup.at(j)->enemyList.size(); k++){
        if(soldierGroup.at(j)->enemyList.at(k)->groupImIn == soldierGroup.at(j)->groupImIn){
            //if enemies are in the same group as me,
            //then update my count only if i was recently moved
            //if(soldierGroup.at(j)->addedToGroup){
            // && soldierGroup.at(j)->enemyList.at(k)->addedToGroup){
                soldierGroup.at(j)->enemiesInGroup++; //update your count including enemy
          //  }
        }
      }
     }

     //now go through once more and check that everyone's enemies are only one or less 
     bool passedCheck = true;
     for(Soldier* s: soldierGroup){
         if(s->enemiesInGroup > 1){
            passedCheck = false;
            break;
         }
     }   
    //set verified to whether it passed the check or not
    verified = passedCheck; 
    }

    //once it gets out of that, I should be done so I now update optimal pairings 
    for(Soldier* s: soldierGroup){
        if(s->groupImIn == 0) optimalPairings.at(0).push_back(s);
        else optimalPairings.at(1).push_back(s);
    }
    //then return
    return optimalPairings; 
}

int main(){
 int n, m; 
 cin >> n >> m; 

 if(n == 0){ return 0; } 
 if(n == 2){
     cout<<1<<" "<<2<<endl;
     return 0;
 } 

 //have a group for the soldiers and populate it 
 vector<Soldier*> soldiers(n); 
 for(int i = 0; i < n; i++){ //populate vector of soldiers
    Soldier* s = new Soldier(i+1); // i + 1 should correspond to the ID of the soldier 
    soldiers.at(i) = s;            //add it to the vector holding all the soldiers
 }

 //next update the enemy lists of the soldiers
 for(int j = 0; j < m; j++){
   //iterate through and iterate through the enemy pairs (m) to add them in 
   int soldierId1, soldierId2; 
   cin >> soldierId1 >> soldierId2; 
   //update respective enemy lists 
    soldiers.at(soldierId1 - 1)->enemyList.push_back(soldiers.at(soldierId2 - 1)); 
    soldiers.at(soldierId1 - 1)->enemiesInGroup++;

    soldiers.at(soldierId2 - 1)->enemyList.push_back(soldiers.at(soldierId1 - 1)); 
    soldiers.at(soldierId2 - 1)->enemiesInGroup++;
 } 

 vector<vector<Soldier*>> optimalGroups = createGroups( soldiers );
 //print out the answer: 
 cout<<optimalGroups.size()<<endl; 
 printGroups(optimalGroups);

 return 0; 
}