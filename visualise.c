#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <stdint.h>

//--------------------------- Array Lists -----------------------

// Needed for returning the output strings defined in the scope of a function

const int INITIAL_CAPACITY = 9; //defualt as one byte with one bit for a space for a nibble

typedef char item;

struct list {
    int length;
    int capacity;
    item *items;
};
typedef struct list list;

// create a new empty list
list *newList() {
    list *l = malloc(sizeof(list));
    item *items = malloc(INITIAL_CAPACITY * sizeof(item));
    l -> length = 0;
    l -> capacity = INITIAL_CAPACITY;
    l -> items = items;
    return l;
}

// free memory
void freeList(list *l) {
    free(l->items);
    free(l);
}

//increase list size if trying to add with no space
void expand(list *l) {
    l->capacity = l->capacity * 1.5;
    l->items = realloc(l->items, l->capacity * sizeof(item));
}

//add an item to the list
void add(list *l, item n) {
    if (l->length == l->capacity) expand(l);
    l->items[l->length] = n;
    l->length++;
}

// add a string to a list wihtout it being const
void addString(list *l, char s[]){
    int i=0;
    while(s[i] != 0){
        add(l,s[i]);
        i++;
    }
    add(l,0);
}

// initise a new list and imediately array structure with a string
list *newListWithItems(char s[]){
    list *l = newList();
    addString(l,s);
    return l;
}

//display the list in the terminal
void showList(list *l){
    printf("%s\n",l->items);
}

//------------------------- Managing and Printing Errors -------------------------------

void displayError(){
    printf("Input error.\n");
}

// for returning an error -  needed for testing to work properly
list* errorList(){
    list *l = newListWithItems("Input error.");
    return l;
}

//------------------------- Funcs From Decimal to Binary -------------------------------

/* CHAR */

// checking all chars in the data are numbers or a minus (hyphen) and there are no leading 0s unless 0
bool validDataChar(char data[]){
    int i = 0;
    char current;
    bool valid = data[0] == '0' ? false : true; // don't allow leading 0s
    if (data[0] == '-') i++; //ignore minus in checking of digits
    while(valid && data[i] != 0){
        current = data[i];
        if(current < '0' || current > '9') valid = false;
        i++;
    }

    //allow 0
    if(data[1] == 0 && data[0] == '0') return true;

    return valid;
}

// Decimal to binary where inputs are numbers
// Signed using two's compliment
// Restricted to two nibbles : -128 to 127
// Big endian
list *decToBinChar(char data[]){
    if(!validDataChar(data)) return errorList();
    int num=0,i=0, n=0;
    bool twosComp = false; // for neg numbers
    list *binaryOut = newList();

    if(data[0] == '-'){
        twosComp = true;
        while(data[n] != 0){
            n++;
        }
        n-=2;
        i++;
        while(data[i] != 0){
            num += (data[i]-'0') * pow(10,n-i+1); //+1 to ignore the minus
            i++;
        }
    }
    else{
        while(data[n] != 0){
            n++;
        }
        n--;
        while(data[i] != 0){
            num += (data[i]-'0') * pow(10,n-i);
            i++;
        }
    }

    //formatting the binary nibbles
    int bitVal = 0;
    if(twosComp && num <=128){ // can do this by minusing 1 and flipping all the bits - then treat as +ve
        num --;
        add(binaryOut, '1');
        for(i=2;i<5;i++){//nibble 1
            // if respective bit is needed binaryout then set to 1 otherwise 0
            bitVal = pow(2,8-i);
            if(num-bitVal >=0){
                num -= bitVal;
                add(binaryOut, '0');
            }
            else{
                add(binaryOut, '1');
            }
        }
        add(binaryOut, ' ');
        for(i=5;i<9;i++){//nibble 2
            bitVal = pow(2,8-i);
            if(num-bitVal >=0){
                num -= bitVal;
                add(binaryOut, '0');
            }
            else{
                add(binaryOut, '1');
            }
            
        }
    }
    else if(num <=127){
        add(binaryOut, '0');
        for(i=1;i<4;i++){//nibble 1
            // if respective bit is needed binaryout then set to 1 otherwise 0
            bitVal = pow(2,7-i);
            if(num-bitVal >=0){
                num -= bitVal;
                add(binaryOut, '1');
            }
            else{
                add(binaryOut, '0');
            }
        }
        add(binaryOut, ' ');
        for(i=4;i<8;i++){//nibble 2
            bitVal = pow(2,7-i);
            if(num-bitVal >=0){
                num -= bitVal;
                add(binaryOut, '1');
            }
            else{
                add(binaryOut, '0');
            }
            
        }
    }
    else{
        return errorList();
    }
    add(binaryOut,0);
    return binaryOut;
}

/* UNSIGNED CHAR */

// checking all chars in the data are numbers and there are no leading 0s unless 0 itself
bool validDataUnsignedChar(char data[]){
    int i = 0;
    char current;
    bool valid = data[0] == '0' ? false : true; // don't allow leading 0s
    while(valid && data[i] != 0){
        current = data[i];
        if(current < '0' || current > '9') valid = false;
        i++;
    }

    //allow 0
    if(data[1] == 0 && data[0] == '0') return true;

    return valid;
}

// Decimal to binary where inputs are numbers
// Unsigned
// Restricted to two nibbles : 0 - 255
// Big endian
list *decToBinUnsignedChar(char data[]){
    if(!validDataUnsignedChar(data)) return errorList();
    int num=0,i=0,n=0;
    list *binaryOut = newList();

    while(data[n] != 0){
            n++;
    }
    n--;
    while(data[i] != 0){
        num += (data[i]-'0') * pow(10,n-i);
        i++;
    }

    //formatting the binary nibbles
    int bitVal = 0;
    if(num <=255){
        for(i=0;i<4;i++){//nibble 1
            // if respective bit is needed binaryout then set to 1 otherwise 0
            bitVal = pow(2,7-i);
            if(num-bitVal >=0){
                num -= bitVal;
                add(binaryOut, '1');
            }
            else{
                add(binaryOut, '0');
            }
        }
        add(binaryOut, ' ');
        for(i=4;i<8;i++){//nibble 2
            bitVal = pow(2,7-i);
            if(num-bitVal >=0){
                num -= bitVal;
                add(binaryOut, '1');
            }
            else{
                add(binaryOut, '0');
            }
            
        }
    }
    else{
        return errorList();
    }
    add(binaryOut,0);
    return binaryOut;
}


