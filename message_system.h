#pragma once
#include <array>

#define MESSAGE_SYSTEM_DECLARE_MESSAGE(Domain, Name, ID)                                    \
    struct Name;                                                                            \
    template<> struct message_system::MessageIdToType<Domain::ID> { using type = Name;};    \
    struct Name : message_system::IMessge<Domain::ID, Name>

#define DECLARE_MESSAGE(Domain, Name)                                                                       \
    struct Message##Name;                                                                                   \
    template<> struct message_system::MessageIdToType<Domain::Name> { using type = Message##Name;};         \
    struct Message##Name : message_system::IMessge<Domain::Name, Message##Name>


namespace message_system {

template<typename MessageDomain>
struct MessageBase {
    virtual ~MessageBase() {}
    virtual MessageDomain GetId() const = 0;
};

template<auto MsgId, typename Msg = void>
struct IMessge : MessageBase<decltype(MsgId)> {  
    using Message = Msg;
    using MessageDomain = decltype(MsgId);

    virtual MessageDomain GetId() const override { return MsgId; }
};

template<auto id> struct MessageIdToType {};


template<typename Class, typename Domain>
class System {
    std::array<void(System::*)(const MessageBase<Domain>* msg), static_cast<size_t>(Domain::COUNT)> handlers;

    template<Domain realId>
    void OnMessageHandler(const MessageBase<Domain>* msg) {
        using RealMsgType = typename MessageIdToType<realId>::type;
        const auto realMessage = static_cast<const RealMsgType*>(msg);
        return static_cast<Class*>(this)->OnMessage(realMessage);
    }

    template<Domain I>
    bool init_helper() {
        handlers[static_cast<size_t>(I)] = OnMessageHandler<I>;
        return true;
    }

    template<typename E, std::size_t... I>
    void init_handlers (std::index_sequence<I...>) {
       bool tmp[] = {init_helper<E(I)>()...};
       (void)tmp;
    }

    template<typename E>
    void init_handlers () {
        constexpr size_t count = static_cast<size_t>(E::COUNT);
        init_handlers<E>(std::make_index_sequence<count>{});
    }

public:

    void handle(const MessageBase<Domain>* msg) {
        const size_t id = static_cast<size_t>(msg->GetId());
        if (0 <= id && id < static_cast<size_t>(Domain::COUNT)) {
            auto handler = handlers[id];
            (this->*handler)(msg);
        } else {
            onError(id, msg);
        }
    }

    void onError(size_t id, const MessageBase<Domain>* msg) { }


    System () {
        init_handlers<Domain>();
    }
};

} // namespace message_system

