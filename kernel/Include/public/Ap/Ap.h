#ifndef AP_H
#define AP_H


struct Ap
{
    enum DocLockMode {
        kNone = 0x00,
        kAutoWrite = 0x01,
        kNotLocked = 0x02,
        kWrite = 0x04,
        kProtectedAutoWrite = 0x14,
        kRead = 0x20,
        kXWrite = 0x40
    };
};


#endif