/* UNSIGNED INT */

// checking all chars in the data are numbers and there are no leading 0s unless 0 itself
bool validDataUnsignedInt(char data[]){
    int i = 0;
    char current;
    bool valid = data[0] == '0' ? false : true; // don't allow leading 0s
    while(valid && data[i] != 0){
        current = data[i];
        if(current < '0' || current > '9') valid = false;
        i++;
    }

    //allow 0
    if(data[1] == 0 && data[0] == '0') return true;

    return valid;
}

// Decimal to binary where inputs are numbers
// Unsigned
// Restricted to 8 nibbles - 32 bits
// Big endian
list *decToBinUnsignedInt(char data[]){

    if(!validDataUnsignedInt(data)) return errorList();
    int i=0, n=0;

    // if we use int instead of num here we have the same upper bound betweeen what we can accept and what we want to display
    // therefore if we enter a higher number than 2^31 -1nwe get the runtime error:
    // runtime error: 2.14748e+09 is outside the range of representable values of type 'int'
    // So we cannot get our desired output "Invalid input"
    // We could solve this issues and give our own error by using long and long long (when we get to testing long)
    long num =0;

    list *binaryOut = newList();

    while(data[n] != 0){
            n++;
        }
        n--;
        while(data[i] != 0){
            num += (data[i]-'0') * pow(10,n-i);
            i++;
        }

    //formatting the binary nibbles
    long bitVal = 0;
    if(num <= pow(2,32)-1){
        for(i=0;i<32;i++){//nibble 1
            // if respective bit is needed binaryout then set to 1 otherwise 0
            bitVal = pow(2,31-i);
            if(num-bitVal >=0){
                num -= bitVal;
                add(binaryOut, '1');
            }
            else{
                add(binaryOut, '0');
            }
            if(i != 31 && i%4==3) add(binaryOut, ' '); //formatting nibbles - mod 3 as starts with 1 
        }
    }
    else{
        return errorList();
    }
    add(binaryOut,0);
    return binaryOut;
}

/* INT */

// checking all chars in the data are numbers or minus (hypen) and there are no leading 0s unless 0 itself
bool validDataInt(char data[]){
    int i = 0;
    char current;
    bool valid = data[0] == '0' ? false : true; // don't allow leading 0s
    if (data[0] == '-') i++; //ignore minus in checking of digits
    while(valid && data[i] != 0){
        current = data[i];
        if(current < '0' || current > '9') valid = false;
        i++;
    }

    //allow 0
    if(data[1] == 0 && data[0] == '0') return true;

    return valid;
}

// Decimal to binary where inputs are numbers
// Signed
// Restricted to 8 nibbles - 32 bits
// Big endian
list *decToBinInt(char data[]){

    if(!validDataInt(data)) return errorList();
    int i=0, n=0;

    // if we use int instead of num here we have the same upper bound betweeen what we can accept and what we want to display
    // therefore if we enter a higher number than 2^31 -1 or lower than -2^31 we get the runtime error:
    // runtime error: [depends on upper or lower] is outside the range of representable values of type 'int'
    // So we cannot get our desired output "Invalid input"
    // We could solve this issues and give our own error by using long and long long (when we get to testing long)
    long num =0;
    bool twosComp = false; // for neg numbers
    list *binaryOut = newList();

    if(data[0] == '-'){
        twosComp = true;
        while(data[n] != 0){
            n++;
        }
        n-=2;
        i++;
        while(data[i] != 0){
            num += (data[i]-'0') * pow(10,n-i+1); //+1 to ignore the minus
            i++;
        }
    }
    else{
        while(data[n] != 0){
            n++;
        }
        n--;
        while(data[i] != 0){
            num += (data[i]-'0') * pow(10,n-i);
            i++;
        }
    }

    //formatting the binary nibbles
    int bitVal = 0;
    if(twosComp && num <= pow(2,31)){ // can do this by minusing 1 and flipping all the bits - then treat as +ve
        num --;
        add(binaryOut, '1');
        for(i=2;i<33;i++){//nibble 1
            // if respective bit is needed binaryout then set to 1 otherwise 0
            bitVal = pow(2,32-i);
            if(num-bitVal >=0){
                num -= bitVal;
                add(binaryOut, '0');
            }
            else{
                add(binaryOut, '1');
            }
            if(i!= 32 && i%4==0) add(binaryOut, ' '); //formatting nibbles
        }
    }
    else if(num <= pow(2,31)-1){
        add(binaryOut, '0');
        for(i=1;i<32;i++){//nibble 1
            // if respective bit is needed binaryout then set to 1 otherwise 0
            bitVal = pow(2,31-i);
            if(num-bitVal >=0){
                num -= bitVal;
                add(binaryOut, '1');
            }
            else{
                add(binaryOut, '0');
            }
            if(i != 31 && i%4==3) add(binaryOut, ' '); //formatting nibbles - mod 3 as starts with 1 
        }
    }
    else{
        return errorList();
    }
    add(binaryOut,0);
    return binaryOut;
}

/* Long */

// checking all chars in the data are numbers or minus (hypen) and there are no leading 0s unless 0 itself
bool validDataLong(char data[]){
    int i = 0;
    char current;
    bool valid = data[0] == '0' ? false : true; // don't allow leading 0s
    if (data[0] == '-') i++; //ignore minus in checking of digits
    while(valid && data[i] != 0){
        current = data[i];
        if(current < '0' || current > '9') valid = false;
        i++;
    }

    //allow 0
    if(data[1] == 0 && data[0] == '0') return true;

    return valid;
}

