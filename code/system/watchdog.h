
#ifndef watchdog__h
#define watchdog__h

namespace ns_wdogtimer
{
    __monitor void enabled();
    __monitor void disabled();
    __monitor void reset();
    class WatchDogOff
    {
    private:
        unsigned char stat;
    public:
        WatchDogOff();
        ~WatchDogOff();
    };
}
#endif
