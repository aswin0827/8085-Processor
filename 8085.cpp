#include<bits/stdc++.h>
#include<fstream>
#include <cstdlib>
using namespace std;

#ifdef _WIN32
#define CLEAR_SCREEN "cls"
#else
#define CLEAR_SCREEN "clear"
#endif

short SP = 65535; //stack pointer pre-defined value;
short memory[65536];
short reg[7];
short M;
string F="00000000";
// sign zero - ac - parity - carry

int binToDec(string binary) {
    int decimal = bitset<8>(binary).to_ulong();
    return decimal;
}

string decToBin(int decimal) {
    string binary = bitset<8>(decimal).to_string();
    return binary;
}

int hexToDec(string hex){
    int dec = 0;
    int base = 1;
    for (int i = hex.length() - 1; i >= 0; i--) {
        if (hex[i] >= '0' && hex[i] <= '9'){
            dec += (hex[i] - '0') * base;
        }
        else if (hex[i] >= 'A' && hex[i] <= 'F'){
            dec += (hex[i] - 'A' + 10) * base;
        }
        else if (hex[i] >= 'a' && hex[i] <= 'f'){
            dec += (hex[i] - 'a' + 10) * base;
        }
        else{
            throw 105;
        }
        base *= 16;
    }
    return dec;
}

short stPop(){
    //return and increase SP;
    return memory[SP++];
}

void stPush(short val){
    //decrease sp and insert;
    memory[--SP] = val;
}

string decToHex(int dec){
    string hex = "";
    int remainder;
    string alpha = "0123456789ABCDEF";
    if(dec <= 15){
        hex += "0";
        hex += alpha[dec];
        return hex;
    }
    while (dec > 0) {
        remainder = dec % 16;
        if(remainder < 10){
            hex = char(remainder + 48) + hex;
        }
        else{
            hex = char(remainder + 55) + hex;
        }
        dec /= 16;
    }
    return hex;
}

int mem(string loc){
    int decloc = hexToDec(loc);
    return memory[decloc];
}

short get_reg(char r='A'){
    if(r == 'A'){
        return reg[0];
    }
    else if(r == 'B'){
        return reg[1];
    }
    else if(r == 'C'){
        return reg[2];
    }
    else if(r == 'D'){
        return reg[3];
    }
    else if(r == 'E'){
        return reg[4];
    }
    else if(r == 'H'){
        return reg[5];
    }
    else if(r == 'L'){
        return reg[6];
    }
    else if(r=='F'){
        return binToDec(F);
    }
    else{
        throw 101;
        return -1;
    }

}

string getPointer(){
    //returns the address of HL pointer
    return decToHex(get_reg('H')) + decToHex(get_reg('L'));
}

short get_M(){
    string s1 = decToHex(get_reg('H'));
    string s2 = decToHex(get_reg('L'));
    string HLpointer = s1+s2;
    return mem(HLpointer);
}

void put_M(short val){
    //adds the given value to the memory at HL;
    memory[hexToDec(getPointer())] = val;
}

string increaseHexByOne(string hex){
    int dec = stoi(hex, 0, 16);
    dec += 1;
    string result = "";
    while (dec != 0) {
        int remainder = dec % 16;
        if (remainder < 10){
            result += char(remainder + 48);
        }
        else{
            result += char(remainder + 55);
        }
        dec /= 16;
    }
    reverse(result.begin(), result.end());
    if (hexToDec(result) > 65535){
        throw 103;
    }
    return result;
}

string decreaseHex(string hexNumber) {
    int len = hexNumber.length();
    int i = len - 1;
    while(i>=0 && hexNumber[i] == '0'){
        hexNumber[i] = 'f';
        i--;
    }
    if(i>=0){
        hexNumber[i] = char(hexNumber[i] - 1);
    }
    return hexNumber;
}

void savetofile(){
    fstream new_file;
    new_file.open("memory.txt",ios::out);
    if(!new_file){
        return;
    }
    else{
        for(int i=0; i < 65536; i++){
            new_file << memory[i]<<"\n";
        }
    }
}

void loadmemory(){
    ifstream inputFile("memory.txt");
    if (!inputFile.is_open()) {
        return ;    
    }
    string line;
    int i=0;
    while (getline(inputFile, line)) {
        memory[i++]=stoi(line);
    }
}


bool check_flag(int opcode){
    return true;
}

void xra(int val){
    reg[0]=reg[0]^val;
}

void cma(int val){
    reg[0]= ~val;
}

int parity(int num){
    int count=0;
    while(num!=0){
      count++;
      num=num&(num-1);
    }
    if(count % 2 == 0){
      return 1;//the parity is even
    } 
    else {
        return 0;//the parity is odd
    }
    return 0;
}

void change_flag(int val){
    //[ S | 0 | - | AC | - | P | - | C ]

    if(val < 0){// To check is less than negitive or not
        F[0] = '1';
    } 
    else{
        F[0] = '0';
    }
    if(val == 0){// To check is zero or not
        F[1] = '1';
    }
    else{
        F[1] = '0';
    }
    if(parity(val)){
        F[5] = '1';
    }
    else{
        F[5] = '0';
    }
    if(val > 255 || val<0){// To check for Carry
        val = abs(val);
        string val_ = decToHex(val).substr(decToHex(val).length()-2,2);
        F[7] =  '1';
        reg[0] = hexToDec(val_);
    }
    else{
        F[7] = '0';
    }
}

void ora(int val){
    reg[0]=reg[0] | val;
}

void ana(int val){
    reg[0]=reg[0] & val;
}