// Decimal to binary where inputs are numbers
// Signed
// Restricted to 16 nibbles - 64 bits
// Big endian
list *decToBinLong(char data[]){
    if(!validDataLong(data)) return errorList();
    int i=0, n=0;

    //since long and long long are both 64 bits here really a external library is needed to sort out the following:
    //  boundary testing cannot be done
    //  overflow causes numbers bigger than allowed to pass the test to see if numbers are too large
    //an external library that can hold larger numbers was not used as stated in the rules of the CW
    long long num = 0; 
    bool twosComp = false; // for neg numbers
    list *binaryOut = newList();

    if(data[0] == '-'){
        twosComp = true;
        while(data[n] != 0){
            n++;
        }
        n-=2;
        i++;
        while(data[i] != 0){
            num += (data[i]-'0') * pow(10,n-i+1); //+1 to ignore the minus
            i++;
        }
    }
    else{
        while(data[n] != 0){
            n++;
        }
        n--;
        while(data[i] != 0){
            num += (data[i]-'0') * pow(10,n-i);
            i++;
        }
    }

    //formatting the binary nibbles
    long long bitVal = 0;
    if(twosComp && num <= pow(2,63)){ // can do this by minusing 1 and flipping all the bits - then treat as +ve
        num --;
        add(binaryOut, '1');
        for(i=2;i<65;i++){
            // if respective bit is needed binaryout then set to 1 otherwise 0
            bitVal = pow(2,64-i);
            if(num-bitVal >=0){
                num -= bitVal;
                add(binaryOut, '0');
            }
            else{
                add(binaryOut, '1');
            }
            if(i!= 64 && i%4==0) add(binaryOut, ' '); //formatting nibbles
        }
    }
    else if(num <= pow(2,63)-1){
        add(binaryOut, '0');
        for(i=1;i<64;i++){
            // if respective bit is needed binaryout then set to 1 otherwise 0
            bitVal = pow(2,63-i);
            if(num-bitVal >=0){
                num -= bitVal;
                add(binaryOut, '1');
            }
            else{
                add(binaryOut, '0');
            }
            if(i != 63 && i%4==3) add(binaryOut, ' '); //formatting nibbles - mod 3 as starts with 1 
        }
    }
    else{
        return errorList();
    }
    add(binaryOut,0);
    return binaryOut;
}

/* DOUBLE */

// checking all chars in the data are numbers or minus (hypen) or decimal point (.) and there are no leading 0s unless 0 itself
bool validDataDouble(char data[]){
    int i = 0;
    char current;
    bool valid = true;
    bool decimalFound = false;

    //leading 0 is only allowed if the number is between -1 and 1
    if((data[0] == '0' && data[1]!= '.') || (data[0] == '-' && data[1]== '0' && data[2] != '.')){
        valid = false;
    }

    if (data[0] == '-') i++; //ignore minus in checking of digits
    while(valid && data[i] != 0){
        current = data[i];
        if(((!decimalFound && current != '.') && (current < '0' || current > '9'))||((decimalFound && current == '.') && (current < '0' || current > '9'))) valid = false;
        if(current == '.'){
            decimalFound = true;
        }
        i++;
    }

    //allow 0
    if(data[1] == 0 && data[0] == '0') return true;

    return valid;
}

void concatan(list *l,list *l1, list *l2){
    add(l,'0'); // for addding later
    for(int i=0;i<l1->length; i++){
        add(l,l1->items[i]);
    }
    for(int i=0;i<l2->length; i++){
        add(l,l2->items[i]);
    }
    add(l,0);
} 

void roundBin(list *l){
    if(l->length >2){
        int point = ((l -> length)-2) > 53? 53: ((l -> length)-2);
        if(l->items[point] == '1'){
            l->items[point] = '0';
            for(int i = point-1 ;i>=0;i--){
                if(l->items[i] == '0'){
                    l-> items[i] = '1';
                    break;
                }
                else if(l->items[i] == '1'){
                    l-> items[i] = '0';
                }
            }
        }
    }
}

// Decimal to binary where inputs are numbers / possibly not integers
// Signed with one sign bit
// Restricted to 16 nibbles - 64 bytes
// Comprises of exponent and mantissa and sign bit
// exponent is 11 bits
// mantissa is 52 bits
// Big endian
list *decToBinDouble(char data[]){
    // Sotred in the from:
    // [sign bit] [normalised exponent] [significant bits] [0s to fill space if needed]
    // where the sign bit is 1 for -ve and 0 for +ve
    // where the number has been normalised in binary 
    // significant bits are the bits after the 1. in normalised floating point form
    // the normalised exponent is calculated by the exponent from the normalised floating point number
    // added to the bias (2^(number of exponent bits -2)-1) = 1023
    // and then converted back to binary

    if(!validDataDouble(data)) return errorList();
    int i=0, n=0, nI=0, nF=0, length =0; // so we don't have to access binaryOut -> length every time
    long numI =0, numF =0; 
    bool fraction = false;
    list *binaryOut = newList();
    
    //obtaining the integer part of the number and the decimal part of the number
    if(data[0] == '-'){
        add(binaryOut, '1');// negative sign
        length++;
        i++;
        n++;

        while(data[n] != 0 && !fraction){//for exp of the ineteger part
            if (data[n] == '.'){
                fraction = true;
                n--;
            }
            n++;
        }
        nI=n-1;
        while(data[n] != 0){// for exp for the |x| < 1 part
            nF++;
            n++;
        }
        while(data[i] != 0 && i<=nI){
            numI += (data[i]-'0') * pow(10,nI-i); // increase of both n and i for the minus sign creates no problems here
            i++;
        }
        i++;
        if(data[i-1] != 0){
            while(data[i] != 0 && i-nI<=nF){
                numF += (data[i]-'0') * pow(10,nF-(i-nI));
                i++;
            }
        }
    }
    else{
        add(binaryOut, '0');// positive sign
        length++;

        while(data[n] != 0 && !fraction){//for exp of the ineteger part
            if (data[n] == '.'){
                fraction = true;
                n--;
            }
            n++;
        }
        nI=n-1;
        while(data[n] != 0){ // for exp for the |x| < 1 part
            nF++;
            n++;
        }
        while(data[i] != 0 && i<=nI){
            numI += (data[i]-'0') * pow(10,nI-i);
            i++;
        }
        i++;
        if(data[i-1]!=0){
            while(data[i] != 0 && i-nI<=nF){
                numF += (data[i]-'0') * pow(10,nF-(i-nI));
                i++;
            }
        }
    }

    double numFR = numF / (pow(10,nF-1));
    double integerPart = numI;
    double fractionalPart = numFR;
    
    // converting the number into binary

    double bitVal = 0;
    double bitValF = 0;
    double two = 2;
    list *binI = newList(); // largest number able to be held by a double (with lost precision) is roughly 1.8*10^308
    list *binF = newList(); // but limited by size of standard numerical data types
    bool bitPlaced = false; // to stop leading or tailing 0s

    for(double k=0;k<1026;k++){
        bitVal = pow(two,1025-k); // log2 of max for double (1.8....*10^308) = 1024.07...
        if(numI-bitVal >=0){
            numI -= bitVal;
            add(binI, '1');
            bitPlaced = true;
        }
        else{
            if(bitPlaced) add(binI, '0');
        }
    }
    for(double k=1024;k>=0;k--){
        // rounding occurs naturally due to using doubles if decimal is purely too small or need infinite representation
        bitValF = 1/pow(two,1025-k);
        if(numFR == 0){
            break;
        }
        if(numFR-bitValF >=0){
            numFR -= bitValF;
            add(binF, '1');
        }
        else{
            add(binF, '0');
        }
    }

    //calc mantissa and exponent 

    if(integerPart == 0 && fractionalPart ==0){
        for(int i=0;i<11;i++){
            add(binaryOut, '0');
            length++;
            if(length%4==0){
                add(binaryOut, ' ');
            }
        }
    }
    else{

        list *combinedMantissa = newList();
        concatan(combinedMantissa,binI,binF);

        int firstOneIndex = 0;
        i = 0;
        while(firstOneIndex == 0){
            if(combinedMantissa -> items[i] == '1') firstOneIndex = i;
            i++;
        }

        if(binI-> length > 0 && combinedMantissa -> length - firstOneIndex > 53) roundBin(combinedMantissa);

        // calculating exponent

        int pointLocation = 0;
        if (binI -> length > 0){
            pointLocation = binI -> length-1;
        }
        else{
            bool significatFound = false;
            int f = 0;
            while(!significatFound){
                if(binF ->items[f] == '1'){
                    significatFound = true;
                    pointLocation = -1*(f +1);
                }
                f++;
            }
        }

        int normalisedExp = 1023 + pointLocation; //added to bias (see top of func)
        n =0;
        i=0;
        if(normalisedExp < pow(2,11) && normalisedExp >= 0){
            for(i=0;i<11;i++){
                // if respective bit is needed binaryout then set to 1 otherwise 0
                bitVal = pow(2,10-i);
                if(normalisedExp-bitVal >=0){
                    normalisedExp -= bitVal;
                    add(binaryOut, '1');
                    length++;
                }
                else{
                    add(binaryOut, '0');
                    length++;
                }
                if(length%4==0){
                    add(binaryOut, ' ');
                }
            }
        }

        // adding signficant part of mantissa 

    // find first 1
    firstOneIndex = 0;
    i = 0;
    while(firstOneIndex == 0){
        if(combinedMantissa -> items[i] == '1') firstOneIndex = i;
        i++;
    }

    for(i= (firstOneIndex+1); i<combinedMantissa->length-1;i++){ //exclude first 1 and don't print the null
        if(length <64){
            add(binaryOut, (combinedMantissa -> items[i]));
            length++;
            if(length%4==0){
                add(binaryOut, ' ');
            }
        }
    }

    freeList(combinedMantissa);
}

    // adding tailing zeros
    while(length<64){
        add(binaryOut, '0');
        length++;
        if(length%4==0 && length != 64){
            add(binaryOut, ' ');
        }
    }
    
    add(binaryOut,0);
    freeList(binI);
    freeList(binF);

    return binaryOut;
}

