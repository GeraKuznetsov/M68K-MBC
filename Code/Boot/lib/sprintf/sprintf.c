#include <stdarg.h>
//#include <stdio.h>
#include <string.h>
//#include <windows.h>

//static void ftoa_fixed(char *buffer, double value);
//static void ftoa_sci(char *buffer, double value);

int isNum(u8 c){
	return c >= '0' && c <= '9';
}
char* toHexString = "0123456789ABCDEF";
void panic(u8* message);
int _sprintf(char *buffer, char const *fmt, va_list arg) {

    //double double_temp;

    char ch;
    int length = 0;
	int num;
    //char buffer[512];
	i32 iVal;
	u32 uVal;
	char *string_temp;
	u8 cVal;
	char *revStart, *revEnd;
	
	while ((ch = *fmt++)) {
		num = -1;
		if ( '%' == ch ) {
			ch = *fmt++;
			if(isNum(ch)){
				num = ch - '0';
				ch = *fmt++;
			}
            switch (ch) {
                /* %% - print out a single %    */
                case '%':
                    //fputc('%', file);
					*buffer++ = '%';
                    length++;
                    break;

                /* %c: print out a character    */
                case 'c':
					cVal = va_arg(arg, int);
					*buffer++ = cVal;
                    length++;
                    break;

                /* %s: print out a string       */
                case 's':
                    string_temp = va_arg(arg, char *);
					while(*string_temp){
						*buffer++ = *string_temp++;
						length++;
					}
                    break;

                case 'u':
                    uVal = va_arg(arg, int);
					if(!uVal){
						*buffer++ = '0';
						length++;
						break;						
					}
					if(uVal < 0){
						*buffer++ = '-';
						length++;
						uVal = -uVal;
					}
					revStart = buffer;
					while(uVal){
						int rem = uVal % 10;
						*buffer++ = rem + '0';
						length++;
						uVal /= 10;
					}
					revEnd = buffer-1;
					while(revEnd > revStart){ //reverse the string
						char swap = *revStart;
						*revStart = *revEnd;
						*revEnd = swap;
						revStart++;
						revEnd--;
					}
                    break;
                case 'd':
                    iVal = va_arg(arg, int);
					if(!iVal){
						*buffer++ = '0';
						length++;
						break;						
					}
					if(iVal < 0){
						*buffer++ = '-';
						length++;
						iVal = -iVal;
					}
					revStart = buffer;
					while(iVal){
						int rem = iVal % 10;
						*buffer++ = rem + '0';
						length++;
						iVal /= 10;
					}
					revEnd = buffer-1;
					while(revEnd > revStart){ //reverse the string
						char swap = *revStart;
						*revStart = *revEnd;
						*revEnd = swap;
						revStart++;
						revEnd--;
					}
                    break;

                /* %x: print out an int in hex  */
				case 'p':
					num = 8;
                case 'x':
					if(num == -1) num = 4;
                    uVal = va_arg(arg, int);
					
					while(num){
						num--;
						u32 toPrint = (uVal >> (num * 4)) & 0x0F;
						*buffer++ = toHexString[toPrint];
						length++;
					}
                    break;
				/*
                case 'f':
                    double_temp = va_arg(arg, double);
                    ftoa_fixed(buffer, double_temp);
                    fputs(buffer, file);
                    length += strlen(buffer);
                    break;

                case 'e':
                    double_temp = va_arg(arg, double);
                    ftoa_sci(buffer, double_temp);
                    fputs(buffer, file);
                    length += strlen(buffer);
                    break;*/
            }
        }
        else {
            *buffer++ = ch;
            length++;
        }
    }
	*buffer++ = 0;
	length++;
    return length;
}

/*int normalize(double *val) {
    int exponent = 0;
    double value = *val;

    while (value >= 1.0) {
        value /= 10.0;
        ++exponent;
    }

    while (value < 0.1) {
        value *= 10.0;
        --exponent;
    }
    *val = value;
    return exponent;
}*/

//static void ftoa_fixed(char *buffer, double value) {  
//    /* carry out a fixed conversion of a double value to a string, with a precision of 5 decimal digits. 
//     * Values with absolute values less than 0.000001 are rounded to 0.0
//     * Note: this blindly assumes that the buffer will be large enough to hold the largest possible result.
//     * The largest value we expect is an IEEE 754 double precision real, with maximum magnitude of approximately
//     * e+308. The C standard requires an implementation to allow a single conversion to produce up to 512 
//     * characters, so that's what we really expect as the buffer size.     
//     */
//
//    int exponent = 0;
//    int places = 0;
//    static const int width = 4;
//
//    if (value == 0.0) {
//        buffer[0] = '0';
//        buffer[1] = '\0';
//        return;
//    }         
//
//    if (value < 0.0) {
//        *buffer++ = '-';
//        value = -value;
//    }
//
//    exponent = normalize(&value);
//
//    while (exponent > 0) {
//        int digit = value * 10;
//        *buffer++ = digit + '0';
//        value = value * 10 - digit;
//        ++places;
//        --exponent;
//    }
//
//    if (places == 0)
//        *buffer++ = '0';
//
//    *buffer++ = '.';
//
//    while (exponent < 0 && places < width) {
//        *buffer++ = '0';
//        --exponent;
//        ++places;
//    }
//
//    while (places < width) {
//        int digit = value * 10.0;
//        *buffer++ = digit + '0';
//        value = value * 10.0 - digit;
//        ++places;
//    }
//    *buffer = '\0';
//}
//
//void ftoa_sci(char *buffer, double value) {
//    int exponent = 0;
//    int places = 0;
//    static const int width = 4;
//
//    if (value == 0.0) {
//        buffer[0] = '0';
//        buffer[1] = '\0';
//        return;
//    }
//
//    if (value < 0.0) {
//        *buffer++ = '-';
//        value = -value;
//    }
//
//    exponent = normalize(&value);
//
//    int digit = value * 10.0;
//    *buffer++ = digit + '0';
//    value = value * 10.0 - digit;
//    --exponent;
//
//    *buffer++ = '.';
//
//    for (int i = 0; i < width; i++) {
//        int digit = value * 10.0;
//        *buffer++ = digit + '0';
//        value = value * 10.0 - digit;
//    }
//
//    *buffer++ = 'e';
//    itoa(exponent, buffer, 10);
//}

int sprintf(char *buffer, char const *fmt, ...) {
    va_list arg;
    int length;

    va_start(arg, fmt);
    length = _sprintf(buffer, fmt, arg);
    va_end(arg);
    return length;
}