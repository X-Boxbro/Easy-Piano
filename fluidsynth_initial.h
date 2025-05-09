#ifndef FLUIDSYNTH_INITIAL_H
#define FLUIDSYNTH_INITIAL_H


#include"fluidsynth.h"

#include<string>

class fluidsynth_initial
{
    friend class mappingmanager;
public:
    fluidsynth_initial(const std::string soundfontpath);
    ~fluidsynth_initial();
    bool loadsoundfont(const std::string soundfontpath);
    void playNote(int channel,int key,int velocity);
    void stopNote(int channel,int key);

private:
    fluid_settings_t* settings;
    fluid_synth_t* synth;
    fluid_audio_driver_t* audioDriver;

};

#endif // FLUIDSYNTH_INITIAL_H
