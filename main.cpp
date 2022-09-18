#include "helper.h" // 212 Стариков Илья

using namespace std;

int main()
{
    interface MAIN;

    MAIN.Fill();

    MAIN.ReadCommand();

    for (auto& el : MAIN.instructions)
    {

        if (el == "EXIT"){ //Program finished successfully
            cout << "all good! Program finished successfully!" << endl;
            return 0;
        }
        int code = MAIN.CONTROLLER(el);

    }
    return 0;
}