//------------------------- Funcs From Binary to Decimal -------------------------------

/* CHAR */

bool binCharDataValid(char data[]){
    int length =0,i=0;

    while(data[i]!= 0){
        length++;
        i++;
    }
    return length==9;
}

list *binToDecChar(char data[]){
    if(!binCharDataValid(data)) return errorList();

    int numVal = 0,power =0;
    list *decimalOut = newList();

    if(data[0] == '0'){
        for(int i=1; i<10; i++){ //calculating value from binary
            if(data[i] == ' ') continue;
            if(data[i]== '1') numVal += pow(2,6-power);
            power++;
        }
    }
    else{
        add(decimalOut, '-');
        numVal = -1 *pow(2,7);
        for(int i=1; i<10; i++){ //calculating value from binary
            if(data[i] == ' ') continue;
            if(data[i]== '1') numVal += pow(2,6-power);
            power++;
        }
        numVal = numVal *-1;
    }

    if(numVal == 0) add(decimalOut,'0');
    if(numVal == 1) add(decimalOut,'1');

    double digits = log10(numVal); //determine how long the number is
    int currentDigit = 0,count=0;
    char backwardsNum[3]; // at biggest 3 digits long
     
    // don't need to check the range of the number as out of range numbers cannot be produced

    for(int i=0; i<digits;i++){
        currentDigit = numVal % (int)pow(10,i+1);
        numVal -= currentDigit;
        backwardsNum[i] = currentDigit / pow(10,i);
        count++;
    }
    for(int i=count-1; i>=0;i--){
        add(decimalOut,backwardsNum[i]+48);
    }

    add(decimalOut, 0);
    return decimalOut;
}

/* UNSIGNED CHAR*/

bool binUnsignedCharDataValid(char data[]){
    int length = 0, i=0;

    if(data[0]){
        while(data[i] != 0){
            length++;
            i++;
        }
    }

    return length == 9;
}

list *binToDecUnsignedChar(char data[]){
    if(!binUnsignedCharDataValid(data)) return errorList();

    int numVal = 0,power =0;
    list *decimalOut = newList();

    for(int i=0; i<10; i++){ //calculating value from binary
        if(data[i] == ' ') continue;
        if(data[i]== '1') numVal += pow(2,7-power);
        power++;
    }

    if(numVal == 0) add(decimalOut,'0');
    if(numVal == 1) add(decimalOut,'1');

    double digits = log10(numVal); //determine how long the number is
    int currentDigit = 0,count=0;
    char backwardsNum[3];
     
    // don't need to check the range of the number as out of range numbers cannot be produced

    for(int i=0; i<digits;i++){
        currentDigit = numVal % (int)pow(10,i+1);
        numVal -= currentDigit;
        backwardsNum[i] = currentDigit / pow(10,i);
        count++;
    }
    for(int i=count-1; i>=0;i--){
        add(decimalOut,backwardsNum[i]+48);
    }

    add(decimalOut, 0);
    return decimalOut;

}

/* INT*/

bool binIntDataValid(char data[]){
    int length = 0, i=0;

    if(data[0]){
        while(data[i] != 0){
            length++;
            i++;
        }
    }

    return length == (32+7); //32 bits + 7 spaces
}

