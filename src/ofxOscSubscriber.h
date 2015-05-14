//
//  ofxOscSubscriber.h
//
//  Created by ISHII 2bit on 2015/05/10.
//
//

#pragma once

#include "ofMain.h"
#include "ofxOsc.h"

#include "details/ofx_type_traits.h"

namespace ofx {
    class OscSubscriber {
        struct AbstractParameter {
            virtual void read(ofxOscMessage &message) {}
        };
        
        struct SetImplementation {
        protected:
#define define_set_arithmetic(type) \
            inline void set(ofxOscMessage &m, type &v, size_t offset = 0) { \
                if(m.getArgType(offset) == OFXOSC_TYPE_INT32) v = m.getArgAsInt32(offset); \
                else if(m.getArgType(offset) == OFXOSC_TYPE_INT64) v = m.getArgAsInt64(offset); \
                else if(m.getArgType(offset) == OFXOSC_TYPE_FLOAT) v = m.getArgAsFloat(offset); \
            }
            define_set_arithmetic(bool);
            define_set_arithmetic(char);
            define_set_arithmetic(unsigned char);
            define_set_arithmetic(short);
            define_set_arithmetic(unsigned short);
            define_set_arithmetic(int);
            define_set_arithmetic(unsigned int);
            define_set_arithmetic(long);
            define_set_arithmetic(unsigned long);
            
            define_set_arithmetic(float);
            define_set_arithmetic(double);
#undef define_set_arithmetic
            
            inline void set(ofxOscMessage &m, string &v, size_t offset = 0) {
                v = m.getArgAsString(offset);
            }
            
            inline void set(ofxOscMessage &m, ofBuffer &v, size_t offset = 0) {
                v = m.getArgAsBlob(offset);
            }
            
            inline void set(ofxOscMessage &m, ofColor &v, size_t offset = 0)      { setColor<unsigned char>(m, v, 255, offset); }
            inline void set(ofxOscMessage &m, ofShortColor &v, size_t offset = 0) { setColor<unsigned short>(m, v, 65535, offset); }
            inline void set(ofxOscMessage &m, ofFloatColor &v, size_t offset = 0) { setColor<float>(m, v, 1.0f, offset); }
            
            template <typename U>
            inline void setColor(ofxOscMessage &m, ofColor_<U> &v, U defaultValue, size_t offset = 0) {
                if(m.getNumArgs() == 1) {
                    set(m, v.r, offset);
                    set(m, v.g, offset);
                    set(m, v.b, offset);
                    v.a = defaultValue;
                } else if(m.getNumArgs() == 3) {
                    set(m, v.r, offset + 0);
                    set(m, v.g, offset + 1);
                    set(m, v.b, offset + 2);
                    v.a = defaultValue;
                } else {
                    set(m, v.r, offset + 0);
                    set(m, v.g, offset + 1);
                    set(m, v.b, offset + 2);
                    set(m, v.a, offset + 3);
                }
            }
            
            inline void set(ofxOscMessage &m, ofVec2f &v, size_t offset = 0)      { setVec<2>(m, v, offset); }
            inline void set(ofxOscMessage &m, ofVec3f &v, size_t offset = 0)      { setVec<3>(m, v, offset); }
            inline void set(ofxOscMessage &m, ofVec4f &v, size_t offset = 0)      { setVec<4>(m, v, offset); }
            inline void set(ofxOscMessage &m, ofQuaternion &v, size_t offset = 0) { setVec<4>(m, v, offset); }
            inline void set(ofxOscMessage &m, ofMatrix3x3 &v, size_t offset = 0)  { setVec<9>(m, v, offset); }
            
            template <size_t n, typename U>
            inline void setVec(ofxOscMessage &m, U &v, size_t offset = 0) {
                for(int i = 0; i < min(static_cast<size_t>(m.getNumArgs()), n); i++) {
                    set(m, v[i], offset + i);
                }
            }
            
            inline void set(ofxOscMessage &m, ofMatrix4x4 &v, size_t offset = 0) {
                for(int j = 0; j < 4; j++) for(int i = 0; i < 4; i++) {
                    set(m, v(i, j), offset + 4 * j + i);
                }
            }
            
            inline void set(ofxOscMessage &m, ofRectangle &v, size_t offset = 0) {
                set(m, v.x,      offset + 0);
                set(m, v.y,      offset + 1);
                set(m, v.width,  offset + 2);
                set(m, v.height, offset + 3);
            }
            
            template <typename U, size_t size>
            inline void set(ofxOscMessage &m, U v[size], size_t offset = 0) {
                for(int i = 0; i < min(size, m.getNumArgs() / ofx_type_traits<U>::size); i++) {
                    set(m, v[i], offset + i * ofx_type_traits<U>::size);
                }
            }
            