void runprogram(int loc){
    cout<<"EXECUTING"<<endl;
    while(loc < 65536){
        string hex_mem = decToHex(memory[loc]);
        //skipping IF
        char teleport = hex_mem[0];
        if(teleport == '0')
            goto ZERO;
        else if(teleport == '1')
            goto ONE;
        else if(teleport == '2')
            goto TWO;
        else if(teleport == '3')
            goto THREE;
        else if(teleport == '4')
            goto FOUR;
        else if(teleport == '5')
            goto FIVE;
        else if(teleport == '6')
            goto SIX;
        else if(teleport == '7')
            goto SEVEN;
        else if(teleport == '8')
            goto EIGHT;
        else if(teleport == '9')
            goto NINE;
        else if(teleport == 'A')
            goto A;
        else if(teleport == 'B')
            goto B;
        else if(teleport == 'C'){
            goto C;
        }
        else if(teleport == 'D')
            goto D;
        else if(teleport == 'E')
            goto E;
        else if(teleport == 'F')
            goto F;
        else
            throw 104;
        
ZERO:
        if(hex_mem == "00"){// NOP
        }
        else if(hex_mem == "01"){// LXI B, data16 - Load 16-bit immediate data into registers B and C
            loc++;
            reg[2]=memory[loc];
            loc++;
            reg[1]=memory[loc];
        }
        else if(hex_mem == "02"){// STAX B - Store accumulator indirect
            string s = decToHex(reg[1])+decToHex(reg[2]);
            memory[hexToDec(s)]=reg[0];
        }
        else if(hex_mem == "03"){// INX B - Increment registers B and C
            string s = decToHex(reg[1])+decToHex(reg[2]);
            s = increaseHexByOne(s);
            string s1 = "  ";
            s1[0]=s[0];
            s1[1]=s[1];
            reg[1]=hexToDec(s1);
            string s2 = "  ";
            s2[0]=s[2];
            s2[1]=s[3];
            reg[2]=hexToDec(s2);//Flag doesnot change
        }
        else if(hex_mem == "04"){// INR B - Increment register B
            reg[1]++;
            int x = reg[0];
            change_flag(reg[1]);
            reg[0]=x;
        }
        else if(hex_mem == "05"){// DCR B - Decrement register B
            reg[1]--;
            int x = reg[0];
            change_flag(reg[1]);
            reg[0]=x;
        }
        else if(hex_mem == "06"){// MVI B, data8 - Move immediate 8-bit data into register B
            loc++;
            reg[1]=memory[loc];
        }
        else if(hex_mem == "07"){ // RLC - Rotate accumulator left

        }
        else if(hex_mem == "09"){// DAD B - Double add registers B and C to HL

        }
        else if(hex_mem == "0A"){// LDAX B - Load accumulator indirect
            string s = decToHex(reg[1])+decToHex(reg[2]);
            reg[0]=mem(s);
        }
        else if(hex_mem == "0B"){// DCX B - Decrement registers B and C
            string s = decToHex(reg[1])+decToHex(reg[2]);
            s = decreaseHex(s);
            string s1 = "  ";
            s1[0]=s[0];
            s1[1]=s[1];
            reg[1]=hexToDec(s1);
            string s2 = "  ";
            s2[0]=s[2];
            s2[1]=s[3];
            reg[2]=hexToDec(s2);
        }
        else if(hex_mem == "0C"){// INR C - Increment register C
            reg[2]++;
            int x = reg[0];
            change_flag(reg[2]);
            reg[0]=x;
        }
        else if(hex_mem == "0D"){// DCR C - Decrement register C
            reg[2]--;
            int x = reg[0];
            reg[0]=x;
            change_flag(reg[2]);
        }
        else if(hex_mem == "0E"){// MVI C, data8 - Move immediate 8-bit data into register C
            loc++;
            reg[2]=memory[loc];
        }
        else if(hex_mem == "0F"){// RRC - Rotate accumulator right
        }
        goto END;
ONE:
        if(hex_mem == "11"){// LXI D, data16 - Load 16-bit immediate data into registers D and E
            loc++;
            reg[4]=memory[loc];
            loc++;
            reg[3]=memory[loc];
        }
        else if(hex_mem == "12"){// STAX D - Store accumulator indirect
            string s = decToHex(reg[3])+decToHex(reg[4]);
            memory[hexToDec(s)]=reg[0];
        }
        else if(hex_mem == "13"){// INX D - Increment registers D and E
            string s = decToHex(reg[3])+decToHex(reg[4]);
            s = increaseHexByOne(s);
            string s1 = "  ";
            s1[0]=s[0];
            s1[1]=s[1];
            reg[3]=hexToDec(s1);
            string s2 = "  ";
            s2[0]=s[2];
            s2[1]=s[3];
            reg[4]=hexToDec(s2);
        }
        else if(hex_mem == "14"){// INR D - Increment register D
            reg[3]++;
            int x = reg[0];
            change_flag(reg[3]);
            reg[0]=x;
        }
        else if(hex_mem == "15"){// DCR D - Decrement register D
            reg[3]--;
            int x = reg[0];
            change_flag(reg[3]);
            reg[0]=x;
        }
        else if(hex_mem == "16"){// MVI D, data8 - Move immediate 8-bit data into register D
            loc++;
            reg[3]=memory[loc];
        }
        else if(hex_mem == "17"){// RAL - Rotate accumulator left through carry
        }
        else if(hex_mem == "19"){// DAD D - Double add registers D and E to HL
        }
        else if(hex_mem == "1A"){// LDAX D - Load accumulator indirect
            string s = decToHex(reg[3])+decToHex(reg[4]);
            reg[0]=mem(s);
        }
        else if(hex_mem == "1B"){// DCX D - Decrement registers D and E
            string s = decToHex(reg[3])+decToHex(reg[4]);
            s = decreaseHex(s);
            string s1 = "  ";
            s1[0]=s[0];
            s1[1]=s[1];
            reg[3]=hexToDec(s1);
            string s2 = "  ";
            s2[0]=s[2];
            s2[1]=s[3];
            reg[4]=hexToDec(s2);
        }
        else if(hex_mem == "1C"){// INR E - Increment register E
            reg[4]++;
            int x = reg[0];
            change_flag(reg[4]);
            reg[0]=x;
        }
        else if(hex_mem == "1D"){// DCR E - Decrement register E
            reg[4]--;
            int x = reg[0];
            change_flag(reg[4]);
            reg[0]=x;
        }
        else if(hex_mem == "1E"){// MVI E, data8 - Move immediate 8-bit data into register E
            loc++;
            reg[4]=memory[loc];
        }
        else if(hex_mem == "1F"){// RAR - Rotate accumulator right through carry
        }
        goto END;
TWO:
        if(hex_mem == "20"){// RIM - Read interrupt mask
        }
        else if(hex_mem == "21"){// LXI H, data16 - Load 16-bit immediate data into registers H and L
            loc++;
            reg[6]=memory[loc];
            loc++;
            reg[5]=memory[loc];
        }
        else if(hex_mem == "22"){// SHLD address - Store H and L registers direct
            loc++;
            string s=decToHex(loc);
            loc++;
            s+=decToHex(loc);
            memory[hexToDec(s)]=reg[6];
            s=increaseHexByOne(s);
            memory[hexToDec(s)]=reg[5];
        }
        else if(hex_mem == "23"){// INX H - Increment registers H and L
            string s = decToHex(reg[5])+decToHex(reg[6]);
            s = increaseHexByOne(s);
            string s1 = "  ";
            s1[0]=s[0];
            s1[1]=s[1];
            reg[5]=hexToDec(s1);
            string s2 = "  ";
            s2[0]=s[2];
            s2[1]=s[3];
            reg[6]=hexToDec(s2);
        }
        else if(hex_mem == "24"){// INR H - Increment register H
            reg[5]++;
            int x = reg[0];
            change_flag(reg[5]);
            reg[0]=x;
        }
        else if(hex_mem == "25"){// DCR H - Decrement register H
            reg[5]--;
            int x = reg[0];
            change_flag(reg[5]);
            reg[0]=x;
        }
        else if(hex_mem == "26"){// MVI H, data8 - Move immediate 8-bit data into register H
            loc++;
            reg[5]=memory[loc];
        }
        else if(hex_mem == "27"){// DAA - Decimal adjust accumulator 
        }
        else if(hex_mem == "29"){// DAD H - Double add registers H and L to HL
        }
        else if(hex_mem == "2A"){// LHLD address - Load H and L registers direct
            loc++;
            string s = decToHex(loc);
            loc++;
            s+=decToHex(loc);
            reg[6]=mem(s);
            s = increaseHexByOne(s);
            reg[5]=mem(s);
        }
        else if(hex_mem == "2B"){// DCX H - Decrement registers H and L
            string s = decToHex(reg[5])+decToHex(reg[6]);
            s = decreaseHex(s);
            string s1 = "  ";
            s1[0]=s[0];
            s1[1]=s[1];
            reg[5]=hexToDec(s1);
            string s2 = "  ";
            s2[0]=s[2];
            s2[1]=s[3];
            reg[6]=hexToDec(s2);
        }
        else if(hex_mem == "2C"){// INR L - Increment register L
            reg[6]++;
            int x = reg[0];
            change_flag(reg[6]);
            reg[0]=x;
        }
        else if(hex_mem == "2D"){// DCR L - Decrement register L
            reg[6]--;
            int x = reg[0];
            change_flag(reg[6]);
            reg[0]=x;
        }
        else if(hex_mem == "2E"){// MVI L, data8 - Move immediate 8-bit data into register L
            loc++;
            reg[6]=memory[loc];
        }
        else if(hex_mem == "2F"){// CMA - Complement accumulator
            cma(reg[0]);
        }
        goto END;
THREE:
        if(hex_mem == "30"){// SIM - Set interrupt mask
        }
        else if(hex_mem == "31"){// LXI SP, data16 - Load 16-bit immediate data into stack pointer
        }
        else if(hex_mem == "32"){// STA address - Store accumulator direct
            loc++;
            string s2 = decToHex(memory[loc]);
            loc++;
            string s1 = decToHex(memory[loc]);
            string s3 = s1+s2;
            memory[hexToDec(s3)]=reg[0];
        }
        else if(hex_mem == "33"){// INX SP - Increment stack pointer
        }
        else if(hex_mem == "34"){// INR M - Increment memory pointed by HL
            string s = decToHex(reg[5])+decToHex(reg[6]);
            memory[hexToDec(s)]++;
        }
        else if(hex_mem == "35"){// DCR M - Decrement memory pointed by HL
            string s = decToHex(reg[5])+decToHex(reg[6]);
            memory[hexToDec(s)]--;
        }
        else if(hex_mem == "36"){// MVI M, data8 - Move immediate 8-bit data into memory pointed by HL
            loc++;
            int x = memory[loc];
            string s = decToHex(reg[5])+decToHex(reg[6]);
            memory[hexToDec(s)]=x;
        }
        else if(hex_mem == "37"){// STC - Set carry
            F[7]='1';
        }
        else if(hex_mem == "39"){// DAD SP - Double add stack pointer to HL
        }
        else if(hex_mem == "3A"){// LDA address - Load accumulator direct
            loc++;
            string s2 = decToHex(memory[loc]);
            loc++;
            string s1 = decToHex(memory[loc]);
            string s = s1+s2;
            reg[0]=mem(s);
        }
        else if(hex_mem == "3B"){// DCX SP - Decrement stack pointer
        }
        else if(hex_mem == "3C"){// INR A - Increment accumulator
            reg[0]++;
            change_flag(reg[0]);
        }
        else if(hex_mem == "3D"){// DCR A - Decrement accumulator
            reg[0]--;
            change_flag(reg[0]);
        }
        else if(hex_mem == "3E"){// MVI A, data8 - Move immediate 8-bit data into accumulator
            loc++;
            reg[0]=memory[loc];
        }
        else if(hex_mem == "3F"){// CMC - Complement carry
            if(F[7]== '1'){
                F[7]='0';
            }
            else{
                F[7]='1';
            }
        }
        goto END;
FOUR:
        if(hex_mem == "41"){//MOV B,C
            reg[1]=reg[2];
        }
        else if(hex_mem == "42"){//MOV B,D
            reg[1]=reg[3];
        }
        else if(hex_mem == "43"){//MOV B,E
            reg[1]=reg[4];
        }
        else if(hex_mem == "44"){//MOV B,H
            reg[1]=reg[5];
        }
        else if(hex_mem == "45"){//MOV B,L
            reg[1]=reg[6];
        }
        else if(hex_mem == "46"){//MOV B,M
            reg[1] = get_M();
        }
        else if(hex_mem == "47"){//MOV B,A
            reg[1]=reg[0];
        }
        else if(hex_mem == "48"){//MOV C,B
            reg[2]=reg[1];
        }
        else if(hex_mem == "4A"){//MOV C,D
            reg[2]=reg[3];
        }
        else if(hex_mem == "4B"){//MOV C,E
            reg[2]=reg[4];
        }
        else if(hex_mem == "4C"){//MOV C,H
            reg[2]=reg[5];
        }
        else if(hex_mem == "4D"){//MOV C,L
            reg[2]=reg[6];
        }
        else if(hex_mem == "4E"){//MOV C,M
            reg[2] = get_M();
        }
        else if(hex_mem == "4F"){//MOV C,A
            reg[2]=reg[0];
        }
        goto END;
FIVE:
        if(hex_mem == "50"){//MOV D,B
            reg[3]=reg[1];
        }
        else if(hex_mem == "51"){//MOV D,C
            reg[3]=reg[2];
        }
        else if(hex_mem == "53"){//MOV D,E
            reg[3]=reg[4];
        }
        else if(hex_mem == "54"){//MOV D,H
            reg[3]=reg[5];
        }
        else if(hex_mem == "55"){//MOV D,L
            reg[3]=reg[6];
        }
        else if(hex_mem == "56"){//MOV D,M
            reg[3] = get_M();
        }
        else if(hex_mem == "57"){//MOV D,A
            reg[3]=reg[0];
        }
        else if(hex_mem == "58"){//MOV E,B
            reg[4]=reg[1];
        }
        else if(hex_mem == "59"){//MOV E,C
            reg[4]=reg[2];
        }
        else if(hex_mem == "5A"){//MOV E,D
            reg[4]=reg[3];
        }
        else if(hex_mem == "5C"){//MOV E,H
            reg[4]=reg[5];
        }
        else if(hex_mem == "5D"){//MOV E,L
            reg[4]=reg[6];
        }
        else if(hex_mem == "5E"){//MOV E,M
            reg[4] = get_M();
        }
        else if(hex_mem == "5F"){//MOV E,A
            reg[4]=reg[0];
        }
        goto END;
SIX:
        if(hex_mem == "60"){//MOV H,B
            reg[5]=reg[1];
        }
        else if(hex_mem == "61"){//MOV H,C
            reg[5]=reg[2];
        }
        else if(hex_mem == "62"){//MOV H,D
            reg[5]=reg[3];
        }
        else if(hex_mem == "63"){//MOV H,E
            reg[5]=reg[4];
        }
        else if(hex_mem == "65"){//MOV H,L
            reg[5]=reg[6];
        }
        else if(hex_mem == "66"){//MOV H,M
            reg[5] = get_M();
        }
        else if(hex_mem == "67"){//MOV H,A
            reg[5]=reg[0];
        }
        else if(hex_mem == "68"){//MOV L,B
            reg[6]=reg[1];
        }
        else if(hex_mem == "69"){//MOV L,C
            reg[6]=reg[2];
        }
        else if(hex_mem == "6A"){//MOV L,D
            reg[6]=reg[3];
        }
        else if(hex_mem == "6B"){//MOV L,E
            reg[6]=reg[4];
        }
        else if(hex_mem == "6C"){//MOV L,H
            reg[6]=reg[5];
        }
        else if(hex_mem == "6E"){//MOV L,M
            reg[6] = get_M();
        }
        else if(hex_mem == "6F"){//MOV L,A
            reg[6]=reg[0];
        }
        goto END;
SEVEN:
        if(hex_mem == "70"){//MOV M,B
            put_M(reg[1]);
        }
        else if(hex_mem == "71"){//MOV M,C
            put_M(reg[2]);
        }
        else if(hex_mem == "72"){//MOV M,D
            put_M(reg[3]);
        }
        else if(hex_mem == "73"){//MOV M,E
            put_M(reg[4]);
        }
        else if(hex_mem == "74"){//MOV M,H
            put_M(reg[5]);
        }
        else if(hex_mem == "75"){//MOV M,L
            put_M(reg[6]);
        }
        else if(hex_mem == "77"){//MOV M,A
            put_M(reg[0]);
        }
        else if(hex_mem == "78"){// Mov A,B
            reg[0]=reg[1];
        }
        else if(hex_mem == "79"){//MOV A,C
            reg[0]=reg[2];
        }
        else if(hex_mem == "7A"){//MOV A,D
            reg[0]=reg[3];
        }
        else if(hex_mem == "7B"){//MOV A,E
            reg[0]=reg[4];
        }
        else if(hex_mem == "7C"){//MOV A,H
            reg[0]=reg[5];
        }
        else if(hex_mem == "7D"){//MOV A,L
            reg[0]=reg[6];
        }
        else if(hex_mem == "7E"){//MOV A,M
            reg[0] = get_M();
        }
        goto END;
EIGHT:
        if(hex_mem == "80"){// ADD B
            reg[0]=reg[0]+reg[1];
            change_flag(reg[0]);
        }
        else if(hex_mem == "81"){// ADD C
            reg[0]=reg[0]+reg[2];
            change_flag(reg[0]);
        }
        else if(hex_mem == "82"){// ADD D
            reg[0]=reg[0]+reg[3];
            change_flag(reg[0]);
        }
        else if(hex_mem == "83"){// ADD E
            reg[0]=reg[0]+reg[4];
            change_flag(reg[0]);
        }
        else if(hex_mem == "84"){// ADD H
            reg[0]=reg[0]+reg[5];
            change_flag(reg[0]);
        }
        else if(hex_mem == "85"){// ADD L
            reg[0]=reg[0]+reg[6];
            change_flag(reg[0]);
        }
        else if(hex_mem == "86"){// ADD M
            reg[0]=reg[0]+get_M();
            change_flag(reg[0]);
        }
        else if(hex_mem == "87"){// ADD A
            reg[0]=reg[0]+reg[0];
            change_flag(reg[0]);
        }
        else if(hex_mem == "88"){// ADC B
            reg[0]+=reg[1]+int(F[7]);
            change_flag(reg[0]);
        }
        else if(hex_mem == "89"){// ADC C
            reg[0]+=reg[2]+int(F[7]);
            change_flag(reg[0]);
        }
        else if(hex_mem == "8A"){// ADC D
            reg[0]+=reg[3]+int(F[7]);
            change_flag(reg[0]);
        }
        else if(hex_mem == "8B"){// ADC E
            reg[0]+=reg[4]+int(F[7]);
            change_flag(reg[0]);
        }
        else if(hex_mem == "8C"){// ADC H
            reg[0]+=reg[5]+int(F[7]);
            change_flag(reg[0]);
        }
        else if(hex_mem == "8D"){// ADC L
            reg[0]+=reg[6]+int(F[7]);
            change_flag(reg[0]);
        }
        else if(hex_mem == "8E"){// ADC M
            reg[0]+=get_M()+int(F[7]);
            change_flag(reg[0]);
        }
        else if(hex_mem == "8F"){// ADC A
            reg[0]+=reg[0]+int(F[7]);
            change_flag(reg[0]);
        }
        goto END;
NINE:
        if(hex_mem == "90"){// SUB B
            reg[0]=reg[0]-reg[1];
            change_flag(reg[0]);
        }
        else if(hex_mem == "91"){// SUB C
            reg[0]=reg[0]-reg[2];
            change_flag(reg[0]);
        }
        else if(hex_mem == "92"){// SUB D
            reg[0]=reg[0]-reg[3];
            change_flag(reg[0]);
        }
        else if(hex_mem == "93"){// SUB E
            reg[0]=reg[0]-reg[4];
            change_flag(reg[0]);
        }
        else if(hex_mem == "94"){// SUB H
            reg[0]=reg[0]-reg[5];
            change_flag(reg[0]);
        }
        else if(hex_mem == "95"){// SUB L
            reg[0]=reg[0]-reg[6];
            change_flag(reg[0]);
        }
        else if(hex_mem == "96"){// SUB M
            reg[0]=reg[0]-get_M();
            change_flag(reg[0]);
        }
        else if(hex_mem == "97"){// SUB A
            reg[0]=reg[0]-reg[0];
            change_flag(reg[0]);
        }
        else if(hex_mem == "98"){// SBB B   
            reg[0]=reg[0]-reg[1]-int(F[7]);
            change_flag(reg[0]);
        }
        else if(hex_mem == "99"){// SBB C
            reg[0]=reg[0]-reg[2]-int(F[7]);
            change_flag(reg[0]);
        }
        else if(hex_mem == "9A"){// SBB D
            reg[0]=reg[0]-reg[3]-int(F[7]);
            change_flag(reg[0]);
        }
        else if(hex_mem == "9B"){// SBB E
            reg[0]=reg[0]-reg[4]-int(F[7]);
            change_flag(reg[0]);
        }
        else if(hex_mem == "9C"){// SBB H
            reg[0]=reg[0]-reg[5]-int(F[7]);
            change_flag(reg[0]);
        }
        else if(hex_mem == "9D"){// SBB L
            reg[0]=reg[0]-reg[6]-int(F[7]);
            change_flag(reg[0]);
        }
        else if(hex_mem == "9E"){// SBB M
            reg[0]=reg[0]-get_M()-int(F[7]);
            change_flag(reg[0]);
        }
        else if(hex_mem == "9F"){// SBB A
            reg[0]=reg[0]-reg[0]-int(F[7]);
            change_flag(reg[0]);
        }
        goto END;
A:
        if(hex_mem == "A0"){// ANA B
            ana(reg[1]);
            change_flag(reg[0]);
        }
        else if(hex_mem == "A1"){// ANA C
            ana(reg[2]);
            change_flag(reg[0]);
        }
        else if(hex_mem == "A2"){// ANA D
            ana(reg[3]);
            change_flag(reg[0]);
        }
        else if(hex_mem == "A3"){// ANA E
            ana(reg[4]);
            change_flag(reg[0]);
        }
        else if(hex_mem == "A4"){// ANA H
            ana(reg[5]);
            change_flag(reg[0]);
        }
        else if(hex_mem == "A5"){// ANA L  
            ana(reg[6]);
            change_flag(reg[0]);
        }
        else if(hex_mem == "A6"){// ANA M
            ana(get_M());
            change_flag(reg[0]);
        }
        else if(hex_mem == "A7"){// ANA A
            ana(reg[0]);
            change_flag(reg[0]);
        }
        else if(hex_mem == "A8"){// XRA B
            xra(reg[1]);
            change_flag(reg[0]);
        }
        else if(hex_mem == "A9"){// XRA C
            xra(reg[2]);
            change_flag(reg[0]);
        }
        else if(hex_mem == "AA"){// XRA D
            xra(reg[3]);
            change_flag(reg[0]);
        }
        else if(hex_mem == "AB"){// XRA E
            xra(reg[4]);
            change_flag(reg[0]);
        }
        else if(hex_mem == "AC"){// XRA H
            xra(reg[5]);
            change_flag(reg[0]);
        }
        else if(hex_mem == "AD"){// XRA L
            xra(reg[6]);
            change_flag(reg[0]);
        }
        else if(hex_mem == "AE"){// XRA M
            xra(get_M());
            change_flag(reg[0]);
        }
        else if(hex_mem == "AF"){// XRA A
            xra(reg[0]);
            change_flag(reg[0]);
        }
        goto END;
B:
        if(hex_mem == "B0"){// ORA B
            ora(reg[1]);
            change_flag(reg[0]);
        }
        else if(hex_mem == "B1"){// ORA C
            ora(reg[2]);
            change_flag(reg[0]);
        }
        else if(hex_mem == "B2"){// ORA D
            ora(reg[3]);
            change_flag(reg[0]);
        }
        else if(hex_mem == "B3"){// ORA E
            ora(reg[4]);
            change_flag(reg[0]);
        }
        else if(hex_mem == "B4"){// ORA H
            ora(reg[5]);
            change_flag(reg[0]);
        }
        else if(hex_mem == "B5"){// ORA L
            ora(reg[6]);
            change_flag(reg[0]);
        }
        else if(hex_mem == "B6"){// ORA M
            ora(get_M());
            change_flag(reg[0]);
        }
        else if(hex_mem == "B7"){// ORA A
            ora(reg[0]);
            change_flag(reg[0]);
        }
        else if(hex_mem == "B8"){// CMP B
            int x=reg[0];
            change_flag(reg[0]-reg[1]);
            reg[0]=x;
        }
        else if(hex_mem == "B9"){// CMP C
            int x=reg[0];
            change_flag(reg[0]-reg[2]);
            reg[0]=x;
        }
        else if(hex_mem == "BA"){// CMP D
            int x=reg[0];
            change_flag(reg[0]-reg[3]);
            reg[0]=x;
        }
        else if(hex_mem == "BB"){// CMP E
            int x=reg[0];
            change_flag(reg[0]-reg[4]);
            reg[0]=x;
        }
        else if(hex_mem == "BC"){// CMP H
            int x=reg[0];
            change_flag(reg[0]-reg[5]);
            reg[0]=x;
        }
        else if(hex_mem == "BD"){// CMP L
            int x=reg[0];
            change_flag(reg[0]-reg[6]);
            reg[0]=x;
        }
        else if(hex_mem == "BE"){// CMP M
            int x=reg[0];
            change_flag(reg[0]-get_M());
            reg[0]=x;
        }
        else if(hex_mem == "BF"){// CMP A
            int x=reg[0];
            change_flag(reg[0]-reg[0]);
            reg[0]=x;
        }
        goto END;
C:
        if(hex_mem == "C0"){// RNZ
            if(F[1] == '0'){
                loc = stPop();
                continue;
            }
        }
        else if(hex_mem == "C1"){// POP B
            //store intp C then B;
            reg[2] = stPop();
            reg[1] = stPop();
        } 
        else if(hex_mem == "C2"){// JNZ
            if(F[1]=='0'){
                loc++;
                string s2 = decToHex(memory[loc]);
                loc++;
                string s1 = decToHex(memory[loc]);
                string s = s1+s2;
                loc=hexToDec(s);
                continue;
            }
            else{
                loc++;
                loc++;
            }
        }
        else if(hex_mem == "C3"){// JMP
            loc++;
            string s2 = decToHex(memory[loc]);
            loc++;
            string s1 = decToHex(memory[loc]);
            string s = s1+s2;
            loc=hexToDec(s);
            continue;
        }
        else if(hex_mem == "C4"){// CNZ
            if(F[1]=='0'){
                loc++;
                string s2 = decToHex(memory[loc]);
                loc++;
                string s1 = decToHex(memory[loc]);
                string s = s1+s2;
                stPush(++loc);
                loc=hexToDec(s);
                continue;
            }
            else{
                loc++;
                loc++;
            }
        }
        else if(hex_mem == "C5"){// PUSH B
            //store value of B then C into the Stack
            stPush(get_reg('B'));
            stPush(get_reg('C'));
        }
        else if(hex_mem == "C6"){// ADI
        }
        else if(hex_mem == "C7"){// RST 0
        }
        else if(hex_mem == "C8"){// RZ
            if(F[1]=='1'){
                loc = stPop();
                continue;
            }
        }
        else if(hex_mem == "C9"){// RET
            loc = stPop();
            continue;
        }
        else if(hex_mem == "CA"){// JZ Address
            if(F[1]=='1'){
                loc++;
                string s2 = decToHex(memory[loc]);
                loc++;
                string s1 = decToHex(memory[loc]);
                string s = s1+s2;
                loc=hexToDec(s);
                continue;
            }
            else{
                loc++;
                loc++;
            }
        }
        else if(hex_mem == "CC"){// CZ
            if(F[1]=='1'){
                loc++;
                string s2 = decToHex(memory[loc]);
                loc++;
                string s1 = decToHex(memory[loc]);
                string s = s1+s2;
                stPush(++loc);
                loc=hexToDec(s);
                continue;
            }
            else{
                loc++;
                loc++;
            }
        }
        else if(hex_mem == "CD"){// CALL
            loc++;
            string s2 = decToHex(memory[loc]);
            loc++;
            string s1 = decToHex(memory[loc]);
            string s = s1+s2;
            stPush(++loc);
            loc=hexToDec(s);
            continue;
        }
        else if(hex_mem == "CE"){// ACI
            loc++;
            reg[0]=reg[0]+memory[loc]+int(F[7]);
            change_flag(reg[0]);
        }
        else if(hex_mem == "CF"){// RST 1
        }
        goto END;
D:
        if(hex_mem == "D0"){// RNC
            if(F[7]=='0'){
                loc = stPop();
                continue;
            }
        }
        else if(hex_mem == "D1"){// POP D
            reg[4] = stPop();
            reg[3] = stPop();
        }
        else if(hex_mem == "D2"){// JNC
            if(F[7]=='1'){
                loc++;
                loc++;
            }
            else{
                loc++;
                string s2 = decToHex(memory[loc]);
                loc++;
                string s1 = decToHex(memory[loc]);
                string s = s1+s2;
                loc=hexToDec(s);
                continue;
            }
        }
        else if(hex_mem == "D3"){// OUT
        }
        else if(hex_mem == "D4"){// CNC
            if(F[7]=='1'){
                loc++;
                loc++;
            }
            else{
                loc++;
                string s2 = decToHex(memory[loc]);
                loc++;
                string s1 = decToHex(memory[loc]);
                string s = s1+s2;
                stPush(++loc);
                loc=hexToDec(s);
                continue;
            }
        }
        else if(hex_mem == "D5"){// PUSH D
            stPush(get_reg('D'));
            stPush(get_reg('E'));
        }
        else if(hex_mem == "D6"){// SUI
            loc++;
            reg[0]=reg[0]-memory[loc];
            change_flag(reg[0]);
        }
        else if(hex_mem == "D7"){// RST 2
        }
        else if(hex_mem == "D8"){// RC
            if(F[7]=='1'){
                loc = stPop();
                continue;
            }
        }
        else if(hex_mem == "DA"){// JC
            if(F[7]=='1'){
                loc++;
                string s2 = decToHex(memory[loc]);
                loc++;
                string s1 = decToHex(memory[loc]);
                string s = s1+s2;
                loc=hexToDec(s);
                continue;
            }
            else{
                loc++;
                loc++;
            }
        }
        else if(hex_mem == "DB"){// IN
        }
        else if(hex_mem == "DC"){// CC
            if(F[7]=='1'){
                loc++;
                string s2 = decToHex(memory[loc]);
                loc++;
                string s1 = decToHex(memory[loc]);
                string s = s1+s2;
                stPush(++loc);
                loc=hexToDec(s);
                continue;
            }
            else{
                loc++;
                loc++;
            }
        }
        else if(hex_mem == "DE"){// SBI
        }
        else if(hex_mem == "DF"){// RST 3
        }
        goto END;
E:
        if(hex_mem == "E0"){// RPO
            if(F[5] == '0'){
                loc = stPop();
                continue;
            }
        }
        else if(hex_mem == "E1"){// POP H
            reg[6] = stPop();
            reg[5] = stPop();
        }
        else if(hex_mem == "E2"){// JPO
            if(F[5]=='0'){
                loc++;
                string s2 = decToHex(memory[loc]);
                loc++;
                string s1 = decToHex(memory[loc]);
                string s = s1+s2;
                loc=hexToDec(s);
                continue;
            }
            else{
                loc++;
                loc++;
            }
        }
        else if(hex_mem == "E3"){// XTHL - exchange HL with top stack pointer sp & sp+1
        }
        else if(hex_mem == "E4"){// CPO
            if(F[5]=='0'){
                loc++;
                string s2 = decToHex(memory[loc]);
                loc++;
                string s1 = decToHex(memory[loc]);
                string s = s1+s2;
                stPush(++loc);
                loc=hexToDec(s);
                continue;
            }
            else{
                loc++;
                loc++;
            }
        }
        else if(hex_mem == "E5"){// PUSH H
            stPush(get_reg('H'));
            stPush(get_reg('L'));
        }
        else if(hex_mem == "E6"){// ANI
        }
        else if(hex_mem == "E7"){// RST 4
        }
        else if(hex_mem == "E8"){// RPE
            if(F[5] == '1'){
                loc = stPop();
                continue;
            }
        }
        else if(hex_mem == "E9"){// PCHL
            string s = decToHex(reg[5])+decToHex(reg[6]);
            loc = hexToDec(s);
            continue;
        }
        else if(hex_mem == "EA"){// JPE
            if(F[5]=='1'){
                loc++;
                string s2 = decToHex(memory[loc]);
                loc++;
                string s1 = decToHex(memory[loc]);
                string s = s1+s2;
                loc=hexToDec(s);
                continue;
            }
            else{
                loc++;
                loc++;
            }
        }
        else if(hex_mem == "EB"){// XCHG
            //hl to de
            int x = reg[3];
            int y = reg[4];
            reg[3]=reg[5];
            reg[4]=reg[6];
            reg[5]=x;
            reg[6]=y;
        }
        else if(hex_mem == "EC"){// CPE
            if(F[5]=='1'){
                loc++;
                string s2 = decToHex(memory[loc]);
                loc++;
                string s1 = decToHex(memory[loc]);
                string s = s1+s2;
                stPush(++loc);
                loc=hexToDec(s);
                continue;
            }
            else{
                loc++;
                loc++;
            }
        }
        else if(hex_mem == "EE"){// XRI
        }
        else if(hex_mem == "EF"){// RST 5
            break;
        }
        goto END;
F:
        if(hex_mem == "F0"){// RP
            if(F[0] == '0'){
                loc = stPop();
                continue;
            }
        }
        else if(hex_mem == "F1"){// POP PSW
            F = decToBin(stPop());
        }
        else if(hex_mem == "F2"){// JP
            if(F[0]=='0'){
                loc++;
                string s2 = decToHex(memory[loc]);
                loc++;
                string s1 = decToHex(memory[loc]);
                string s = s1+s2;
                loc=hexToDec(s);
                continue;
            }
            else{
                loc++;
                loc++;
            }
        }
        else if(hex_mem == "F3"){// DI
        }
        else if(hex_mem == "F4"){// CP
            if(F[0]=='0'){
                loc++;
                string s2 = decToHex(memory[loc]);
                loc++;
                string s1 = decToHex(memory[loc]);
                string s = s1+s2;
                stPush(++loc);
                loc=hexToDec(s);
                continue;
            }
            else{
                loc++;
                loc++;
            }
        }
        else if(hex_mem == "F5"){// PUSH PSW
            stPush(binToDec(F));
        }
        else if(hex_mem == "F6"){// ORI
        }
        else if(hex_mem == "F7"){// RST 6
        }
        else if(hex_mem == "F8"){// RM
            if(F[0] == '1'){
                loc = stPop();
                continue;
            }
        }
        else if(hex_mem == "F9"){// SPMHL
        }
        else if(hex_mem == "FA"){// JM
            if(F[0]=='1'){
                loc++;
                string s2 = decToHex(memory[loc]);
                loc++;
                string s1 = decToHex(memory[loc]);
                string s = s1+s2;
                loc=hexToDec(s);
                continue;
            }
            else{
                loc++;
                loc++;
            }
        }
        else if(hex_mem == "FB"){// EI
        }
        else if(hex_mem == "FC"){// CM
            if(F[0]=='1'){
                loc++;
                string s2 = decToHex(memory[loc]);
                loc++;
                string s1 = decToHex(memory[loc]);
                string s = s1+s2;
                stPush(++loc);
                loc=hexToDec(s);
                continue;
            }
            else{
                loc++;
                loc++;
            }
        }
        else if(hex_mem == "FE"){// CPI
        }
        else if(hex_mem == "FF"){// RST 7
        }
END:
        loc++;
    }
    system(CLEAR_SCREEN);
}