list *binToDecInt(char data[]){
    if(!binIntDataValid(data)) return errorList();

    int numVal = 0,power =0;
    list *decimalOut = newList();

    if(data[0] == '0'){
        for(int i=1; i<39; i++){ //calculating value from binary
            if(data[i] == ' ') continue;
            if(data[i]== '1') numVal += pow(2,30-power);
            power++;
        }
    }
    else{
        add(decimalOut, '-');
        numVal = -1*pow(2,31);
        for(int i=1; i<39; i++){ //calculating value from binary
            if(data[i] == ' ') continue;
            if(data[i]== '1') numVal += pow(2,30-power);
            power++;
        }
        numVal = numVal *-1;
    }

    if(numVal == 0) add(decimalOut,'0');
    if(numVal == 1) add(decimalOut,'1');

    double digits = log10(numVal); //determine how long the number is
    long currentDigit = 0,count=0;
    char backwardsNum[10]; // -2147483648 and 2147483647 are 10 digits long
     
    // don't need to check the range of the number as out of range numbers cannot be produced

    for(int i=0; i<digits;i++){
        currentDigit = numVal % (long)pow(10,i+1);
        numVal -= currentDigit;
        backwardsNum[i] = currentDigit / pow(10,i);
        count++;
    }
    for(int i=count-1; i>=0;i--){
        add(decimalOut,backwardsNum[i]+48);
    }

    add(decimalOut, 0);
    return decimalOut;
}

/* UNSIGNED INT*/

bool binUnsignedIntDataValid(char data[]){
    int length = 0, i=0;

    if(data[0]){
        while(data[i] != 0){
            length++;
            i++;
        }
    }

    return length == (32+7); //32 bits + 7 spaces
}

list *binToDecUnsignedInt(char data[]){
    if(!binUnsignedIntDataValid(data)) return errorList();

    long numVal = 0,power =0;
    list *decimalOut = newList();

    for(int i=0; i<39; i++){ //calculating value from binary
        if(data[i] == ' ') continue;
        if(data[i]== '1') numVal += pow(2,31-power);
        power++;
    }

    if(numVal == 0) add(decimalOut,'0');
    if(numVal == 1) add(decimalOut,'1');

    double digits = log10(numVal); //determine how long the number is
    long currentDigit = 0, count=0;
    char backwardsNum[10]; // 4294967296 is 10 digits long
     
    // don't need to check the range of the number as out of range numbers cannot be produced

    for(int i=0; i<digits;i++){
        currentDigit = numVal % (long)pow(10,i+1);
        numVal -= currentDigit;
        backwardsNum[i] = currentDigit / pow(10,i);
        count++;
    }
    for(int i=count-1; i>=0;i--){
        add(decimalOut,backwardsNum[i]+48);
    }

    add(decimalOut, 0);
    return decimalOut;

}

/* LONG*/

bool binLongDataValid(char data[]){
    int length = 0, i=0;

    if(data[0]){
        while(data[i] != 0){
            length++;
            i++;
        }
    }

    return length == (64+15); //64 bits + 15 spaces
}

list *binToDecLong(char data[]){
    if(!binLongDataValid(data)) return errorList();

    long numVal = 0,power =0;
    list *decimalOut = newList();

    if(data[0] == '0'){
        for(int i=1; i<79; i++){ //calculating value from binary
            if(data[i] == ' ') continue;
            if(data[i]== '1') numVal += pow(2,62-power);
            power++;
        }
    }
    else{
        add(decimalOut, '-');
        numVal = -1 *pow(2,63);
        for(int i=1; i<79; i++){ //calculating value from binary
            if(data[i] == ' ') continue;
            if(data[i]== '1') numVal += pow(2,62-power);
            power++;
        }
        numVal = numVal *-1;
    }

    if(numVal == 0) add(decimalOut,'0');
    if(numVal == 1) add(decimalOut,'1');

    double digits = log10(numVal); //determine how long the number is
    double currentDigit = 0,count=0;
    char backwardsNum[19]; // +/- 4.611686e+18 are 19 digits long
     
    // don't need to check the range of the number as out of range numbers cannot be produced

    for(int i=0; i<digits;i++){
        currentDigit = numVal % (long)pow(10,i+1);
        numVal -= currentDigit;
        backwardsNum[i] = currentDigit / pow(10,i);
        count++;
    }
    for(int i=count-1; i>=0;i--){
        add(decimalOut,backwardsNum[i]+48);
    }

    add(decimalOut, 0);
    return decimalOut;
}

/* DOUBLE */

bool binDoubleDataValid(char data[]){
    int length = 0, i=0;

    if(data[0]){
        while(data[i] != 0){
            length++;
            i++;
        }
    }

    return length == (64+15); //64 bits + 15 spaces
}

list *binToDecDouble(char data[]){
    if(!binDoubleDataValid(data)) return errorList();

    list *decimalOut = newList();
     
    // obtaining sign

    if(data[0] == '1'){
        add(decimalOut, '-');
    }

    // checking 'weird' all zero case
    int num0 = 0;
    for(int i=1;i<79;i++){
        if(data[i] == '0'){
            num0++;
        }
    }
    if(num0 == 63){
        add(decimalOut, '0');
        add(decimalOut, 0);
        return decimalOut;
    }

    // obtaining exponent

    int exponentVal = 0 ,power = 0;

    for(int i=1; i<14; i++){ //calculating value from binary
        if(data[i] == ' ') continue;
        if(data[i]== '1') exponentVal += pow(2,10-power); //12 as i starts with 1
        power++;
    }

    exponentVal = exponentVal == 0? 0: exponentVal-1023; // taking away base unless = 0 

    // obtaining mantissa

    double mantissaVal = 1; //for the 1 that is excluded
    power = 1; //start with 1/2

    for(int i=15; i<79;i++){
        if(data[i] == ' ') continue;
        if(data[i]== '1') mantissaVal += 1 / pow(2,power);
        power++;
    }

    //applying exponent

    if(exponentVal < 0){
        for(int i=0;i<-1*exponentVal;i++){
            mantissaVal = mantissaVal /2;
        }   
    }
    else{
        for(int i=0;i<exponentVal;i++){
            mantissaVal = mantissaVal *2;
        }   
    }

    //split the integer and fractional part

    double intTemp;
    long integer,fraction;
    int i =0;

    integer = (long)mantissaVal;

    while(modf(mantissaVal,&intTemp) != 0){
        mantissaVal = mantissaVal * 10;
        i++;
    }

    fraction = mantissaVal - integer * pow(10,i);

    // add integer part

    double digits = log10(integer); //determine how long the number is
    double currentDigit = 0,count=0;
    char backwardsNum[309]; // 1.8*10^308

    if(integer== 0) add(decimalOut,'0');
    else if(integer == 1) add(decimalOut,'1');
    else{
            
        // don't need to check the range of the number as out of range numbers cannot be produced

        for(int i=0; i<digits;i++){
            currentDigit = integer % (long)pow(10,i+1);
            integer -= currentDigit;
            backwardsNum[i] = currentDigit / pow(10,i);
            count++;
        }
        for(int i=count-1; i>=0;i--){
            add(decimalOut,backwardsNum[i]+48);
        }
        
    }

    // add fractional part

    if(fraction != 0){

        add(decimalOut, '.');

        if(fraction == 1) add(decimalOut,'1');

        double digits = log10(fraction); //determine how long the number is
        int currentDigit = 0;
        int count=0;
        char backwardsNum2[309];

        for(int i=0; i<digits;i++){
            currentDigit = fraction % (long)pow(10,i+1);
            fraction -= currentDigit;
            backwardsNum2[i] = currentDigit / pow(10,i);
            count++;
        }
        for(int i=count-1; i>=0;i--){
            add(decimalOut,backwardsNum2[i]+48);
        }
    }


    add(decimalOut,0);

    return decimalOut;
}


