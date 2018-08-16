//
//  InstantiationPublish.inl
//
//  Created by ISHII 2bit on 2018/08/17.
//

#define INSTANTIATE_FUNCTION(...) \
namespace ofx {\
    namespace PubSubOsc {\
        namespace Publish {\
            BBB_EXTERN template struct Parameter<__VA_ARGS__, true>;\
            BBB_EXTERN template struct Parameter<__VA_ARGS__, false>;\
            BBB_EXTERN template struct ConstParameter<__VA_ARGS__, true>;\
            BBB_EXTERN template struct ConstParameter<__VA_ARGS__, false>;\
            BBB_EXTERN template auto Publisher::make_parameter_ref<__VA_ARGS__>(__VA_ARGS__ &value, bool = false) -> enable_if_t<!is_callable<__VA_ARGS__>::value, ParameterRef>;\
            BBB_EXTERN template auto Publisher::make_parameter_ref<__VA_ARGS__>(const __VA_ARGS__ &value, bool = false) -> enable_if_t<!is_callable<__VA_ARGS__>::value, ParameterRef>;\
            BBB_EXTERN template PublishIdentifier Publisher::publish<__VA_ARGS__ &>(const std::string &, __VA_ARGS__ &, bool = true);\
            BBB_EXTERN template PublishIdentifier Publisher::publish<const __VA_ARGS__ &>(const std::string &, const __VA_ARGS__ &, bool = true);\
            BBB_EXTERN template struct FunctionParameter<__VA_ARGS__, true>;\
            BBB_EXTERN template struct FunctionParameter<__VA_ARGS__, false>;\
            BBB_EXTERN template ParameterRef Publisher::make_parameter_ref<__VA_ARGS__>(std::function<__VA_ARGS__()>, bool = false);\
            BBB_EXTERN template PublishIdentifier Publisher::publish<__VA_ARGS__>(const std::string &, std::function<__VA_ARGS__()>, bool = true);\
        };\
    };\
};\
BBB_EXTERN template ofxOscPublisherIdentifier ofxPublishOsc<const __VA_ARGS__ &>(const std::string &ip, std::uint16_t port, const std::string &address, const __VA_ARGS__& valueOrFunction, bbb::explicit_bool whenValueIsChanged = true);\
BBB_EXTERN template ofxOscPublisherIdentifier ofxPublishOsc<__VA_ARGS__ &>(const std::string &ip, std::uint16_t port, const std::string &address, __VA_ARGS__& valueOrFunction, bbb::explicit_bool whenValueIsChanged = true);\
BBB_EXTERN template void ofxSendOsc<const __VA_ARGS__ &>(const std::string &ip, std::uint16_t port, const std::string &address, const __VA_ARGS__ &arg)

#include "InstantiationList.inl"
#undef INSTANTIATE_FUNCTION
