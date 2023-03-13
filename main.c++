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

//function for finding funct3
int find_func3(){
    int func3=0,mul=1;
    for(int i=12;i<=14;i++){

        if(fetchResult[i]!=0){
            func3=func3+mul;
        }
        mul*=2;
    }
    // cout<<"func3 : "<<func3;
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
    // cout<<"func7 : "<<func7;
    return func7;
}

//evaluates opcode
string find_opcode(){
    string opcode="";
    for(int i=6;i>=0;i--){
        if(fetchResult[i]==0){
            opcode+='0';
        }else{
           opcode+='1';
        }
        
           
        
    }
    // cout<<"OPcode : "<<opcode<<" ";
    return opcode;
}

//evaluates source register 1
int find_rs1(){
    int rs1=0, mul=1;
    for(int i=15;i<=19;i++){
        if(fetchResult[i]!=0){
            rs1=rs1+mul;
        }
        mul*=2;
    }
    
    // cout<<"rs1 : "<<rs1;
    return rs1;
}

//evaluates source register 2
int find_rs2(){
    int rs2=0, mul=1;
    for(int i=20;i<=24;i++){
        if(fetchResult[i]!=0){
            rs2=rs2+mul;
        }
        mul*=2;
    }
    // cout<<"rs2 : "<<rs2;
    return rs2;
}

//evaluates destination register
int find_rd(){
    int rd=0, mul=1;
    for(int i=7;i<=11;i++){
        if(fetchResult[i]!=0){
            rd=rd+mul;
        }
        mul*=2;
    }
    // cout<<"rd : "<<rd;
    return rd;
}

//evaluates immediate
int find_immed(char instruction_type){
    int immed=0,mul=1;
    switch (instruction_type)
    {
    case 'R':
        
        break;
    case 'I':

        for(int i=20;i<=31;i++){
            if(fetchResult[i]!=0){
                immed=immed+mul;
            }
            mul*=2;
        }
        if(fetchResult[31]==1){
            immed-=4096;
        }
        break;
    case 'S':

        for(int i=7;i<=11;i++){
            if(fetchResult[i]!=0){
                immed=immed+mul;
            }
            mul*=2;
        }
        for(int i=25;i<=31;i++){
            if(fetchResult[i]!=0){
                immed=immed+mul;    
            }
            mul*=2;
        }
        if(fetchResult[31]==1){
            immed-=4096;
        }
        break;
    
    case 'B':

        for(int i=8;i<=11;i++){
            if(fetchResult[i]!=0){
                immed=immed+mul;
            }
            mul*=2;
        }
        for(int i=25;i<=30;i++){
            if(fetchResult[i]!=0){
                immed=immed+mul;    
            }
            mul*=2;
        }
        for(int i=7;i<=7;i++){
            if(fetchResult[i]!=0){
                immed=immed+mul;    
            }
            mul*=2;
        }
        for(int i=31;i<=31;i++){
            if(fetchResult[i]!=0){
                immed=immed+mul;    
            }
            mul*=2;
        }
        immed<<=1;
        if(fetchResult[31]==1){
            immed-=4096*2;
        }
        break;
    case 'U':

        for(int i=12;i<=31;i++){
            if(fetchResult[i]!=0){
                immed=immed+mul;
            }
            mul*=2;
        }
        if(fetchResult[31]==1){
            immed-=1048576;
        }
        break;
    
    case 'J':
            if (opcode=="1101111"){
            for(int i=21;i<=30;i++){
                if(fetchResult[i]!=0){
                    immed=immed+mul;
                }
                mul*=2;
            }
            for(int i=20;i<=20;i++){
                if(fetchResult[i]!=0){
                    immed=immed+mul;
                }
                mul*=2;
            }
            for(int i=12;i<=19;i++){
                if(fetchResult[i]!=0){
                    immed=immed+mul;
                }
                mul*=2;
            }
            for(int i=31;i<=31;i++){
                if(fetchResult[i]!=0){
                    immed=immed+mul;
                }
                mul*=2;
            }
            //left shift to ensure - 0th bit is aligned;
            immed<<=1;
            // cout<<"Immediat: "<<immed<<endl;
            if(fetchResult[31]==1){
                immed-=1048576*2;
            }
        }else{
            cout<<"error in Jal instruction";
        }

        break;
    default:
        break;
    }
    // cout<<"opcode is "<< opcode;
    // cout<<"Immediate is of type  "<<instruction_type<<"the value is : "<<immed;
    return immed;
}
