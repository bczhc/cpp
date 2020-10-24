#include "../Concurrent.h"
#include "../utf8.h"
#include "./sapi.h"
#include "pers_zhc_test_jni_JNI.h"

using namespace bczhc;
using namespace utf8;
using namespace linearlist;
using namespace std;
using namespace concurrent;

class TTS {
private:
    ISpVoice *pVoice{};

    class R : public Runnable {
    private:
        const char *s;

    public:
        explicit R(const char *s) : s(s) {}
        void run() override {
            TTS tts;
            tts.say(this->s);
            delete this;
        }
    };

public:
    TTS() {
        if (FAILED(::CoInitialize(nullptr)))
            throw 1;
        CoCreateInstance(CLSID_SpVoice, nullptr, CLSCTX_ALL, IID_ISpVoice, (void **) &pVoice);
    }

    ~TTS() {
        pVoice->lpVtbl->Release(pVoice);
        pVoice = nullptr;
        ::CoUninitialize();
    }

    int say(const char *s) {
        wchar_t *wc = utf8ToWChar(s);
        pVoice->lpVtbl->SetPriority(pVoice, static_cast<SPVPRIORITY>(2));
        pVoice->lpVtbl->SetRate(pVoice, 100);
        pVoice->lpVtbl->Speak(pVoice, wc, SPEAKFLAGS::SPF_DEFAULT, nullptr);
        delete wc;
        return 0;
    }

    static void asyncSay(const char *s) {
        R *r = new R(s);
        Thread t2(r);
    }
};


int main(int argc, char **argv) {
}

JNIEXPORT void JNICALL Java_pers_zhc_test_jni_JNI_speak(JNIEnv *env, jclass cls, jstring str) {
    const char *s = env->GetStringUTFChars(str, (boolean *) nullptr);
    TTS::asyncSay(s);
}