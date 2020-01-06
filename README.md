This repo features a C++ implementation of the Scattering Delay Network reverberation algorithm described by De Sena et al. [1].

It used high level parameters such as room, source and microphone geometry, and the absorption characteristics of the air and walls to produce a realistic artificial reverberation of a cuboidal room.

Usage of the implementation is demonstrated in a JUCE VST.

# Reverb

Report describing the algorithm and implementation details is [here](https://github.com/chrisyeoward/ScatteringDelayNetwork/blob/master/assets/scattering-delay-network.pdf).


# VST
You should be able to open the .jucer file and build the VST for your architecture.

UI:
![alt text](https://github.com/chrisyeoward/ScatteringDelayNetwork/blob/master/assets/UI.png "User Interface")

# Examples
[Processed audio](https://github.com/chrisyeoward/ScatteringDelayNetwork/blob/master/assets/drums_processed.mp3)

[Unprocessed](https://github.com/chrisyeoward/ScatteringDelayNetwork/blob/master/assets/drums_unprocessed.mp3)

# References
[1] De Sena, E et al. (2015)
    Efficient Synthesis of Room Acoustics via Scattering Delay Networks - IEEE
