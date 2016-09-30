#include <stdio.h>
#include <string.h>
#define _AUDIO_CPP_
#include "main.h"
#include "game.h"

void audioStream::initializeSound()
{
	result = System_Create(&fmodSystem);
	//result = fmodSystem->init(NUM_CHANNELS, FMOD_INIT_NORMAL, 0);
	//ERRCHECK(result);
	/*result = fmodSystem->getVersion(&version);
	//ERRCHECK(result);
	if(version < FMOD_VERSION)
	{
		printf("Error! You are using an old version of FMOD %08x. This program requires %08x\n", version, FMOD_VERSION);
		return;
	}*/
	result = fmodSystem->getNumDrivers(&numdrivers);
	//ERRCHECK(result);
	if(numdrivers == 0)
	{
		result = fmodSystem->setOutput(FMOD_OUTPUTTYPE_NOSOUND);
		//ERRCHECK(result);
	}
	else
	{
		result = fmodSystem->getDriverCaps(0, &caps, 0, &speakermode);
		//ERRCHECK(result);
		/*
		Set the user selected speaker mode.
		*/
		result = fmodSystem->setSpeakerMode(speakermode);
		//ERRCHECK(result);
		if(caps & FMOD_CAPS_HARDWARE_EMULATED)
		{
			/*
			The user has the 'Acceleration' slider set to off! This is really bad
			for latency! You might want to warn the user about this.
			*/
			result = fmodSystem->setDSPBufferSize(1024, 10);
			//ERRCHECK(result);
		}
		result = fmodSystem->getDriverInfo(0, name, 256, 0);
		//ERRCHECK(result);
		if(strstr(name, "SigmaTel"))
		{
			/*
			Sigmatel sound devices crackle for some reason if the format is PCM 16bit.
			PCM floating point output seems to solve it.
			*/
			result = fmodSystem->setSoftwareFormat(48000, FMOD_SOUND_FORMAT_PCMFLOAT, 0,0, FMOD_DSP_RESAMPLER_LINEAR);
			//ERRCHECK(result);
		}
	}
	result = fmodSystem->init(NUM_GAME_SOUNDS, FMOD_INIT_NORMAL, 0);
	if(result == FMOD_ERR_OUTPUT_CREATEBUFFER)
	{
		/*
		Ok, the speaker mode selected isn't supported by this soundcard. Switch it
		back to stereo...
		*/
		result = fmodSystem->setSpeakerMode(FMOD_SPEAKERMODE_STEREO);
		//ERRCHECK(result);
		/*
		... and re-init.
		*/
		result = fmodSystem->init(100, FMOD_INIT_NORMAL, 0);
	}
	//ERRCHECK(result);
}

void audioStream::updateSound()
{
	result = fmodSystem->update();
}

void audioStream::loadSounds()
{
	for(unsigned char i = 0; i < NUM_GAME_SOUNDS; i++)
	{
		if(sound_info[i].channel == GAME_SOUND_CHANNEL_MUSIC) result = fmodSystem->createStream(sound_info[i].name, FMOD_SOFTWARE|FMOD_LOOP_NORMAL, 0, &sound[i]);
		else result = fmodSystem->createSound(sound_info[i].name, FMOD_DEFAULT, 0, &sound[i]);
		sound[i]->setMode(sound_info[i].loop);
	}
}

void audioStream::playSound(GAME_SOUND_ENUM sound_id)
{
	if(sound_info[sound_id].channel == GAME_SOUND_CHANNEL_MUSIC)
	{
		result = fmodSystem->playSound(FMOD_CHANNEL_REUSE, sound[sound_id], false, &channel[GAME_SOUND_CHANNEL_MUSIC]);
		result = channel[GAME_SOUND_CHANNEL_MUSIC]->setVolume(game_volume.music);
		//result = channel[GAME_SOUND_CHANNEL_MUSIC]->setPaused(false);
	}
	else
	{
		result = fmodSystem->playSound(FMOD_CHANNEL_FREE, sound[sound_id], false, &channel[sound_info[sound_id].channel]);
		result = channel[sound_info[sound_id].channel]->setVolume(game_volume.sounds);
		//result = channel[sound_info[sound_id].channel]->setPaused(false);
	}
}

void audioStream::pauseChannel(GAME_SOUND_CHANNEL_ENUM channel_id)
{
	result = channel[channel_id]->setPaused(true);
}

void audioStream::unpauseChannel(GAME_SOUND_CHANNEL_ENUM channel_id)
{
	result = channel[channel_id]->setPaused(false);
}

void audioStream::setVolume(GAME_SOUND_CHANNEL_ENUM channel_id, float volume)
{
	result = channel[channel_id]->setVolume(volume);
}