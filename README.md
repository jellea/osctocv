# oscpixi
OSC Port: `5000`
RTP Midi Port: `5004`
REST API Port: `80`

unipolar value are 0v to 10v
bipolar are -5v to +5v
API values are converted from these range into [-1, 1] for bipolar and [0, 1] for unipolar.

## OSC API
### Inputs
`/in/<[1-20]>/<trig|gate|cvuni|cvbi>`
### Outputs
`/out/<[1-20]>/<trig|gate|cvuni|cvbi> <float>`

### DAC: Output voltage
```/dac/vout/5 4.33```  
sets channel 5 to 4.33V.

```/dac/vouts 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0```  
sets all 20 channels to 1V.

### ADC: Read voltage
```/adc/subscribe 192.168.178.10 8400```  
receive osc messages on your ip+port

you'll receive messages in this form:
```/adc/vin/ 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0```

## REST API
on port 80. a welcome form help you to play with the api.
### Inputs
`/in?mode=<trig|gate|cvuni|cvbi>&index=<[1-20]>`
### Outputs
`/out?mode=<trig|gate|cvuni|cvbi>&index=<[1-20]>&value=<float>`

## RTP Midi API
TODO