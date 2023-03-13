#include<bits/stdc++.h>
using namespace std;

#define M 32


//Global declration
//for storing values of registers
map<int,int>registerFile;

int lastBit=0;

//main programme memory of the program
map<int,int>main_mem;
// program counter
int pc=0,nextPc,branchPc;  // global declration of programme counter 

string opcode;            //stores opcode of the running programe
int isBranchTaken=0;     //variable to identify branch taken or not
char instr_type;         //stores the instruction type of the running instruction
bitset<32> fetchResult; // stores the fetch result in binary;
int  rs1, rs2, rd,immediate,func3,func7,aluop_result,op1,op2;
string  alu_op;        // variable for determining alu operation type

//takes integer input of opcode and return the instruction type R,S U J B etc;
map<string,char> pair_of_instr_type_opcode;

// instruction memry is stored as integer string
map<int,string> instructrion_memory_map;
string hex2bin(string);   //function to convert hexadecimal to binary
string dec2hex();        //function to convert decimal to hexadecimal
void RiscVSimulator();   //function to run risc-v simulator
void fetch();
void decode();
void execute();
void memory();
void write_back();

int main(){

    freopen("output.txt","w",stdout); 

    //making pair of opcode and instruction type ;
    pair_of_instr_type_opcode["0110011"]='R';
    pair_of_instr_type_opcode["0010011"]='I';
    pair_of_instr_type_opcode["0000011"]='I';
    pair_of_instr_type_opcode["0100011"]='S';
    pair_of_instr_type_opcode["1100011"]='B';
    pair_of_instr_type_opcode["1101111"]='J';
    pair_of_instr_type_opcode["1100111"]='I';
    pair_of_instr_type_opcode["0110111"]='U';
    pair_of_instr_type_opcode["0010111"]='U';


    // initialisation of register file 
    for(int i=0;i<32;i++){
        registerFile[i]=0;
    }

    registerFile[2]=2147483632;
    for(int i=0;i<1e7;i++){
        main_mem[i]=0;
    }
    // main_mem[100]=3;

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
            instructrion_memory_map[counter_next_instruction]="-1";
        }
        else {
            cout<<"Error in file2 opening"<<endl;
        }
        
    }
    }else{
        cout<<"Error in file opening"<<endl;
    }




    // Risc V simulation begins()
    int counter =1010;
    while (counter--)
    {

        RiscVSimulator();
        if(instructrion_memory_map[pc]=="-1"){




            cout<<"===========Final===== Main Memory ========"<<endl;
   
   for(int i=0;i<=1000;i++){
        cout<<"{ "<<i<< " : "<<main_mem[i]<<"} ";
    }
    cout<<endl;

    cout<<endl;cout<<endl;
     cout<<"===========Final====== Register-File-Data ======="<<endl;
    for (int i=0;i<32;i++) {
        cout<<"{ "<<i<< " : "<<registerFile[i]<<"} ";
    }
    cout<<endl;
    
            // cout<<"final result : "<<registerFile[10]<<endl;
            exit(-1);
        }
    }
    
}
