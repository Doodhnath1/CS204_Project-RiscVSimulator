#include<bits/stdc++.h>
using namespace std;

#define M 32


//Global declration


int pc=0;  // global declration of programme counter 
bitset<32> fetchResult; // stores the fetch result in binary;
int opcode, rs1, rs2, rd,immediate,func3,func7;
string alu_op;

//takes integer input of opcode and return the instruction type R,S U J B etc;
map<string,char> pair_of_instr_type_opcode;

// instruction memry is stored as integer string
map<int,string> instructrion_memory_map;
string hex2bin(string );
int dec2hex();
void RiscVSimulator();
void fetch();
void decode();

int main(){
    //making pair of opcode and instruction type ;
    pair_of_instr_type_opcode["0110011"]='R';
    pair_of_instr_type_opcode["0010011"]='I';
    pair_of_instr_type_opcode["0000011"]='L';
    pair_of_instr_type_opcode["0100011"]='S';
    pair_of_instr_type_opcode["1100011"]='B';
    pair_of_instr_type_opcode["1101111"]='J';
    pair_of_instr_type_opcode["0110111"]='U';
    

    //taking instruction from mc dumb file

    ifstream myfile;
    
    myfile.open("instruction.mc");
    ofstream outfile;
    outfile.open("instruction_copy.mc");
    int counter_next_instruction=0;//counter 

    if(myfile.is_open()){
    while(myfile){
        string temp_instruction;
        myfile>>temp_instruction;


        // stores instruction for execution in map stl
        instructrion_memory_map[counter_next_instruction]=temp_instruction;
        




        //makes the instrction of the format "0x\d   xxxxxxxx"
        if(outfile.is_open() && temp_instruction!=""){
            outfile<<"0x"<<counter_next_instruction<<" "<<temp_instruction<<endl;
            counter_next_instruction+=4;
        }else if(outfile.is_open()){
            
        }
        else {
            cout<<"Error in file2 opening";
        }
        
    }
    }else{
        cout<<"Error in file opening";
    }




    // Risc V simulation begins()
    while (1)
    {
        RiscVSimulator();
    }
    
    




}



int find_func3(){
    int func3=0,mul=1;
    for(int i=12;i<=14;i++){

        if(fetchResult[i]!=0){
            func3=func3+mul;
        }
        mul*=2;
    }
    cout<<"func3 : "<<func3;
    return func3;
}
//evaluates func7 value;
int find_func7(){
    int func7=0,mul=1;
    for(int i=25;i<=31;i++){

        if(fetchResult[i]!=0){
            func7=func7+mul;
        }
        mul*=2;
    }
    cout<<"func7 : "<<func7;
    return func7;
}

string find_opcode(){
    string opcode="";
    for(int i=6;i>=0;i--){
        if(fetchResult[i]==0){
            opcode+='0';
        }else{
           opcode+='1';
        }
        
           
        
    }
    cout<<"OPcode : "<<opcode<<" ";
    return opcode;
}
