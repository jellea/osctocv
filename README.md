# oscpixi

oscpixi interfaces eurorack modular signals (cv, trigger, gate) with OSC, RTP MIDI and Rest APIs

Physical input and output while unipolar are [0v, 10v], and when bipolar are [-5v, +5v].
Input and Output values are converted from these range into [-1, 1] for bipolar and [0, 1] for unipolar in OSC and REST API
and [0, 127] in MIDI CCs.

LFO frequencies are in Hertz, ranging [0Hz, 10Hz]

PWM value are between [0, 1].



## OSC API

OSC Port: `5000`

### Inputs

`/in/<[1-20]>/<mode> <ip> <port>`

mode is one of : trig, gate, flipflop, cvuni, cvbi

### Outputs

`/out/<[1-20]>/<mode> <float>`

mode is one of : trig, gate, flipflop, cvuni, cvbi, lfosine, lfosaw, lfotri, lfosquare

## REST API

REST API Port: `80`

A welcome form help you to play with the REST API.

### Inputs

`/in?mode=<mode>&index=<[1-20]>&ip=<ip>&port=<port>`

mode is one of : trig, gate, flipflop, cvuni, cvbi

### Outputs

`/out?mode=<mode>&index=<[1-20]>&value=<float>`

mode is one of : trig, gate, flipflop, cvuni, cvbi, lfosine, lfosaw, lfotri, lfosquare

## RTP MIDI API

RTP Midi Port: `5004`

TODO