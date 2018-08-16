//
//  InstantiationSubscribe.inl
//
//  Created by ISHIi 2bit on 2018/08/17.
//

#define INSTANTIATE_FUNCTION(...) \
namespace ofx {\
    namespace PubSubOsc {\
        namespace Subscribe {\
            BBB_EXTERN template struct Parameter<__VA_ARGS__>;\
            BBB_EXTERN template auto Subscriber::make_parameter_ref(__VA_ARGS__ &value) -> enable_if_t<!is_callable<__VA_ARGS__>::value, ParameterRef>;\
            BBB_EXTERN template SubscribeIdentifier Subscriber::subscribe<__VA_ARGS__ &>(const std::string &, __VA_ARGS__ &);\
            BBB_EXTERN template struct SetterFunctionParameter<void, __VA_ARGS__>;\
            BBB_EXTERN template ParameterRef Subscriber::make_parameter_ref(std::function<void(__VA_ARGS__)>);\
            BBB_EXTERN template SubscribeIdentifier Subscriber::subscribe<const std::function<void(__VA_ARGS__)> &>(const std::string &, const std::function<void(__VA_ARGS__)> &);\
            BBB_EXTERN template SubscribeIdentifier Subscriber::subscribe< std::function<void(__VA_ARGS__)> &&>(const std::string &, std::function<void(__VA_ARGS__)> &&);\
        };\
    };\
};\
BBB_EXTERN template ofxOscSubscriberIdentifier ofxSubscribeOsc<__VA_ARGS__>(std::uint16_t port, const std::string &address, __VA_ARGS__ &value);

#include "InstantiationList.inl"
#undef INSTANTIATE_FUNCTION

#define instantiate_parameters(RET, ...) \
namespace ofx {\
    namespace PubSubOsc {\
        namespace Subscribe {\
            BBB_EXTERN template struct SetterFunctionParameter<RET, __VA_ARGS__>;\
            BBB_EXTERN template ParameterRef Subscriber::make_parameter_ref(std::function<RET(__VA_ARGS__)>);\
            BBB_EXTERN template SubscribeIdentifier Subscriber::subscribe<const std::function<RET(__VA_ARGS__)> &>(const std::string &, const std::function<RET(__VA_ARGS__)> &);\
            BBB_EXTERN template SubscribeIdentifier Subscriber::subscribe< std::function<RET(__VA_ARGS__)> &&>(const std::string &, std::function<RET(__VA_ARGS__)> &&);\
        };\
    };\
};

instantiate_parameters(void, ofxOscMessage);
instantiate_parameters(void, ofxOscMessageEx);
instantiate_parameters(void, void);
instantiate_parameters(void, float, float);
instantiate_parameters(void, float, float, float);

#undef instantiate_parameters
