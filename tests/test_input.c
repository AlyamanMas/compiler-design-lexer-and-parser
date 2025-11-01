/* This is a test file for the lexical analyzer */

int main() {
    int x = 10;
    int y_value = 20;
    int z.field = 30;
    int a#member = 40;
    int b$var = 50;
    
    /* Test case insensitivity */
    IF (x < y_value) {
        RETURN x;
    } else {
        return y_value;
    }
    
    /* Test numbers */
    int num1 = 123;
    int num2 = 45.67;
    int num3 = 89.;
    int num4 = 1.23e10;
    int num5 = 4.56E-3;
    int num6 = 78e+2;
    
    /* Test all special symbols */
    x = x + 1;
    y_value = y_value - 2;
    z.field = z.field * 3;
    a#member = a#member / 4;
    
    while (x <= 100) {
        x = x + 1;
    }
    
    if (x >= 100) {
        x = 0;
    }
    
    if (x == 0) {
        x = 1;
    }
    
    if (x != 0) {
        x = 2;
    }
    
    int arr[10];
    arr[0] = 5;
    
    void test(int a, int b) {
        return;
    }
}

/* Test error cases */

/* This comment is properly closed */

int error1 = 12ab;  /* Number followed by identifier - scanner produces num then id */

/* Uncomment the following lines to test errors:

/* Unclosed comment - will cause error at EOF

int bad_id = aaa_*;  /* Malformed identifier 

int bad_num = 2.3eX; /* Malformed number 

int invalid = @#$;   /* Invalid characters 

*/
