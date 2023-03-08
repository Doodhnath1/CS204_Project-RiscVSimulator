#include<bits/stdc++.h>
using namespace std;

#define M 32


//Global declration


int pc=0;  // global declration of programme counter 
bitset<32> fetchResult; // stores the fetch result in binary;
int opcode, rs1, rs2, rd,immediate,func3,func7;


//takes integer input of opcode and return the instruction type R,S U J B etc;
map<int,char> pair_of_instr_type_opcode;


char find_instruction_type(){

}
// instruction memry is stored as integer string
map<int,string> instructrion_memory_map;
string hex2bin(string );
void RiscVSimulator();
void fetch();
void decode();

int main(){
    //making pair of opcode and instruction type ;
    pair_of_instr_type_opcode[0110011]='R';
    pair_of_instr_type_opcode[0010011 ]='I';
    pair_of_instr_type_opcode[0100011]='S';
    pair_of_instr_type_opcode[1100011]='B';
    pair_of_instr_type_opcode[1101111]='J';
    pair_of_instr_type_opcode[1100011]='B';
    

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
    for(int i=0;i<=6;i++){

        if(fetchResult[i]!=0){
            func3=func3+mul;
        }
        mul*=10;
    }
    cout<<"func3 : "<<func3;
    return func3;
}
//evaluates func7 value;
int find_func7(){
    int func7=0,mul=1;
    for(int i=0;i<=6;i++){

        if(fetchResult[i]!=0){
            func7=func7+mul;
        }
        mul*=10;
    }
    cout<<"func7 : "<<func7;
    return func7;
}

int find_opcode(){
    int opcode=0,mul=1;
    for(int i=0;i<=6;i++){

        if(fetchResult[i]!=0){
            opcode=opcode+mul;
        }
        mul*=10;
    }
    cout<<"OPcode : "<<opcode;
    return opcode;
}


int find_rs1(){
    int rs1=0, mul=1;
    for(int i=15;i<=19;i++){
        if(fetchResult[i]!=0){
            rs1=rs1+mul;
        }
        mul*=2;
    }
    
    cout<<"rs1 : "<<rs1;
    return rs1;
}

int find_rs2(){
    int rs2=0, mul=1;
    for(int i=20;i<=24;i++){
        if(fetchResult[i]!=0){
            rs2=rs2+mul;
        }
        mul*=2;
    }
    cout<<"rs2 : "<<rs2;
    return rs2;
}

int find_rd(){
    int rd=0, mul=1;
    for(int i=7;i<=11;i++){
        if(fetchResult[i]!=0){
            rd=rd+mul;
        }
        mul*=2;
    }
    cout<<"rd : "<<rd;
    return rd;
}
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
        break;
    case 'U':

        for(int i=12;i<=31;i++){
            if(fetchResult[i]!=0){
                immed=immed+mul;
            }
            mul*=2;
        }
        break;
    
    case 'J':

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
        break;
    default:
        break;
    }
    cout<<"Immediate is of type  "<<instruction_type<<"the value is : "<<immed;
    return immed;
}
//hex 2 binary 
string hex2bin(string s){
     string binary_num = "";

    for (int i = 0; i < s.length(); i++) {
        switch (s[i]) {
            case '0': binary_num += "0000"; break;
            case '1': binary_num += "0001"; break;
            case '2': binary_num += "0010"; break;
            case '3': binary_num += "0011"; break;
            case '4': binary_num += "0100"; break;
            case '5': binary_num += "0101"; break;
            case '6': binary_num += "0110"; break;
            case '7': binary_num += "0111"; break;
            case '8': binary_num += "1000"; break;
            case '9': binary_num += "1001"; break;
            case 'A': case 'a': binary_num += "1010"; break;
            case 'B': case 'b': binary_num += "1011"; break;
            case 'C': case 'c': binary_num += "1100"; break;
            case 'D': case 'd': binary_num += "1101"; break;
            case 'E': case 'e': binary_num += "1110"; break;
            case 'F': case 'f': binary_num += "1111"; break;
            default:
                return "";
        }
    }
    return binary_num;

}


void RiscVSimulator(){
        fetch();
        decode();
        // execute();
        // if(is_mem) mem();
        // exit(-1);
    };



void fetch(){
    string current_instruction=instructrion_memory_map[pc];
    string s=hex2bin(current_instruction);
    bitset<M> bset1(s);
    cout<<"FETCH: Instruction  "<<current_instruction<<" fetched from address "<<"0x"<<pc<<endl;
    fetchResult=bset1;
};

void decode(){
    find_opcode();
    find_rs1();
    find_rs2();
    find_rd();
    find_immed('B');
}
