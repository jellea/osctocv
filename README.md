# osctocv
Port: `8888`

## OSC API

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

### Configure channels
Every channels can be separately configured.

- ADC/DAC
- Voltage range

```/conf/5 x x```
