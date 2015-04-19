#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define ABS(x) ((x < 0) ? -x : x)
#define FOREACH_IN(a,b,c) for(a = b; a <= c; a++)
#define NOT(x) (!(x)) /* ? 1:0) nicht noetig? */
#define UPPER_BOUND 50
#define LOWER_BOUND -10

bool test_ABS_Macro(int a){
  int absolut_value_of_a;
  absolut_value_of_a = ABS(a);
  printf("|%d| = %d\n",a,absolut_value_of_a);
  if (absolut_value_of_a < 0) {
   return false;
  }
  return true;
}

bool test_FOREACH_IN_Makro(){
  int a, b, c , sum = 0;
  b = 1;
  c = 10;
  FOREACH_IN(a,b,c){
   sum = sum + a;
  }
  printf("The sum of 1 to 10 is %d\n", sum);
  if (sum != 55){
    return false;
  }
  return true;

}

bool test_NOT_Makro(){
  if (NOT(1==0)){
    printf("1 != 0\n");
    return true;
  }
  printf("1 == 0\n");
  return false;
}

void sum_up_absolut_values(int array[], unsigned size_of_array){
  int i, sum = 0;
  FOREACH_IN(i,0,size_of_array-1){
    sum = sum + ABS(array[i]);
  }
  printf("Sum of the test array %d\n", sum);
}

void check_boundaries(int array[], unsigned size_of_array){
  int i;
  printf("Values between boundaries: ");
  FOREACH_IN(i,0,size_of_array-1){
    int value = array[i];
    if ( value > LOWER_BOUND && value < UPPER_BOUND){
      printf("%d ",value);
    }
  }
  printf("\n");

  printf("Values not between boundaries: ");
  FOREACH_IN(i,0,size_of_array-1){
    int value = array[i];
    if (NOT(value > LOWER_BOUND && value < UPPER_BOUND)){
      printf("%d ",value);
    }
  }
  printf("\n");

}
int main(void){
  int ar[10] = {2, -1, 3, -40 , 50,  200, 133, -16, 39, -100};
  /** test macros */
  bool tests_passed;
  tests_passed = test_ABS_Macro(2)
              && test_ABS_Macro(-4)
              && test_FOREACH_IN_Makro()
              && test_NOT_Makro();
  if (tests_passed != true){
    printf("ERROR: Macros do not work corretly!\n");
    return (EXIT_FAILURE);
  }
  printf("All macro tests passed\n\n");

  /** test macro combination */
  sum_up_absolut_values(ar, 10);
  check_boundaries(ar, 10);
  return (EXIT_SUCCESS);
}
