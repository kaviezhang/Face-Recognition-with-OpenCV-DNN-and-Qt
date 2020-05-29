#include "wire.h"
#include "cglobal.h"

Wire::Wire()
{

}
void Wire::runSlot_widget()
{
    printf("runSlot_widget\n");
    run_=true;
    int count=0;
    while(run_)
    {
    }
}
void Wire::runSlot_edit()
{
    printf("runSlot_edit\n");
    run_=true;
    int count=0;
    while(run_)
    {

    }
}
void Wire::runSlot_choose()
{
    printf("runSlot_choose\n");
    run_=true;
    int count=0;
    while(run_)
    {

    }
}
void Wire::runSlot_result()
{
    printf("runSlot_result\n");
    run_=true;
    int count=0;
    while(run_)
    {

    }
    led_status=true;
}