            template <typename U>
            inline void set(ofxOscMessage &m, vector<U> &v, size_t offset = 0) {
                if(v.size() < m.getNumArgs() / ofx_type_traits<U>::size) { v.resize(m.getNumArgs() / ofx_type_traits<U>::size); }
                for(int i = 0; i < v.size(); i++) {
                    set(m, v[i], offset + i * ofx_type_traits<U>::size);
                }
            }
        };
        
        template <typename T>
        struct Parameter : AbstractParameter, SetImplementation {
            Parameter(T &t) : t(t) {}
            virtual void read(ofxOscMessage &message) { set(message, t); }
        
        private:
            
            T &t;
        };
        
        struct CallbackParameter : AbstractParameter, SetImplementation {
        public:
            typedef void (*Callback)(ofxOscMessage &);
            CallbackParameter(Callback callback)
            : callback(callback) {}
            
            virtual void read(ofxOscMessage &message) {callback(message); }

        private:
            Callback callback;
        };

        template <typename T>
        struct MethodCallbackParameter : AbstractParameter, SetImplementation {
            typedef void (T::*Callback)(ofxOscMessage &);
            MethodCallbackParameter(T &that, Callback callback)
            : that(that), callback(callback) {}
            MethodCallbackParameter(T *that, Callback callback)
            : that(*that), callback(callback) {}
            
            virtual void read(ofxOscMessage &message) { (that.*callback)(message); }
            
        private:
            Callback callback;
            T &that;
        };

        typedef shared_ptr<AbstractParameter> ParameterRef;
        typedef shared_ptr<ofxOscReceiver> OscReceiverRef;
        typedef map<string, ParameterRef> Targets;
        typedef map<int, pair<OscReceiverRef, Targets> > TargetsMap;
        typedef map<int, ParameterRef> LeakPickers;
        typedef map<int, vector<ofxOscMessage> > LeakedOscMessages;
    public:
        static OscSubscriber &getSharedInstance() {
            static OscSubscriber *sharedInstance = new OscSubscriber;
            return *sharedInstance;
        }
        
        inline void subscribe(int port, const string &address, ParameterRef ref) {
            LeakedOscMessages::iterator _ = leakedOscMessages.find(port);
            if(_ == leakedOscMessages.end()) {
                leakedOscMessages.insert(make_pair(port, vector<ofxOscMessage>()));
            }

            if(targetsMap.find(port) == targetsMap.end()) {
                OscReceiverRef receiver(new ofxOscReceiver);
                receiver->setup(port);
                targetsMap.insert(make_pair(port, make_pair(receiver, Targets())));
            }
            
            Targets::iterator it = targetsMap[port].second.find(address);
            if(it == targetsMap[port].second.end()) {
                targetsMap[port].second.insert(make_pair(address, ref));
            } else {
                it->second = ref;
            }
        }
        
        template <typename T>
        inline void subscribe(int port, const string &address, T &value) {
            subscribe(port, address, ParameterRef(new Parameter<T>(value)));
        }
        
        inline void subscribe(int port, const string &address, void (*callback)(ofxOscMessage &)) {
            subscribe(port, address, ParameterRef(new CallbackParameter(callback)));
        }
        
        template <typename T>
        inline void subscribe(int port, const string &address, T &that, void (T::*callback)(ofxOscMessage &)) {
            subscribe(port, address, ParameterRef(new MethodCallbackParameter<T>(that, callback)));
        }

        template <typename T>
        inline void subscribe(int port, const string &address, T *that, void (T::*callback)(ofxOscMessage &)) {
            subscribe(port, address, ParameterRef(new MethodCallbackParameter<T>(that, callback)));
        }
        
        inline void unsubscribe(int port, const string &address) {
            if(targetsMap.find(port) == targetsMap.end()) return;
            targetsMap[port].second.erase(address);
        }
        
        inline void unsubscribe(int port) {
            targetsMap.erase(port);
        }
        
        inline void setLeakPicker(int port, ParameterRef ref) {
            LeakPickers::iterator it = leakPickers.find(port);
            if(it == leakPickers.end()) {
                leakPickers.insert(make_pair(port, ref));
            } else {
                it->second = ref;
            }
        }
        
        inline void setLeakPicker(int port, void (*callback)(ofxOscMessage &)) {
            setLeakPicker(port, ParameterRef(new CallbackParameter(callback)));
        }
        
        template <typename T>
        inline void setLeakPicker(int port, T &that, void (T::*callback)(ofxOscMessage &)) {
            setLeakPicker(port, ParameterRef(new MethodCallbackParameter<T>(that, callback)));
        }
        
        template <typename T>
        inline void setLeakPicker(int port, T *that, void (T::*callback)(ofxOscMessage &)) {
            setLeakPicker(port, ParameterRef(new MethodCallbackParameter<T>(that, callback)));
        }
        
        inline void removeLeakPicker(int port) {
            if(isLeakedOscCovered(port)) {
                leakPickers.erase(port);
            }
        }
        