//------------------------------- General Functions ------------------------------------

//formatting data entered as binary

void formatData(int n, char *args[n]){

    int j =0 ,counter =0;
    char data[300]; // large to stop errors with entering too large data

    int startingIndex = 2;
    if(args[2][0] != '0' && args[2][0] != '1' ) startingIndex++;

    for(int i=startingIndex; i<n;i++){
        j=0;
        while(args[i][j] !=0){
            data[counter] = args[i][j];
            counter++;
            j++;
        }
        data[counter] = ' ';
        counter++;
    }
    data[counter-1] = 0;

    args[3] = data;
}

// decminal to binary = true | binary to decimal = false
bool decToBin(char data[]){
    if (data[4]){
        if(data[4] == ' ') return false;
    }
    return true;
}

// determine what type is being converted
void runfuncForType(char type1[], char type2[], char data[]){
    if(decToBin(data)){ // decimal to binary
        if(type1[0] == 'c' && type1[1] == 'h' && type1[2] == 'a' && type1[3] == 'r'){
            showList(decToBinChar(data));
        }
        else if(type1[0] == 'i' && type1[1] == 'n' && type1[2] == 't'){
            showList(decToBinInt(data));
        }
        else if(type1[0] == 'u' && type1[1] == 'n' && type1[2] == 's'&& type1[3] == 'i'&& type1[4] == 'g'&& type1[5] == 'n'
        && type1[6] == 'e'&& type1[7] == 'd' && type2[0] == 'c'&& type2[1] == 'h'&& type2[2] == 'a'&& type2[3] == 'r'){
            showList(decToBinUnsignedChar(data));
        }
        else if(type1[0] == 'u' && type1[1] == 'n' && type1[2] == 's'&& type1[3] == 'i'&& type1[4] == 'g'&& type1[5] == 'n'
        && type1[6] == 'e'&& type1[7] == 'd' && type2[0] == 'i'&& type2[1] == 'n'&& type2[2] == 't'){
            showList(decToBinUnsignedInt(data));
        }
        else if(type1[0] == 'l' && type1[1] == 'o' && type1[2] == 'n' && type1[3] == 'g'){
            showList(decToBinLong(data));
        }
        else if(type1[0] == 'd' && type1[1] == 'o' && type1[2] == 'u' && type1[3] == 'b'&& type1[4] == 'l'&& type1[5] == 'e'){
            showList(decToBinDouble(data));
        }
        else{
            displayError();
        }
    }
    else{ // binary to decimal
        if(type1[0] == 'c' && type1[1] == 'h' && type1[2] == 'a' && type1[3] == 'r'){
            showList(binToDecChar(data));
        }
        else if(type1[0] == 'i' && type1[1] == 'n' && type1[2] == 't'){
            showList(binToDecInt(data));
        }
        else if(type1[0] == 'u' && type1[1] == 'n' && type1[2] == 's'&& type1[3] == 'i'&& type1[4] == 'g'&& type1[5] == 'n'
        && type1[6] == 'e'&& type1[7] == 'd' && type2[0] == 'c'&& type2[1] == 'h'&& type2[2] == 'a'&& type2[3] == 'r'){
            showList(binToDecUnsignedChar(data));
        }
        else if(type1[0] == 'u' && type1[1] == 'n' && type1[2] == 's'&& type1[3] == 'i'&& type1[4] == 'g'&& type1[5] == 'n'
        && type1[6] == 'e'&& type1[7] == 'd' && type2[0] == 'i'&& type2[1] == 'n'&& type2[2] == 't'){
            showList(binToDecUnsignedInt(data));
        }
        else if(type1[0] == 'l' && type1[1] == 'o' && type1[2] == 'n' && type1[3] == 'g'){
            showList(binToDecLong(data));
        }
        else if(type1[0] == 'd' && type1[1] == 'o' && type1[2] == 'u' && type1[3] == 'b'&& type1[4] == 'l'&& type1[5] == 'e'){
            showList(binToDecDouble(data));
        }
        else{
            printf("reached\n");
            displayError();
        }

    }
}

//------------------------- Testing Start -------------------------------

// The tests use an enumeration to say which function to call.
enum {DTBChar, DTBUChar, DTBInt, DTBUInt, DTBLong, DTBDouble, BTDChar, BTDUChar, BTDInt, BTDUInt, BTDLong, BTDDouble};
typedef int function;

// A replacement for the library assert function.
void assert(int line, bool b) {
  if (b) return;
  printf("The test on line %d fails.\n", line);
  exit(1);
}

// Call a given function with a possible data and type string arguments
list *call(function f, char dataIn[]) {
    list *result = newList(); // to silence [-Wsometimes-uninitialized]
    list *tmp;
    tmp = result; // stop leak
    switch (f) {
        case DTBChar: result = decToBinChar(dataIn); break;
        case DTBUChar: result = decToBinUnsignedChar(dataIn); break;
        case DTBInt: result = decToBinInt(dataIn); break;
        case DTBUInt: result = decToBinUnsignedInt(dataIn); break;
        case DTBLong: result = decToBinLong(dataIn); break;
        case DTBDouble: result = decToBinDouble(dataIn); break;
        case BTDChar: result = binToDecChar(dataIn); break;
        case BTDUChar: result = binToDecUnsignedChar(dataIn); break;
        case BTDInt: result = binToDecInt(dataIn); break;
        case BTDUInt: result = binToDecUnsignedInt(dataIn); break;
        case BTDLong: result = binToDecLong(dataIn); break;
        case BTDDouble: result = binToDecDouble(dataIn); break;
        default: assert(__LINE__, false);
    }
    freeList(tmp);
    return result;
}

