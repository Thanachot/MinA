#include <iostream>
#include <map>
#include "MinA/common/Minimizer.h"
#include "MinA/common/Result.h"
#include "myfunction.cpp"
#include "MinA/algorithm/Simplex.h"
#include <utility>

int main(void)
{
    // bt.setHandle(KILL);
    std::shared_ptr<BadTen> bt(new BadTen);
    std::shared_ptr<Simplex> altest(new Simplex);
    Minimizer mina;
    int count = 0;
    for (int i = 0; i < 10; i++) {
        try {
            mina.minimize(bt, altest);
        }
        catch (Arithmetical_Exception& ae) {
            count++;
        }
    }
    std::cout << count << std::endl;
    return !(count == 10);
}
