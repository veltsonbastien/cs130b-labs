#include <iostream>
#include <vector> 
#include <cmath> 
#include <assert.h> 
using namespace std; 

#define D

//a Ledger object, where each person will be able to compartmentalize their info
struct Ledger{
 int id; 
 vector<int> payable;
 vector<int> receivable;
 vector<pair<int,int>> bankAccount; 
 int cash = 0;
 int outstanding = 0;
 int owed = 0;
 Ledger(); 
 Ledger(int personId){
    id = personId;
    payable.resize(3, 0); 
    receivable.resize(3, 0);
    bankAccount.resize(6);
 }
};

//Helpers for the Ledger struct
 int calcBankAmount(vector<pair<int,int>> bank){
     int total = 0; 
     for(int i = 0; i < 6; i++) total+= bank[i].first * bank[i].second;  
     return total; 
 }

 int calcOwed(vector<int> rec){
     int owed = 0; 
     for(int i: rec) owed+= i; 
     return owed;
 }

  int calcOutstanding(vector<int> pay, vector<int> rec){
     int outstandingAmt = 0; 
     for(int i: pay) outstandingAmt+= -1*i; 
     for(int i: rec) outstandingAmt+= i; 
     return outstandingAmt;
 }
// End of Helpers 
bool debtSettled(vector<Ledger> characters){
    int total = 0; 
    for(Ledger person: characters) for(int i: person.payable) total+= -1*i; 
    return (total == 0)? true : false; 
}


//Helpers for the code as a whole:
void printLedger(Ledger person){
    string name; 
    cout<<"**********"<<endl;
    switch(person.id){
        case 0: name = "Alice"; break;
        case 1: name = "Bob"; break;
        case 2: name = "Cynthia"; break;
    }
    cout<<"Account For: "<<name<<endl;
    cout<<"Amount in Bank: "<<person.cash<<endl;
    cout<<"Bank account"<<endl; 
    for(pair<int,int> i: person.bankAccount){
        cout<<i.first<<","; 
    } 
    cout<<endl;
    cout<<"Payable: "<<endl; 
    for(int i: person.payable){
        cout<<i<<",";
    }
    cout<<endl;
    cout<<"Receivable: "<<endl; 
    for(int i: person.receivable){
        cout<<i<<",";
    }
    cout<<endl;
    cout<<"Outsanding:"<<person.outstanding<<endl;
    cout<<"**********"<<endl;
}
void printGeneralLedger(vector<Ledger> characters){
    cout<<endl;
    cout<<"********************"<<endl;
    cout<<"PRINTING GENERAL LEDGER LEDGER: "<<endl;
    string debtsettled; 
    (debtSettled(characters))? debtsettled = "Yes" : debtsettled = "No"; 
    cout<<"Debt settled? : "<<debtsettled<<endl; 
    for(Ledger p: characters){
        printLedger(p);
    }
    cout<<"********************"<<endl;
    cout<<endl;
}

bool gcd(int amount, int debt){
    if (amount == debt) return true; 
    if (amount < debt) return false;
    return gcd(amount-debt, debt);
}

pair<pair<int,int>,vector<pair<int,int>>> amountToMake(vector<pair<int,int>> bankAccount, int amount){
    //helper function see how many transactions it takes for a bank account to make an int n
    //if it's not able to make it, it'll return the smallest denomination (that the bank account has) larger than n
    pair<pair<int,int>,vector<pair<int,int>>> result; 
    result.second.resize(6,make_pair(0,0));
    int maxDenomLargerThanAmount = -1; //if not set, we handle it below
    int transactions = 0; 
    for(int i = 0; i < 6; i++){
        int denom = bankAccount[i].second; 
         result.second[i].second = denom;
        //now set the maxdenom if it's too large for the amount 
        if(bankAccount[i].first == 0) continue; 
        if(denom > amount){ maxDenomLargerThanAmount = i; continue; }
        //if we are here, that means that our denom is smaller, and we have some, so we add as many as we can
        int j = 0; 
        while( j < bankAccount[i].first && amount - denom >= 0){
            amount = amount - denom; //decrease amount
          //  cout<<"denom:"<<denom<<" and amount: "<<amount<<endl;
            result.second[i].first++; //increase the count in the result 
            transactions++; //increase the transactions 
            //cout<<"transactions:"<<transactions<<endl;
            j++; //update j
            if(amount <= 0) break;
        } 
    }
    //if amount doesn't equal zero, that means that it wasn't able to do it- so we return a -1 as the main int portion 
    if(amount != 0){
        //cout<<"maxDenom:"<<maxDenomLargerThanAmount<<endl;
        result.first.first = -1; 
        result.first.second = maxDenomLargerThanAmount;
        return result;
    }
    //else, it did zero out, so we can return how many transactions it took to do that, and the bills used will also be there
    result.first.first = transactions;
    result.first.second = maxDenomLargerThanAmount;
    return result; 
}