// Check that a given function does the right thing
// Checks that the expected output matches when data is run through
bool check(function f, char dataIn[], list *expOut) {
    list *result = call(f, dataIn);

    // checking same
    bool ok = expOut -> length == result -> length;
    if(ok){
        for(int i=0; i< expOut -> length-1;i++){
            if(expOut->items[i]!=result-> items[i]) ok = false;
            //printf("%c %c\n",expOut->items[i],result-> items[i]);
            //printf("%d\n", expOut->items[i]==result-> items[i]);
        }
    }
    freeList(result);
    return ok;
}

void testDTBChar() {
    list *xs;

    //normal
    assert(__LINE__, check(DTBChar, "7", xs = newListWithItems("0000 0111")));
    assert(__LINE__, check(DTBChar, "-128", xs = newListWithItems("1000 0000")));
    assert(__LINE__, check(DTBChar, "-1", xs = newListWithItems("1111 1111")));
    assert(__LINE__, check(DTBChar, "0", xs = newListWithItems("0000 0000")));
    assert(__LINE__, check(DTBChar, "127", xs = newListWithItems("0111 1111")));

    // error
    assert(__LINE__, check(DTBChar, "255", xs = newListWithItems("Input error.")));
    assert(__LINE__, check(DTBChar, "08", xs));
    assert(__LINE__, check(DTBChar, "-x0",  xs));
    assert(__LINE__, check(DTBChar, "-0-7",  xs));

    freeList(xs);
}

void testDTBUnsignedChar() {
    list *xs;

    //normal
    assert(__LINE__, check(DTBUChar, "7", xs = newListWithItems("0000 0111")));
    assert(__LINE__, check(DTBUChar, "0", xs = newListWithItems("0000 0000")));
    assert(__LINE__, check(DTBUChar, "127", xs = newListWithItems("0111 1111")));
    assert(__LINE__, check(DTBUChar, "255", xs = newListWithItems("1111 1111")));

    // error
    assert(__LINE__, check(DTBUChar, "08", xs = newListWithItems("Input error.")));
    assert(__LINE__, check(DTBUChar, "-x0",  xs));
    assert(__LINE__, check(DTBUChar, "-1", xs));

    freeList(xs);
}

void testDTBInt() {
    list *xs;

    //normal
    assert(__LINE__, check(DTBInt, "7", xs = newListWithItems("0000 0000 0000 0000 0000 0000 0000 0111")));
    assert(__LINE__, check(DTBInt, "10000000", xs = newListWithItems("0000 0000 1001 1000 1001 0110 1000 0000")));
    assert(__LINE__, check(DTBInt, "0", xs = newListWithItems("0000 0000 0000 0000 0000 0000 0000 0000")));
    assert(__LINE__, check(DTBInt, "2147483647", xs = newListWithItems("0111 1111 1111 1111 1111 1111 1111 1111")));
    assert(__LINE__, check(DTBInt, "-1", xs = newListWithItems("1111 1111 1111 1111 1111 1111 1111 1111")));
    assert(__LINE__, check(DTBInt, "-2147483648", xs = newListWithItems("1000 0000 0000 0000 0000 0000 0000 0000")));
    assert(__LINE__, check(DTBInt, "-458", xs = newListWithItems("1111 1111 1111 1111 1111 1110 0011 0110")));

    // error
    assert(__LINE__, check(DTBInt, "2147483648", xs = newListWithItems("Input error.")));
    assert(__LINE__, check(DTBInt, "08", xs));
    assert(__LINE__, check(DTBInt, "-x0",  xs));
    assert(__LINE__, check(DTBInt, "-0-7",  xs));

    freeList(xs);
}

void testDTBUnsignedInt() {
    list *xs;

    //normal
    assert(__LINE__, check(DTBUInt, "7", xs = newListWithItems("0000 0000 0000 0000 0000 0000 0000 0111")));
    assert(__LINE__, check(DTBUInt, "10000000", xs = newListWithItems("0000 0000 1001 1000 1001 0110 1000 0000")));
    assert(__LINE__, check(DTBUInt, "0", xs = newListWithItems("0000 0000 0000 0000 0000 0000 0000 0000")));
    assert(__LINE__, check(DTBUInt, "4294967295", xs = newListWithItems("1111 1111 1111 1111 1111 1111 1111 1111")));

    // error
    assert(__LINE__, check(DTBUInt, "4294967296", xs = newListWithItems("Input error.")));
    assert(__LINE__, check(DTBUInt, "-1", xs ));
    assert(__LINE__, check(DTBUInt, "-2147483648", xs));
    assert(__LINE__, check(DTBUInt, "08", xs));
    assert(__LINE__, check(DTBUInt, "-x0",  xs));

    freeList(xs);
}

void testDTBLong() {
    list *xs;

    //normal
    assert(__LINE__, check(DTBLong, "10000000", xs = newListWithItems("0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 1001 1000 1001 0110 1000 0000")));
    assert(__LINE__, check(DTBLong, "0", xs = newListWithItems("0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000")));
    assert(__LINE__, check(DTBLong, "-1", xs = newListWithItems("1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111")));

    // error
    assert(__LINE__, check(DTBLong, "08", xs = newListWithItems("Input error.")));
    assert(__LINE__, check(DTBLong, "-x0",  xs));
    assert(__LINE__, check(DTBChar, "-0-7",  xs));

    freeList(xs);
}

void testDTBDouble() {
    list *xs;

    //normal - used https://www.binaryconvert.com/convert_double.html for verification
    assert(__LINE__, check(DTBDouble, "1.25", xs = newListWithItems("0011 1111 1111 0100 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000")));
    assert(__LINE__, check(DTBDouble, "-1.25", xs = newListWithItems("1011 1111 1111 0100 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000")));
    assert(__LINE__, check(DTBDouble, "0.1", xs = newListWithItems("0011 1111 1011 1001 1001 1001 1001 1001 1001 1001 1001 1001 1001 1001 1001 1010")));
    assert(__LINE__, check(DTBDouble, "0", xs = newListWithItems("0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000")));

    // error
    assert(__LINE__, check(DTBDouble, "08", xs = newListWithItems("Input error.")));
    assert(__LINE__, check(DTBDouble, "-x0",  xs));
    assert(__LINE__, check(DTBDouble, "-0-7",  xs));
    assert(__LINE__, check(DTBDouble, "0.5.5",  xs));

    freeList(xs);
}

