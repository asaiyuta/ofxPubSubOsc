//
//  ofxPubSubOscTemplateInstantiation.cpp
//
//  Created by ISHII 2bit on 2018/08/14.
//

#include "ofxPubSubOsc.h"

#define BBB_EXTERN
#include "details/Instantiation/InstantiationSubscribe.inl"
#undef BBB_EXTERN

#define BBB_EXTERN
#include "details/Instantiation/InstantiationPublish.inl"
#undef BBB_EXTERN
