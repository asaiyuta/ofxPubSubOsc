//
//  ofxPubSubOscTemplateInstantiation.cpp
//  ofxPubSubOscUnitTest
//
//  Created by ISHII 2bit on 2018/08/14.
//

#include "ofxPubSubOsc.h"

namespace ofx {
    namespace PubSubOsc {
        namespace Subscribe {
#define instantiate_parameters(...) \
            template struct Parameter<__VA_ARGS__>;\
            template auto Subscriber::make_parameter_ref(__VA_ARGS__ &value) -> enable_if_t<!is_callable<__VA_ARGS__>::value, ParameterRef>;\
            template SubscribeIdentifier Subscriber::subscribe<__VA_ARGS__ &>(const std::string &, __VA_ARGS__ &);\
            template struct SetterFunctionParameter<void, __VA_ARGS__>;\
            template ParameterRef Subscriber::make_parameter_ref(std::function<void(__VA_ARGS__)>);\
            template SubscribeIdentifier Subscriber::subscribe<const std::function<void(__VA_ARGS__)> &>(const std::string &, const std::function<void(__VA_ARGS__)> &);\
            template SubscribeIdentifier Subscriber::subscribe< std::function<void(__VA_ARGS__)> &&>(const std::string &, std::function<void(__VA_ARGS__)> &&);

            instantiate_parameters(bool);
            instantiate_parameters(char);
            instantiate_parameters(unsigned char);
            instantiate_parameters(short);
            instantiate_parameters(unsigned short);
            instantiate_parameters(int);
            instantiate_parameters(unsigned int);
            instantiate_parameters(long);
            instantiate_parameters(unsigned long);
            instantiate_parameters(long long);
            instantiate_parameters(unsigned long long);
            
            instantiate_parameters(float);
            instantiate_parameters(double);
            
            instantiate_parameters(std::string);
            instantiate_parameters(ofBuffer);
            
            instantiate_parameters(ofxOscMessage);
            instantiate_parameters(ofxOscMessageEx);

            instantiate_parameters(ofColor);
            instantiate_parameters(ofShortColor);
            instantiate_parameters(ofFloatColor);
            
            instantiate_parameters(ofVec2f);
            instantiate_parameters(ofVec3f);
            instantiate_parameters(ofVec4f);
            instantiate_parameters(ofQuaternion);
            instantiate_parameters(ofMatrix3x3);
            instantiate_parameters(ofMatrix4x4);
            
#ifdef GLM_VERSION
            instantiate_parameters(glm::vec2);
            instantiate_parameters(glm::vec3);
            instantiate_parameters(glm::vec4);
            instantiate_parameters(glm::quat);
            instantiate_parameters(glm::mat2);
            instantiate_parameters(glm::mat3);
            instantiate_parameters(glm::mat4);
#endif
            
            instantiate_parameters(ofRectangle);
            
#undef instantiate_parameters
            
#define instantiate_parameters(RET, ...) \
            template struct SetterFunctionParameter<RET, __VA_ARGS__>;\
            template ParameterRef Subscriber::make_parameter_ref(std::function<RET(__VA_ARGS__)>);\
            template SubscribeIdentifier Subscriber::subscribe<const std::function<RET(__VA_ARGS__)> &>(const std::string &, const std::function<RET(__VA_ARGS__)> &);\
            template SubscribeIdentifier Subscriber::subscribe< std::function<RET(__VA_ARGS__)> &&>(const std::string &, std::function<RET(__VA_ARGS__)> &&);

            instantiate_parameters(void, void);
            instantiate_parameters(void, float, float);
            instantiate_parameters(void, float, float, float);

#undef instantiate_parameters
        }
    }
}
