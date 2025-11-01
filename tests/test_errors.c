/* Test file for error handling */

int main() {
    /* Test 1: Valid tokens */
    int x = 10;
    
    /* Test 2: Invalid character */
    int y = @100;
    
    /* Test 3: Malformed identifier with invalid char after separator */
    int bad_id = aaa_*bbb;
    
    /* Test 4: Malformed number - letter after exponent 'e' */
    int bad_num1 = 2.3eX;
    
    /* Test 5: Another malformed number */
    int bad_num2 = 1.5e+ABC;
    
    /* Test 6: Invalid special character */
    int z = 50 ~ 2;
    
    /* Test 7: Multiple invalid characters */
    int result = x & y | z;
    
    /* Test 8: Unclosed comment at EOF - uncomment to test
    /* This comment never closes...
    int never_reached = 99;
    */
    
    return 0;
}
