# Modbus RTU temperature sensor

The sensor based on STM8F003 chip uses ADT7420 to measure temperature.

## Registers table

Data | Table | Number | R/W | Range | Default
--- | --- | --- | --- | --- | ---
Temperature | Analog Input Registers | 0x0000 | R |  | Nan
Sensor Version | Analog Input Registers | 0x0001 | R | 0x01...0xFF | 0x01
MODBUS ID | Analog Output Holding Registers | 0x0000 | R/W | 0x01...0x80 | 0x01


## Tempreature request example
```
Request:  01 04 00 00 00 01 31 f9
Response: 01 04 02 00 f9 7d 73

Value: 0xF9 => 249
Temperature: 24.9°C
```

## Schematic

<img src=./readme_src/schematic.png>