pair<pair<pair<int,int>,int>, vector<pair<int,int>>> makeChange(Ledger payer, Ledger receiver, int amount){
    //Helper function to make change between two people, returns the transactions it would take to do so
    pair<pair<int,int>,vector<pair<int,int>>> payerPaysAmountData = amountToMake(payer.bankAccount, amount);
    int amountPayerGives = payer.bankAccount[payerPaysAmountData.first.second].second; //this is what I set that max denom to
     //now, we try to have the receiver make change just in case and we will compare the moves. 
     int changeToMake = amountPayerGives - amount; 
     pair<pair<int,int>,vector<pair<int,int>>> receiverMakesChangeData = amountToMake(receiver.bankAccount, changeToMake);
    //this means that payer wasn't able to make exact change, so we can return the amount it took to make change 
     int rf = receiverMakesChangeData.first.first; 
     int rs = receiverMakesChangeData.first.second;
     int pf = payerPaysAmountData.first.first; 
     int ps = payerPaysAmountData.first.second;
     if(pf == -1) return make_pair(make_pair(make_pair(rf,rs), 1), receiverMakesChangeData.second); //if -1, then we'll know it's impossible
     if(rf == -1) return make_pair(make_pair(make_pair(pf,ps), 0), payerPaysAmountData.second); //if receiver isn't able to make change but i'm able to pay, return that
     if(pf <= rf ) return make_pair(make_pair(make_pair(pf,ps), 0), payerPaysAmountData.second); //return the amount it takes for paying on your own if that's smaller
     return make_pair(make_pair(make_pair(rf,rs), 1), receiverMakesChangeData.second); //finally return this only if both are able to make change / pay but this one is smaller
}

void doTransfer(vector<pair<int,int>> transfer, Ledger& payer, Ledger& receiver){
    //transfer a certain amount specified in a transfer vector of bills, to a person "payTo"'s account
    //receiver will always recieve what's in the transfer, so do that first and update their receivables and cash: 
    for(int i = 0; i < 6; i++) receiver.bankAccount[i].first += transfer[i].first;
    receiver.cash = calcBankAmount(receiver.bankAccount); 
    //payer will always pay what's in the transfer, so do that and update bank and cash
    for(int i = 0; i < 6; i++) payer.bankAccount[i].first -= transfer[i].first; 
    payer.cash = calcBankAmount(payer.bankAccount); 
}

int min (int a, int b){
    //simple helper to return min: 
    if(a <= b) return a; 
    return b;
}

int settleDebt(vector<Ledger>& characters, int& transactions){
return 0;
}

string figureOutDebt(vector<Ledger>& characters){
  if(debtSettled(characters)) return to_string(0); 
  int transactions = 0;
  transactions = settleDebt(characters, transactions);  
  if(transactions == -1) return "impossible";
  return to_string(transactions); 
}

int main(){
//First get the amount of trials:
 int trials;
 cin >> trials;
 //vector for the answers
 vector<string> answers; 
 for(int i = 0; i<trials; i++){
    string transactions;   
    vector<Ledger> characters;
    int ab, bc, ca; 
    cin >> ab >> bc >> ca;   
    //initialize and push each person's ledger 
    characters.push_back(Ledger(0)); 
    characters.push_back(Ledger(1));
    characters.push_back(Ledger(2));
    //set everything with wicked ternaries
    //Alice to Bob and vice versa:
    (ab >= 0)? (characters[0].payable[1]= abs(ab), characters[1].receivable[0]= abs(ab)): (characters[0].receivable[1]= abs(ab), characters[1].payable[0]=abs(ab) );
    //Bob to Cynthia and vice versa:
    (bc >= 0)? (characters[1].payable[2]= abs(bc), characters[2].receivable[1]= abs(bc)): (characters[1].receivable[2]= abs(bc), characters[2].payable[1]=abs(bc) );
    //Cynthina to Alice and vice versa: 
    (ca >= 0)? (characters[2].payable[0]= abs(ca), characters[0].receivable[2]= abs(ca)): (characters[2].receivable[0]= abs(ca), characters[0].payable[2]=abs(ca) );
    //Update the banks: 
    for(int j = 0; j<3; j++){
      int hundreds, fifties, twenties, tens, fives, ones; 
      cin >> hundreds >> fifties >> twenties >> tens >> fives >> ones; 
      characters[j].bankAccount[0] = make_pair(hundreds, 100); 
      characters[j].bankAccount[1] = make_pair(fifties, 50); 
      characters[j].bankAccount[2] = make_pair(twenties, 20); 
      characters[j].bankAccount[3] = make_pair(tens, 10); 
      characters[j].bankAccount[4] = make_pair(fives, 5); 
      characters[j].bankAccount[5] = make_pair(ones, 1); 
      //calculate the amounts 
      characters[j].cash = calcBankAmount(characters[j].bankAccount); 
      characters[j].outstanding = calcOutstanding(characters[j].payable, characters[j].receivable);
      characters[j].owed = calcOwed(characters[j].receivable);
    }  
    //sent out to a helper function to go get it settled 
    transactions = figureOutDebt(characters); 
    //push back the answer at the end
    answers.push_back(transactions); 
 }//end of trials

//print everything out
 for(string answer: answers){
     cout<<answer<<endl;
 }

return 0;
}