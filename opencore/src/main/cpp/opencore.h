#ifndef OPENCORESDK_OPENCORE_H
#define OPENCORESDK_OPENCORE_H

#include <jni.h>
#include <string>

#define ELFCOREMAGIC "CORE"
/*
             ---------- <-
            |          |  \
          --| Program  |   \
         |  | 1 Header |   |
         |  |          |   |
         |   ----------    |
         |  |          |   |        ----------
       --|--| Program  |   |       |          |
      |  |  | 2 Header |   |       |ELF Header|
      |  |  |          |   \       |          |
      |  |   ----------     ------- ----------       ----> ----------
      |  |  |**********|           |          |     |     |          |
      |  |  |**********|           |          |     |     | Thread 1 |
      |  |  |**********|           |  Program |     |     | Registers|
      |  |  |**********|           |  Headers |     |     |          |
      |  |  |**********|           |          |     |      ----------
      |  |  |**********|           |          |    /      |          |
      |  |  |**********|           |          |   /       | Thread 2 |
      |  |   ----------            |          |  /        | Registers|
      |  |  |          |     ------ ---------- --         |          |
    --|--|--| Program  |    /      |          |            ----------
   |  |  |  | N Header |   |     ->|  Segment |           |**********|
   |  |  |  |          |   /    |  | (PT_NOTE)|           |**********|
   |  |  |   ---------- <--   --   |          |           |**********|
   |  |  |                   |      ---------- --         |**********|
   |  |   -------------------      |          |  \         ----------
   |  |                            |  Segment |   \       |          |
   |   --------------------------->| (PT_LOAD)|    |      | Thread N |
   |                               |          |    |      | Registers|
   |                                ----------     |      |          |
   |                               |          |    |       ----------
   |                               |  Segment |    \      |          |
   |                               | (PT_LOAD)|     \     |   AUXV   |
   |                               |          |      \    |          |
   |                                ----------        ---> ----------
   |                               |**********|
   |                               |**********|
   |                               |**********|
   |                               |**********|
   |                               |**********|
   |                                ----------
   |                               |          |
    ------------------------------>|  Segment |
                                   | (PT_LOAD)|
                                   |          |
                                    ----------
*/
#define NT_GNU_PROPERTY_TYPE_0 5

struct userdata {
    jclass gCoredump;
    jmethodID gCallbackEvent;
};

class Opencore {
public:
    static const int MODE_PTRACE = 1 << 0;
    static const int MODE_COPY = 1 << 1;

    static Opencore* GetInstance();
    static bool IsFilterSegment(std::string segment);
    static void HandleSignal(int);
    static void dump(bool java);
    static void callback(void *user);
    static bool enable();
    static bool disable();
    static void setDir(std::string dir);
    static void setUserData(userdata *u);
    static void setMode(int mode);

    Opencore() { mode = MODE_COPY | MODE_PTRACE; }
    virtual bool DoCoreDump() = 0;
    std::string GetCoreDir() { return dir; }
    userdata* GetUser() { return user; }
    int GetMode() { return mode; }
private:
    void SetCoreDir(std::string d) { dir = d; }
    void SetUserData(userdata *u) { user = u; }
    void SetMode(int m) { mode = m & (MODE_COPY | MODE_PTRACE); }

    userdata* user;
    std::string dir;
    int mode;
};

#endif //OPENCORESDK_OPENCORE_H
