# message_system
This is a simple **example** of a **type-safe** message-passing system.
This is only a convenient to be able to declare messages and some 
type-safety checks that handlers are defined


## Short Example
```c++
#include <iostream>
#include "message_system.h"

// declares message identifiers
enum class ServerMessage {
    A,
    B,
    C,
    COUNT // This is required to be the last enumeration and named exactly `COUNT`
};


// Variant 1
MESSAGE_SYSTEM_DECLARE_MESSAGE(ServerMessage, ServerMessageA, A) {
    /* place here members and serialization code*/
};

// Variant 2
MESSAGE_SYSTEM_DECLARE(ServerMessage, B) {
    /* ... */
};

// Variant 3
struct ServerMessageC : message_system::IMessge<ServerMessage::C, ServerMessageC> {
    /* ... */
};
template<> struct message_system::MessageIdToType<ServerMessage::C> { using type = ServerMessageC;};



struct MyClass : message_system::System<MyClass, ServerMessage> {
    void OnMessage(const ServerMessageA* msg) { std::cout << "A handled with id = " << static_cast<int>(msg->GetId()) << "\n"; }
    void OnMessage(const ServerMessageB* msg) { std::cout << "B handled with id = " << static_cast<int>(msg->GetId()) << "\n"; }
    void OnMessage(const ServerMessageC* msg) { std::cout << "C handled with id = " << static_cast<int>(msg->GetId()) << "\n"; }
};


int main() {
    MyClass test;

    message_system::MessageBase<ServerMessage>* msg = new ServerMessageB();
    test.handle(msg);

    msg = new ServerMessageA();
    test.handle(msg);

    msg = new ServerMessageC();
    test.handle(msg);
}

```
