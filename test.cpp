#include "sphere.hpp"

int main() {
    sphere *s = new sphere(vec3(0.,0.,-1),0.5);
    int my_mutable = 0;
    int second_var = 1;
    const int *pp;
    pp = &my_mutable;
    pp = &second_var;
    *pp = 3;
    printf("%d\n", *pp);
    my_mutable =2;
    printf("%d\n", *pp);


}
