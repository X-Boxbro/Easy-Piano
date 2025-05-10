#include "fluidsynth_initial.h"

#include<iostream>

fluidsynth_initial::fluidsynth_initial(const std::string soundfontpath) : settings(nullptr), synth(nullptr), audioDriver(nullptr) {

    settings = new_fluid_settings();

    fluid_settings_setnum(settings, "synth.sample-rate", 44100.0);
    fluid_settings_setnum(settings, "synth.reverb.damp", 0.3);
    fluid_settings_setnum(settings, "synth.reverb.level", 0.7);
    fluid_settings_setnum(settings, "synth.reverb.room-size", 1.0);
    fluid_settings_setnum(settings, "synth.reverb.width", 0.8);
    fluid_settings_setnum(settings, "synth.chorus.depth", 3.6);
    fluid_settings_setnum(settings, "synth.chorus.level", 0.55);
    fluid_settings_setint(settings, "synth.chorus.nr", 4);
    fluid_settings_setnum(settings, "synth.chorus.speed", 0.36);
    fluid_settings_setint(settings, "synth.reverb.active", 1);
    fluid_settings_setint(settings, "synth.chorus.active", 1);
    fluid_settings_setnum(settings, "synth.gain", 1.5);
    synth = new_fluid_synth(settings);
    audioDriver = new_fluid_audio_driver(settings, synth);
    if (!loadsoundfont(soundfontpath)) {
        std::cerr << "Failed to load default SoundFont: " << soundfontpath << std::endl;
    }
}

bool fluidsynth_initial::loadsoundfont(const std::string soundfontpath){
    int result = fluid_synth_sfload(synth, soundfontpath.c_str(), 1);
    if (result == FLUID_FAILED) {
        std::cerr << "Error: Failed to load SoundFont: " << soundfontpath << std::endl;
        return false;
    }
    return true;
}

void fluidsynth_initial::playNote(int channel,int key,int velocity){
    fluid_synth_noteon(synth,channel,key,velocity);
}
//stopNote函数实现
void fluidsynth_initial::stopNote(int channel,int key){
    fluid_synth_noteoff(synth,channel,key);
}

fluidsynth_initial::~fluidsynth_initial(){
    delete_fluid_audio_driver(audioDriver);
    delete_fluid_synth(synth);
    delete_fluid_settings(settings);
}
