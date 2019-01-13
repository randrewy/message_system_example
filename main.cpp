#include <iostream>
#include "message_system.h"


enum class ServerMessage {
    A,
    B,
    COUNT
};
std::ostream& operator << (std::ostream& os, ServerMessage m) { return os << static_cast<int>(m); }

MESSAGE_SYSTEM_DECLARE_MESSAGE(ServerMessage, MessageA, A) {};
DECLARE_MESSAGE(ServerMessage, B) {};


struct MyClass : message_system::System<MyClass, ServerMessage> {

    void OnMessage(const MessageA* msg) { std::cout << "A handled with id = " << msg->GetId() << "\n"; }
    void OnMessage(const MessageB* msg) { std::cout << "B handled with id = " << msg->GetId() << "\n"; }

    MyClass() {
        message_system::MessageBase<ServerMessage>* msg = new MessageB();
        handle(msg);

        msg = new MessageA();
        handle(msg);
    }
};


int main() {
    MyClass s;
}