        inline bool isSubscribed(int port) const {
            return targetsMap.find(port) != targetsMap.end();
        }

        inline bool isSubscribed(int port, const string &address) const {
            return isSubscribed(port) && (targetsMap.at(port).second.find(address) != targetsMap.at(port).second.end());
        }
        
        inline bool isLeakedOscCovered(int port) const {
            return leakPickers.find(port) != leakPickers.end();
        }
        
        void clearLeakedOscMessages() {
            LeakedOscMessages::iterator it;
            for(it = leakedOscMessages.begin(); it != leakedOscMessages.end(); it++) {
                it->second.clear();
            }
        }
        
        void clearLeakedOscMessages(int port) {
            LeakedOscMessages::iterator it = leakedOscMessages.find(port);
            if(it != leakedOscMessages.end()) {
                it->second.clear();
            }
        }
        
        inline bool hasWaitingLeakedOscMessages(int port) const {
            if(isLeakedOscCovered(port)) {
                return false;
            } else {
                LeakedOscMessages::const_iterator it = leakedOscMessages.find(port);
                if(it == leakedOscMessages.end()) return false;
                return 0 < it->second.size();
            }
        }
        
        inline bool getNextLeakedOscMessage(int port, ofxOscMessage &m) {
            if(hasWaitingLeakedOscMessages(port)) {
                m.copy(leakedOscMessages[port].back());
                leakedOscMessages[port].pop_back();
                return true;
            } else {
                return false;
            }
        }
        
    private:
        void update(ofEventArgs &args) {
            clearLeakedOscMessages();
            for(TargetsMap::iterator _ = targetsMap.begin(); _ != targetsMap.end(); _++) {
                int port = _->first;
                Targets &targets = _->second.second;
                ofxOscReceiver *receiver = _->second.first.get();
                ofxOscMessage m;
                ParameterRef leakPicker;
                LeakPickers::iterator it = leakPickers.find(port);
                if(it != leakPickers.end()) {
                    leakPicker = it->second;
                }
                while(receiver->hasWaitingMessages()) {
                    receiver->getNextMessage(&m);
                    const string &address = m.getAddress();
                    if(targets.find(address) != targets.end()) {
                        targets[address]->read(m);
                    } else {
                        if(leakPicker) {
                            leakPicker->read(m);
                        } else {
                            leakedOscMessages[port].push_back(m);
                        }
                    }
                }
            }
        }
        
        OscSubscriber() {
            ofAddListener(ofEvents().update, this, &OscSubscriber::update, OF_EVENT_ORDER_BEFORE_APP);
        }
        virtual ~OscSubscriber() {
            ofRemoveListener(ofEvents().update, this, &OscSubscriber::update, OF_EVENT_ORDER_BEFORE_APP);
        }
        TargetsMap targetsMap;
        LeakPickers leakPickers;
        LeakedOscMessages leakedOscMessages;
    };
};

typedef ofx::OscSubscriber ofxOscSubscriber;

inline ofxOscSubscriber &ofxGetOscSubscriber() {
    return ofxOscSubscriber::getSharedInstance();
}

#pragma mark interface about subscribe

template <typename T>
inline void ofxSubscribeOsc(int port, const string &address, T &value) {
    ofxGetOscSubscriber().subscribe(port, address, value);
}

inline void ofxSubscribeOsc(int port, const string &address, void (*callback)(ofxOscMessage &)) {
    ofxGetOscSubscriber().subscribe(port, address, callback);
}

template <typename T>
inline void ofxSubscribeOsc(int port, const string &address, T &that, void (T::*callback)(ofxOscMessage &)) {
    ofxGetOscSubscriber().subscribe(port, address, that, callback);
}

template <typename T>
inline void ofxSubscribeOsc(int port, const string &address, T *that, void (T::*callback)(ofxOscMessage &)) {
    ofxGetOscSubscriber().subscribe(port, address, that, callback);
}

inline void ofxUnsubscribeOsc(int port, const string &address) {
    ofxGetOscSubscriber().unsubscribe(port, address);
}

inline void ofxUnsubscribeOsc(int port) {
    ofxGetOscSubscriber().unsubscribe(port);
}

#pragma mark interface about leaked osc

inline void ofxSetLeakedOscPicker(int port, void (*callback)(ofxOscMessage &)) {
    ofxGetOscSubscriber().setLeakPicker(port, callback);
}

template <typename T>
inline void ofxSetLeakedOscPicker(int port, T *that, void (T::*callback)(ofxOscMessage &)) {
    ofxGetOscSubscriber().setLeakPicker(port, that, callback);
}

template <typename T>
inline void ofxSetLeakedOscPicker(int port, T &that, void (T::*callback)(ofxOscMessage &)) {
    ofxGetOscSubscriber().setLeakPicker(port, that, callback);
}

inline void ofxRemoveLeakedOscPicker(int port) {
    ofxGetOscSubscriber().removeLeakPicker(port);
}