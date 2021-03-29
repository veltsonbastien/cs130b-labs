#include <iostream>
#include <vector>
using namespace std; 

//Struct for a Vertex
struct Vertex{
  Vertex *parent; 
  int vertexNumber;
  int numberOfMarbles; 
  int surplus_defecit; 
  vector<Vertex*> childrenVector; 
  Vertex();
  Vertex(int vn, int mn, int ca){
      //we don't know who's whose parent yet, so it's null for now
      parent = nullptr; 
      //set the vertex number as an identfying int 
      vertexNumber = vn; 
      //set the marble amount 
      numberOfMarbles = mn;
      //we will know the children amount as per the input, so create new vector with that
      childrenVector = vector<Vertex*>(); 
  }
};

int getMovesRecursive(Vertex*& v){
  //this function will be in charge of the DFS traversal and updating the vertex surplus_defecit 
  v->surplus_defecit =  v->numberOfMarbles - 1; 
  //then we continue through the DFS if this vector has children 
   if(v->childrenVector.size() > 0){
    for(int i = 0; i < v->childrenVector.size(); i++){
      //sum up the surplus defecits
      v->surplus_defecit += getMovesRecursive(v->childrenVector.at(i)); 
    }
   } 
  // return the surplus defecits
   return v->surplus_defecit;
}

Vertex* getRoot(Vertex*& v){ 
    Vertex*& root = v; 
    Vertex* temp = v->parent;
    while(temp != nullptr){
        root = temp; 
        temp = temp->parent;
    }
    return root; 
}

int getMoves(vector<Vertex*>& caseObject, int& totalMoves){  
  //1) Do a DFS to go through and update the surplus and defecit 
  //find the root 
  Vertex* v = caseObject.at(0);
  Vertex* root = getRoot(v);
  //next call the recursive helper to update all the surplus defecits
  getMovesRecursive(root);


  //2 finally go through all the surplus defecits and add them to total moves 
  for(Vertex* v: caseObject){
     // cout<<"add to total moves, v is: "<<v->vertexNumber<<" and surplus defecit is: "<<v->surplus_defecit<<endl;
      totalMoves += abs(v->surplus_defecit); 
  }
  return totalMoves; 
}

int main(){

vector<int> answers; 

//Begin getting input: 
//1) Get the amount in a case: 
int n; 
cin >> n; 

while(n != 0){  //continuously look for the n input 
//if it's zero, that's the terminator, end the program 
if(n==0) exit(0); 

   vector<Vertex*> caseObject; 
   caseObject.resize(n, nullptr); 

    //if not, continue through, and start a for loop to get everything else 
    for(int i = 0; i<n; i++){
        //get the vertex number, marbles amount, and the children amount
            int v, m, d; 
            cin >> v >> m >> d; 

            //This creates new Vertices if it wasn't created before
            if(caseObject.at(i) == nullptr ){
                Vertex* vertexObject = new Vertex(v,m,d);
                caseObject.at(i) = vertexObject;
            } else {
            //if not null, means it was already added, so just edit what is inside
                caseObject.at(i)->numberOfMarbles = m; 
            }

            //TIME SAVER: only go in here if it has children 
            if(d > 0){ 
                //now iterate through the amount of children and add them to the vertex's children list 
                //resize the current vector's children vector to d
                for(int j = 0; j<d; j++){
                //while(cin >> dChildrenNumber){
                int dChildrenNumber;
                cin >> dChildrenNumber; 
                //once done figuring that out, add in the dChildren to the current vector's children
                //This creates a new Vertices when they are children of the current vertices
                if(caseObject.at(dChildrenNumber-1) == nullptr ){
                    Vertex* vertexObject = new Vertex(dChildrenNumber,0,0);
                    vertexObject->parent = caseObject.at(i); //set the 
                    caseObject.at(dChildrenNumber-1) = vertexObject; //add it to the case object for record keeping 
                    caseObject.at(i)->childrenVector.push_back(vertexObject); //push it back to the current vertex's children vector 
                } else {
                   //if it's already added, update the parent 
                   caseObject.at(dChildrenNumber-1)->parent = caseObject.at(i);  
                   caseObject.at(i)->childrenVector.push_back( caseObject.at(dChildrenNumber-1) ); //push it back to the current vertex's children vector 
                }
            }//end of for loop through children
        } //end of if d > 0 if there is children
    } //end of for loop through vertex
 

    //now calculate answer
     int totalMoves = 0; //acts as a counter I can just pass around
     int answer = getMoves(caseObject, totalMoves);  
     answers.push_back(answer);

     for(Vertex* v: caseObject){
         delete v; 
     }

    cin >> n;
} //end of while loop checking cin input for n

for(int answer: answers){
    cout<<answer<<endl;
}

return 0; 
}