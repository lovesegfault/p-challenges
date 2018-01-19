#include "person-generator.h"

int main() {
    person p;
    generate_person(&p);
    p.print(&p);
   // p.kill(&p);
    return 0;
}