void testBTDChar() {
    list *xs;

    //normal
    assert(__LINE__, check(BTDChar, "0000 1111", xs = newListWithItems("15")));
    assert(__LINE__, check(BTDChar, "0000 0000", xs = newListWithItems("0")));
    assert(__LINE__, check(BTDChar, "1111 1111", xs = newListWithItems("-1")));
    assert(__LINE__, check(BTDChar, "1111 1011", xs = newListWithItems("-5")));

    // error
    assert(__LINE__, check(BTDChar, "08", xs = newListWithItems("Input error.")));
    assert(__LINE__, check(BTDChar, "00001111",  xs));
    assert(__LINE__, check(BTDChar, "-0000 1111",  xs));

    freeList(xs);
}

void testBTDUnsignedChar() {
    list *xs;

    //normal
    assert(__LINE__, check(BTDUChar, "0000 1111", xs = newListWithItems("15")));
    assert(__LINE__, check(BTDUChar, "0000 0000", xs = newListWithItems("0")));
    assert(__LINE__, check(BTDUChar, "1111 1111", xs = newListWithItems("255")));

    // error
    assert(__LINE__, check(BTDUChar, "08", xs = newListWithItems("Input error.")));
    assert(__LINE__, check(BTDUChar, "00001111",  xs));
    assert(__LINE__, check(BTDUChar, "-0000 1111",  xs));

    freeList(xs);
}

void testBTDInt() {
    list *xs;

    //normal
    assert(__LINE__, check(BTDInt, "0000 1111 0000 0000 0000 0000 0000 0001", xs = newListWithItems("251658241")));
    assert(__LINE__, check(BTDInt, "0000 0000 0000 0000 0000 0000 0000 0000", xs = newListWithItems("0")));
    assert(__LINE__, check(BTDInt, "1111 1111 1111 1111 1111 1111 1111 1111", xs = newListWithItems("-1")));
    assert(__LINE__, check(BTDInt, "1111 1111 1111 1111 1001 1111 1111 1111", xs = newListWithItems("-24577")));

    // error
    assert(__LINE__, check(BTDInt, "08", xs = newListWithItems("Input error.")));
    assert(__LINE__, check(BTDInt, "11111111111111111111111111111111",  xs));
    assert(__LINE__, check(BTDInt, "-0000 1111 0000 0000 0000 0000 0000 0001",  xs));

    freeList(xs);
}

void testBTDUnsignedInt() {
    list *xs;

    //normal
    assert(__LINE__, check(BTDUInt, "0000 1111 0000 0000 0000 0000 0000 0001", xs = newListWithItems("251658241")));
    assert(__LINE__, check(BTDUInt, "0000 0000 0000 0000 0000 0000 0000 0000", xs = newListWithItems("0")));
    assert(__LINE__, check(BTDUInt, "1111 1111 1111 1111 1111 1111 1111 1111", xs = newListWithItems("4294967295")));

    // error
    assert(__LINE__, check(BTDUInt, "08", xs = newListWithItems("Input error.")));
    assert(__LINE__, check(BTDUInt, "11111111111111111111111111111111",  xs));
    assert(__LINE__, check(BTDUInt, "-0000 1111 0000 0000 0000 0000 0000 0001",  xs));

    freeList(xs);
}

void testBTDLong() {
    list *xs;

    //normal
    assert(__LINE__, check(BTDLong, "0000 0000 0000 0000 0000 0011 0000 0001 0000 1111 0000 0000 0000 0000 0000 0001", xs = newListWithItems("3303081508865")));
    assert(__LINE__, check(BTDLong, "0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000", xs = newListWithItems("0")));
    assert(__LINE__, check(BTDLong, "1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111", xs = newListWithItems("-1")));

    // error
    assert(__LINE__, check(BTDLong, "08", xs = newListWithItems("Input error.")));
    assert(__LINE__, check(BTDLong, "1111111111111111111111111111111111111111111111111111111111111111",  xs));
    assert(__LINE__, check(BTDLong, "-0000 1111 0000 0000 0000 0000 0000 0001 0000 1111 0000 0000 0000 0000 0000 0001",  xs));

    freeList(xs);
}

void testBTDDouble() {
    list *xs;

    //normal
    assert(__LINE__, check(BTDDouble, "1011 1111 1111 0100 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000", xs = newListWithItems("-1.25")));
    assert(__LINE__, check(BTDDouble, "0100 0000 1001 0000 0000 0011 1111 1000 0000 0000 0000 0000 0000 0000 0000 0000", xs = newListWithItems("1024.9921875")));
    assert(__LINE__, check(BTDDouble, "1000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000", xs = newListWithItems("-0")));
    assert(__LINE__, check(BTDDouble, "0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000", xs = newListWithItems("0")));


    // error
    assert(__LINE__, check(BTDDouble, "08", xs = newListWithItems("Input error.")));
    assert(__LINE__, check(BTDDouble, "1111111111111111111111111111111111111111111111111111111111111111",  xs));
    assert(__LINE__, check(BTDDouble, "-0000 1111 0000 0000 0000 0000 0000 0001 0000 1111 0000 0000 0000 0000 0000 0001",  xs));

    freeList(xs);
}

// Run the tests.
void test() {
    
    testDTBChar();
    testDTBUnsignedChar();
    testDTBInt();
    testDTBUnsignedInt();
    testDTBLong();
    testDTBDouble();
    testBTDChar();
    testBTDUnsignedChar();
    testBTDInt();
    testBTDUnsignedInt();
    testBTDLong();
    testBTDDouble();

    printf("All tests pass.\n");
    
}

// Deal with input and output
// If there are no arguments call test
// Otherwise:
//   check if valid input type
//   determine the type and run function for that type
int main(int n, char *args[n]) {
    setbuf(stdout, NULL);
    if (n == 1) test();
    else if (n==3){
        runfuncForType(args[1], NULL, args[2]);
    }
    else if (n==4){
        if(args[2][0] == '1' || args[2][0] == '0'){
            formatData(n,args);
        }
        runfuncForType(args[1],args[2],args[3]);
    }
    else{
        formatData(n,args);
        if(args[2][0] == '1' || args[2][0] == '0'){
            runfuncForType(args[1], NULL, args[3]);
        }
        else{
            runfuncForType(args[1],args[2],args[3]);
        }
    }
    return 0;
}
