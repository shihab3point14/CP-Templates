   Divisibility Rule:

   2. If even;
   3. Sum of the digit is divisible by 3;
   4. last two digit is divisible by 4;
   5. number ends in a 5 or 0;
   6. number is divisible by both 2 and 3;
   7. double the last digit and substract it from the remaining number 
        if then the number is divisible by 7.
        example : 3976 last digit is 6 double it 6*2 = 12
                    substract 397 - 12 = 385
                    still 385 is large so do the following operation again
                    385 = 5*2 = 10 now 38-10 = 28
                    now 28 is small we can now direct divide 28/7
   8. last three digit is divisible by 8;
   9. sum of the digit is divisible by 9;
   10. number ends in a zero;
   11. add odd digit(index) , add even digit(index), find the difference
        is the result divisible by 11? then 
        example : 693 here index 1 : 6, index 2 : 9, index 3 : 3
                add all odd index digit = 6 + 3 = 9
                add all even index  digit = 9
                difference = 9-9 = 0 is divisible by 11;
        example :8153783 is divisible by 11;
                odd index sum = 8+5+7+3 = 23
                even index sum = 1+3+8 = 12
                difference = 23 - 12 = 11 is divisible
   12. if number is divisible by both 3 and 4
   13. if(digit==3) multiply 4 to the last digit and add it to the remaining number
        if then divisible then it is divisible by 13
        example : 585 = 4 * 5 = 20 now 58 + 20 = 78 is divisible

        if(digit > 3) then make the given number into groups of 3 digit
        from right to left, Now if the difference between the sums of alternate
        groups is divisible by 13 then the given number also divislbe by 13.
        example : 1108503773 = (1)(108)(503)(773)
        sum of first group = 773 + 108 = 881
        sum of second group = 503 + 1 = 504
        difference = 377 now 377 is divisible by 13
   15. if number is divisible by both 3 and 5