int main(){
    system(CLEAR_SCREEN);
    memset(memory,0,sizeof(memory));
    loadmemory();
    while(true){
        try{
            cout<<"_STUDENT_85"<<endl;
            char in;
            cin>>in;
            system(CLEAR_SCREEN);
            if(in=='M' || in=='m'){
                string loc;
                cin>>loc;
                if(loc.size()>4){
                    throw 102;
                    cout<<"Error!"<<endl;
                    break;
                }
                string cond="";
                getline(cin,cond);// to flush the std input
                while(cond!="$"){
                    cout<<"M"<<loc<<":";
                    cout<<decToHex(mem(loc))<<"-";
                    getline(cin,cond);
                    system(CLEAR_SCREEN);
                    if(cond.empty() || (cond.size() == 1 && cond[0] == '\n')){
                        loc=increaseHexByOne(loc);
                    }
                    else if(cond!="$"){
                        int l = cond.size();
                        if(l==1 || l==2){
                            int dec_loc = hexToDec(loc);
                            memory[dec_loc]=hexToDec(cond);
                        }
                        else{
                            int dec_loc = hexToDec(loc);
                            string val = cond.substr(cond.size() - 2);
                            reverse(val.begin(), val.end());
                            memory[dec_loc] = hexToDec(val);
                        }
                        loc = increaseHexByOne(loc);
                    }
                    else if(cond == "$"){
                        savetofile();
                    }
                }
            }
            else if(in=='R' || in=='r'){
                int i=0;
                string cond;
                string registe = "ABCDEHLF";
                getline(cin,cond);//to flush the std input
                while(i < 8){
                    cout<<registe[i]<<":";
                    cout<<decToHex(get_reg(registe[i]))<<"-";
                    getline(cin,cond);
                    system(CLEAR_SCREEN);
                    if(cond != "$"){
                        int l = cond.size();
                        if(l==1 || l==2){
                            reg[i]=hexToDec(cond);
                        }
                        else if(l!=0){
                            string val = cond.substr(cond.size() - 2);
                            reverse(val.begin(), val.end());
                            reg[i] = hexToDec(val);
                        }
                        i++;
                    }
                    else if(cond == "$"){
                        break;
                    }
                    else{
                        throw 000;
                        break;
                    }
                }
            }
            else if(in=='G' || in=='g'){
                string loc;
                cin>>loc;
                system(CLEAR_SCREEN);
                runprogram(hexToDec(loc));
            }
            // else if(in=='T' || in=='t'){//condition for testing the function, remove after testing
            //     cout<<get_M()<<endl;
            // }
        }
        catch(int errorCode){
            string val;
            switch(errorCode){
                case 000:
                    cout<<"\n"<<errorCode<<"Unexpected Error \n Press Enter"<<endl;
                    break;
                case 101:
                    cout<<"\n"<<errorCode<<"Unrecognized register \n Press Enter"<<endl;
                    break;
                case 102:
                    cout<<"\n"<<errorCode<<"Unrecognized Memory Location \n Press Enter"<<endl;
                    break;
                case 103:
                    cout<<"\n"<<errorCode<<"Memory Location Exceeded \n Press Enter"<<endl;
                    break;
                case 104:
                    cout<<"\n"<<errorCode<<"Wrong Opcode detected \n Press Enter"<<endl;
                    break;
                case 105:
                    cout<<"\n"<<errorCode<<"Input not recognized \n Press Enter"<<endl;
                    break;
            }
            getline(cin,val);
            getline(cin,val);
            system(CLEAR_SCREEN);
        }
    }
}