# oscpixi

unipolar value are 0v to 10v

bipolar are -5v to +5v

API values are converted from these range into [-1, 1] for bipolar and [0, 1] for unipolar.


## OSC API

OSC Port: `5000`

### Inputs

`/in/<[1-20]>/<trig|gate|cvuni|cvbi> <ip> <port>`

### Outputs

`/out/<[1-20]>/<trig|gate|cvuni|cvbi> <float>`

## REST API

REST API Port: `80`

on port 80. a welcome form help you to play with the api.

### Inputs

`/in?mode=<trig|gate|cvuni|cvbi>&index=<[1-20]>&ip=<ip>&port=<port>`

### Outputs

`/out?mode=<trig|gate|cvuni|cvbi>&index=<[1-20]>&value=<float>`

## RTP Midi API

RTP Midi Port: `5004`

TODO